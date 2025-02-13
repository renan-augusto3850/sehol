package parser

import (
	"fmt"
	"os"
	"regexp"
	Token "seahorse/token"
)

type Parser struct {
	Tree []Token.Token
	Index int
	CurrentToken *Token.Token
}

func advance(parser Parser) {
	if len(parser.Tree) >= parser.Index {
		parser.Index++
		parser.CurrentToken = &parser.Tree[parser.Index]
		fmt.Println(parser.CurrentToken)
	}
}

func factor(parser Parser) map[string]interface{} {
	var token = parser.CurrentToken
	var index any
	if token.Kind == Token.TOKEN_NUMERIC || token.Kind == Token.TOKEN_ID || token.Kind == Token.TOKEN_STRING {
		advance(parser)
		 if parser.CurrentToken.Value == "[" {
			advance(parser)
			index = expression(parser)
		}
		return map[string]any{"token": token, "index": index}
	} else if token.Kind == Token.TOKEN_OPENPAREN {
		advance(parser)
		result := expression(parser)
		if parser.CurrentToken.Kind == Token.TOKEN_CLOSEPAREN {
			advance(parser)
			return result
		}
	}
	return nil
}


func term(parser Parser) map[string]interface{} {
	var result = factor(parser)
	var token = parser.CurrentToken
	var index any
	if token.Kind == Token.TOKEN_NUMERIC || token.Kind == Token.TOKEN_ID || token.Kind == Token.TOKEN_STRING {
		advance(parser)
		if parser.CurrentToken.Value == "[" {
			advance(parser)
			index = expression(parser)
		}
		return map[string]interface{}{"token": token, "index": index}
	} else if token.Kind == Token.TOKEN_OPENPAREN {
		advance(parser)
		result = expression(parser)
		if parser.CurrentToken.Kind == Token.TOKEN_CLOSEPAREN {
			advance(parser)
			return result
		}
	}
	return nil
}

func expect(parser Parser, t string) {
	if t != parser.CurrentToken.Value {
		x := parser.CurrentToken.Value
		if len(x) <= 0 {
			x = "EOF"
		}
		fmt.Println("Seahorse: SyntaxError: Expected " + t + ", but got " + x + ".")
		os.Exit(1)
	}
}

func expression(parser Parser) map[string]interface{} {
	var result = term(parser)
	for parser.CurrentToken.Kind == Token.TOKEN_PLUS || parser.CurrentToken.Kind == Token.TOKEN_MINUS {
		var token = parser.CurrentToken
		if token.Kind == Token.TOKEN_PLUS {
			advance(parser)
			result = map[string]interface{}{
				"name":  "expression",
				"op":    "plusOp",
				"left":  result,
				"right": term(parser),
			}
		} else if token.Kind == Token.TOKEN_MINUS {
			advance(parser)
			result = map[string]interface{}{
				"name":  "expression",
				"op":    "minusOp",
				"left":  result,
				"right": term(parser),
			}
		}
	}
	advance(parser)
	return result
}

func parseFn(parser Parser) map[string]interface{} {
	var call = parser.CurrentToken.Value
	advance(parser)
	expect(parser, "(")
	advance(parser)
	var args = []map[string]interface{}{}
	var indexArray = ""
	for parser.CurrentToken.Value != ")" {
		if parser.CurrentToken.Value != "," && parser.CurrentToken.Value != "[" && parser.CurrentToken.Value != "]" {
			if parser.Index+1 < len(parser.Tree) && parser.Tree[parser.Index+1].Value != "[" {
				args = append(args, map[string]interface{}{"w": parser.CurrentToken.Value, "str": parser.CurrentToken.Kind == Token.TOKEN_STRING})
			} else {
				var id = parser.CurrentToken.Value
				advance(parser)
				if parser.CurrentToken.Value == "[" {
					for parser.CurrentToken.Value != "]" {
						advance(parser)
						if parser.CurrentToken.Kind == Token.TOKEN_EOF {
							fmt.Println("Seahorse: SyntaxError: You miss a \"]\".")
						}
						indexArray += parser.CurrentToken.Value
					}
				}
				args = append(args, map[string]interface{}{"w": []string{id, indexArray}, "str": parser.CurrentToken.Kind == Token.TOKEN_STRING})
			}
		}
		advance(parser)
	}

	advance(parser)
	expect(parser, ";")
	advance(parser)
	return map[string]interface{}{
		"name": "fnCall",
		"call": call,
		"args": args,
	}
}

func typeThis(t any) string {
	tokenTypes := []struct {
		regex *regexp.Regexp
		typ   string
	}{
		{regexp.MustCompile(`^\d+`), "Number"},
		{regexp.MustCompile(`^(true|false)$`), "Boolean"},
		{regexp.MustCompile(`^\[([^\]]*)\]$`), "ListArray"},
		{regexp.MustCompile(`^[a-zA-Z_]\w*`), "String"},
	}
	for _, tokenType := range tokenTypes {
		if tokenType.regex.MatchString(fmt.Sprintf("%v", t)) {
			return tokenType.typ
		}
	}
	// if _, ok := t.(map[string]interface{})["call"]; ok {
	// 	return "nativeFn"
	// }
	return ""
}

func parseVar(parser Parser) map[string]interface{} {
	advance(parser)
	var id = parser.CurrentToken.Value
	advance(parser)
	var typeOfVar string
	if parser.CurrentToken.Value == ":" {
		advance(parser)
		typeOfVar = parser.CurrentToken.Value
		advance(parser)
	}
	expect(parser, "=")
	advance(parser)
	value := interface{}("")
	if parser.CurrentToken.Value == "[" {
		for parser.CurrentToken.Value != "]" {
			if parser.CurrentToken.Kind == Token.TOKEN_EOF {
				fmt.Println("Seahorse: SyntaxError: You miss a \"]\".")
			}
			value = value.(string) + parser.CurrentToken.Value
			advance(parser)
		}
		value = value.(string) + "]"
		advance(parser)
		expect(parser, ";")
		advance(parser)
	} else if parser.CurrentToken.Value == "-" {
		value = value.(string) + parser.CurrentToken.Value
		advance(parser)
		value = value.(string) + parser.CurrentToken.Value
		advance(parser)
		expect(parser, ";")
		advance(parser)
	} else {
		value = interface{}(statement(parser))
		if valueOf, ok := value.(map[string]interface{}); ok {
			if valueOf["w"] != nil {
				value = valueOf["w"]
				advance(parser)
				expect(parser, ";")
				advance(parser)
			}
		}
	}
	// if v, ok := value.(map[string]interface{}); ok {
	// 	value = v["Value"]
	// }
	var valueType = typeThis(value)
	if typeOfVar != "" && valueType != typeOfVar {
		const errorMsg = `Seahorse: RuntimeError: The attribution of type \"${valueType}\" is different from the type of variable \"${id}\" that is \"${type}\".`
		fmt.Println(errorMsg)
	}
	return map[string]interface{}{
		"name":  "varDeclaration",
		"id":    id,
		"type":  valueType,
		"value": value,
	}
}

func statement(parser Parser) map[string]interface{} {
	if parser.CurrentToken.Value == "var" {
		return parseVar(parser)

	} else if parser.CurrentToken.Kind == Token.TOKEN_ID {
		return parseFn(parser)
	} else {
		if parser.CurrentToken.Kind == Token.TOKEN_EOF {
			return nil
		}
		if parser.CurrentToken.Kind == Token.TOKEN_STRING {
			return map[string]interface{}{
				"w":   parser.CurrentToken.Value,
				"str": true,
			}
		}
		return expression(parser)
	}
}

func Parse(parser Parser) []map[string]interface{} {
	var result = []map[string]interface{}{}
	var run = true

	for run {
		call := statement(parser)
		if call != nil {
			result = append(result, call)
		} else {
			run = false
		}
	}
	return result
}
