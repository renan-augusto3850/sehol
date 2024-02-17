#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

struct AstChildren{
    string name;
    int startLine;
    int finalLine;
    string type;
    vector<string> params;
    AstChildren* firstChildren;
    AstChildren* nextChildren;
    AstChildren() : nextChildren(nullptr) {}
};

struct AstNode{
    string name;
    int startLine;
    int finalLine;
    string type;
    vector<string> params;
    AstChildren* firstChildren;
    AstNode* nextNode;
    AstNode() : nextNode(nullptr) {}
};


AstNode* parser(string expression, int startLine, int finalLine, int childrenLine){
    AstNode* node = new AstNode();
    AstChildren* newChildren = new AstChildren();
    AstChildren* firstChildren;
    AstChildren* currentChild;
    if(expression.find("drawWindow") != string::npos){
        int indexName1 = expression.find("\"");
        int indexName2 = expression.find("\"", indexName1 + 1);
        string resultName = expression.substr(indexName1 + 1, indexName2 - indexName1 - 1);
        int indexCommaWidth1 = expression.find(",");
        int indexCommaWidth2 = expression.find(",", indexCommaWidth1 + 1);
        string resultCommaWidth = expression.substr(indexCommaWidth1 + 1, indexCommaWidth2 - indexCommaWidth1 - 1);
        string resultCommaHeigth = expression.substr(indexCommaWidth2 + 2, expression.find(")") - indexCommaWidth2 - 2);
        node->name = "drawWindow";
        node->startLine = startLine;
        node->finalLine = finalLine;
        node->type = "drawWindowElement";
        node->params = {resultName + "," + resultCommaWidth + "," + resultCommaHeigth};
        if(expression.find("background-color") != string::npos && expression.find("background-color") > expression.find("{")){
            AstChildren* children = new AstChildren();
            children->name = "background-color";
            children->startLine = childrenLine;
            children->finalLine = childrenLine;
            children->type = "backgroundChange";
            children->params = {expression.substr(expression.find("background-color") + 17)};
            if (firstChildren) {
                firstChildren = children;
                node->firstChildren = children;
                cout << "Primeira criança definida" << endl;
            } else {
                AstChildren* currentChild = firstChildren;
                while (!currentChild->nextChildren->name.empty()) {
                    currentChild = currentChild->nextChildren;
                }
                node->firstChildren->nextChildren = children;
                cout << "A Proxima criança foi definida" << endl;
            }
        }
    }
    if(expression.find("drawText") != string::npos){
        int indexName1 = expression.find("\"");
        int indexName2 = expression.find("\"", indexName1 + 1);
        string resultText = expression.substr(indexName1 + 1, indexName2 - indexName1 - 1);
        node->name = "drawText";
        node->startLine = startLine;
        node->finalLine = finalLine;
        node->type = "drawTextElement";
        node->params = {resultText};
        if(expression.find("color") != string::npos && expression.find("color") > expression.find("{")){
            AstChildren* children = new AstChildren();
            children->name = "color";
            children->startLine = childrenLine;
            children->finalLine = childrenLine;
            children->type = "colorChange";
            children->params = {expression.substr(expression.find("color") + 17)};
            if (firstChildren) {
                firstChildren = children;
                node->firstChildren = children;
                cout << "Primeira criança definida" << endl;
            } else {
                AstChildren* currentChild = firstChildren;
                while (currentChild) {
                    currentChild = currentChild->nextChildren;
                }
                node->firstChildren->nextChildren = children;
                cout << "A Proxima criança foi definida" << endl;
            }
        }
    }
    if(expression.find("drawButton") != string::npos){
        int indexName1 = expression.find("\"");
        int indexName2 = expression.find("\"", indexName1 + 1);
        string resultButton = expression.substr(indexName1 + 1, indexName2 - indexName1 - 1);
        node->name = "drawButton";
        node->startLine = startLine;
        node->finalLine = finalLine;
        node->type = "drawButtonElement";
        node->params = {resultButton};
        if(expression.find("width") != string::npos && expression.find("width") > expression.find("{")){
            AstChildren* children = new AstChildren();
            children->name = "width";
            children->startLine = childrenLine;
            children->finalLine = childrenLine;
            children->type = "widthValue";
            children->params = {expression.substr(expression.find("width") + 9)};
            if (firstChildren) {
                firstChildren = children;
                node->firstChildren = children;
                cout << "Primeira criança definida" << endl;
            } else {
                AstChildren* currentChild = firstChildren;
                while (currentChild) {
                    currentChild = currentChild->nextChildren;
                }
                currentChild->nextChildren = children;
                node->firstChildren->nextChildren = currentChild;
                cout << "A Proxima criança foi definida" << endl;
            }
        }
        if(expression.find("height") != string::npos && expression.find("height") > expression.find("{")){
            AstChildren* children = new AstChildren();
            children->name = "height";
            children->startLine = childrenLine;
            children->finalLine = childrenLine;
            children->type = "heightValue";
            children->params = {expression.substr(expression.find("height") + 8)};
            if (firstChildren) {
                firstChildren = children;
                node->firstChildren = children;
                cout << "Primeira criança definida" << endl;
            } else {
                AstChildren* currentChild = firstChildren;
                while (currentChild) {
                    currentChild = currentChild->nextChildren;
                }
                node->firstChildren->nextChildren = children;
                cout << "A Proxima criança foi definida" << endl;
            }
        }
        if(expression.find("x") != string::npos && expression.find("x") > expression.find("{")){
            AstChildren* children = new AstChildren();
            children->name = "x-axis";
            children->startLine = childrenLine;
            children->finalLine = childrenLine;
            children->type = "xAxisValue";
            children->params = {expression.substr(expression.find("x") + 2)};
            if (firstChildren) {
                firstChildren = children;
                node->firstChildren = children;
                cout << "Primeira criança definida" << endl;
            } else {
                AstChildren* currentChild = firstChildren;
                while (currentChild) {
                currentChild = currentChild->nextChildren;
                }
            node->firstChildren->nextChildren = children;
            cout << "A Proxima criança foi definida" << endl;
            }
        }
        if(expression.find("y") != string::npos && expression.find("y") > expression.find("{")){
            AstChildren* children = new AstChildren();
            children->name = "y-axis";
            children->startLine = childrenLine;
            children->finalLine = childrenLine;
            children->type = "yAxisValue";
            children->params = {expression.substr(expression.find("y") + 2)};
            if (firstChildren->name.empty()) {
                firstChildren = children;
                node->firstChildren = children;
                cout << "Primeira criança definida" << endl;
            } else {
                currentChild = node->firstChildren;
                while (currentChild) {
                    currentChild = currentChild->nextChildren;
                }
                node->firstChildren->nextChildren = children;
                cout << "A Proxima criança foi definida" << endl;
            }
        }
    }
    return node;
}

void printChildren(const AstChildren* primeiraCrianca) {
    cout << primeiraCrianca->name << endl;
    const AstChildren* currentChild = primeiraCrianca;
        cout << "Child: " << currentChild->type;
        cout << ": " << currentChild->name;
        cout << " (linha " << currentChild->startLine << ")";
        if (!currentChild->params.empty()) {
            cout << " [Parâmetros: ";
            for (const auto& param : currentChild->params)
                cout << param << " ";
            cout << "]";
        }
        cout << endl;

        currentChild = currentChild->nextChildren;
        if(currentChild){
            printChildren(currentChild->firstChildren);
        } else{
            cout << "Acabou" << endl;
        }

}


void printAST(const AstNode* node, int depth = 0) {
    for (int i = 0; i < depth; ++i)
        cout << "  "; 

    cout << "Element: " << node->type;
    if (!node->name.empty())
        cout << ": " << node->name;
    cout << " , linha " << node->startLine << ",";
    if (!node->params.empty()) {
        cout << " Parâmetros: ";
        for (const auto& param : node->params)
            cout << param << " ";
        cout << ";";
    }
    cout << endl;
    cout << "Vamos imprimir as childens" << endl;
    printChildren(node->firstChildren);
}

int main(int argc, char *argv[]){
    string line;
    string archive;
    AstNode* expression;
    int startline = 0;
    int validator = 0;
    if(argv[1] != NULL){
        ifstream file;
        const char* nameArchive = argv[1];
        file.open(nameArchive);
        ofstream codeFinal("codeFinal.asm");
        int lineCout = 1;
        if(file.is_open()){
            int childrenLine;
            while(getline(file, line)){
                archive += "\n" + line;
                if(validator != 1){
                    startline = lineCout;
                    validator = 1;
                }
                if(line.find("{") != string::npos){
                    childrenLine = lineCout + 1;
                }
                if(line.find("}") != string::npos){
                    expression = parser(archive, startline, lineCout, childrenLine);
                    cout << "Vamos imprimir o node" << endl;
                    printAST(expression);
                    validator = 0;
                }
                lineCout++;
            }
        } else{
            cerr << "Falha ao ler o arquivo : (" << endl;
            return 1;
        }
    }
}