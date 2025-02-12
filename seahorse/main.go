package main

import (
	"fmt"
	"os"
	"seahorse/lexer"
	"seahorse/parser"
	"seahorse/runtime"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	if len(os.Args) < 2 {
		fmt.Printf("Seahorse: Usage: %s FILENAME\n", os.Args[0])
		fmt.Println("Seahorse: Expected positional argument FILENAME.")
		os.Exit(1)
	}
	file := os.Args[1]
	dat, err := os.ReadFile(file)
	check(err)
	
	tokens := lexer.Lexer(string(dat))

	a := parser.Parser {
		Index: 0,
		Tree: tokens,
		CurrentToken: &tokens[0],
	}

	ast := parser.Parse(a)

	runtime.Run(ast, 0)
}
