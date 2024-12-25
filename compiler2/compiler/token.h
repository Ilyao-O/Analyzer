#pragma once
#include <string>

using namespace std;

class Token {
public:
    string type;
    string lexeme;

    Token(){}

    Token(const string& type, const string& lexeme): type(type), lexeme(lexeme) {}

};