#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "token.hpp"
#include "lexer.hpp"

void Lexer::analyze() {
    line = 1;
    auto inputStream = std::ifstream(fileName);
    tokens.clear();
    int state = 0;
    char ch;
    bool loopFlag = true;
    std::string word;
    while (loopFlag) {
        ch = inputStream.get();
        switch (state) {
        case 0:
            if (isUnderscore(ch) || isLetter(ch)) {
                word.push_back(ch);
                state = 1;
            } else if (ch == '0') {
                word.push_back(ch);
                state = 2;
            } else if (ch >= '1' && ch <= '9') {
                word.push_back(ch);
                state = 9;
            } else if (ch == '.') {
                word.push_back(ch);
                state = 10;
            } else if (ch == '\'') {
                word.push_back(ch);
                state = 11;
            } else if (ch == '(') {
                tokens.push_back(makeToken("(", Token::OPEN_PAREN));
            } else if (ch == ')') {
                tokens.push_back(makeToken(")", Token::CLOSE_PAREN));
            } else if (ch == '[') {
                tokens.push_back(makeToken("[", Token::OPEN_BRACKET));
            } else if (ch == ']') {
                tokens.push_back(makeToken("]", Token::CLOSE_BRACKET));
            } else if (ch == '{') {
                tokens.push_back(makeToken("{", Token::OPEN_BRACE));
            } else if (ch == '}') {
                tokens.push_back(makeToken("}", Token::CLOSE_BRACE));
            } else if (ch == '+') {
                tokens.push_back(makeToken("+", Token::PLUS));
            } else if (ch == '-') {
                tokens.push_back(makeToken("-", Token::MINUS));
            } else if (ch == '*') {
                tokens.push_back(makeToken("*", Token::TIMES));
            } else if (ch == '/') {
                tokens.push_back(makeToken("/", Token::DIVIDE));
            } else if (ch == ';') {
                tokens.push_back(makeToken(";", Token::SEMICOL));
            } else if (ch == ',') {
                tokens.push_back(makeToken(",", Token::COMMA));
            } else if (isWhiteSpace(ch)) {
                if (ch == '\n') {
                    line++;
                    // tokens.push_back(makeToken("\n", Token::WHITESPACE));
                }
            } else if (ch == '%') {
                tokens.push_back(makeToken("%", Token::MOD));
            } else if (ch == '>') {
                word.push_back(ch);
                state = 14;
            } else if (ch == '<') {
                word.push_back(ch);
                state = 15;
            } else if (ch == '=') {
                word.push_back(ch);
                state = 16;
            } else if (ch == '!') {
                word.push_back(ch);
                state = 17;
            } else if (ch == '&') {
                word.push_back(ch);
                state = 18;
            } else if (ch == '|') {
                word.push_back(ch);
                state = 19;
            } else if (isEof(ch)) {
                loopFlag = false;
            } else {
                std::string errorMsg = "Lexical error: illegal character at line ";
                errorMsg.append(std::to_string(line));
                tokens.push_back(makeToken(errorMsg, Token::ERROR_TOKEN));
                panic(inputStream, loopFlag, word, state);
            }
            break;

        case 1:
            if (isUnderscore(ch) || isLetter(ch) || isDigit(ch)) {
                word.push_back(ch);
            } else {
                inputStream.unget();
                if (keywords.count(word)) {
                    tokens.push_back(makeToken(word, keywords[word]));
                } else {
                    tokens.push_back(makeToken(word, Token::IDENTIFIER));
                }
                word.clear();
                state = 0;
            }
            break;

        case 2:
            if (isDigit(ch)) {
                word.push_back(ch);
                state = 3;
            } else if (ch == 'x' || ch == 'X') {
                word.push_back(ch);
                state = 4;
            } else if (ch == '.') {
                word.push_back(ch);
                state = 6;
            } else if (ch == 'e' || ch == 'E') {
                word.push_back(ch);
                state = 7;
            } else {
                if (ch == 'L') {
                    word.push_back(ch);
                    tokens.push_back(makeToken(word, Token::CONST_LONG));
                } else {
                    inputStream.unget();
                    tokens.push_back(makeToken(word, Token::CONST_INT));
                }
                word.clear();
                state = 0;
            }
            break;

        case 3:
            if (isDigit(ch)) {
                word.push_back(ch);
            } else if (ch == '.') {
                word.push_back(ch);
                state = 6;
            } else if (ch == 'e' || ch == 'E') {
                word.push_back(ch);
                state = 7;
            } else {
                if (ch == 'L') {
                    word.push_back(ch);
                    tokens.push_back(makeToken(word, Token::CONST_LONG));
                } else {
                    inputStream.unget();
                    tokens.push_back(makeToken(word, Token::CONST_INT));
                }
                word.clear();
                state = 0;
            }
            break;

        case 4:
            if (isHexadecimal(ch)) {
                word.push_back(ch);
                state = 5;
            } else {
                std::string errorMsg = "Lexical error: illegal hex number at line ";
                errorMsg.append(std::to_string(line));
                tokens.push_back(makeToken(errorMsg, Token::ERROR_TOKEN));
                panic(inputStream, loopFlag, word, state);
            }
            break;

        case 5:
            if (isHexadecimal(ch)) {
                word.push_back(ch);
            } else {
                if (ch == 'L') {
                    word.push_back(ch);
                    tokens.push_back(makeToken(word, Token::CONST_LONG));
                } else {
                    inputStream.unget();
                    tokens.push_back(makeToken(word, Token::CONST_INT));
                }
                word.clear();
                state = 0;
            }
            break;

        case 6:
            if (isDigit(ch)) {
                word.push_back(ch);
            } else if (ch == 'e' || ch == 'E') {
                word.push_back(ch);
                state = 7;
            } else {
                inputStream.unget();
                tokens.push_back(makeToken(word, Token::CONST_FLOAT));
                word.clear();
                state = 0;
            }
            break;

        case 7:
            if (isDigit(ch)) {
                word.push_back(ch);
                state = 8;
            } else {
                std::string errorMsg = "Lexical error: illegal exp number at line ";
                errorMsg.append(std::to_string(line));
                tokens.push_back(makeToken(errorMsg, Token::ERROR_TOKEN));
                panic(inputStream, loopFlag, word, state);
            }
            break;

        case 8:
            if (isDigit(ch)) {
                word.push_back(ch);
            } else {
                inputStream.unget();
                tokens.push_back(makeToken(word, Token::CONST_FLOAT));
                word.clear();
                state = 0;
            }
            break;

        case 9:
            if (isDigit(ch)) {
                word.push_back(ch);
                state = 3;
            } else if (ch == '.') {
                word.push_back(ch);
                state = 6;
            } else if (ch == 'e' || ch == 'E') {
                word.push_back(ch);
                state = 7;
            } else {
                if (ch == 'L') {
                    word.push_back(ch);
                    tokens.push_back(makeToken(word, Token::CONST_LONG));
                } else {
                    inputStream.unget();
                    tokens.push_back(makeToken(word, Token::CONST_INT));
                }
                word.clear();
                state = 0;
            }
            break;

        case 10:
            if (isDigit(ch)) {
                word.push_back(ch);
                state = 6;
            } else {
                std::string errorMsg = "Lexical error: illegal floating number at line ";
                errorMsg.append(std::to_string(line));
                tokens.push_back(makeToken(errorMsg, Token::ERROR_TOKEN));
                panic(inputStream, loopFlag, word, state);
            }
            break;

        case 11:
            if (ch == '\\') {
                word.push_back(ch);
                state = 12;
            } else {
                if (ch == '\n' || ch == EOF) {
                    std::string errorMsg = "Lexical error: illegal constant of char at line ";
                    errorMsg.append(std::to_string(line));
                    tokens.push_back(makeToken(errorMsg, Token::ERROR_TOKEN));
                    panic(inputStream, loopFlag, word, state);
                } else {
                    word.push_back(ch);
                    state = 13;
                }
            }
            break;

        case 12:
            if (ch == '\n' || ch == EOF) {
                std::string errorMsg = "Lexical error: illegal constant of char at line ";
                errorMsg.append(std::to_string(line));
                tokens.push_back(makeToken(errorMsg, Token::ERROR_TOKEN));
                panic(inputStream, loopFlag, word, state);
            } else {
                word.push_back(ch);
                state = 13;
            }
            break;

        case 13:
            if (ch == '\'') {
                word.push_back(ch);
                tokens.push_back(makeToken(word, Token::CONST_CHAR));
                word.clear();
                state = 0;
            } else {
                std::string errorMsg = "Lexical error: illegal constant of char at line ";
                errorMsg.append(std::to_string(line));
                tokens.push_back(makeToken(errorMsg, Token::ERROR_TOKEN));
                panic(inputStream, loopFlag, word, state);
            }
            break;

        case 14:
            if (ch == '=') {
                word.push_back(ch);
                tokens.push_back(makeToken(word, Token::GEQUAL));
                word.clear();
                state = 0;
            } else {
                inputStream.unget();
                tokens.push_back(makeToken(word, Token::GREATER));
                word.clear();
                state = 0;
            }
            break;

        case 15:
            if (ch == '=') {
                word.push_back(ch);
                tokens.push_back(makeToken(word, Token::LEQUAL));
                word.clear();
                state = 0;
            } else {
                inputStream.unget();
                tokens.push_back(makeToken(word, Token::LESS));
                word.clear();
                state = 0;
            }
            break;

        case 16:
            if (ch == '=') {
                word.push_back(ch);
                tokens.push_back(makeToken(word, Token::EQUAL));
                word.clear();
                state = 0;
            } else {
                inputStream.unget();
                tokens.push_back(makeToken(word, Token::ASSIGN));
                word.clear();
                state = 0;
            }
            break;

        case 17:
            if (ch == '=') {
                word.push_back(ch);
                tokens.push_back(makeToken(word, Token::NOT_EQUAL));
                word.clear();
                state = 0;
            } else {
                std::string errorMsg = "Lexical error: illegal operator at line ";
                errorMsg.append(std::to_string(line));
                panic(inputStream, loopFlag, word, state);
            }
            break;

        case 18:
            if (ch == '&') {
                word.push_back(ch);
                tokens.push_back(makeToken(word, Token::LOGICAL_AND));
                word.clear();
                state = 0;
            } else {
                std::string errorMsg = "Lexical error: illegal operator at line ";
                errorMsg.append(std::to_string(line));
                panic(inputStream, loopFlag, word, state);
            }
            break;

        case 19:
            if (ch == '|') {
                word.push_back(ch);
                tokens.push_back(makeToken(word, Token::LOGICAL_OR));
                word.clear();
                state = 0;
            } else {
                std::string errorMsg = "Lexical error: illegal operator at line ";
                errorMsg.append(std::to_string(line));
                panic(inputStream, loopFlag, word, state);
            }
            break;

        default:
            throw "ILLEGAL_LEXER_STATE_EXP";
        }
    }
    inputStream.close();
}

bool Lexer::isUnderscore(char c) {
    return c == '_';
}

bool Lexer::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isEof(char c) {
    return c == EOF;
}

bool Lexer::isWhiteSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool Lexer::isHexadecimal(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

void Lexer::inflateKeywords() {
    keywords["void"] = Token::VOID;
    keywords["int"] = Token::INT;
    keywords["char"] = Token::CHAR;
    keywords["short"] = Token::SHORT;
    keywords["long"] = Token::LONG;
    keywords["double"] = Token::DOUBLE;
    keywords["float"] = Token::FLOAT;
    keywords["const"] = Token::CONST;
    keywords["while"] = Token::WHILE;
    keywords["for"] = Token::FOR;
    keywords["continue"] = Token::CONTINUE;
    keywords["break"] = Token::BREAK;
    keywords["if"] = Token::IF;
    keywords["else"] = Token::ELSE;
    keywords["return"] = Token::RETURN;
}

void Lexer::panic(std::ifstream &inputStream, bool &loopFlag, std::string &word, int &state) {
    inputStream.unget();
    word.clear();
    state = 0;
    errorCount++;

    char pch;
    while (loopFlag) {
        pch = inputStream.get();
        if (pch == '\n') {
            inputStream.unget();
            break;
        } else if (pch == EOF) {
            loopFlag = false;
            break;
        }
    }
}

Token Lexer::makeToken(std::string name, Token::TokenType type) {
    return Token(name, type, fileName, line);
}

void Lexer::displayTokens(std::ostream &out, bool sortThem) {
    static const std::vector<std::string> tokenClassNames = {
            "ERROR_TOKEN",
            "VOID",
            "INT",
            "CHAR",
            "SHORT",
            "LONG",
            "DOUBLE",
            "FLOAT",
            "CONST",
            "WHILE",
            "FOR",
            "CONTINUE",
            "BREAK",
            "IF",
            "ELSE",
            "RETURN",
            "IDENTIFIER",
            "CONST_INT",
            "CONST_LONG",
            "CONST_FLOAT",
            "CONST_CHAR",
            "OPEN_PAREN",
            "CLOSE_PAREN",
            "OPEN_BRACKET",
            "CLOSE_BRACKET",
            "OPEN_BRACE",
            "CLOSE_BRACE",
            "PLUS",
            "MINUS",
            "TIMES",
            "DIVIDE",
            "MOD",
            "GREATER",
            "GEQUAL",
            "LESS",
            "LEQUAL",
            "EQUAL",
            "NOT_EQUAL",
            "LOGICAL_AND",
            "LOGICAL_OR",
            "SEMICOL",
            "COMMA",
            "WHITESPACE",
            "ASSIGN",
            "COMMENTS"
    };
    auto toDisplayTokens = tokens;
    if (sortThem) {
        std::sort(toDisplayTokens.begin(), toDisplayTokens.end());
    }
    for (const auto &token : toDisplayTokens) {
        if (token.type == Token::WHITESPACE) {
            continue;
        }
        out << tokenClassNames[token.type] << "\t" << token.name
            << "\tline " << std::to_string(token.line) << "\t" << token.fileName
            << std::endl;
    }
}
