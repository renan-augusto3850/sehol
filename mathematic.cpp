#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>

using namespace std;

// Função para dividir a expressão em tokens
vector<string> dividirExpressao(const string &expressao) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(expressao);
    
    while (tokenStream >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

// Função para avaliar a expressão
double avaliarExpressao(const vector<string> &tokens) {
    if (tokens.empty()) return 0;

    double resultado = stod(tokens[0]);
    
    for (size_t i = 1; i < tokens.size(); i += 2) {
        string operador = tokens[i];
        double valor = stod(tokens[i + 1]);
        
        if(operador == "*"){
            resultado *= valor;
        } else if(operador == "/"){
            resultado /= valor;
        } else if (operador == "+") {
            resultado += valor;
        } else if (operador == "-") {
            resultado -= valor;
        } else if(operador == "~"){
            resultado = sqrt(valor);
        }else{
            throw invalid_argument("Operador desconhecido: " + operador);
        }
    }
    
    return resultado;
}

int main() {
    string expressao;
    getline(cin, expressao);
    cout << "Expressão: " << expressao << endl;
    
    // Dividir a expressão em tokens
    vector<string> tokens = dividirExpressao(expressao);
    
    // Avaliar a expressão
    try {
        double resultado = avaliarExpressao(tokens);
        cout << "Resultado: " << resultado << endl;
    } catch (const exception &e) {
        cerr << "Erro ao avaliar a expressão: " << e.what() << endl;
    }
    
    return 0;
}