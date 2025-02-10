package lexer

import (
	Tokens "sehol/tokens"
	"unicode"
)

func Lexer(input string) []Tokens.Tokens {
	var tree []Tokens.Tokens
	code := input

	for i := 0; i < len(code); {
		c := rune(code[i])
		cStr := string(c)
		switch {
		case unicode.IsSpace(c): // Ignora espaços em branco
			i++
			continue

		case unicode.IsDigit(c): // Números
			start := i
			for i < len(code) && unicode.IsDigit(rune(code[i])) {
				i++
			}
			tree = append(tree, Tokens.Tokens{Kind: "NUMBER", Value: code[start:i], Index: start})
			continue
		case c == '"':
			var value string
			i++ // Pula a primeira aspas
			for i < len(input) && input[i] != '"' {
				value += string(input[i])
				i++
			}

			tree = append(tree, Tokens.Tokens{Kind: "String", Value: value, Index: i})
		case cStr == "+": // Operadores
			tree = append(tree, Tokens.Tokens{Kind: "plusOp", Value: cStr, Index: i})
		case cStr == "-":
			tree = append(tree, Tokens.Tokens{Kind: "minusOp", Value: cStr, Index: i})
		case cStr == "*":
			tree = append(tree, Tokens.Tokens{Kind: "timesOp", Value: cStr, Index: i})
		case cStr == "/":
			tree = append(tree, Tokens.Tokens{Kind: "divOp", Value: cStr, Index: i})
		case cStr == "=":
			tree = append(tree, Tokens.Tokens{Kind: "assignOp", Value: cStr, Index: i})
		case cStr == ";":
			tree = append(tree, Tokens.Tokens{Kind: "semicolon", Value: cStr, Index: i})
		case cStr == "(":
			tree = append(tree, Tokens.Tokens{Kind: "openParen", Value: cStr, Index: i}) // Parêntese esquerdo
		case cStr == ")":
			tree = append(tree, Tokens.Tokens{Kind: "closeParen", Value: cStr, Index: i}) // Parêntese direito

		case unicode.IsLetter(c): // Identificadores (ex: var, nome de variáveis)
			start := i
			for i < len(code) && (unicode.IsLetter(rune(code[i])) || unicode.IsDigit(rune(code[i]))) {
				i++
			}
			tree = append(tree, Tokens.Tokens{Kind: "Identifier", Value: code[start:i], Index: start})
			continue
		}
		i++
	}
	tree = append(tree, Tokens.Tokens{Kind: "EOF", Value: "", Index: 0})
	return tree
}
