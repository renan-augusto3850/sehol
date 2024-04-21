#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <tuple>
#include <cstring>
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
bool isBehind = false;

AstElement* parser(string expression, int startLine, int finalLine, int childrenLine){
    AstElement* element = new AstElement();
    if(expression.find("drawWindow") != string::npos && isBehind == false){
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
    }else if(expression.find("drawText") != string::npos && isBehind == false){
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
    }else if(expression.find("drawButton") != string::npos && isBehind == false){
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
            size_t valor = expression.find("width:") + 7;
            size_t virgula = expression.find(",", valor);
            element->atributes[0][1] = expression.substr(valor, virgula - valor);
            element->atributes[0][2] = to_string(childrenLine).c_str();
            element->atributes[0][3] = expression.substr(expression.find("width")).c_str();
        }
        if(expression.find("height") != string::npos && expression.find("height") > expression.find("{")){
            element->atributes[1][0] = "heightValue";
            size_t valor = expression.find("height:") + 8;
            size_t virgula = expression.find(",", valor);
            element->atributes[1][1] = expression.substr(valor, virgula - valor);
            element->atributes[1][2] = to_string(childrenLine).c_str();
            element->atributes[1][3] = expression.substr(expression.find("height")).c_str();
        }
        if(expression.find("x") != string::npos && expression.find("x") > expression.find("{")){
            element->atributes[2][0] = "xAxis";
            size_t valor = expression.find("x:") + 3;
            size_t virgula = expression.find(",", valor);
            element->atributes[2][1] = expression.substr(valor, virgula - valor);
            element->atributes[2][2] = to_string(childrenLine).c_str();
            element->atributes[2][3] = expression.substr(expression.find("x")).c_str();
        }
        if(expression.find("y") != string::npos && expression.find("y") > expression.find("{")){
            element->atributes[3][0] = "yAxis";
            size_t valor = expression.find("y:") + 3;
            size_t virgula = expression.find(",", valor);
            element->atributes[3][1] = expression.substr(valor, virgula - valor);
            element->atributes[3][2] = to_string(childrenLine).c_str();
            element->atributes[3][3] = expression.substr(expression.find("y")).c_str();
        }
    }else if(expression.find("drawCheckBox") != string::npos) {
        int indexName1 = expression.find("\"");
        int indexName2 = expression.find("\"", indexName1 + 1);
        string resultCheckbox = expression.substr(indexName1 + 1, indexName2 - indexName1 - 1);
        element->name = "drawCheckBox";
        element->startLine = startLine;
        element->finalLine = finalLine;
        element->type = "drawCheckBoxElement";
        element->completeLine = expression;
        element->params = {resultCheckbox};
        if(expression.find("width") != string::npos && expression.find("width") > expression.find("{")){
            element->atributes[0][0] = "widthValue";
            size_t valor = expression.find("width:") + 7;
            size_t virgula = expression.find(",", valor);
            element->atributes[0][1] = expression.substr(valor, virgula - valor);
            element->atributes[0][2] = to_string(childrenLine).c_str();
            element->atributes[0][3] = expression.substr(expression.find("width")).c_str();
        }
        if(expression.find("height") != string::npos && expression.find("height") > expression.find("{")){
            element->atributes[1][0] = "heightValue";
            size_t valor = expression.find("height:") + 8;
            size_t virgula = expression.find(",", valor);
            element->atributes[1][1] = expression.substr(valor, virgula - valor);
            element->atributes[1][2] = to_string(childrenLine).c_str();
            element->atributes[1][3] = expression.substr(expression.find("height")).c_str();
        }
        if(expression.find("x") != string::npos && expression.find("x") > expression.find("{")){
            element->atributes[2][0] = "xAxis";
            size_t valor = expression.find("x:") + 3;
            size_t virgula = expression.find(",", valor);
            element->atributes[2][1] = expression.substr(valor, virgula - valor);
            element->atributes[2][2] = to_string(childrenLine).c_str();
            element->atributes[2][3] = expression.substr(expression.find("x")).c_str();
        }
        if(expression.find("y") != string::npos && expression.find("y") > expression.find("{")){
            element->atributes[3][0] = "yAxis";
            size_t valor = expression.find("y:") + 3;
            size_t virgula = expression.find(",", valor);
            element->atributes[3][1] = expression.substr(valor, virgula - valor);
            element->atributes[3][2] = to_string(childrenLine).c_str();
            element->atributes[3][3] = expression.substr(expression.find("y")).c_str();
        }
    } else if(expression.find("ifClickEvent") != string::npos) {
        size_t pos1 = expression.find("(");
        size_t pos2 = expression.find(")");
        string clickEventId = expression.substr(pos1 + 1, pos2 - pos1 - 1);
        element->name = "ifClickEvent";
        element->startLine = startLine;
        element->finalLine = finalLine;
        element->type = "clickEventChecker";
        element->completeLine = expression;
        element->params = {clickEventId}; 
    
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
    prefixes.close();
    return codePrefix;
}

tuple<string, string, string, bool> devolveSolution(string name, string id){
    tuple<string, string, string, bool> content;
    if(name.find("drawMessageBox") != string::npos){
        size_t pos1 = name.find("\"");
        size_t pos2 = name.find(",");
        string text = name.substr(pos1 + 1, pos2 - pos1 - 2);
        size_t pose1 = name.find(",");
        size_t pose2 = name.find(";");
        string actions = name.substr(pose1 + 1, pose2 - pose1 - 2);
        string messageBox = getPrefixes("drawMessageBox.pref");
        if(actions == " YES_NO"){
            messageBox = replaceValue(messageBox, "actionReplace", "MB_YESNO");
        }
        if(actions == " OK"){
            messageBox = replaceValue(messageBox, "actionReplace", "MB_OK");
        }
        if(actions == " OK_CANCEL"){
            messageBox = replaceValue(messageBox, "actionReplace", "MB_OKCANCEL");
        }
        messageBox = replaceValue(messageBox, "ExitTextReplace", id + "ExitText");
        content = make_tuple(messageBox, id + "ExitText          db textReplace", text, true);
    }
    return content;
}

void generateCodeFinal(AstElement* elements[4]) {
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
    int ButtonY = 10;
    int ButtonHeigth = 100;
    int checkboxWidth = 300;
    int checkboxX = 10;
    int checkboxY = 10;
    int checkboxHeigth = 100;
    string type = "window";
    string clickId;
    bool isBehindCode = false;
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
    for(int i = 0; i < 4; i++) {
        if(elements[i]->type == "drawWindowElement" && isBehindCode == false){
            type = "window";
            string params = elements[i]->params[0];
            size_t paramName = params.find(",");
            string resultName = params.substr(params.find(","));
            string completeline = elements[i]->completeLine;
            size_t indexCommaWidth1 = params.find(",");
            size_t indexCommaWidth2 = completeline.find(",", indexCommaWidth1 + 1);
            string resultCommaWidth = completeline.substr(indexCommaWidth1 + 1, indexCommaWidth2 - indexCommaWidth1 - 1);
            string resultCommaHeigth = completeline.substr(indexCommaWidth2 + 2);
            windowcode += getPrefixes("drawWindow.pref");
            if(!elements[i]->atributes[0][0].empty()){
                braceFinded = 1;
                Element = "drawWindow";
            }
        } if(elements[i]->type == "drawTextElement" && isBehindCode == false){
            type = "window";
            string completeline = elements[i]->completeLine;
            string erasedLine = completeline.erase(completeline.size() - 4);
            int indexText1 = completeline.find("\"");
            int indexText2 = completeline.find(")") - 1;
            string resultText = "\"" + completeline.substr(indexText1 + 1, indexText2 - indexText1 - 1) + "\"";
            size_t pos1 = completeline.find(" ");
            size_t pos2 = completeline.find("(");
            string TextId = completeline.substr(pos1 + 1, pos2 - pos1 - 1);
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
        } if(elements[i]->type == "drawButtonElement" && isBehindCode == false){
            type = "window";
            string completeline = elements[i]->completeLine;
            string erasedLine = completeline.erase(completeline.size() - 4);
            int indexText1 = completeline.find("\"");
            int indexText2 = completeline.find(")") - 1;
            string resultButton = "\"" + completeline.substr(indexText1 + 1, indexText2 - indexText1 - 1) + "\"";
            size_t pos1 = completeline.find(" ");
            size_t pos2 = completeline.find("(");
            string ButtonId = completeline.substr(pos1 + 1, pos2 - pos1 - 1);
            windowcode += getPrefixes("drawButton.pref");
            definitions += "\n" + ButtonId + "Id         EQU " + to_string(generator);
            secBss += "\n " + ButtonId + "           resq 1 \n";
            generator++;
            secData += " " + ButtonId + "Text         db " + resultButton + ", 0 \n";
            windowcode = replaceValue(windowcode, "buttonText", ButtonId + "Text");
            windowcode = replaceValue(windowcode, "buttonId", ButtonId + "Id");
            if(!elements[i]->atributes[0][0].empty()){
                braceFinded = 1;
                Element = "drawButton";
            }
        } if(elements[i]->type == "drawCheckBoxElement"){
            type = "window";
            string completeline = elements[i]->completeLine;
            string erasedLine = completeline.erase(completeline.size() - 4);
            int indexText1 = completeline.find("\"");
            int indexText2 = completeline.find(")") - 1;
            string resultCheckbox = "\"" + completeline.substr(indexText1 + 1, indexText2 - indexText1 - 1) + "\"";
            size_t pos1 = completeline.find(" ");
            size_t pos2 = completeline.find("(");
            string checkboxId = completeline.substr(pos1 + 1, pos2 - pos1 - 1);
            windowcode += getPrefixes("drawCheckbox.pref");
            definitions += "\n" + checkboxId + "Id         EQU " + to_string(generator);
            secBss += "\n " + checkboxId + "           resq 1 \n";
            generator++;
            secData += " " + checkboxId + "Text         db " + resultCheckbox + ", 0 \n";
            windowcode = replaceValue(windowcode, "checkboxText", checkboxId + "Text");
            windowcode = replaceValue(windowcode, "checkboxId", checkboxId + "Id");
            if(!elements[i]->atributes[0][0].empty()){
                braceFinded = 1;
                Element = "drawButton";
            }
        } if(elements[i]->name == "ifClickEvent") {
            isBehindCode = true;
            clickId = elements[i]->params[0];
            string test = getPrefixes("ifclickevent.pref");
            wm_command += "\n" + test;
            wm_command = replaceValue(wm_command, "elementId", clickId + "Id");
            wm_command = replaceValue(wm_command, "elementClass", clickId);
            wm_command_definitions += "." + clickId + ": \n";
            braceFinded = 1;
            Element = "ifClickEvent";
        }
        if(braceFinded == 1){
            if(Element == "drawWindow"){
                if(elements[i]->atributes[0][0] == "backgroundChange"){
                    string color = elements[i]->atributes[0][1];
                    if(color == "black"){
                        codeColor = "0x000000";
                    }
                    if(color == "blue"){
                        codeColor = "0x0052CC";
                    }
                    if(color == "lightblue"){
                        codeColor = "0x80B3FF";
                    }
                    if(color == "lightgray"){
                        codeColor = "0xFFE6E6";
                    }
                    if(color == "white"){
                        codeColor = "0xFFFFFF";
                    }
                    if(color == "gray"){
                        codeColor = "0x808080";
                    }
                    if(color[0] == '#'){
                        codeColor = "0x" + color.substr(1);
                    }
                }
            }else if(Element == "drawButton") {
                if(elements[i]->atributes[0][0] == "widthValue"){
                    string value = elements[i]->atributes[0][1];
                    checkboxWidth =  stoi(value);
                }
                if(elements[i]->atributes[1][0] == "heigthValue"){
                    string value = elements[i]->atributes[1][1];
                    checkboxHeigth =  stoi(value);
                }
                if(elements[i]->atributes[2][0] == "yAxis"){
                    string value = elements[i]->atributes[2][1];
                    checkboxY =  stoi(value);
                }
                if(elements[i]->atributes[3][0] == "xAxis"){
                    string value = elements[i]->atributes[3][1];
                    checkboxX =  stoi(value);
                }
                } else if(Element == "ifClickEvent"){
                    tuple<string, string, string, bool> received = devolveSolution(elements[i]->completeLine, clickId);
                    string content = get<0>(received);
                    wm_command_definitions += "\n" + content;
                    if(get<3>(received) == true){
                        secData += "\n " + get<1>(received);
                        string text = get<2>(received);
                        secData = replaceValue(secData, "textReplace", "\"" + text + "\"" + ", 0 \n");
                    }
                } else {
                cerr << "Unknowm Attribute " << Element << endl;
            }
        }
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
            windowcode = replaceValue(windowcode, "checkboxWidth", to_string(checkboxWidth));
            windowcode = replaceValue(windowcode, "checkboxHeight", to_string(checkboxHeigth));
            windowcode = replaceValue(windowcode, "checkboxX", to_string(checkboxX));
            windowcode = replaceValue(windowcode, "checkboxY", to_string(checkboxY));
        }
        codeFinal << definitions << head << secData << secBss << secText << messageListener << wm_command << wm_command_definitions << windowcode << ctlcolorStatic << footer;
        codeFinal.close();
    }
}


int main(int argc, char *argv[]){
    string line;
    string archive;
    AstElement* expression;
    int startline = 0;
    int count = 0;
    AstElement* elements[4];
    int validator = 0;
    if(argv[1] != NULL){
        ifstream file;
        const char* nameArchiveC = argv[1];
        file.open(nameArchiveC);
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
                        
                    }else {
                        elements[count] = expression;
                        count++;
                    }
                    validator = 0;
                }
                lineCout++;
            }
            generateCodeFinal(elements);
            string nameArchive(nameArchiveC);
            string comNasm = "nasm -f win64 codeFinal.asm -o " + nameArchive.erase(nameArchive.size() - 3) + ".obj > ./bin-helpers/logs/output-nasm.log";
            string comGolink = "golink /entry:Start kernel32.dll user32.dll gdi32.dll " + nameArchive+ ".obj > ./bin-helpers/logs/output-golink.log";
            int codeNasm = system(comNasm.c_str());
            int codeGolink = system(comGolink.c_str());
            if(codeNasm == 0 && codeGolink == 0){
                cout << "Programa " << nameArchive.erase(nameArchive.size()) << ".exe Criado com sucesso!" << endl;
                for (int i = 1; i < argc; ++i) {
                    if (strcmp(argv[i], "-r") == 0) {
                        cout << "Rodando ..." << endl;
                        string running = ".\\" + nameArchive.erase(nameArchive.size()) + ".exe";
                        system(running.c_str());
                    }
                }
                return 0;
            } else{
                cout << "Falha ao compilar :(, veja o que foi imprimido acima!" << endl;
                return 1;
            }
            cout << "Compilation terminated. : )" << endl;
        } else{
            cerr << "Failed to read the archive. : (" << endl;
            return 1;
        }
    }
}