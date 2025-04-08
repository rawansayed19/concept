#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define END_OF_FILE -1

#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

int charClass;
string lexeme;
char nextChar;
int nextToken;
ifstream inFile;

void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);

void addChar() {
    lexeme += nextChar;
}

void getChar() {
    if (inFile.get(nextChar)) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else {
        charClass = END_OF_FILE;
    }
}

void getNonBlank() {
    while (isspace(nextChar)) {
        getChar();
    }
}

int lookup(char ch) {
    switch (ch) {
    case '(': addChar(); return LEFT_PAREN;
    case ')': addChar(); return RIGHT_PAREN;
    case '+': addChar(); return ADD_OP;
    case '-': addChar(); return SUB_OP;
    case '*': addChar(); return MULT_OP;
    case '/': addChar(); return DIV_OP;
    case '=': addChar(); return ASSIGN_OP; // Handle assignment operator
    default:
        addChar();
        return UNKNOWN; // Return UNKNOWN for characters that don't match predefined ones
    }
}

int lex() {
    lexeme.clear();
    getNonBlank();

    switch (charClass) {
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = IDENT;
        break;

    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;

    case UNKNOWN:
        nextToken = lookup(nextChar);
        getChar();
        break;

    case END_OF_FILE:
        nextToken = END_OF_FILE;
        lexeme = "EOF";
        break;
    }

    cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;
    return nextToken;
}

int main() {
    inFile.open("front.in");

    if (!inFile) {
        cerr << "ERROR - cannot open front.in" << endl;
        return 1;
    }

    getChar();
    do {
        lex();
    } while (nextToken != END_OF_FILE);

    inFile.close();
    return 0;
}
