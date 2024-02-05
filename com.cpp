#include <iostream>
#include <vector>
#include <cctype>

// Definição dos tipos de token
enum TokenType {
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    FUNCTION,
    PARAM,
    OBJ,
    MATHFUNCTIONS,
    END
};

// Estrutura para armazenar informações sobre um token
struct Token {
    TokenType type;
    std::string value;
};

// Classe para representar um nó da árvore sintática
class TreeNode {
public:
    TokenType type;
    std::string value;
    std::vector<TreeNode*> children;

    TreeNode(TokenType t, const std::string& val) : type(t), value(val) {}

    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child;
        }
    }
};
TreeNode* parseExpression(std::vector<Token>& tokens, size_t& pos);
void generateIntermediateCode(TreeNode* node, std::vector<std::string>& intermediateCode);
void displayTree(TreeNode* node, int indent);
// Função para realizar a análise léxica
std::vector<Token> lexer(const std::string& input) {
    std::vector<Token> tokens;
    size_t pos = 0;

    while (pos < input.size()) {
        char currentChar = input[pos];

        // Ignora espaços em branco
        if (std::isspace(currentChar)) {
            pos++;
            continue;
        }

        // Verifica o tipo de token
        if (std::isdigit(currentChar)) {
            // Token do tipo INTEGER
            std::string value;
            while (std::isdigit(input[pos])) {
                value += input[pos];
                pos++;
            }
            tokens.push_back({INTEGER, value});
        } else if (currentChar == '+') {
            // Token do tipo PLUS
            tokens.push_back({PLUS, "+"});
            pos++;
        } else if (currentChar == '-') {
            // Token do tipo MINUS
            tokens.push_back({MINUS, "-"});
            pos++;
        } else if (currentChar == '*') {
            // Token do tipo MULTIPLY
            tokens.push_back({MULTIPLY, "*"});
            pos++;
        } else if (currentChar == '/') {
            // Token do tipo DIVIDE
            tokens.push_back({DIVIDE, "/"});
            pos++;
        } else if (currentChar == '(') {
            // Token do tipo LPAREN
            tokens.push_back({LPAREN, "("});
            pos++;
        } else if (currentChar == ')') {
            // Token do tipo RPAREN
            tokens.push_back({RPAREN, ")"});
            pos++;
        } else {
            // Caractere desconhecido
            std::cerr << "Erro: Caractere desconhecido - '" << currentChar << "'" << std::endl;
            break;
        }
    }

    // Adiciona um token de fim (END) para indicar o final da expressão
    tokens.push_back({END, ""});

    return tokens;
}
TreeNode* parseExpression(std::vector<Token>& tokens, size_t& pos);
TreeNode* parseTerm(std::vector<Token>& tokens, size_t& pos);
TreeNode* parsePrimary(std::vector<Token>& tokens, size_t& pos);
// Função auxiliar para criar um nó da árvore
TreeNode* createNode(TokenType type, const std::string& value) {
    return new TreeNode(type, value);
}

// Função auxiliar para liberar a memória da árvore sintática
void deleteTree(TreeNode* root) {
    delete root;
}

// Implementação da análise semântica
// Implementação da análise semântica
bool performSemanticAnalysis(TreeNode* node) {
    if (node == nullptr) {
        std::cerr << "Erro Semântico: Nó nulo encontrado durante a análise semântica." << std::endl;
        return false;
    }

    // Verifica se há um operador sem dois operandos
    if ((node->type == PLUS || node->type == MINUS || node->type == MULTIPLY || node->type == DIVIDE) && node->children.size() < 2) {
        std::cerr << "Erro Semântico: Operador '" << node->value << "' sem dois operandos." << std::endl;
        return false;
    }

    // Aplica recursivamente a análise semântica aos filhos
    for (const auto& child : node->children) {
        if (!performSemanticAnalysis(child)) {
            return false;  // Se um filho falhar, a análise semântica falha
        }
    }

    // Verifica se há multiplicação com operando não inteiro
    if (node->type == MULTIPLY) {
        if (node->children.size() == 2 && (node->children[0]->type != INTEGER || node->children[1]->type != INTEGER)) {
            std::cerr << "Erro Semântico: Multiplicação com operando não inteiro. Tipo: " << node->type << ", Valor: " << node->value << std::endl;
            return false;
        }
    }

    return true;  // Se todas as verificações passarem, a análise semântica é bem-sucedida
}


TreeNode* parseExpression(std::vector<Token>& tokens, size_t& pos) {
    TreeNode* left = parseTerm(tokens, pos);

    while (pos < tokens.size() && (tokens[pos].type == PLUS || tokens[pos].type == MINUS)) {
        TokenType opType = tokens[pos++].type;
        TreeNode* right = parseTerm(tokens, pos);

        // Cria o nó de operação apenas se o operador for adição ou subtração
        if (opType == PLUS || opType == MINUS) {
            TreeNode* opNode = createNode(opType, tokens[pos - 1].value);
            opNode->children.push_back(left);
            opNode->children.push_back(right);
            left = opNode;
        } else {
            std::cerr << "Erro Semantico: Operador inesperado - '" << tokens[pos - 1].value << "'" << std::endl;
            return nullptr;
        }
    }

    return left;
}


TreeNode* parseTerm(std::vector<Token>& tokens, size_t& pos) {
    TreeNode* left = parsePrimary(tokens, pos);

    while (pos < tokens.size() && (tokens[pos].type == MULTIPLY || tokens[pos].type == DIVIDE)) {
        TokenType op = tokens[pos++].type;

        if (op == MULTIPLY || op == DIVIDE) {
            TreeNode* right = parsePrimary(tokens, pos);

            // Cria o nó de operação somente se o operador for multiplicação ou divisão
            TreeNode* node = createNode(op, "");  // Use uma string vazia para o valor do operador
            node->children.push_back(left);
            node->children.push_back(right);
            left = node;
        } else {
            std::cerr << "Erro Semântico: Operador inesperado - '" << tokens[pos - 1].value << "'" << std::endl;
            return nullptr;
        }
    }

    return left;
}



TreeNode* parsePrimary(std::vector<Token>& tokens, size_t& pos) {
    if (pos < tokens.size()) {
        const Token& currentToken = tokens[pos];
        std::cout << "Token: " << currentToken.value << " Tipo: " << currentToken.type << std::endl;

        if (currentToken.type == INTEGER) {
            // Se for um número, cria um nó de número
            TreeNode* node = createNode(INTEGER, currentToken.value);
            pos++;  // Avança para o próximo token
            return node;
        } else if (currentToken.type == LPAREN) {
            // Se for um parêntese de abertura, avança e analisa a expressão dentro dos parênteses
            pos++;
            TreeNode* expression = parseExpression(tokens, pos);

            // Verifica se há um parêntese de fechamento correspondente
            if (currentToken.type == RPAREN) {
                pos++;  // Avança para o próximo token (parêntese de fechamento)
                return expression;
            } else {
                std::cerr << "Erro Semântico: Parêntese de fechamento ausente." << std::endl;
                return nullptr;  // Retorna nullptr em caso de erro
            }
        } else {
            std::cerr << "Erro Semântico: Token inesperado encontrado." << std::endl;
            return nullptr;  // Retorna nullptr em caso de erro
        }
    } else {
        std::cerr << "Erro Semântico: Fim inesperado dos tokens." << std::endl;
        return nullptr;  // Retorna nullptr em caso de erro
    }
}


void generateIntermediateCode(TreeNode* node, std::vector<std::string>& intermediateCode) {
    if (node != nullptr) {
        for (const auto& child : node->children) {
            generateIntermediateCode(child, intermediateCode);
        }

        if (!node->value.empty()) {
            intermediateCode.push_back(node->value);
        }
    }
}


void displayTree(TreeNode* node, int indent) {
    if (node != nullptr) {
        for (int i = 0; i < indent; ++i) {
            std::cout << "  ";  // Dois espaços por nível de indentação
        }
        std::cout << node->type << " " << node->value << std::endl;

        for (const auto& child : node->children) {
            displayTree(child, indent + 1);
        }
    }
}

// ...
int main() {
    std::string input = "6+5*(7*3)";
    std::vector<Token> tokens = lexer(input);

    size_t pos = 0;
    TreeNode* root = parseExpression(tokens, pos);

    // Exibe a árvore sintática antes da análise semântica
    if (root != nullptr) {
        std::cout << "Árvore Sintática antes da Análise Semântica:" << std::endl;
        displayTree(root, 0);
    } else {
        std::cerr << "Erro: Falha na construção da árvore sintática." << std::endl;
        return 1;
    }

    // Realiza a análise semântica
    if (performSemanticAnalysis(root)) {
       std::cout << "Analise sematica feita com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro Semantico: A análise semântica falhou." << std::endl;
    }

    // Liberar a memória da árvore sintática
    deleteTree(root);

    return 0;
}
