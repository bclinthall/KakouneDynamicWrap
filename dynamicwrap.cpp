#include <iostream>
#include <string>
#include <sstream>
#include <regex>

// I need to not just trim but strip the whole indentation sequence from the front of
// the line.
const int SEARCH_BACK_DISTANCE = 30;
const int COLUMNS = 80;

// http://stackoverflow.com/a/217605 for the next three functions
// trim from start (in place)
static inline void ltrim(std::string &s, std::string indent) {
    s.erase(s.begin(), indent.end());
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s, std::string indent) {
    ltrim(s, indent);
    rtrim(s);
}

std::string getIndent(std::string line) {
    std::smatch match;
    std::regex_match(line, match, std::regex("(( |>)*).*"));
    return match[1];
}
int searchBackwardForSpace(int i, int startingPlace, std::string paragraph){
    //search backward up to SEARCH_BACK_DISTANCE char for a space.
    while (i > (startingPlace - SEARCH_BACK_DISTANCE) && paragraph[i] != ' '){
        i--;
    }
    return i;
}
int searchForwardForSpace(int i, std::string paragraph){
    while (i < paragraph.size() && paragraph[i] != ' '){
        i++;
    }
    return i;
}
int breakLine(int i, std::string& paragraph, std::string indent){
    int startingPlace = i;
    i = searchBackwardForSpace(i, startingPlace, paragraph);
    if (paragraph[i] != ' '){
        i = startingPlace;
        i = searchForwardForSpace(i, paragraph);                
    }
    if (paragraph[i] == ' '){
        paragraph[i] = '\n';
        paragraph = paragraph.insert(i+1, indent);
    }
    return i;
}
std::string processParagraph(std::string paragraph){
    std::string indent = getIndent(paragraph);
    int charCounter = 0;
    for (std::string::size_type i = 0; i < paragraph.size(); i++) {
        if (charCounter < COLUMNS) {
            charCounter++;
        } else {
            i = breakLine(i, paragraph, indent);
            charCounter = 0;
        } 
    }
    rtrim(paragraph);
    return paragraph + "\n";
}

bool isIndentChange(std::string line, std::string prevIndent){
    std::string indent = getIndent(line);
    return indent != "" && indent != prevIndent;
}
bool isSpecial(std::string line){ 
    //return std::regex_match(line, std::regex("(\\s*|\\s*\\\\[a-zA-Z]+(\\[.*\\])*\\s*)*(\\{.*\\}\\s*)*)"));
    bool isEmptyLine = std::regex_match(line, std::regex("\\s*"));
    bool isTexCommand = std::regex_match(line, std::regex("\\s*\\\\[a-zA-Z]+(\\{.*\\})*\\s*"));
    return isEmptyLine || isTexCommand;
}

/**
 * The goal of unwrap is to take the standard input and return 
 * a string that only has line breaks where they are syntactically
 * meaningful and none that are just for wrapping.
 */
std::string unwrap(){
    
    std::string line;
    std::string result = "";
    std::string indent;
    bool breakBefore = false;
    bool breakAfter = false;
    if(getline(std::cin, line)){
        indent = getIndent(line);
        result += line;
    }
    while (getline(std::cin, line)) {
        // Special lines (blank lines and tex commands) will 
        // always be preceeded and followed with a line break.  
        // If there is a change of indent (other than a change 
        // to zero indent) then we will begin a new line.
        bool isSpecialLine = isSpecial(line);
        bool isIndentChangeLine = isIndentChange(line, indent);
        if(isSpecial(line)){
            breakBefore = true;
            breakAfter = true;
        }else if(isIndentChange(line, indent)){
            breakBefore = true;
        }
        if(breakBefore){
            rtrim(line);
            result += "\n" + line;
        }else{
            trim(line, indent);
            result += line; 
        }
        if(breakAfter){
            breakBefore = true;
        } else {
            breakBefore = true;
        }
        breakAfter = false;
    }
    return result + "\n";
}
std::string wrap(){
    std::string unwrapped = unwrap();
    std::stringstream ss;
    ss.str(unwrapped);
    std::string line;
    std::string result = "";
    while(getline(ss, line)){
        if(isSpecial(line)){
            result += line + "\n";
        }else{
            result += processParagraph(line);
        }
    }
    return result;
}

int main(){
    std::cout << unwrap();
}
