#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <sstream>
#include <atomic>
using namespace std;

struct AstElement{
    string name;
    int line;
    string type;
    string params;
    string completeLine;
    string action;
    AstElement* element;
    AstElement() : element(nullptr) {}
};

struct variables{
    string name;
    string value;
};

unordered_map<string, variables> variableTable;


AstElement* parser(string expression, int childrenLine) {
    AstElement* element = new AstElement();
    variables var;
    int intoroute = 1;
    if(expression.find("var") != string::npos && intoroute == 1) {
        size_t id1 = expression.find("var") + 3;
        size_t id2 = expression.find("=");
        var.name = expression.substr(id1 + 1, id2 - id1 - 2);
        var.value = expression.substr(id2, id2 - expression.find(";") - 1);
        variableTable[var.name] = var;
    } else if(expression.find("printLog") != string::npos && intoroute == 1) {
            size_t pos1 = expression.find("\"");
            size_t pos2 = expression.find(")");
            element->name = "printLog";
            element->line = childrenLine;
            element->type = "printLogConsole";
            element->params = expression.substr(pos1 + 1, pos2 - pos1 - 2);
            element->completeLine = expression;
        } else if(expression.find("readFile") != string::npos && intoroute == 1) {
            size_t pos1 = expression.find("readFile") + 8;
            size_t pos2 = expression.find(")");
            ifstream file;
            file.open(expression.substr(pos1 + 2, pos2 - pos1 - 3));
            string line, text;
            if(file.is_open()) {
                while(getline(file, line)) {
                    text += line + "\n";
                }
            } else {
                cerr <<  "\033[31m" << "SeholError:" << "File not open." << "\033[0m" << endl;
                cerr << "\033[4m   " << expression.substr(pos1 + 2, pos2 - pos1 - 3) << "\033[0m" << endl;
            }
            if(var.name) {
                variableTable[var.name].value = text;
            }
        } else {
            cerr <<  "\033[31m" << "SeholError:" << "Unknown Syntax in body scoope." << "\033[0m" << endl;
            cerr << "\033[4m   " << expression << "\033[0m" << endl;
            abort();
        }
    return element;
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
            cout << elements->params << endl;
        } else{
            cerr <<  "\033[31m" << "SeholError:" << "Unknown Command on Syntax Three." << "\033[0m" << endl;
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
                archive += "\n" + line;
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
             int result = 0;
            while(result == 0){
                startRuntime(elements, "body");
                Sleep(200);
            }
            cout << "Finished." << endl;
        }
    } else{
        cout << "Sehol Runtime. V.Beta1 Unstable." << endl;
        cout << "   Make as hard to an 14 years old Brazilian student, Renan augusto." << endl;
        string x; // declare an integer variable
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