#include <iostream>
#include <fstream>
#include <string>
#include "../parser.hpp"

void print_ast(const ASTNode& node, const std::string& prefix = "") {
    std::cout << prefix << "Node Type: ";

    switch (node.type) {
        case NodeType::INTEGER_LITERAL:
            std::cout << "Integer Literal, Value: " << static_cast<const IntegerLiteral&>(node).value << std::endl;
            break;
        case NodeType::STRING_LITERAL:
            std::cout << "String Literal, Value: " << static_cast<const StringLiteral&>(node).value << std::endl;
            break;
        case NodeType::VARIABLE:
            std::cout << "Variable, Name: " << static_cast<const Variable&>(node).name << std::endl;
            break;
        case NodeType::VAR_STATEMENT: {
            const auto& assignment = static_cast<const VarStatement&>(node);
            std::cout << "Assignment" << std::endl;
            std::cout << prefix << "  Variable Name: " << assignment.variable_name << std::endl;
            print_ast(assignment.value, prefix + "  "); // Print the assigned value
            break;
        }
        case NodeType::IF_STATEMENT: {
            const auto& if_statement = static_cast<const IfStatement&>(node);
            std::cout << "If Statement" << std::endl;
            print_ast(if_statement.condition, prefix + "  ");
            std::cout << prefix << "  Then Branch:" << std::endl;
            print_ast(if_statement.then_branch, prefix + "    ");
            break;
        }
        case NodeType::WHILE_STATEMENT: {
            const auto& while_statement = static_cast<const WhileStatement&>(node);
            std::cout << "While Statement" << std::endl;
            print_ast(while_statement.condition, prefix + "  ");
            std::cout << prefix << "  Body:" << std::endl;
            print_ast(while_statement.block, prefix + "    ");
            break;
        }
        case NodeType::FOR_STATEMENT: {
            const auto& for_statement = static_cast<const ForStatement&>(node);
            std::cout << "For Statement" << std::endl;
            std::cout << prefix << "  Variable Name: " << for_statement.name.name << std::endl;
            std::cout << prefix << "  Target:" << std::endl;
            print_ast(for_statement.target, prefix + "    ");
            std::cout << prefix << "  Body:" << std::endl;
            print_ast(for_statement.block, prefix + "    ");
            break;
        }
        case NodeType::BLOCK: {
            const auto& block = static_cast<const Block&>(node);
            std::cout << "Block:" << std::endl;
            for (const auto& stmt : block.statements) {
                print_ast(stmt, prefix + "  ");
            }
            break;
        }
        default:
            std::cout << "Unknown" << std::endl;
            break;
    }
}

int main(int argc, char* argv[]) {
    Lexer l(argv[1]);
    Parser p(&l);
    try {
        auto r = p.parse_program();
        print_ast(r, "");
    } catch (const std::runtime_error e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}