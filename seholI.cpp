#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

struct AstElement{
    string name;
    int line;
    string type;
    string params;
    string completeLine;
    string action;
    string returnTo;
    AstElement* element;
    AstElement() : element(nullptr) {}
};

struct variables{
    string name;
    string value;
};

unordered_map<string, variables> variableTable;


vector<string> tokenize(const string& expression) {
    vector<string> tokens;
    string token;
    bool inString = false;

    for (size_t i = 0; i < expression.size(); ++i) {
        char ch = expression[i];

        if (ch == ' ' && !inString) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (ch == '\"') {
            inString = !inString;
            token += ch;
        } else if (ch == '(' || ch == ')') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, ch));
        } else if (ch == ';') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(";");
        } else {
            token += ch;
        }
    }

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
    if (tokens[0] == "printLog" && tokens.size() >= 4 && tokens[1] == "(" && tokens[3] == ")") {
        element->name = "printLog";
        element->line = childrenLine;
        element->type = "printLogConsole";
        
        size_t pos1 = tokens[2].find("\"");
        size_t pos2 = tokens[2].rfind("\"");
        element->params = tokens[2];
        if (pos1 != string::npos && pos2 != string::npos && pos2 > pos1) {
            element->params = tokens[2].substr(pos1 + 1, pos2 - pos1 - 1);
        }
        element->completeLine = tokens[0] + tokens[1] + tokens[2] + tokens[3] + tokens[4];
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
            element->params = tokens[indice + 2];
            
            if (indice > 0) {
                element->returnTo = tokens[1];
            } else {
                element->returnTo = "body";
            }

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