#include <iostream>
#include <string>
#include <regex>
using namespace std;
string processParagraph(string paragraph){
    int linecounter = 0;
    for (std::string::size_type i = 0; i < paragraph.size(); i++) {
        if (i > 0 && linecounter == 80) {
            int place = i;
            while (i > place - 30 && paragraph[i] != ' '){
                i--;
            }
            if (paragraph[i] != ' '){
                i = place;
                while (i < paragraph.size() && paragraph[i] != ' '){
                    i++;
                }
            }
            if (paragraph[i] == ' '){
                paragraph[i] = '\n';
                linecounter = 0;
            }
        } else {
            linecounter++;
        }
    }
    return paragraph;
}
bool isSpecial(string line){
    if (line == "") {
        return true;
    }
    return regex_match(line, regex("\\s*\\\\[a-zA-Z]+\\{.*\\}\\s*"));
}
int main(){
    string line;
    string paragraph = "";
    string text = "";
    while (getline(cin, line)) {
        if (isSpecial(line)){
            if(paragraph != ""){
                paragraph = processParagraph(paragraph);
                text += paragraph + "\n";
                paragraph = "";
            }
            text += line + "\n";
        } else {
            paragraph += line + " ";
        }
    }
    if(paragraph != ""){
        paragraph = processParagraph(paragraph);
        text += paragraph + "\n";
    }
    text = text.erase(text.size()-1);
    cout << text;
}
