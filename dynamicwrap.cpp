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
    return regex_match(line, regex("(\\s*|\\s*\\\\[a-zA-Z]+(\\{.*\\})?\\s*)"));
}

// http://stackoverflow.com/a/217605 for the next three functions
// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
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
            trim(line);
            paragraph += line + " ";
        }
    }
    if(paragraph != ""){
        paragraph = processParagraph(paragraph);
        text += paragraph + "\n";
    }
    cout << text;
}
