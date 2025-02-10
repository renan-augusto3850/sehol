#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include "mathematics.h"

using namespace std;

struct AstElement{
    string name;
    int line;
    string type;
    string params;
    string completeLine;
    string scoopeBlock;
    string returnTo;
    AstElement* element;
    AstElement() : element(nullptr) {}
};

unordered_map<string, variables> variableTable;


vector<string> tokenize(const string& expression) {
    vector<string> tokens;
    string token;
    bool inString = false;
    bool inBraceKeys = false;
    bool inParentheses = false;
    int braceDepth = 0;

    for (size_t i = 0; i < expression.size(); ++i) {
        char ch = expression[i];

        // Ignorar quebras de linha
        if (ch == '\n') {
            continue;
        }

        // Gerenciar aspas
        if (ch == '\"') {
            inString = !inString;
            token += ch;
            continue;
        }

        // Gerenciar parênteses
        if (ch == '(' && inBraceKeys == false) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            inParentheses = true;
            token += ch;
            continue;
        } else if (ch == ')' && inBraceKeys == false) {
            token += ch;
            tokens.push_back(token);
            token.clear();
            inParentheses = false;
            continue;
        }

        // Gerenciar chaves
        if (ch == '{') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            inBraceKeys = true;
            braceDepth++;
            token += ch;
            continue;
        } else if (ch == '}') {
            token += ch;
            braceDepth--;
            if (braceDepth == 0) {
                tokens.push_back(token);
                token.clear();
                inBraceKeys = false;
            }
            continue;
        }

        // Gerenciar outros caracteres
        if (ch == ' ' && !inString && !inParentheses && !inBraceKeys) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (ch == ';' && inBraceKeys == false) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(";");
        } else {
            token += ch;
        }
    }

    // Adicionar o último token, se houver
    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

AstElement* parseTokens(const vector<string>& tokens, int childrenLine) {
    AstElement* element = new AstElement();
    variables var;
    // for(int i = 0; i < tokens.size(); i++) {
    //    cout << "[ " << tokens[i] << " ]";
    // }
    // cout << endl;   
    if (tokens.empty()) {
        cerr << "\033[31m" << "SeholError:" << "Invalid expression." << "\033[0m" << endl;
        return nullptr;
    }

    if (tokens[0] == "var" && tokens.size() >= 4 && tokens[2] == "=") {
        var.name = tokens[1];
        string value = tokens[3];
        if (!value.empty() && value.front() == '\"' && value.back() == '\"') {
            value = value.substr(1, value.size() - 2);
        }
        var.value = value;
        variableTable[var.name] = var;
    } 
    if (find(tokens.begin(), tokens.end(), "printLog") != tokens.end()) {
        auto it = find(tokens.begin(), tokens.end(), "printLog");
        int indice = distance(tokens.begin(), it);

        element->name = "printLog";
        element->line = childrenLine;
        element->type = "printLogConsole";
        element->params = tokens[indice + 1];

        size_t pos1 = tokens[indice + 1].find("\"");
        size_t pos2 = tokens[indice + 1].rfind("\"");
        
        if (pos1 != string::npos && pos2 != string::npos && pos2 > pos1) {
            element->params = tokens[indice + 1].substr(pos1 + 1, pos2 - pos1 - 1);

        }

        size_t pose1 = tokens[indice + 1].find("(");
        size_t pose2 = tokens[indice + 1].rfind(")");
        
        if (pose1 != string::npos && pose2 != string::npos && pose2 > pose1) {
            element->params = tokens[indice + 1].substr(pose1 + 1, pose2 - pose1 - 1);

        }
    } 
    if (tokens[0] == "readFile" && tokens.size() >= 4 && tokens[1] == "(" && tokens[3] == ")") {
        string filename = tokens[2].substr(1, tokens[2].size() - 2);
        ifstream file(filename);
        string line, text;

        if (file.is_open()) {
            while (getline(file, line)) {
                text += line + "\n";
            }
            file.close();
        } else {
            cerr << "\033[31m" << "SeholError:" << "File not open." << "\033[0m" << endl;
            cerr << "\033[4m   " << filename << "\033[0m" << endl;
            return nullptr;
        }

        if (!var.name.empty()) {
            variableTable[var.name].value = text;
        }
    } if(find(tokens.begin(), tokens.end(), "input") != tokens.end()) {
            auto it = find(tokens.begin(), tokens.end(), "input");
            int indice = distance(tokens.begin(), it);

            element->name = "input";
            element->line = childrenLine;
            element->type = "inputIn";
            element->params = tokens[indice + 1];
            
            if (indice > 0) {
                element->returnTo = tokens[1];
            } else {
                element->returnTo = "body";
            }

        }
        if(tokens[0] == "if") {
            element->name = "if";
            element->line = childrenLine;
            element->type = "ifCondition";
            element->params = tokens[1];
            element->scoopeBlock = tokens[2];
        }
    return element;
}

AstElement* parser(string expression, int childrenLine) {
    vector<string> tokens = tokenize(expression);
    return parseTokens(tokens, childrenLine);
}


/*class grammarProcess {
    public:
        bool checkisInt(string value) {
            for (char c : value) {
                if (!isdigit(c)) {
                    return false;
                }
            }
            return true;
        }
        bool checkhaveQuiote(string value) {
            if(value.find("\"") != string::npos){
                return true;
            }
            return false;
        }
};*/

/*bool grammarAnalisis(const string (&atributes)[4][4]) {
    bool pass = true;
    grammarProcess process;
    for(int i = 0; i < 4; i++) {
        string name = atributes[i][0];
       
    }
    return pass;
}*/

/*string replaceValue(string windowcode, string value, string data) {
    size_t pos = 0;
    while ((pos = windowcode.find(value, pos)) != string::npos) {
        windowcode.replace(pos, value.length(), data);
        pos += data.length();
    }
    return windowcode;
}*/
bool ignoreThis = false;
bool firstRound = false;
void startRuntime(vector<AstElement*> elementsVector, string scoope) {
    for(const auto& elements : elementsVector) {
        if(elements->type == "printLogConsole") {
            auto it = variableTable.find(elements->params);
            if(it != variableTable.end()) {
                cout << variableTable[elements->params].value << endl;
            } else {
                cout << elements->params << endl;
            }
        } if(elements->type == "inputIn") {
            string userInput;
            cout << elements->params << " ";
            getline(std::cin, userInput);

            if (elements->returnTo == "body") {
                cout << userInput << std::endl;
            } else {
                variableTable[elements->returnTo].value = userInput;
            }
        } if(elements->type == "ifCondition") {

            // Dividir a expressão em tokens

            vector<string> tokens = dividirExpressao(elements->params, variableTable);
            // Converter para Notação Polonesa Reversa (RPN)
            vector<string> rpn = paraRPN(tokens);

            // Avaliar a expressão em RPN
             cout << endl << "avaliar"; 
                double resultado = avaliarRPN(rpn);
                if(resultado == 0) {
                    size_t pos1 = elements->scoopeBlock.find("{");
                    size_t pos2 = elements->scoopeBlock.rfind("}");
                    if (pos1 != string::npos && pos2 != string::npos && pos2 > pos1) {
                        elements->scoopeBlock = elements->scoopeBlock.substr(pos1 + 1, pos2 - pos1 - 1);
                    }
                    AstElement* expression = parser(elements->scoopeBlock, 0);
                    vector<AstElement*> elementsBlock;
                    if(!expression->name.empty()) {
                        elementsBlock.push_back(expression);
                        startRuntime(elementsBlock, "blockOfIf");
                    }
                }
            
        }
    }
}

int main(int argc, char *argv[]) {
    string line;
    string archive;
    AstElement* expression;
    int startline = 0;
    int count = 0;
    bool blockOfCommand =  false;
    vector<AstElement*> elements;
    int validator = 0;
    if(argv[1] != NULL){
        ifstream file;
        const char* nameArchiveC = argv[1];
        file.open(nameArchiveC);
        int lineCout = 1;
        if(file.is_open()){
            cout << "ok";
            int childrenLine;
            while(getline(file, line)){
                archive += line + "\n";
                if(validator != 1){
                    startline = lineCout;
                    validator = 1;
                }
                if(line.find("{") != string::npos) {
                    blockOfCommand = true;
                }
                if(line.find("}") != string::npos) {
                    blockOfCommand = false;
                }
                if(blockOfCommand == false) {
                    expression = parser(archive, childrenLine);
                    archive = "";
                    if(!expression->name.empty()) {
                        elements.push_back(expression);
                        count++;
                    }
                }
                    
                validator = 0;
                lineCout++;
            }     
            startRuntime(elements, "body");
        } else{
            cerr << "Error to open the file";
        }
    } else{
        cout << "Sehol Runtime. V.1 Beta1 Unstable." << endl;
        cout << "\tMake as hard to an 15 years old Brazilian student, Renan augusto." << endl;
        string x;
        while(true) {
            cout << ":-) > "; 
            getline(cin, x);
            if(validator != 1){
                validator = 1;
            }
            if(line.find("{") != string::npos) {
                blockOfCommand = true;
            }
            if(line.find("}") != string::npos) {
                blockOfCommand = false;
            }
            if(blockOfCommand == false) {
                expression = parser(x, 0);
                if(!expression->name.empty()) {
                    elements.push_back(expression);
                    count++;
                }
            }
            validator = 0;    
            startRuntime(elements, "runtime");
            elements.clear();
        }
        return 0;
    }
}