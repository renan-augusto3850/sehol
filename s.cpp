#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
using namespace std;

void printLog(string frase) {
    std::cout << frase << std::endl;
}
string inputStr(string texto){
    string value;
    cout << texto;
    cin >> value;
    return value;
}
int inputInt(string textoN){
    int num;
    cout << textoN;
    cin >> num;
    return num;
}
int Exit(){
    return 0;
}