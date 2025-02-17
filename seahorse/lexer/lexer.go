package lexer

import (
	"seahorse/token"
	"unicode"
)

func Lexer(input string) []token.Token {
	var tree []token.Token
	code := input
	for i := 0; i < len(code); {
		c := rune(code[i])
		cStr := string(c)
		switch {
		case c == '\n':
			tree = append(tree, token.Token{Kind: token.TOKEN_LF, Value: "\n", Index: i})
		case unicode.IsSpace(c):
			i++
			continue
		case unicode.IsDigit(c): // Números
			start := i
			for i < len(code) && unicode.IsDigit(rune(code[i])) {
				i++
			}
			tree = append(tree, token.Token{Kind: token.TOKEN_NUMERIC, Value: code[start:i], Index: start})
			continue
		case c == '"':
			var value string
			i++ // Pula a primeira aspas
			for i < len(input) && input[i] != '"' {
				value += string(input[i])
				i++
			}
			tree = append(tree, token.Token{Kind: token.TOKEN_STRING, Value: value, Index: i})
		case cStr == "+": // Operadores
			tree = append(tree, token.Token{Kind: token.TOKEN_PLUS, Value: cStr, Index: i})
		case cStr == "-":
			tree = append(tree, token.Token{Kind: token.TOKEN_MINUS, Value: cStr, Index: i})
		case cStr == "*":
			tree = append(tree, token.Token{Kind: token.TOKEN_MUL, Value: cStr, Index: i})
		case cStr == "/":
			tree = append(tree, token.Token{Kind: token.TOKEN_DIV, Value: cStr, Index: i})
		case cStr == "=":
			if i+1 < len(input) && input[i+1] == '=' {
				tree = append(tree, token.Token{Kind: token.TOKEN_EQ, Value: "==", Index: i})
				i++
			} else {
				tree = append(tree, token.Token{Kind: token.TOKEN_ASSIGN, Value: cStr, Index: i})
			}
		case cStr == "!":
			if i+1 < len(input) && input[i+1] == '=' {
				tree = append(tree, token.Token{Kind: token.TOKEN_NE, Value: "!=", Index: i})
				i++
			} else {
				tree = append(tree, token.Token{Kind: token.TOKEN_NOT, Value: cStr, Index: i})
			}
		case cStr == "<":
			if i+1 < len(input) && input[i+1] == '=' {
				tree = append(tree, token.Token{Kind: token.TOKEN_LE, Value: "<=", Index: i})
				i++
			} else {
				tree = append(tree, token.Token{Kind: token.TOKEN_LT, Value: cStr, Index: i})
			}
		case cStr == ">":
			if i+1 < len(input) && input[i+1] == '=' {
				tree = append(tree, token.Token{Kind: token.TOKEN_GE, Value: ">=", Index: i})
				i++
			} else {
				tree = append(tree, token.Token{Kind: token.TOKEN_GT, Value: cStr, Index: i})
			}
		case cStr == ",":
			tree = append(tree, token.Token{Kind: token.TOKEN_COMMA, Value: cStr, Index: i})
		case cStr == "(":
			tree = append(tree, token.Token{Kind: token.TOKEN_OPENPAREN, Value: cStr, Index: i})
		case cStr == ")":
			tree = append(tree, token.Token{Kind: token.TOKEN_CLOSEPAREN, Value: cStr, Index: i})
		case unicode.IsLetter(c):
			start := i
			for i < len(code) && (unicode.IsLetter(rune(code[i])) || unicode.IsDigit(rune(code[i]))) {
				i++
			}
			value := code[start:i]
			switch value {
			case "var":
				tree = append(tree, token.Token{Kind: token.TOKEN_VAR, Value: value, Index: start})
			default:
				tree = append(tree, token.Token{Kind: token.TOKEN_ID, Value: value, Index: start})
				continue
			} 
		}
		i++
	}
	tree = append(tree, token.Token{Kind: token.TOKEN_EOF, Value: "", Index: 0})
	return tree
}
