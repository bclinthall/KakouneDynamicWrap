#include <iostream>
#include <string>
#include <regex>
using namespace std;
bool isSpecial(string line){
    if (line == "") {
        return true;
    }
    return regex_match(line, regex("\\s*\\\\[a-zA-Z]+\\{.*\\}\\s*"));
}
int main(){
    string line;
    getline(cin, line);
    if(isSpecial(line)){
        cout << "Special";
    }else{
        cout << "Not Special";
    }
}
