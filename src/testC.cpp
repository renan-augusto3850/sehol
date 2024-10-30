#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/FileSystem.h"

#include <cstdlib>
using namespace std;

void compileLLVMIRToExecutable(const std::string &irFile, const std::string &output) {
    cout << "Compiling";
    // Compilar IR para código de máquina
    std::string llcCommand = "llc " + irFile + " -o " + output + ".s";
    std::system(llcCommand.c_str());

    // Compilar código de máquina para executável
    std::string gppCommand = "g++ " + output + ".s -o " + output;
    std::system(gppCommand.c_str());
}
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

struct variables{
    string name;
    string value;
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
        cout << "Tokenizing";
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
    cout << "Parsing";
    AstElement* element = new AstElement();
    variables var;
     for(int i = 0; i < tokens.size(); i++) {
        cout << "[ " << tokens[i] << " ]";
     }
     cout << endl;   
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
        cout << elements->type;
        if(elements->type == "printLogConsole") {
            cout << "Gere";
            llvm::LLVMContext context;
            llvm::Module module("MyModule", context);
            llvm::IRBuilder<> builder(context);

            // Simulação da extração de string
            std::string extractedString = elements->params; // Esta string deve vir da sua lógica de tokens

            // Declaração da função printf
            llvm::FunctionType* printfType = llvm::FunctionType::get(builder.getInt32Ty(), builder.getInt8PtrTy(), true);
            llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module);

            // Criar a função main
            llvm::FunctionType* mainType = llvm::FunctionType::get(builder.getInt32Ty(), false);
            llvm::Function* mainFunc = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, "main", module);
            llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
            builder.SetInsertPoint(entry);

            // Criar a string a partir de extractedString
            llvm::Value* helloStr = builder.CreateGlobalStringPtr(extractedString);

            // Chamar printf
            builder.CreateCall(printfType, llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", module), helloStr);

            // Retornar 0
            builder.CreateRet(builder.getInt32(0));

            // Salvar o módulo em um arquivo LLVM IR
            std::string irFile = "my_module.ll";
            std::error_code EC;
            llvm::ToolOutputFile out(irFile, EC, llvm::sys::fs::OF_None);
            module.print(out.os(), nullptr);
            out.keep();

            // Compilar o LLVM IR para um executável
            compileLLVMIRToExecutable(irFile, "my_program");
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
    cout << argv[0];
    if(argc > 1){
        ifstream file;
        const char* nameArchiveC = argv[1];
        cout << argv[1];
        file.open(nameArchiveC);
        int lineCout = 1;
        if(file.is_open()){
            cout << argv[1];
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