#include "mathematics.h"

int main() {
    string expressao;
    cout << "Digite a expressão: ";
    getline(cin, expressao);

    // Dividir a expressão em tokens
    vector<string> tokens = dividirExpressao(expressao);

    // Converter para Notação Polonesa Reversa (RPN)
    vector<string> rpn = paraRPN(tokens);

    // Avaliar a expressão em RPN
    try {
        double resultado = avaliarRPN(rpn);
        cout << "Resultado: " << resultado << endl;
    } catch (const exception &e) {
        cerr << "Erro ao avaliar a expressão: " << e.what() << endl;
    }

    return 0;
}
