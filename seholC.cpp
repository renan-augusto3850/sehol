#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <string>
using namespace std;

string replaceValue(string code, string value, string data) {
    size_t pos = 0;
    while ((pos = code.find(value, pos)) != string::npos) {
        code.replace(pos, value.length(), data);
        pos += data.length();
    }
    return code;
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

int main(int argc, char *argv[]){
    if(argv[1] != NULL) {
        ifstream file;
        string definitions = getPrefixes("definitions.pref");
        string head = getPrefixes("head.pref");
        string secData = getPrefixes("section-data.pref");
        string secBss = getPrefixes("section-bss.pref");
        string secText = getPrefixes("section-text.pref");
        string footer = getPrefixes("final.pref");
        string code;
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
        file.open(nameArchive);
        ofstream codeFinal("codeFinal.asm");
        string programName = nameArchive.erase(nameArchive.size() - 3);
        int braceFinded = 0;
        string element;
        string textColour = "0x000000";
        string textBackColor = "0xFFFFFF";
        string TextId;
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
                    code += getPrefixes("drawWindow.pref");
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
                    code += getPrefixes("drawText.pref");
                    secData += " " + TextId + "Text      db " + resultText + ", 0 \n";
                    secData += " " + TextId + "Colour       dd " + textColour + ", 0 \n";
                    secData += " " + TextId + "BackColour       dd " + textBackColor + ", 0 \n";
                    //code.replace(code.find("textReplace"), 11, TextId);
                    code = replaceValue(code, "textReplace", TextId + "Text");
                    //code.replace(code.find("textIdReplace"), 13, TextId);
                    code = replaceValue(code, "textColour", TextId + "Colour");
                    code = replaceValue(code, "textBackColourA", TextId + "BackColour");
                    definitions += TextId + "Id            EQU " + to_string(generator);
                    generator++;
                    code = replaceValue(code, "textId", TextId + "Id");
                    footer = replaceValue(footer, "textId", TextId + "Id");
                    footer = replaceValue(footer, "textclass", TextId);
                    secBss = replaceValue(secBss, "textclass", TextId);
                    code = replaceValue(code, "textclass", TextId);
                    footer = replaceValue(footer, "textColorReplace", "static" + TextId);
                    footer = replaceValue(footer, "textColReplace", TextId + "Colour");
                    footer = replaceValue(footer, "textBackReplace", TextId + "BackColour");

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
                    code += getPrefixes("drawButton.pref");
                    definitions += "\n" + ButtonId + "Id         EQU " + to_string(generator);
                    secBss += "\n" + ButtonId + "           resq 1";
                    generator++;
                    secData += " " + ButtonId + "Text         db " + resultButton + ", 0 \n";
                    secData += " buttonClass             db \"BUTTON\", 0 \n";
                    code = replaceValue(code, "buttonText", ButtonId + "Text");
                    code = replaceValue(code, "buttonId", ButtonId + "Id");
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
                    } else {
                        cout << "Estilo não encontrado" << endl;
                    }
                }
            }
                /*if(codeFinal.is_open()){
                    if(type == "window"){
                        definitions = replaceValue(definitions, "widthReplace", resultCommaWidth);
                        definitions = replaceValue(definitions, "heigthReplace", resultCommaHeigth);
                        secData.replace(secData.find("nameReplace"), 11, '"' + resultName + '"');
                        secData.replace(secData.find("backReplace"), 11, codeColor);
                        secData.replace(secData.find("backTReplace"), 12, codeColor);
                        code = replaceValue(code, "buttonWidth", to_string(ButtonWidth));
                        code = replaceValue(code, "buttonHeigth", to_string(ButtonHeigth));
                        code = replaceValue(code, "buttonX", to_string(ButtonX));
                        code = replaceValue(code, "buttonY", to_string(ButtonY));
                    }
                    codeFinal << definitions << head << secData << secBss << secText << code << footer;
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
                }*/
            }
    } else{
        cerr << "Nenhum arquivo foi passado: seholC <nome_do_arquivo>";
        return 1;
    }
}