package main

import (
	"fmt"
	"os"
	"sehol/lexer"
	"sehol/parser"
	"sehol/runtime"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("SeholError: You miss to specify the file. Usage: SeholI <filename>.sehol")
		return
	}
	file := os.Args[1]
	dat, err := os.ReadFile(file)
	check(err)
	tokens := lexer.Lexer(string(dat))

	ast := parser.Parser(tokens)
	runtime.Run(ast, 0)
}
