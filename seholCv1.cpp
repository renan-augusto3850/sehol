#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

struct AstElement{
    string name;
    int startLine;
    int finalLine;
    string type;
    vector<string> params;
    string SimpleAtributes[3];
    string atributes[4][4];
    string completeLine;
    AstElement* nextElement;
    AstElement() : nextElement(nullptr) {}
};


AstElement* parser(string expression, int startLine, int finalLine, int childrenLine){
    AstElement* element = new AstElement();
    if(expression.find("drawWindow") != string::npos){
        int indexName1 = expression.find("\"");
        int indexName2 = expression.find("\"", indexName1 + 1);
        string resultName = expression.substr(indexName1 + 1, indexName2 - indexName1 - 1);
        int indexCommaWidth1 = expression.find(",");
        int indexCommaWidth2 = expression.find(",", indexCommaWidth1 + 1);
        string resultCommaWidth = expression.substr(indexCommaWidth1 + 1, indexCommaWidth2 - indexCommaWidth1 - 1);
        string resultCommaHeigth = expression.substr(indexCommaWidth2 + 2, expression.find(")") - indexCommaWidth2 - 2);
        element->name = "drawWindow";
        element->startLine = startLine;
        element->finalLine = finalLine;
        element->type = "drawWindowElement";
        element->completeLine = expression;
        element->params = {resultName + "," + resultCommaWidth + "," + resultCommaHeigth};
        if(expression.find("background-color") != string::npos && expression.find("background-color") > expression.find("{")){
            element->atributes[0][0] = "backgroundChange";
            element->atributes[0][1] = expression.substr(expression.find("background-color") + 18).c_str();
            element->atributes[0][2] = to_string(childrenLine).c_str();
            element->atributes[0][3] = expression.substr(expression.find("background-color")).c_str();
        }
    }else if(expression.find("drawText") != string::npos){
        int indexName1 = expression.find("\"");
        int indexName2 = expression.find("\"", indexName1 + 1);
        string resultText = expression.substr(indexName1 + 1, indexName2 - indexName1 - 1);
        element->name = "drawText";
        element->startLine = startLine;
        element->finalLine = finalLine;
        element->type = "drawTextElement";
        element->completeLine = expression;
        element->params = {resultText};
        if(expression.find("color") != string::npos && expression.find("color") > expression.find("{")){
            element->atributes[0][0] = "colorChange";
            element->atributes[0][1] = expression.substr(expression.find("color") + 7).c_str();
            element->atributes[0][2] = to_string(childrenLine).c_str();
            element->atributes[0][3] = expression.substr(expression.find("color")).c_str();
        }
    }else if(expression.find("drawButton") != string::npos){
        int indexName1 = expression.find("\"");
        int indexName2 = expression.find("\"", indexName1 + 1);
        string resultButton = expression.substr(indexName1 + 1, indexName2 - indexName1 - 1);
        element->name = "drawButton";
        element->startLine = startLine;
        element->finalLine = finalLine;
        element->type = "drawButtonElement";
        element->completeLine = expression;
        element->params = {resultButton};
        if(expression.find("width") != string::npos && expression.find("width") > expression.find("{")){
            element->atributes[0][0] = "widthValue";
            element->atributes[0][1] = expression.substr(expression.find("width") + 7, expression.find("\n")).c_str();
            element->atributes[0][2] = to_string(childrenLine).c_str();
            element->atributes[0][3] = expression.substr(expression.find("width")).c_str();
        }
        if(expression.find("height") != string::npos && expression.find("height") > expression.find("{")){
            element->atributes[1][0] = "heightValue";
            element->atributes[1][1] = expression.substr(expression.find("height") + 8, expression.find("\n")).c_str();
            element->atributes[1][2] = to_string(childrenLine).c_str();
            element->atributes[1][3] = expression.substr(expression.find("height")).c_str();
        }
        if(expression.find("x") != string::npos && expression.find("x") > expression.find("{")){
            element->atributes[2][0] = "xAxis";
            element->atributes[2][1] = expression.substr(expression.find("x") + 3, expression.find("\n")).c_str();
            element->atributes[2][2] = to_string(childrenLine).c_str();
            element->atributes[2][3] = expression.substr(expression.find("x")).c_str();
        }
        if(expression.find("y") != string::npos && expression.find("y") > expression.find("{")){
            element->atributes[3][0] = "yAxis";
            element->atributes[3][1] = expression.substr(expression.find("y") + 3, expression.find("\n")).c_str();
            element->atributes[3][2] = to_string(childrenLine).c_str();
            element->atributes[3][3] = expression.substr(expression.find("y")).c_str();
        }
    } else{
        cerr << "\033[31m"<< "SeholSyntaxError: Unknown code or syntax detected in the scoope of code." <<  "\033[0m" << endl;
        cerr << "\033[4m   " + expression << "\033[0m" << endl;
    }
    return element;
}

class grammarProcess {
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
};

bool grammarAnalisis(const string (&atributes)[4][4]) {
    cout << "Grammar" << endl;
    bool pass = true;
    grammarProcess process;
    for(int i = 0; i < 4; i++) {
        string name = atributes[i][0];
        if(name == "widthValue") {
            if(!process.checkisInt(atributes[i][1])){
                cerr <<  "\033[31m" << "SeholGrammarError<" << atributes[i][2] << ">: Width value must be a Integer." << "\033[0m" << endl;
                cerr << "\033[4m   " + atributes[i][1] << "\033[0m" << endl;
                cerr << "   Recomendations: Remove any letters or unknown characters." << endl;
                pass = false;
            }
            if(process.checkhaveQuiote(atributes[i][1])) {
                cerr <<  "\033[31m" << "SeholGrammarError<" << atributes[i][2] << ">: Width value must be a Integer." << "\033[0m" << endl;
                cerr << "\033[4m   " + atributes[i][1] << "\033[0m" << endl;
                cerr << "       Recomendations: You just have quiotes, remove them." << endl;
                pass = false;
            }
        }
        if(name == "heightValue") {
            if(!process.checkisInt(atributes[i][1])){
                cerr <<  "\033[31m" << "SeholGrammarError<" << atributes[i][2] << ">: Height value must be a Integer." << "\033[0m" << endl;
                cerr << "\033[4m   " + atributes[i][1] << "\033[0m" << endl;
                cerr << "   Recomendations: Remove any letters or unknown characters." << endl;
                pass = false;
            }
            if(process.checkhaveQuiote(atributes[i][1])) {
                cerr <<  "\033[31m" << "SeholGrammarError<" << atributes[i][2] << ">: Height value must be a Integer." << "\033[0m" << endl;
                cerr << "\033[4m   " + atributes[i][1] << "\033[0m" << endl;
                cerr << "       Recomendations: You just have quiotes, remove them." << endl;
                pass = false;
            }
        }
        if(name == "xAxis") {
            if(!process.checkisInt(atributes[i][1])){
                cerr <<  "\033[31m" << "SeholGrammarError<" << atributes[i][2] << ">: x value must be a Integer." << "\033[0m" << endl;
                cerr << "\033[4m   " + atributes[i][1] << "\033[0m" << endl;
                cerr << "   Recomendations: Remove any letters or unknown characters." << endl;
                pass = false;
            }
            if(process.checkhaveQuiote(atributes[i][1])) {
                cerr <<  "\033[31m" << "SeholGrammarError<" << atributes[i][2] << ">: x value must be a Integer." << "\033[0m" << endl;
                cerr << "\033[4m   " + atributes[i][1] << "\033[0m" << endl;
                cerr << "       Recomendations: You just have quiotes, remove them." << endl;
                pass = false;
            }
        }
        if(name == "yAxis") {
            if(!process.checkisInt(atributes[i][1])){
                cerr <<  "\033[31m" << "SeholGrammarError<" << atributes[i][2] << ">: y value must be a Integer." << "\033[0m" << endl;
                cerr << "\033[4m   " + atributes[i][1] << "\033[0m" << endl;
                cerr << "   Recomendations: Remove any letters or unknown characters." << endl;
                pass = false;
            }
            if(process.checkhaveQuiote(atributes[i][1])) {
                cerr <<  "\033[31m" << "SeholGrammarError<" << atributes[i][2] << ">: y value must be a Integer." << "\033[0m" << endl;
                cerr << "\033[4m   " + atributes[i][1] << "\033[0m" << endl;
                cerr << "       Recomendations: You just have quiotes, remove them." << endl;
                pass = false;
            }
        }
    }
    return pass;
}

string replaceValue(string windowcode, string value, string data) {
    size_t pos = 0;
    while ((pos = windowcode.find(value, pos)) != string::npos) {
        windowcode.replace(pos, value.length(), data);
        pos += data.length();
    }
    return windowcode;
}

string getPrefixes(string name){
    ifstream prefixes;
    string codePrefix;
    string line;
    prefixes.open("prefixes/" + name);
    if(prefixes.is_open()){
        while(getline(prefixes, line)){
            codePrefix += line + "\n";
        }
    }
    return codePrefix;
}
void generateCodeFinal(AstElement* elements[3]) {
    string definitions = getPrefixes("definitions.pref");
    string secData = getPrefixes("section-data.pref");
    string head = getPrefixes("head.pref");
    string secBss = getPrefixes("section-bss.pref");
    string secText = getPrefixes("section-text.pref");
    string footer = getPrefixes("final.pref");
    string messageListener = getPrefixes("messagesListener.pref");
    string wm_command = getPrefixes("wmcommand.pref");
    string ctlcolorStatic = getPrefixes("ctlcolorStatic.pref");
    string windowcode;
    string codeColor = "0xFFFFFF";
    string resultCommaWidth = "500";
    string resultCommaHeigth = "500";
    int ButtonWidth = 300;
    int ButtonX = 10;
    int ButtonHeigth = 100;
    string type = "window";
    int ButtonY = 10;
    bool isBehind = false;
    bool haveText = false;
    ofstream codeFinal("codeFinal.asm");
    string wm_command_definitions;
    string Element;
    int braceFinded = 0;
    string textBackColor = "0xFFFFFF";
    string textColour = "0x000000";
    string resultButton;
    string TextId;
    int generator = 104;
    string resultName;
    cout << "Generating" << endl;
    for(int i = 0; i < 3; i++) {
        cout << "Number: " << i << "Element: " << elements[i]->type << endl;
        if(elements[i]->type == "drawWindowElement" && isBehind == false){
            //type = "window";
            string params = elements[i]->params[0];
            size_t paramName = params.find(",");
            string resultName = params.substr(params.find(","));
            string completeline = elements[i]->completeLine;
            size_t indexCommaWidth1 = params.find(",");
            size_t indexCommaWidth2 = completeline.find(",", indexCommaWidth1 + 1);
            string resultCommaWidth = completeline.substr(indexCommaWidth1 + 1, indexCommaWidth2 - indexCommaWidth1 - 1);
            string resultCommaHeigth = completeline.substr(indexCommaWidth2 + 2);
            windowcode += getPrefixes("drawWindow.pref");
            cout << "Você criou o elemento drawWindow com: " << resultName << " " << resultCommaWidth << " " << resultCommaHeigth << endl;
            /*if(completeline.find("{")){
                int braceFinded = 1;
                string element = "drawWindow";
            }*/
        }
        if(elements[i]->type == "drawTextElement" && isBehind == false){
            type = "window";
            string completeline = elements[i]->completeLine;
            string erasedLine = completeline.erase(completeline.size() - 4);
            cout << "line command: " << completeline << " Size: " << completeline.size() << endl;
            int indexText1 = completeline.find("\"");
            int indexText2 = completeline.find("\"", indexText1);
            string resultText = "\"" + completeline.substr(indexText1 + 1, indexText2 - indexText1 - 1) + "\"";
            size_t pos1 = completeline.find(" ");
            size_t pos2 = completeline.find("(");
            string TextId = completeline.substr(pos1 + 1, pos2 - pos1 - 1);
            cout << "Você criou o elemento drawText com: " << resultText << " E id: " << TextId << endl;
            windowcode += getPrefixes("drawText.pref");
            secData += " " + TextId + "Text      db " + resultText + ", 0 \n";
            secData += " " + TextId + "Colour       dd " + textColour + ", 0 \n";
            secData += " " + TextId + "BackColour       dd " + textBackColor + ", 0 \n";
            windowcode = replaceValue(windowcode, "textReplace", TextId + "Text");
            wm_command_definitions += replaceValue(getPrefixes("textClass.pref"), "textReplace", TextId + "Text");
            wm_command_definitions = replaceValue(wm_command_definitions, "textColour", TextId + "Colour");
            wm_command_definitions = replaceValue(wm_command_definitions, "textBackColourA", TextId + "BackColour");
            wm_command = replaceValue(wm_command, "textclass", TextId);
            wm_command_definitions = replaceValue(wm_command_definitions, "textclass", TextId);
            wm_command = replaceValue(wm_command, "textId", TextId + "Id");
            definitions += TextId + "Id            EQU " + to_string(generator);
            generator++;
            windowcode = replaceValue(windowcode, "textId", TextId + "Id");
            ctlcolorStatic = replaceValue(ctlcolorStatic, "textclass", TextId);
            ctlcolorStatic += replaceValue(getPrefixes("textColor.pref"), "textclass", TextId);
            secBss += "\n " + TextId + "            resq 1 \n"; 
            windowcode = replaceValue(windowcode, "textclass", TextId);
            ctlcolorStatic = replaceValue(ctlcolorStatic, "textColorReplace", "static" + TextId);
            ctlcolorStatic = replaceValue(ctlcolorStatic, "textColReplace", TextId + "Colour");
            ctlcolorStatic = replaceValue(ctlcolorStatic, "textBackReplace", TextId + "BackColour");
            ctlcolorStatic = replaceValue(ctlcolorStatic, "textId", TextId + "Id");
            /*if(completeline.find("{")){
                braceFinded = 1;
                element = "drawText";
            }*/
        }
        if(elements[i]->type == "drawButtonElement" && isBehind == false){
            //type = "window";
            string completeline = elements[i]->completeLine;
            string erasedLine = completeline.erase(completeline.size() - 4);
            cout << "line command: " << completeline << " Size: " << completeline.size() << endl;
            int indexText1 = completeline.find("\"");
            int indexText2 = completeline.find("\"", indexText1);
            string resultButton = "\"" + completeline.substr(indexText1 + 1, indexText2 - indexText1 - 1) + "\"";
            size_t pos1 = completeline.find(" ");
            size_t pos2 = completeline.find("(");
            string ButtonId = completeline.substr(pos1 + 1, pos2 - pos1 - 1);
            cout << "Você criou o elemento drawButton com: " << resultButton << " E id: " << ButtonId << endl;
            windowcode += getPrefixes("drawButton.pref");
            definitions += "\n" + ButtonId + "Id         EQU " + to_string(generator);
            secBss += "\n" + ButtonId + "           resq 1 \n";
            generator++;
            secData += " " + ButtonId + "Text         db " + resultButton + ", 0 \n";
            secData += " buttonClass             db \"BUTTON\", 0 \n";
            windowcode = replaceValue(windowcode, "buttonText", ButtonId + "Text");
            windowcode = replaceValue(windowcode, "buttonId", ButtonId + "Id");
            /*if(completeline.find("{")){
                braceFinded = 1;
                element = "drawButton";
            }*/
        }
        if(codeFinal.is_open()){
            if(type == "window"){
                definitions = replaceValue(definitions, "widthReplace", resultCommaWidth);
                definitions = replaceValue(definitions, "heigthReplace", resultCommaHeigth);
                secData.replace(secData.find("nameReplace"), 11, '"' + resultName + '"');
                secData.replace(secData.find("backReplace"), 11, codeColor);
                secData.replace(secData.find("backTReplace"), 12, codeColor);
                windowcode = replaceValue(windowcode, "buttonWidth", to_string(ButtonWidth));
                windowcode = replaceValue(windowcode, "buttonHeigth", to_string(ButtonHeigth));
                windowcode = replaceValue(windowcode, "buttonX", to_string(ButtonX));
                windowcode = replaceValue(windowcode, "buttonY", to_string(ButtonY));
            }
        codeFinal << definitions << head << secData << secBss << secText << messageListener << wm_command << wm_command_definitions << windowcode << ctlcolorStatic << footer;
        codeFinal.close();
        //return definitions + head + secData + secBss + secText + messageListener + wm_command + wm_command_definitions + windowcode + ctlcolorStatic + footer;
        }
    }
}

int main(int argc, char *argv[]){
    string line;
    string archive;
    AstElement* expression;
    int startline = 0;
    int count = 0;
    AstElement* elements[3];
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
                    if(!expression->atributes[0][0].empty()){
                        if(grammarAnalisis(expression->atributes)){
                            elements[count] = expression;
                            count++;
                        }
                    }
                    validator = 0;
                }
                lineCout++;
            }
            generateCodeFinal(elements);
            cout << "Compilation terminated. : )" << endl;
        } else{
            cerr << "Failed to read the archive. : (" << endl;
            return 1;
        }
    }
}