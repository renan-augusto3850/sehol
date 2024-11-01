#include <iostream>
#include <stdio.h>
#include "../lexer.hpp"
using namespace std;

void print_token(Token token) {
    printf("Token { value = \"%s\", kind = %s }\n", token.value.c_str(), tokenkind_readable(token.kind).c_str());
}

int main() {
    Lexer a("fn a(a: string): string {}");
    Token t;

    while(t.kind != TOK_EOF) {
        t = next_token(&a);
        print_token(t);
    }
    
    return 0;
}