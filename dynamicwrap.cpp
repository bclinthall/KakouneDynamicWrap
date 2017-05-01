#include <iostream>
#include <string>
#include <regex>

// I need to not just trim but strip the whole indentation sequence from the front of
// the line.
const int SEARCH_BACK_DISTANCE = 30;
const int COLUMNS = 80;

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
int breakLine(int i, std::string& paragraph, std::string paragraphIndent){
    int startingPlace = i;
    i = searchBackwardForSpace(i, startingPlace, paragraph);
    if (paragraph[i] != ' '){
        i = startingPlace;
        i = searchForwardForSpace(i, paragraph);                
    }
    if (paragraph[i] == ' '){
        paragraph[i] = '\n';
        paragraph = paragraph.insert(i+1, paragraphIndent);
    }
    return i;
}
std::string processParagraph(std::string paragraph, std::string paragraphIndent, std::string result){
    int charCounter = 0;
    for (std::string::size_type i = 0; i < paragraph.size(); i++) {
        if (charCounter < COLUMNS) {
            charCounter++;
        } else {
            i = breakLine(i, paragraph, paragraphIndent);
            charCounter = 0;
        } 
    }
    rtrim(paragraph);
    return result + paragraph + "---\n";
}

std::string getIndent(std::string line) {
    std::smatch match;
    std::regex_match(line, match, std::regex("(( |>)*).*"));
    return match[1];
}
bool isIndentChange(std::string line, std::string paragraphIndent){
    std::string indent = getIndent(line);
    return indent != "" && indent != paragraphIndent;
}
bool isSpecial(std::string line){ 
    //return std::regex_match(line, std::regex("(\\s*|\\s*\\\\[a-zA-Z]+(\\[.*\\])*\\s*)*(\\{.*\\}\\s*)*)"));
    bool isEmptyLine = std::regex_match(line, std::regex("\\s*"));
    bool isTexCommand = std::regex_match(line, std::regex("\\s*\\\\[a-zA-Z]+(\\{.*\\})*\\s*"));
    return isEmptyLine || isTexCommand;
}
int main(){
    std::string line;
    std::string paragraph = "";
    std::string result = "";
    std::string paragraphIndent;
    while (getline(std::cin, line)) {
        if(paragraph == "") paragraphIndent = getIndent(line);
        bool isSpecialLine = isSpecial(line);
        bool isIndentChangeLine = isIndentChange(line, paragraphIndent);
        if (!isSpecialLine && !isIndentChangeLine){
            std::cout << "accumulate\n";
            // We are going to accumulate a paragraph of non-special lines.
            trim(line);
            paragraph += line + " ";
        } else {
            std::cout << "don't accumulate\n";
            if(isSpecialLine){
                std::cout << "isSpecialLine\n";
            }else {
                std::cout << "indentChange\n";
            }
            if(paragraph != ""){
                // Special lines (empty lines and tex commands) and indent changes 
                // signal that the paragraph we've been accumulating is done.  So, 
                // process the paragraph (if any) and add it to the result.
                result = processParagraph(paragraph, paragraphIndent, result);
                paragraph = "";
            }
            // Now deal with the line that signaled the end of the previous paragraph.
            // Either by making it the start of a new paragraph or by appending it to 
            // result.
            if(!isSpecialLine && isIndentChangeLine){
                // The new paragraph was triggered by an indent change. 
                // Make line the start of a new paragraph.
                paragraphIndent = getIndent(line);
                trim(line);
                paragraph += line + " ";
            } else {
                // If the new paragraph wasn't triggered by an indent change,
                // then it must have been triggered by a special line.
                // Append it directly to the result.
                result += line + "\n";
            }
        }
    }
    if(paragraph != ""){
        result = processParagraph(paragraph, paragraphIndent, result);
    }
    std::cout << result;
}
