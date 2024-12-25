#pragma once
#include "Token.h"
#include <iostream>
#include <fstream>
#include <iomanip>

int TABLE_SIZE = 10000;

class HashTable {
    Token* table;

    int hash(const string& lexeme) {
        int hash = 0;
        for (char c : lexeme) {
            hash = (hash * 31 + c) % TABLE_SIZE;
        }
        return hash;
    }

public:
    HashTable() {
        table = new Token[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i].lexeme = "";
            table[i].type = "";
        }
    }



    void resize(int newsize) {
        Token* newTable = new Token[newsize];
        for (int i = 0; i < newsize; i++) {
            newTable[i].lexeme = "";
            newTable[i].type = "";
        }

        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].lexeme != "") {
                int newIndex = hash(table[i].lexeme) % newsize;
                while (newTable[newIndex].lexeme != "") {
                    newIndex = (newIndex + 1) % newsize;
                }
                newTable[newIndex] = table[i];
            }
        }

        delete[] table;
        table = newTable;
        TABLE_SIZE = newsize;
    }


    void insert(Token token) {
        int index = hash(token.lexeme);
        while (table[index].lexeme != "" && token.lexeme != table[index].lexeme) {
            index++;
            if (index == TABLE_SIZE - 1) {
                index = 0;
            }
            else if (index == hash(token.lexeme)) {
                resize(TABLE_SIZE * 2);
                TABLE_SIZE *= 2;
            }
        }
        table[index].lexeme = token.lexeme;
        table[index].type = token.type;
    }

    void print(ofstream& outputFile) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].lexeme != "") {
                outputFile << setw(20) << table[i].type << " | " << setw(20) << table[i].lexeme << " | " << setw(20) << i << '\n';
            }
        }
    }
};