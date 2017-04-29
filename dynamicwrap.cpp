#include <iostream>
#include <string>
#include <regex>
using namespace std;

// a function called 'searchForwardForSpace' and another
// called 'searchBackwardForSpace' would be helpful.
string processParagraph(string paragraph){
    int charCounter = 0;
    for (std::string::size_type i = 0; i < paragraph.size(); i++) {
        if (charCounter < 80) {
            charCounter++;
        } else {
            int startingPlace = i;
            //search backward up to 30 char for a space.
            while (i > startingPlace - 30 && paragraph[i] != ' '){
                i--;
            }
            if (paragraph[i] != ' '){
                // if we didn't find one, go back to startingPlace
                // and search forward for a space. 
                i = startingPlace;
                while (i < paragraph.size() && paragraph[i] != ' '){
                    i++;
                }
            }
            if (paragraph[i] == ' '){
                // we found a space
                //if (i>0 && paragraph[i-1] == u'') {
                //    paragraph = paragraph.insert(i-1, "_\n");
                //}else{
                    paragraph[i] = '\n';
                //}
                charCounter = 0;
            }
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
    string result = "";
    while (getline(cin, line)) {
        if (!isSpecial(line)){
            // We are going to accumulate a paragraph of non-special lines.
            trim(line);
            paragraph += line + " ";
        } else {
            // Special lines (empty lines and tex commands) signal that the paragraph
            // we've been accumulating is done.  So, process the 
            // paragraph and add it to the result.
            // Then throw the special line on the result.
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
        result += paragraph;
    }
    cout << result;
}
