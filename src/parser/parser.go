package parser

import (
	"fmt"
	"regexp"
	Tokens "sehol/tokens"
)

func Parser(tree []Tokens.Tokens) []map[string]interface{} {
	var index = 0
	var currentToken = tree[index]

	advance := func() {
		if len(tree)-1 > index {
			index++
			currentToken = tree[index]
		} else {
			fmt.Println("Sehol: Advance exceed bounds.")
			return
		}
	}

	expect := func(t string) {
		if t != currentToken.Value {
			panic("SeholSyntaxError: Is expected " + t + ", but we got" + currentToken.Value + ".")
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
		if token.Kind == "Number" || token.Kind == "Identifier" || token.Kind == "String" {
			advance()
			if currentToken.Value == "[" {
				advance()
				index = expression()
			}
			return map[string]any{"token": token, "index": index}
		} else if token.Kind == "openParen" {
			advance()
			result := expression()
			if currentToken.Kind == "closeParen" {
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
		if token.Kind == "Number" || token.Kind == "Identifier" || token.Kind == "String" {
			advance()
			if currentToken.Kind == "[" {
				advance()
				index = expression()
			}
			return map[string]interface{}{"token": token, "index": index}
		} else if token.Kind == "openParen" {
			advance()
			result = expression()
			if currentToken.Kind == "closeParen" {
				advance()
				return result
			}
		}
		return nil
	}

	expression = func() map[string]interface{} {
		var result = term()
		for currentToken.Value == "plusOp" || currentToken.Kind == "minusOp" {
			var token = currentToken
			if token.Kind == "plusOp" {
				advance()
				result = map[string]interface{}{
					"name":  "expression",
					"op":    "plusOp",
					"left":  result,
					"right": term(),
				}
			} else if token.Kind == "minusOp" {
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
					args = append(args, map[string]interface{}{"w": currentToken.Value, "str": currentToken.Kind == "String"})
				} else {
					var id = currentToken.Value
					advance()
					if currentToken.Value == "[" {
						for currentToken.Value != "]" {
							advance()
							if currentToken.Kind == "EOF" {
								fmt.Println("SeholSyntaxError: You miss an \"]\".")
							}
							indexArray += currentToken.Value
						}
					}
					args = append(args, map[string]interface{}{"w": []string{id, indexArray}, "str": currentToken.Kind == "String"})
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
				if currentToken.Kind == "EOF" {
					fmt.Println("SeholSyntaxError: You miss an \"]\".")
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
			const errorMsg = `SeholRuntimeError: The attribution of type \"${valueType}\" is different from the type of variable \"${id}\" that is \"${type}\".`
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

		} else if currentToken.Kind == "Identifier" {
			return parseFn()
		} else {
			if currentToken.Kind == "EOF" {
				return nil
			}
			if currentToken.Kind == "String" {
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
