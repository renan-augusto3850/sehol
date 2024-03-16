#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

struct AstNode{
    string name;
    int startLine;
    int finalLine;
    string type;
    vector<string> params;
    string SimpleChildren[3];
    string childrens[4][3];
    AstNode* nextNode;
    AstNode() : nextNode(nullptr) {}
};


AstNode* parser(string expression, int startLine, int finalLine, int childrenLine){
    AstNode* node = new AstNode();
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
            string SimpleChildren[3] = {"backgroundChange",expression.substr(expression.find("background-color") + 18).c_str(), to_string(childrenLine).c_str()};
            node->SimpleChildren[0] = SimpleChildren[0];
            node->SimpleChildren[1] = SimpleChildren[1];
            node->SimpleChildren[2] = SimpleChildren[2];
            cout << node->SimpleChildren[1] << endl;
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
            const char* SimpleChildren[3] = {"colorChange", expression.substr(expression.find("color") + 7).c_str(), to_string(childrenLine).c_str()};
            node->SimpleChildren[0] = SimpleChildren[0];
            node->SimpleChildren[1] = SimpleChildren[1];
            node->SimpleChildren[2] = SimpleChildren[2];
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
            node->childrens[0][0] = "widthValue";
            node->childrens[0][1] = expression.substr(expression.find("width") + 7, 3).c_str();
            node->childrens[0][2] = to_string(childrenLine).c_str();
            }
        }
        if(expression.find("heigth") != string::npos && expression.find("height") > expression.find("{")){
            node->childrens[1][0] = "heigthValue";
            node->childrens[1][1] = expression.substr(expression.find("heigth") + 8).c_str();
            node->childrens[1][2] = to_string(childrenLine).c_str();
        }
        if(expression.find("x") != string::npos && expression.find("x") > expression.find("{")){
            node->childrens[2][0] = "xAxis";
            node->childrens[2][1] = expression.substr(expression.find("x") + 3, 3).c_str();
            node->childrens[2][2] = to_string(childrenLine).c_str();
        }
        if(expression.find("y") != string::npos && expression.find("y") > expression.find("{")){
            node->childrens[3][0] = "yAxis";
            node->childrens[3][1] = expression.substr(expression.find("y") + 3, 3).c_str();
            node->childrens[3][2] = to_string(childrenLine).c_str();
        }
    return node;
}

/*bool grammarAnalisis(string childrens[3]) {

}*/
class grammarBidimensionalProcess {
    public:
        bool checkisInt(string value) {
            for (char c : value) {
                if (!std::isdigit(c)) {
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
};

bool grammarAnalisisBidmensional(const string (&childrens)[4][3]) {
    bool pass = true;
    grammarBidimensionalProcess process;
    for(int i = 0; i < 4; i++) {
        string name = childrens[i][0];
        if(name == "widthValue") {
            if(!process.checkisInt(childrens[i][1])){
                cerr <<  "\033[31m" << "SeholGrammarError<" << childrens[i][2] << ">: Width value must be a Integer." << "\033[0m" << endl;
                cerr << "   Recomendations: Remove any letters or unknown characters." << endl;
                pass = false;
            }
            if(process.checkhaveQuiote(childrens[i][1])) {
                cerr <<  "\033[31m" << "SeholGrammarError<" << childrens[i][2] << ">: Width value must be a Integer." << "\033[0m" << endl;
                cerr << "\033[4m   " + childrens[i][1] << "\033[0m" << endl;
                cerr << "       Recomendations: You just have quiotes, remove them." << endl;
                pass = false;
            }
        }
    }
    return pass;
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
                    archive = "";
                    if(!expression->SimpleChildren[0].empty() && expression->type != "drawButtonElement") {
                        for(int i = 0; i < 3; i++) { 
                            switch(i) {
                                case 0:
                                    cout << "Type: ";
                                case 1:
                                    cout << "Value: ";
                                case 2:
                                    cout << "Line: ";
                                default:
                                    cout << "Not Finded";
                            }
                            cout << expression->SimpleChildren[i] << endl;
                        }   
                    } else {
                        if(grammarAnalisisBidmensional(expression->childrens)){
                            cout << "Passed" << endl;
                        }
                    }
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