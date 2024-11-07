#include <iostream>
#include <memory>
#include <fstream>
#include "../parser.hpp"

std::string nodeTypeToString(NodeType type) {
    switch (type) {
        case NodeType::INTEGER_LITERAL: return "INTEGER_LITERAL";
        case NodeType::STRING_LITERAL: return "STRING_LITERAL";
        case NodeType::BINARY_OP: return "BINARY_OP";
        case NodeType::VARIABLE: return "VARIABLE";
        case NodeType::VAR_STATEMENT: return "VAR_STATEMENT";
        case NodeType::IF_STATEMENT: return "IF_STATEMENT";
        case NodeType::WHILE_STATEMENT: return "WHILE_STATEMENT";
        case NodeType::FOR_STATEMENT: return "FOR_STATEMENT";
        case NodeType::BLOCK: return "BLOCK";
        default: return "UNKNOWN";
    }
}

// Função para imprimir um nó AST.
void printASTNode(const std::shared_ptr<ASTNode>& node, int indent = 0) {
    if (!node) return;

    // Indentar com espaços para cada nível de profundidade.
    for (int i = 0; i < indent; ++i) std::cout << "  ";
    std::cout << nodeTypeToString(node->type);

    // Se o nó for um tipo de expressão ou declaração específica, exibe detalhes adicionais.
    if (auto intLit = std::dynamic_pointer_cast<IntegerLiteral>(node)) {
        std::cout << " (value: " << intLit->value << ")";
    } else if (auto strLit = std::dynamic_pointer_cast<StringLiteral>(node)) {
        std::cout << " (value: \"" << strLit->value << "\")";
    } else if (auto var = std::dynamic_pointer_cast<Variable>(node)) {
        std::cout << " (name: " << var->name << ")";
    } else if (auto binOp = std::dynamic_pointer_cast<BinaryOp>(node)) {
        std::cout << " (op: " << binOp->op << ")";
        std::cout << "\n";
        printASTNode(std::make_shared<Expression>(binOp->left), indent + 1);
        printASTNode(std::make_shared<Expression>(binOp->right), indent + 1);
        return;
    } else if (auto varStmt = std::dynamic_pointer_cast<VarStatement>(node)) {
        std::cout << " (variable_name: " << varStmt->variable_name << ")";
        std::cout << "\n";
        printASTNode(std::make_shared<Expression>(varStmt->value), indent + 1);
        return;
    } else if (auto ifStmt = std::dynamic_pointer_cast<IfStatement>(node)) {
        std::cout << "\n";
        std::cout << "Condition:\n";
        printASTNode(std::make_shared<Expression>(ifStmt->condition), indent + 1);
        std::cout << "Then:\n";
        printASTNode(std::make_shared<Statement>(ifStmt->then_branch), indent + 1);
        std::cout << "Else:\n";
        printASTNode(std::make_shared<Statement>(ifStmt->else_branch), indent + 1);
        return;
    } else if (auto block = std::dynamic_pointer_cast<Block>(node)) {
        std::cout << " {BLOCK}";
        std::cout << "\n";
        for (const auto& stmt : block->statements) {
            printASTNode(std::make_shared<Statement>(stmt), indent + 1);
        }
        return;
    }

    std::cout << "\n";
}

// Função principal para iniciar a impressão da árvore.
void printAST(const Block& root) {
    printASTNode(std::make_shared<Block>(root), 0);
}

int main(int argc, char* argv[]) {
    // Verifica se o arquivo de entrada foi fornecido
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <caminho_do_arquivo>" << std::endl;
        return 1;
    }

    // Abre o arquivo de entrada
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << argv[1] << std::endl;
        return 1;
    }

    // Lê o conteúdo do arquivo em uma única string
    std::string line, text;
    while (std::getline(file, line)) {
        text += line + "\n";
    }
    file.close();

    // Cria o Lexer e o Parser
    Lexer lexer(text);
    Parser parser(&lexer);

    try {
        // Gera a AST a partir do código-fonte
        auto root = parser.parse_program();

        // Imprime a AST
        printAST(root); // Certifique-se de que `printAST` esteja corretamente implementada e importada
    }
    catch (const std::exception& e) {
        std::cerr << "Erro durante o parsing: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}