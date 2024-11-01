#ifndef EXPRESSAO_H
#define EXPRESSAO_H

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "variables.h"

using namespace std;

// Função para definir a precedência dos operadores
inline int precedencia(const string &op) {
    if (op == "==" || op == "!=" || op == ">" || op == "<") return 0;  // Comparação tem menor precedência
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "~") return 3;  // Operador unário
    return -1;
}

inline double aplicarOperador(const string &op, const string &a, const string &b = "") {
    // Operadores de comparação para strings
    if (op == "==") return a == b ? 1 : 0;
    if (op == "!=") return a != b ? 1 : 0;
    
    throw invalid_argument("Operador desconhecido para strings: " + op);
}

// Função para aplicar o operador
inline double aplicarOperador(const string &op, double a, double b = 0) {
    // Operadores aritméticos
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return a / b;
    if (op == "~") return sqrt(a);  // Operador unário
    
    // Operadores de comparação
    if (op == ">") return a > b ? 0 : 1;
    if (op == "<") return a < b ? 0 : 1;
    if (op == "==") return a == b ? 0 : 1;
    if (op == "!=") return a != b ? 0 : 1;

    throw invalid_argument("Operador desconhecido: " + op);
}

// Função para converter a expressão para Notação Polonesa Reversa (RPN)
inline vector<string> paraRPN(const vector<string> &tokens) {
    stack<string> operadores;
    vector<string> saida;

    for (const auto &token : tokens) {
        if (isdigit(token[0])) {
            saida.push_back(token);  // Número vai direto para a saída
        } else if (token == "(") {
            operadores.push(token);  // Abre parêntese vai para a pilha
        } else if (token == ")") {
            // Fecha parêntese - desempilha até encontrar o abre parêntese
            while (!operadores.empty() && operadores.top() != "(") {
                saida.push_back(operadores.top());
                operadores.pop();
            }
            operadores.pop();  // Remove o abre parêntese da pilha
        } else {
            cout << "["<< token << "]";
            // Operador - desempilha até encontrar um operador de menor precedência
            while (!operadores.empty() && precedencia(operadores.top()) >= precedencia(token)) {
                saida.push_back(operadores.top());
                operadores.pop();
            }
            operadores.push(token);  // Empilha o operador atual
        }
    }

    // Desempilha os operadores restantes
    while (!operadores.empty()) {
        saida.push_back(operadores.top());
        operadores.pop();
    }

    return saida;
}

// Função para avaliar a expressão em Notação Polonesa Reversa (RPN)
inline double avaliarRPN(const vector<string> &tokens) {
    stack<string> pilha;

    for (const auto &token : tokens) {
        try {
            // Verifica se o token é uma string entre aspas
            if (token.front() == '"' && token.back() == '"') {
                pilha.push(token.substr(1, token.size() - 2)); // Remove aspas
            } 
            else {
                // Tenta converter o token para número
                double num = stod(token);
                pilha.push(to_string(num)); // Armazena como string para consistência
            }
        } catch (const invalid_argument&) {
            // Operador unário
            if (token == "~") {
                if (pilha.empty()) throw runtime_error("Pilha vazia para operador unário");
                double a = stod(pilha.top()); pilha.pop();
                pilha.push(to_string(aplicarOperador(token, a)));
            }
            // Operadores binários
            else {
                if (pilha.size() < 2) throw runtime_error("Pilha insuficiente para operador binário");
                string b = pilha.top(); pilha.pop();
                string a = pilha.top(); pilha.pop();

                // Verifica se os operandos são numéricos ou strings
                try {
                    double a_num = stod(a);
                    double b_num = stod(b);
                    pilha.push(to_string(aplicarOperador(token, a_num, b_num)));
                } catch (const invalid_argument&) {
                    // Comparação de strings
                    pilha.push(to_string(aplicarOperador(token, a, b)));
                }
            }
        }
    }

    if (pilha.size() != 1) throw runtime_error("Expressão RPN inválida");
    return stod(pilha.top());
}

// Função para dividir a expressão em tokens, separando corretamente operadores e parênteses
inline vector<string> dividirExpressao(const string &expressao, unordered_map<string, variables> &variableTable) {
    vector<string> tokens;
    string token;

    for (size_t i = 0; i < expressao.size(); ++i) {
        char c = expressao[i];

        if (isspace(c)) continue;  // Ignorar espaços em branco

        if (isdigit(c) || c == '.' || isalpha(c)) {
            token += c;  // Constrói números, floats e letras (para variáveis)
        } else {
            if (!token.empty()) {
                // Verifica se o token é uma variável no mapa 'variableTable'
                if (variableTable.find(token) != variableTable.end()) {
                    // Substitui o token pelo valor da variável
                    tokens.push_back(variableTable[token].value);  // Supondo que 'value' seja um std::string
                } else {
                    tokens.push_back(token);  // Se não for uma variável, adiciona o token diretamente
                }
                token.clear();
            }

            // Ignorar os parênteses
            if (c == '(' || c == ')') {
                continue;  // Apenas pula os parênteses sem adicioná-los
            }

            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '!' || c == '>' || c == '<') {
                // Tratar operadores (incluindo ==, !=, >, <)
                if (i + 1 < expressao.size() && expressao[i + 1] == '=') {
                    tokens.push_back(string(1, c) + '=');  // Operadores como == ou !=
                    i++;  // Avançar para o próximo caractere
                } else {
                    tokens.push_back(string(1, c));  // Operadores simples como +, -, *, etc.
                }
            } else if (c == '"') {
                // Tratamento para strings com aspas (exemplo: "Renan")
                size_t pos = expressao.find('"', i + 1);
                if (pos != string::npos) {
                    tokens.push_back(expressao.substr(i, pos - i + 1));  // Adiciona a string completa
                    i = pos;  // Avança o índice até o fim da string
                }
            }
        }
    }

    if (!token.empty()) {
        // Verifica se o último token é uma variável
        if (variableTable.find(token) != variableTable.end()) {
            tokens.push_back(variableTable[token].value);  // Substitui o valor da variável
        } else {
            tokens.push_back(token);  // Adiciona o último token
        }
    }

    return tokens;
}

#endif  // EXPRESSAO_H