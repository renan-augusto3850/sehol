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
			tree = append(tree, Token.Token{Kind: Token.TOKEN_NUMERIC, Value: code[start:i], Index: start})
			continue
		case c == '"':
			var value string
			i++ // Pula a primeira aspas
			for i < len(input) && input[i] != '"' {
				value += string(input[i])
				i++
			}

			tree = append(tree, Token.Token{Kind: Token.TOKEN_STRING, Value: value, Index: i})
		case cStr == "+": // Operadores
			tree = append(tree, Token.Token{Kind: Token.TOKEN_PLUS, Value: cStr, Index: i})
		case cStr == "-":
			tree = append(tree, Token.Token{Kind: Token.TOKEN_MINUS, Value: cStr, Index: i})
		case cStr == "*":
			tree = append(tree, Token.Token{Kind: Token.TOKEN_MUL, Value: cStr, Index: i})
		case cStr == "/":
			tree = append(tree, Token.Token{Kind: Token.TOKEN_DIV, Value: cStr, Index: i})
		case cStr == "=":
			tree = append(tree, Token.Token{Kind: Token.TOKEN_ASSIGN, Value: cStr, Index: i})
		case cStr == ";":
			tree = append(tree, Token.Token{Kind: Token.TOKEN_SEMI, Value: cStr, Index: i})
		case cStr == "(":
			tree = append(tree, Token.Token{Kind: Token.TOKEN_OPENPAREN, Value: cStr, Index: i}) // Parêntese esquerdo
		case cStr == ")":
			tree = append(tree, Token.Token{Kind: Token.TOKEN_CLOSEPAREN, Value: cStr, Index: i}) // Parêntese direito

		case unicode.IsLetter(c): // Identificadores (ex: var, nome de variáveis)
			start := i
			for i < len(code) && (unicode.IsLetter(rune(code[i])) || unicode.IsDigit(rune(code[i]))) {
				i++
			}
			tree = append(tree, Token.Token{Kind: Token.TOKEN_ID, Value: code[start:i], Index: start})
			continue
		}
		i++
	}
	tree = append(tree, Token.Token{Kind: Token.TOKEN_EOF, Value: "", Index: 0})
	return tree
}
