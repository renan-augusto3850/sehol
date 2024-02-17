#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <string>
#include <tuple>
using namespace std;

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

tuple<string, string, string, bool> devolveSolution(string name, string id){
    tuple<string, string, string, bool> content;
    if(name.find("drawMessageBox") != string::npos){
        size_t pos1 = name.find("(");
        size_t pos2 = name.find(")");
        string text = name.substr(pos1 + 1, pos2 - pos1 - 1);
        size_t pose1 = name.find(",");
        size_t pose2 = name.find(")");
        string actions = name.substr(pose1 + 1, pose2 - pose1 - 1);
        cout << "Action: " << actions << endl;
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

int main(int argc, char *argv[]){
    if(argv[1] != NULL) {
        ifstream file;
        string definitions = getPrefixes("definitions.pref");
        string head = getPrefixes("head.pref");
        string secData = getPrefixes("section-data.pref");
        string secBss = getPrefixes("section-bss.pref");
        string secText = getPrefixes("section-text.pref");
        string footer = getPrefixes("final.pref");
        string messageListener = getPrefixes("messagesListener.pref");
        string wm_command = getPrefixes("wmcommand.pref");
        string ctlcolorStatic = getPrefixes("ctlcolorStatic.pref");
        string windowcode;
        string line;
        string codeColor = "0xFFFFFF";
        const char* drawWindow = "drawWindow";
        const char* drawText = "drawText";
        const char* drawButton = "drawButton";
        const  char* nameArchiveC = argv[1];
        string resultName;
        string resultCommaWidth = "500";
        string resultCommaHeigth = "500";
        int ButtonWidth = 300;
        int ButtonHeigth = 100;
        int ButtonX = 10;
        int ButtonY = 10;
        string completeline;
        string erasedLine;
        string type = "window";
        string resultText;
        string ButtonId;
        bool haveText = false;
        bool isBehind = false;
        string nameArchive(nameArchiveC);
        string wm_command_definitions;
        file.open(nameArchive);
        ofstream codeFinal("codeFinal.asm");
        string programName = nameArchive.erase(nameArchive.size() - 3);
        int braceFinded = 0;
        string element;
        string textColour = "0x000000";
        string textBackColor = "0xFFFFFF";
        string TextId;
        string clickHereId;
        string resultButton;
        int generator = 104;
        if(file.is_open()){
            while(getline(file, line)){
                cout << line << endl;
                if(line.find(drawWindow) == 0 && isBehind == false){
                    type = "window";
                    completeline = line.substr(11);
                    erasedLine = completeline.erase(completeline.size() - 3);
                    cout << "line command: " << completeline << " Size: " << completeline.size() << endl;
                    int indexName1 = completeline.find("\"");
                    int indexName2 = completeline.find("\"", indexName1 + 1);
                    resultName = completeline.substr(indexName1 + 1, indexName2 - indexName1 - 1);
                    int indexCommaWidth1 = completeline.find(",");
                    int indexCommaWidth2 = completeline.find(",", indexCommaWidth1 + 1);
                    resultCommaWidth = completeline.substr(indexCommaWidth1 + 1, indexCommaWidth2 - indexCommaWidth1 - 1);
                    resultCommaHeigth = completeline.substr(indexCommaWidth2 + 2);
                    cout << "Você criou o elemento drawWindow com: " << resultName << " " << resultCommaWidth << " " << resultCommaHeigth << endl;
                    windowcode += getPrefixes("drawWindow.pref");
                    if(completeline.find("{")){
                        braceFinded = 1;
                        element = "drawWindow";
                    }
                }
                if(line.find(drawText) == 0 && isBehind == false){
                    type = "window";
                    completeline = line.substr(9);
                    erasedLine = completeline.erase(completeline.size() - 4);
                    cout << "line command: " << completeline << " Size: " << completeline.size() << endl;
                    int indexText1 = completeline.find("\"");
                    int indexText2 = completeline.find("\"", indexText1);
                    resultText = "\"" + completeline.substr(indexText1 + 1, indexText2 - indexText1 - 1) + "\"";
                    size_t pos1 = line.find(" ");
                    size_t pos2 = line.find("(");
                    TextId = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    cout << "Você criou o elemento drawText com: " << resultText << " E id: " << TextId << endl;
                    windowcode += getPrefixes("drawText.pref");
                    secData += " " + TextId + "Text      db " + resultText + ", 0 \n";
                    secData += " " + TextId + "Colour       dd " + textColour + ", 0 \n";
                    secData += " " + TextId + "BackColour       dd " + textBackColor + ", 0 \n";
                    //windowcode.replace(windowcode.find("textReplace"), 11, TextId);
                    windowcode = replaceValue(windowcode, "textReplace", TextId + "Text");
                    wm_command_definitions += replaceValue(getPrefixes("textClass.pref"), "textReplace", TextId + "Text");
                    //windowcode.replace(windowcode.find("textIdReplace"), 13, TextId);
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
                    if(completeline.find("{")){
                        braceFinded = 1;
                        element = "drawText";
                    }
                }
                if(line.find(drawButton) == 0 && isBehind == false){
                    type = "window";
                    completeline = line.substr(9);
                    erasedLine = completeline.erase(completeline.size() - 4);
                    cout << "line command: " << completeline << " Size: " << completeline.size() << endl;
                    int indexText1 = completeline.find("\"");
                    int indexText2 = completeline.find("\"", indexText1);
                    resultButton = "\"" + completeline.substr(indexText1 + 1, indexText2 - indexText1 - 1) + "\"";
                    size_t pos1 = line.find(" ");
                    size_t pos2 = line.find("(");
                    ButtonId = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    cout << "Você criou o elemento drawButton com: " << resultButton << " E id: " << ButtonId << endl;
                    windowcode += getPrefixes("drawButton.pref");
                    definitions += "\n" + ButtonId + "Id         EQU " + to_string(generator);
                    secBss += "\n" + ButtonId + "           resq 1 \n";
                    generator++;
                    secData += " " + ButtonId + "Text         db " + resultButton + ", 0 \n";
                    secData += " buttonClass             db \"BUTTON\", 0 \n";
                    windowcode = replaceValue(windowcode, "buttonText", ButtonId + "Text");
                    windowcode = replaceValue(windowcode, "buttonId", ButtonId + "Id");
                    if(completeline.find("{")){
                        braceFinded = 1;
                        element = "drawButton";
                    }
                }
                if(line.find("@behind") != string::npos){
                    isBehind = true;
                }
                if(line.find("ifClickEvent") == 0){
                    completeline = line.substr(12);
                    cout << "Um evento de clique com: " << completeline << endl;
                    size_t pos1 = line.find("(");
                    size_t pos2 = line.find(")");
                    string clickHereEventId = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    cout << "Aplicando ação caso seja clicado " << clickHereEventId << endl;
                    wm_command += "\n" + getPrefixes("ifClickevent.pref");
                    wm_command = replaceValue(wm_command, "elementId", clickHereEventId + "Id");
                    wm_command = replaceValue(wm_command, "elementClass", clickHereEventId);
                    wm_command_definitions += "." + clickHereEventId + ": \n";
                    if(line.find("{") != string::npos){
                        braceFinded = 1;
                        element = "ifClickEvent";
                        size_t pose1 = line.find("(");
                        size_t pose2 = line.find(")");
                        clickHereId = line.substr(pose1 + 1, pose2 - pose1 - 1);
                    }
                }
                if(braceFinded == 1){
                    if(line == "}"){
                        braceFinded = 0;
                        cout << "Estilos aplicados a " << element << endl;
                        element = "";
                    } else
                    if(element == "drawWindow"){
                        cout << "Line Brace: " << line << endl;
                        if(line.find("background-color")){
                            int indexColor1 = line.find("\"");
                            int indexColor2 = line.find("\"", indexColor1 + 1);
                            string color = line.substr(indexColor1 + 1, indexColor2 - indexColor1 - 1);
                            cout << color << endl;
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
                    }
                    if(element == "drawButton") {
                        if(line.find("width") != string::npos){
                            string value = line.substr(10);
                            cout << "Você definiu drawButton com a largura de: " << value << endl;
                            ButtonWidth =  stoi(value);
                        }
                        if(line.find("heigth") != string::npos){
                            string value = line.substr(11);
                            cout << "Você definiu drawButton com a altura de: " << value << endl;
                            ButtonHeigth =  stoi(value);
                        }
                        if(line.find("y") != string::npos){
                            string value = line.substr(6);
                            cout << "Você definiu drawButton com eixo Y de: " << value << endl;
                            ButtonY =  stoi(value);
                        }
                        if(line.find("x") != string::npos){
                            string value = line.substr(6);
                            cout << "Você definiu drawButton com eixo X de: " << value << endl;
                            ButtonX =  stoi(value);
                        }
                    } 
                    if(element == "ifClickEvent"){
                        tuple<string, string, string, bool> received = devolveSolution(line, clickHereId);
                        string content = get<0>(received);
                        wm_command_definitions += "\n" + content;
                        cout << get<1>(received) << get<2>(received) << get<3>(received) << endl;
                        if(get<3>(received) == true){
                            secData += "\n " + get<1>(received);
                            string text = get<2>(received);
                            size_t pos1 = line.find("\"");
                            size_t pos2 = line.find(",");
                            string finaltext = line.substr(pos1 + 1, pos2 - pos1 - 1);
                            secData = replaceValue(secData, "textReplace", "\"" + finaltext + ", 0 \n");
                        }
                    } else {
                        cout << "Estilo não encontrado" << endl;
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
                    }
                    codeFinal << definitions << head << secData << secBss << secText << messageListener << wm_command << wm_command_definitions << windowcode << ctlcolorStatic << footer;
                    codeFinal.close();
                    string comNasm = "nasm -f win64 codeFinal.asm -o " + programName + ".obj";
                    string comGolink = "golink /entry:Start kernel32.dll user32.dll gdi32.dll " + programName + ".obj";
                    int codeNasm = system(comNasm.c_str());
                    int codeGolink = system(comGolink.c_str());
                    if(codeNasm == 0 && codeGolink == 0){
                        cout << "Programa " << programName << ".exe Criado com sucesso!" << endl;
                        return 0;
                    } else{
                        cout << "Falha ao compilar :(, veja o que foi imprimido acima!" << endl;
                        return 1;
                    }
                } else{
                    cerr << "Erro ao escrever um arquivo!" << endl;
                    return 1;
                }
            }
    } else{
        cerr << "Nenhum arquivo foi passado: seholC <nome_do_arquivo>";
        return 1;
    }
}