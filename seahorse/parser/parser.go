package parser

import (
	"fmt"
	"os"
	"regexp"
	Token "seahorse/token"
)

func Parser(tree []Token.Token) []map[string]interface{} {
	var index = 0
	var currentToken = tree[index]

	advance := func() {
		if len(tree)-1 > index {
			index++
			currentToken = tree[index]
		} else {
			fmt.Println("Seahorse: Advance exceed bounds.")
			return
		}
	}

	expect := func(t string) {
		if t != currentToken.Value {
			x := currentToken.Value
			if len(x) <= 0 {
				x = "EOF"
			}
			fmt.Println("Seahorse: SyntaxError: Expected " + t + ", but got " + x + ".")
			os.Exit(1)
		}
	}

	var factor func() map[string]interface{}
	var term func() map[string]interface{}
	var expression func() map[string]interface{}
	var parseFn func() map[string]interface{}
	var statement func() map[string]interface{}

	factor = func() map[string]interface{} {
		var token = currentToken
		var index any
		if token.Kind == Token.TOKEN_NUMERIC || token.Kind == Token.TOKEN_ID || token.Kind == Token.TOKEN_STRING {
			advance()
			if currentToken.Value == "[" {
				advance()
				index = expression()
			}
			return map[string]any{"token": token, "index": index}
		} else if token.Kind == Token.TOKEN_OPENPAREN {
			advance()
			result := expression()
			if currentToken.Kind == Token.TOKEN_CLOSEPAREN {
				advance()
				return result
			}
		}
		return nil
	}

	term = func() map[string]interface{} {
		var result = factor()
		var token = currentToken
		var index any
		if token.Kind == Token.TOKEN_NUMERIC || token.Kind == Token.TOKEN_ID || token.Kind == Token.TOKEN_STRING {
			advance()
			if currentToken.Value == "[" {
				advance()
				index = expression()
			}
			return map[string]interface{}{"token": token, "index": index}
		} else if token.Kind == Token.TOKEN_OPENPAREN {
			advance()
			result = expression()
			if currentToken.Kind == Token.TOKEN_CLOSEPAREN {
				advance()
				return result
			}
		}
		return nil
	}

	expression = func() map[string]interface{} {
		var result = term()
		for currentToken.Kind == Token.TOKEN_PLUS || currentToken.Kind == Token.TOKEN_MINUS {
			var token = currentToken
			if token.Kind == Token.TOKEN_PLUS {
				advance()
				result = map[string]interface{}{
					"name":  "expression",
					"op":    "plusOp",
					"left":  result,
					"right": term(),
				}
			} else if token.Kind == Token.TOKEN_MINUS {
				advance()
				result = map[string]interface{}{
					"name":  "expression",
					"op":    "minusOp",
					"left":  result,
					"right": term(),
				}
			}
		}
		advance()
		return result
	}

	typeThis := func(t any) string {
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

	parseFn = func() map[string]interface{} {
		var call = currentToken.Value
		advance()
		expect("(")
		advance()
		var args = []map[string]interface{}{}
		var indexArray = ""
		for currentToken.Value != ")" {
			if currentToken.Value != "," && currentToken.Value != "[" && currentToken.Value != "]" {
				if index+1 < len(tree) && tree[index+1].Value != "[" {
					args = append(args, map[string]interface{}{"w": currentToken.Value, "str": currentToken.Kind == Token.TOKEN_STRING})
				} else {
					var id = currentToken.Value
					advance()
					if currentToken.Value == "[" {
						for currentToken.Value != "]" {
							advance()
							if currentToken.Kind == Token.TOKEN_EOF {
								fmt.Println("Seahorse: SyntaxError: You miss a \"]\".")
							}
							indexArray += currentToken.Value
						}
					}
					args = append(args, map[string]interface{}{"w": []string{id, indexArray}, "str": currentToken.Kind == Token.TOKEN_STRING})
				}
			}
			advance()
		}

		advance()
		expect(";")
		advance()
		return map[string]interface{}{
			"name": "fnCall",
			"call": call,
			"args": args,
		}
	}

	parseVar := func() map[string]interface{} {
		advance()
		var id = currentToken.Value
		advance()
		var typeOfVar string
		if currentToken.Value == ":" {
			advance()
			typeOfVar = currentToken.Value
			advance()
		}
		expect("=")
		advance()
		value := interface{}("")
		if currentToken.Value == "[" {
			for currentToken.Value != "]" {
				if currentToken.Kind == Token.TOKEN_EOF {
					fmt.Println("Seahorse: SyntaxError: You miss a \"]\".")
				}
				value = value.(string) + currentToken.Value
				advance()
			}
			value = value.(string) + "]"
			advance()
			expect(";")
			advance()
		} else if currentToken.Value == "-" {
			value = value.(string) + currentToken.Value
			advance()
			value = value.(string) + currentToken.Value
			advance()
			expect(";")
			advance()
		} else {
			value = interface{}(statement())
			if valueOf, ok := value.(map[string]interface{}); ok {
				if valueOf["w"] != nil {
					value = valueOf["w"]
					advance()
					expect(";")
					advance()
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

	statement = func() map[string]interface{} {
		if currentToken.Value == "var" {
			return parseVar()

		} else if currentToken.Kind == Token.TOKEN_ID {
			return parseFn()
		} else {
			if currentToken.Kind == Token.TOKEN_EOF {
				return nil
			}
			if currentToken.Kind == Token.TOKEN_STRING {
				return map[string]interface{}{
					"w":   currentToken.Value,
					"str": true,
				}
			}
			return expression()
		}
	}

	var result = []map[string]interface{}{}
	var run = true

	for run {
		call := statement()
		if call != nil {
			result = append(result, call)
		} else {
			run = false
		}
	}
	return result
}
