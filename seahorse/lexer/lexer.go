package lexer

import (
	Token "seahorse/token"
	"unicode"
)

func Lexer(input string) []Token.Token {
	var tree []Token.Token
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
			tree = append(tree, Token.Token{Kind: "NUMBER", Value: code[start:i], Index: start})
			continue
		case c == '"':
			var value string
			i++ // Pula a primeira aspas
			for i < len(input) && input[i] != '"' {
				value += string(input[i])
				i++
			}

			tree = append(tree, Token.Token{Kind: "String", Value: value, Index: i})
		case cStr == "+": // Operadores
			tree = append(tree, Token.Token{Kind: "plusOp", Value: cStr, Index: i})
		case cStr == "-":
			tree = append(tree, Token.Token{Kind: "minusOp", Value: cStr, Index: i})
		case cStr == "*":
			tree = append(tree, Token.Token{Kind: "timesOp", Value: cStr, Index: i})
		case cStr == "/":
			tree = append(tree, Token.Token{Kind: "divOp", Value: cStr, Index: i})
		case cStr == "=":
			tree = append(tree, Token.Token{Kind: "assignOp", Value: cStr, Index: i})
		case cStr == ";":
			tree = append(tree, Token.Token{Kind: "semicolon", Value: cStr, Index: i})
		case cStr == "(":
			tree = append(tree, Token.Token{Kind: "openParen", Value: cStr, Index: i}) // Parêntese esquerdo
		case cStr == ")":
			tree = append(tree, Token.Token{Kind: "closeParen", Value: cStr, Index: i}) // Parêntese direito

		case unicode.IsLetter(c): // Identificadores (ex: var, nome de variáveis)
			start := i
			for i < len(code) && (unicode.IsLetter(rune(code[i])) || unicode.IsDigit(rune(code[i]))) {
				i++
			}
			tree = append(tree, Token.Token{Kind: "Identifier", Value: code[start:i], Index: start})
			continue
		}
		i++
	}
	tree = append(tree, Token.Token{Kind: "EOF", Value: "", Index: 0})
	return tree
}
