package parser

import (
	"fmt"
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
			fmt.Errorf("Sehol: Advance exceed bounds.")
		}
	}

	expect := func(t string) {
		if t != currentToken.Value {
			fmt.Errorf("SeholSyntaxError: Is expected " + t + ", but we got" + currentToken.Value + ".")
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
								fmt.Errorf("SeholSyntaxError: You miss an \"]\".")
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

	statement = func() map[string]interface{} {
		if currentToken.Kind == "Identifier" {
			return parseFn()
		}
		return nil
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
