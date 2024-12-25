#pragma once
#include <fstream>
#include <cctype>
#include "dfaparams.h"
#include "Token.h"
#include "dfa.h"
#include "HashTable.h"
#include "Syntax_Analyzer.h"

using namespace std;

class Lexer
{
    ifstream& inputFile;

    int line = 1;

    bool isWhitespace(char c) {
        return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v';
    }
    void islexeme(const string& lexeme, HashTable& hashTable, vector<string>& errorMessages)
    {
        try
        {
            if (D.isAccept(lexeme))
            {
                if (lexeme == "procedure") {
                    Token token("Procedure", lexeme);
                    currentType = "Procedure";
                    hashTable.insert(token);
                }
                else if (lexeme == "end") {
                    Token token("End", lexeme);
                    currentType = "End";
                    hashTable.insert(token);
                } 
                else if (lexeme == "begin") {
                    Token token("Begin", lexeme);
                    currentType = "Begin";
                    hashTable.insert(token);
                }
                else if (lexeme == "var") {
                    Token token("Begin", lexeme);
                    currentType = "Begin";
                    hashTable.insert(token);
                }
                else if (lexeme == "integer") {
                    Token token("integer", lexeme);
                    currentType = "integer";
                    hashTable.insert(token);
                }
                else if (lexeme == "repeat") {
                    Token token("repeat", lexeme);
                    currentType = "repeat";
                    hashTable.insert(token);
                }
                else if(lexeme == "until" ) {
                    Token token("until", lexeme);
                    currentType = "until";
                    hashTable.insert(token);
                }
                else if (lexeme == "-" || lexeme == "+" ||
                    lexeme == "<" || lexeme == ">" ||
                    lexeme == "=" || lexeme == ":=" ||
                    lexeme == "<>") {
                    Token token("operator", lexeme);
                    currentType = "operator";
                    hashTable.insert(token);
                }
                else if (lexeme == "(" || lexeme == ")" || lexeme == ";" || lexeme == ":" || lexeme == ",") {
                    Token token("Delimiter", lexeme);
                    currentType = "Delimiter";
                    hashTable.insert(token);
                }
                else if (isdigit(lexeme[0])) {
                    Token token("int_num", lexeme);
                    currentType = "int_num";
                    hashTable.insert(token);
                }
                else {
                    Token token("id_name", lexeme);
                    currentType = "id_name";
                    hashTable.insert(token);
                }
            }
            else {
                errorMessages.push_back(lexeme);
                currentType = "ERROR";
            }
        }
        catch (...) {
            errorMessages.push_back(lexeme);
            currentType = "ERROR";
        }
    }



public:
    vector<string> errorMessages;
    HashTable& hashTable;
    dfa D;
    string currentType;
    string currentLexeme;
    char c;
    bool g = true;
    Lexer(ifstream& inputFile, HashTable& hashTable)
        : inputFile(inputFile), hashTable(hashTable), currentType("ERROR"), currentLexeme(""), c(' '), D(6, alph, finalst, transit)
    {}


    void nextlexeme() {
        string curLex = "";
        int state = 1;
        bool f = true;
        while (f && !inputFile.eof())
        {
            if (g) {
                if (inputFile.get(c)) {
                    f = true;
                }
                else {
                    break;
                }
            }
            g = true;
            switch (state)
            {
            case 1:
                if (isWhitespace(c)) {
                    if (!curLex.empty())
                    {
                        islexeme(curLex, hashTable, errorMessages);
                        f = false;
                    }
                    if (c == '\n') line++;
                    continue;
                }
                else if (c == ';' || c == '(' || c == ')' || c == '=' ||
                    c == '+' || c == '-' || c == ',' || c == '>') {
                    curLex += c;
                    islexeme(curLex, hashTable, errorMessages);
                    f = false;
                }
                else if (c == ':') {
                    curLex += c;
                    state = 2;
                }
                else if (c == '<') {
                    curLex += c;
                    state = 3;
                }
                else if (isalpha(c) || isdigit(c)) {
                    curLex += c;
                    state = 4;
                }
                else {
                    curLex += c;
                }
                
                break;

            case 2:
                if (c == '=')
                {
                    curLex += c;
                    islexeme(curLex, hashTable, errorMessages);
                    f = false;
                }
                else if (isWhitespace(c))
                {
                    islexeme(curLex, hashTable, errorMessages);
                    f = false;
                    if (c == '\n') line++;
                }
                else {
                    curLex += c;
                    state = 4;
                }
                break;

            case 3:
                if (c == '>') {
                    curLex += c;
                    islexeme(curLex, hashTable, errorMessages);
                    f = false;
                }
                else {
                    islexeme(curLex, hashTable, errorMessages);
                    f = false;
                    if (c == '\n') line++;
                }
                break;

            case 4:
                if (isalpha(c) || isdigit(c) ||
                    !(c == ';' || c == '(' || c == ')' ||
                        c == '=' || c == '+' || c == '-' ||
                        c == ':' || c == '<' || c == '>' ||
                        c == ',' || isWhitespace(c)))
                {
                    curLex += c;
                }
                else
                {
                    islexeme(curLex, hashTable, errorMessages);
                    f = false;
                    g = false;
                }
                break;
            }
        }
        if (curLex != "") {
            islexeme(curLex, hashTable, errorMessages);
        }
        currentLexeme = curLex;
    }

    void analyze(Node& n) {
        Syntactic_analysis s;
        while (!inputFile.eof()) {
            nextlexeme();
            s.setlex(currentLexeme, currentType);
            s.parse(n,line);
        }
    }

    void showErrors(ofstream& outputFile)
    {
        if (!errorMessages.empty()) {
            for (const auto& error : errorMessages) {
                outputFile << "Некорректная лексема: " << error << '\n';
            }
        }
        else {
            outputFile << "OK";
        }
    }
};
