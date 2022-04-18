#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    std::string str_no_space;
    for (auto letter: str){
        if (letter != ' ') {
            str_no_space += letter;
        }
    }
    std::string str_no_space_reverse = str_no_space;
    std::reverse(str_no_space_reverse.begin(), str_no_space_reverse.end());
    return str_no_space_reverse == str_no_space;
}
