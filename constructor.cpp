#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <string>
using namespace std;

string sPath;
string argumentMake(string argument){
    cout << argument << endl;
    if(argument != "!type Console"){
        sPath = "s.cpp";
    }
    if(argument == "!type Windowed"){
        sPath = "s-windowed.cpp";
     }
    else{
        sPath = "s.cpp";
        cout << "O programa foi definido como Console devido a argumentos invalidos! " << argument << endl;
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " <command>" << std::endl;
        return 1;
    }
    string name = argv[1];
    cout << name << std::endl;
    
    string line;
    string sline;
    string code;
    string scode;
    ifstream file;
    file.open(name);

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line[0] != '!') {
                code += line + '\n';
            }
            else{
                argumentMake(line);
            }
        }
    }

    ifstream sfi(sPath);
    if (sfi.is_open()){
        while (getline(sfi, sline)){
            scode += sline + '\n';
        }
    }
    std::ofstream final;
    final.open("code.cpp");
    if(final.is_open()){
        final << scode << std::endl;
        final << code;
    }
    ifstream path("path.txt");
    string pline;
    getline(path, pline);
    string cmnd = "g++ " + pline + "code.cpp -o " + pline + "output";
    const char* command = cmnd.c_str();
    std::cout << "Usando: " << command << endl;
    int check = system(command);
    if (check != 0){
        std::cout << "A compilacao falhou :( Mas tente rodar o comando: " << cmnd << std::endl;
        return 1;
    }
    file.close();
    final << "" << std::endl;
    final.close();
    sfi.close();
    if(check == 0) {
        std::cout << "O construtor concluiu as operacoes! ;)" << std::endl;
        return 0;
    }
}