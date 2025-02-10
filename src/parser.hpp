#include <string>
#include <vector>
#include <stdexcept>
#include "lexer.hpp"

enum class NodeType {
    INTEGER_LITERAL,
    STRING_LITERAL,
    BINARY_OP,
    VARIABLE,
    VAR_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    BLOCK
};

struct ASTNode {
    NodeType type;
    ASTNode(NodeType type) : type(type) {}
};

struct Expression : ASTNode {
    Expression(NodeType type) : ASTNode(type) {}
};

struct IntegerLiteral : Expression {
    int value;
    IntegerLiteral(int value) : Expression(NodeType::INTEGER_LITERAL), value(value) {}
};

struct StringLiteral : Expression {
    std::string value;
    StringLiteral(const std::string &value) : Expression(NodeType::STRING_LITERAL), value(value) {}
};

struct Variable : Expression {
    std::string name;
    Variable(const std::string &name) : Expression(NodeType::VARIABLE), name(name) {}
};

struct BinaryOp : Expression {
    Expression left;
    std::string op;
    Expression right;
    BinaryOp(const Expression &left, const std::string &op, const Expression &right)
        : Expression(NodeType::BINARY_OP), left(left), op(op), right(right) {}
};

struct Statement : ASTNode {
    Statement(NodeType type) : ASTNode(type) {}
};

struct VarStatement : Statement {
    std::string variable_name;
    Expression value;
    VarStatement(const std::string &variable_name, const Expression &value)
        : Statement(NodeType::VAR_STATEMENT), variable_name(variable_name), value(value) {}
};

struct IfStatement : Statement {
    Expression condition;
    Statement then_branch;
    Statement else_branch;
    IfStatement(const Expression &condition, const Statement &then_branch, const Statement &else_branch)
        : Statement(NodeType::IF_STATEMENT), condition(condition), then_branch(then_branch), else_branch(else_branch) {}
};

struct WhileStatement : Statement {
    Expression condition;
    Statement block;
    WhileStatement(const Expression &condition, const Statement &block)
        : Statement(NodeType::WHILE_STATEMENT), condition(condition), block(block) {}
};

struct ForStatement : Statement {
    Variable name;
    Expression target;
    Statement block;
    ForStatement(const Variable &name, const Expression &target, const Statement &block)
        : Statement(NodeType::FOR_STATEMENT), name(name), target(target), block(block) {}
};

struct Block : Statement {
    std::vector<Statement> statements;
    Block() : Statement(NodeType::BLOCK) {}
};

struct Parser {
    Lexer lexer;
    Token current_token;

    Parser(Lexer* lexer) : lexer(*lexer) {
        current_token = next_token(&this->lexer);
    }

    void advance() {
        current_token = next_token(&lexer);
    }

    void expect(TokenKind kind) {
        if (current_token.kind != kind) {
            throw std::runtime_error("Expected token: " + tokenkind_readable(kind));
        }
        advance();
    }

    void expect_value(const std::string& value) {
        if (current_token.value != value) {
            throw std::runtime_error("Expected token with value: " + value);
        }
        advance();
    }

    Block parse_program() {
        Block block;
        while (current_token.kind != TOK_EOF) {
            block.statements.push_back(parse_statement());
        }
        return block;
    }

    Statement parse_statement() {
        const Token a = current_token;
        if (a.value == "if") {
            return parse_if_statement();
        } else if (a.value == "while") {
            return parse_while_statement();
        } else if (a.value == "for") {
            return parse_for_statement();
        } else if (a.value == "var") {
            return parse_var_statement();
        } else {
            throw std::runtime_error("Unexpected statement type");
        }
    }

    IfStatement parse_if_statement() {
        expect_value("if");
        auto condition = parse_expression();
        expect(OPENBRACE);
        auto then_branch = parse_block();
        expect(CLOSEBRACE);
        return IfStatement(condition, then_branch, Block());
    }

    WhileStatement parse_while_statement() {
        expect_value("while");
        auto condition = parse_expression();
        expect(OPENBRACE);
        auto body = parse_block();
        expect(CLOSEBRACE);
        return WhileStatement(condition, body);
    }

    ForStatement parse_for_statement() {
        expect_value("for");
        expect(IDENTIFIER);
        Variable variable(current_token.value);
        expect_value("in");
        auto iterable = parse_expression();
        expect(OPENBRACE);
        auto body = parse_block();
        expect(CLOSEBRACE);
        return ForStatement(variable, iterable, body);
    }

    VarStatement parse_var_statement() {
        expect_value("var");
        std::string variable_name = current_token.value;
        expect(IDENTIFIER);
        expect(ASSIGN);

        auto value = parse_expression();
        expect(SEMI);
        return VarStatement(variable_name, value);
    }

    Block parse_block() {
        Block block;
        while (current_token.kind != CLOSEBRACE && current_token.kind != TOK_EOF) {
            block.statements.push_back(parse_statement());
        }
        return block;
    }

    Expression parse_expression() {
        auto left = parse_primary();
        while (current_token.kind == EQ || current_token.kind == LT ||
               current_token.kind == GT || current_token.kind == ADD ||
               current_token.kind == SUB || current_token.kind == INTEGER) {
            std::string op = current_token.value;
            advance();
            auto right = parse_primary();
            left = BinaryOp(left, op, right);
        }
        return left;
    }

    Expression parse_primary() {
        switch (current_token.kind) {
            case INTEGER:
                return IntegerLiteral(std::stoi(current_token.value));
            case STRING:
                return StringLiteral(current_token.value);
            case IDENTIFIER:
                return Variable(current_token.value);
        }
    }
};
