#include <iostream>
#include <string>
#include <regex>

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
int breakLine(int i, std::string& paragraph){
    int startingPlace = i;
    i = searchBackwardForSpace(i, startingPlace, paragraph);
    if (paragraph[i] != ' '){
        i = startingPlace;
        i = searchForwardForSpace(i, paragraph);                
    }
    if (paragraph[i] == ' '){
        paragraph[i] = '\n';
    }
    return i;
}
std::string processParagraph(std::string paragraph){
//    int charCounter = 0;
//    for (std::string::size_type i = 0; i < paragraph.size(); i++) {
//        if (charCounter < COLUMNS) {
//            charCounter++;
//        } else {
//            i = breakLine(i, paragraph);
//            charCounter = 0;
//        } 
//    }
    rtrim(paragraph);
    return paragraph;
}



bool isSpecial(std::string line){
    //return std::regex_match(line, std::regex("(\\s*|\\s*\\\\[a-zA-Z]+(\\[.*\\])*\\s*)*(\\{.*\\}\\s*)*)"));
    return std::regex_match(line, std::regex("(\\s*|\\s*\\\\[a-zA-Z]+(\\{.*\\})?\\s*)"));
}
int main(){
    std::string line;
    std::string paragraph = "";
    std::string result = "";
    while (getline(std::cin, line)) {
        if (!isSpecial(line)){
            // We are going to accumulate a paragraph of non-special lines.
            trim(line);
            paragraph += line + " ";
        } else {
            // Special lines (empty lines and tex commands) signal that the paragraph
            // we've been accumulating is done.  So, process the 
            // paragraph and add it to the result.
            // Then append the special line to the result.
            if(paragraph != ""){
                paragraph = processParagraph(paragraph);
                result += paragraph + "\n";
                paragraph = "";
            }
            result += line + "\n";
        }
    }
    if(paragraph != ""){
        paragraph = processParagraph(paragraph);
        result += paragraph + "\n";
    }
    std::cout << result;
}
