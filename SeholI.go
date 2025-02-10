package main

import (
	"sehol/lexer"
	"sehol/parser"
	"sehol/runtime"
)

func main() {
	code := `printLog("Helloworld", "guy");`
	tokens := lexer.Lexer(code)

	ast := parser.Parser(tokens)

	runtime.Run(ast)
}
