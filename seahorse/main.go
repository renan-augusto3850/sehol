package main

import (
	"fmt"
	"seahorse/lexer"
	"seahorse/parser"
	"seahorse/transpiler"
)

func main() {
	input := "var a = 10\nprint(a"
	tokens := lexer.Lexer(input, "file.se")
	p := parser.NewParser(tokens)
	ast := p.Parse()
	if ast == nil {
		return
	}
	i := transpiler.NewInstance(ast)
	fmt.Println(i.Transpile().Text)
}

// func printAST(node *parser.Node, indent int) {
// 	if node == nil { return }
// 	for i := 0; i < indent; i++ {
// 		fmt.Print("  ")
// 	}
// 	switch node.Kind {
// 	case parser.NODE_BINOP: fmt.Printf("Binary Operation: %s\n", node.Value)
// 	case parser.NODE_UNOP: fmt.Printf("Unary Operation: %s\n", node.Value)
// 	case parser.NODE_ID: fmt.Printf("Identifier: %s\n", node.Value)
// 	case parser.NODE_FUNCALL: fmt.Printf("Function Call: %s\n", node.Value)
// 	case parser.NODE_NUMERIC: fmt.Printf("Numeric: %s\n", node.Value)
// 	case parser.NODE_STRING: fmt.Printf("String: \"%s\"\n", node.Value)
// 	case parser.NODE_VAR: fmt.Printf("Var: \"%s\"\n", node.Value)
// 	}

// 	printAST(node.Left, indent+1)
// 	printAST(node.Right, indent+1)
// 	printASTList(node.List, indent+1)
// }

// func printASTList(list []*parser.Node, ident int) {
// 	for i := 0; i < len(list); i++ {
// 		fmt.Printf("\t--- %d ---\n", i);
// 		printAST(list[i], ident);
// 	}
// }