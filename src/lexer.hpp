#include <string>
#include <cctype>

using namespace std;

typedef struct Lexer {
    string input;
    size_t index;

    Lexer(string input) : input(input), index(0) {}
} Lexer;

enum TokenKind {
    COMMA,
    INTEGER,
    IDENTIFIER,
    STRING,
    EQ,
    GT,
    LT,
    GE,
    LE,
    NE,
    DIV,
    MUL,
    SUB,
    ADD,
    NOT,
    SQRT,
    ASSIGN,
    OPENPAREN,
    CLOSEPAREN,
    OPENBRACE,
    CLOSEBRACE,
    OPENBRACK,
    CLOSEBRACK,
    SEMI,
    TOK_EOF,
};

string tokenkind_readable(TokenKind t) {
    switch (t) {
        case COMMA: return "COMMA";
        case INTEGER: return "INTEGER";
        case IDENTIFIER: return "IDENTIFIER";
        case STRING: return "STRING";
        case EQ: return "EQ";
        case GT: return "GT";
        case LT: return "LT";
        case GE: return "GE";
        case LE: return "LE";
        case NE: return "NE";
        case DIV: return "DIV";
        case MUL: return "MUL";
        case SUB: return "SUB";
        case ADD: return "ADD";
        case NOT: return "NOT";
        case SQRT: return "SQRT";
        case ASSIGN: return "ASSIGN";
        case OPENPAREN: return "OPENPAREN";
        case CLOSEPAREN: return "CLOSEPAREN";
        case OPENBRACE: return "OPENBRACE";
        case CLOSEBRACE: return "CLOSEBRACE";
        case OPENBRACK: return "OPENBRACK";
        case CLOSEBRACK: return "CLOSEBRACK";
        case SEMI: return "SEMI";
        case TOK_EOF: return "TOK_EOF";
        // Add other cases as necessary
        default: return "UNKNOWN TOKEN";
    }
}

typedef struct Token {
    string value;
    TokenKind kind;
} Token;

Token make_token(TokenKind kind, const string &value) {
    return { value, kind };
}

void skip_whitespace(Lexer *target) {
    while (target->index < target->input.size() && isspace(target->input[target->index])) {
        target->index++;
    }
}

Token next_token(Lexer* target) {
    skip_whitespace(target);

    if (target->index >= target->input.size()) {
        return make_token(TOK_EOF, "");
    }

    char current = target->input[target->index];

    switch (current) {
        case ',': target->index++; return make_token(COMMA, ",");
        case '(': target->index++; return make_token(OPENPAREN, "(");
        case ')': target->index++; return make_token(CLOSEPAREN, ")");
        case '{': target->index++; return make_token(OPENBRACE, "{");
        case '}': target->index++; return make_token(CLOSEBRACE, "}");
        case '[': target->index++; return make_token(OPENBRACK, "[");
        case ']': target->index++; return make_token(CLOSEBRACK, "]");
        case ';': target->index++; return make_token(SEMI, ";");
        case '+': target->index++; return make_token(ADD, "+");
        case '-': target->index++; return make_token(SUB, "-");
        case '*': target->index++; return make_token(MUL, "*");
        case '/': target->index++; return make_token(DIV, "/");
        
        case '=':
            target->index++;
            if (target->input[target->index] == '=') {
                target->index++;
                return make_token(EQ, "==");
            }
            return make_token(ASSIGN, "=");

        case '>':
            target->index++;
            if (target->input[target->index] == '=') {
                target->index++;
                return make_token(GE, ">=");
            }
            return make_token(GT, ">");

        case '<':
            target->index++;
            if (target->input[target->index] == '=') {
                target->index++;
                return make_token(LE, "<=");
            }
            return make_token(LT, "<");

        case '!':
            target->index++;
            if (target->input[target->index] == '=') {
                target->index++;
                return make_token(NE, "!=");
            }
            return make_token(NOT, "!");
        
        case '"': {
            target->index++;
            size_t start = target->index;
            while (target->index < target->input.size() && target->input[target->index] != '"') {
                target->index++;
            }
            string str = target->input.substr(start, target->index - start);
            target->index++;
            return make_token(STRING, str);
        }
    }

    if (isdigit(current)) {
        size_t start = target->index;
        while (isdigit(target->input[target->index])) {
            target->index++;
        }
        return make_token(INTEGER, target->input.substr(start, target->index - start));
    }

    if (isalpha(current)) {
        size_t start = target->index;
        while (isalnum(target->input[target->index])) {
            target->index++;
        }
        string word = target->input.substr(start, target->index - start);
        return make_token(IDENTIFIER, word);
    }

    return make_token(TOK_EOF, "");
}
