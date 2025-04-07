/* lexical_analyzer.cpp - A simple lexical analyzer using a state transition diagram in C++ */
#include <iostream>
#include <cctype>
#include <string>

// Global Declarations
int charClass;
std::string lexeme;
char nextChar;
int lexLen;
int nextToken;

// Hardcoded input string and index to traverse it
std::string inputString = "9+420(C-1)A*B";
size_t inputIndex = 0;

// Character Classes
#define LETTER 0
#define DIGIT  1
#define UNKNOWN 99

// Token Codes
#define INT_LIT      10
#define IDENT        11
#define ASSIGN_OP    20
#define ADD_OP       21
#define SUB_OP       22
#define MULT_OP      23
#define DIV_OP       24
#define LEFT_PAREN   25
#define RIGHT_PAREN  26

// Function Declarations
void addChar();
void getChar();
void getNonBlank();
int lookup(char ch);
int lex();

int main() {
    // Initialize by reading the first character from inputString
    getChar();
    // Continuously call lex() until EOF (represented by -1) is reached
    do {
        lex();
    } while (nextToken != -1);
    
    return 0;
}

/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch) {
    switch(ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        default:
            addChar();
            nextToken = -1; // Unrecognized character; treat as EOF for simplicity
            break;
    }
    return nextToken;
}

/* addChar - a function to add nextChar to lexeme */
void addChar() {
    // Append the character to lexeme
    lexeme.push_back(nextChar);
    lexLen++;
}

/* getChar - a function to get the next character from the input string and determine its character class */
void getChar() {
    if (inputIndex < inputString.size()) {
        nextChar = inputString[inputIndex++];
        if (std::isalpha(nextChar))
            charClass = LETTER;
        else if (std::isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        // End of input; use -1 to represent EOF
        charClass = -1;
    }
}

/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank() {
    while (std::isspace(nextChar))
        getChar();
}

/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    // Reset lexeme and lexLen for new token
    lexeme.clear();
    lexLen = 0;
    getNonBlank();
    
    switch(charClass) {
        /* Parse identifiers: letter followed by letters or digits */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
            
        /* Parse integer literals: sequence of digits */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
            
        /* Parentheses and arithmetic operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
            
        /* EOF */
        case -1:
            nextToken = -1;
            lexeme = "EOF";
            break;
    }
    
    std::cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << std::endl;
    return nextToken;
}
