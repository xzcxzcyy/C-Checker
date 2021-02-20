#include <vector>
#include "token.hpp"
#include "lexer.hpp"

void Lexer::analyze() {
    tokens.clear();
    int state = 0;
    int line = 1;
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
                tokens.emplace_back("(", Token::OPEN_PAREN);
            } else if (ch == ')') {
                tokens.emplace_back(")", Token::CLOSE_PAREN);
            } else if (ch == '[') {
                tokens.emplace_back("[", Token::OPEN_BRACKET);
            } else if (ch == ']') {
                tokens.emplace_back("]", Token::CLOSE_BRACKET);
            } else if (ch == '{') {
                tokens.emplace_back("{", Token::OPEN_BRACE);
            } else if (ch == '}') {
                tokens.emplace_back("}", Token::CLOSE_BRACE);
            } else if (ch == '+') {
                tokens.emplace_back("+", Token::PLUS);
            } else if (ch == '-') {
                tokens.emplace_back("-", Token::MINUS);
            } else if (ch == '*') {
                tokens.emplace_back("*", Token::TIMES);
            } else if (ch == '/') {
                tokens.emplace_back("/", Token::DIVIDE);
            } else if (ch == ';') {
                tokens.emplace_back(";", Token::SEMICOL);
            } else if (ch == ',') {
                tokens.emplace_back(",", Token::COMMA);
            } else if (isWhiteSpace(ch)) {
                if (ch == '\n') {
                    line++;
                }
            } else if (ch == '%') {
                tokens.emplace_back("%", Token::MOD);
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
                tokens.emplace_back(errorMsg, Token::ERROR_TOKEN);
                panic(loopFlag, word, state);
            }
            break;

        case 1:
            if (isUnderscore(ch) || isLetter(ch) || isDigit(ch)) {
                word.push_back(ch);
            } else {
                inputStream.unget();
                if (keywords.count(word)) {
                    tokens.emplace_back(word, keywords[word]);
                } else {
                    tokens.emplace_back(word, Token::IDENTIFIER);
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
                    tokens.emplace_back(word, Token::CONST_LONG);
                } else {
                    inputStream.unget();
                    tokens.emplace_back(word, Token::CONST_INT);
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
                    tokens.emplace_back(word, Token::CONST_LONG);
                } else {
                    inputStream.unget();
                    tokens.emplace_back(word, Token::CONST_INT);
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
                tokens.emplace_back(errorMsg, Token::ERROR_TOKEN);
                panic(loopFlag, word, state);
            }
            break;

        case 5:
            if (isHexadecimal(ch)) {
                word.push_back(ch);
            } else {
                if (ch == 'L') {
                    word.push_back(ch);
                    tokens.emplace_back(word, Token::CONST_LONG);
                } else {
                    inputStream.unget();
                    tokens.emplace_back(word, Token::CONST_INT);
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
                tokens.emplace_back(word, Token::CONST_FLOAT);
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
                tokens.emplace_back(errorMsg, Token::ERROR_TOKEN);
                panic(loopFlag, word, state);
            }
            break;

        case 8:
            if (isDigit(ch)) {
                word.push_back(ch);
            } else {
                inputStream.unget();
                tokens.emplace_back(word, Token::CONST_FLOAT);
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
                    tokens.emplace_back(word, Token::CONST_LONG);
                } else {
                    inputStream.unget();
                    tokens.emplace_back(word, Token::CONST_INT);
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
                tokens.emplace_back(errorMsg, Token::ERROR_TOKEN);
                panic(loopFlag, word, state);
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
                    tokens.emplace_back(errorMsg, Token::ERROR_TOKEN);
                    panic(loopFlag, word, state);
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
                tokens.emplace_back(errorMsg, Token::ERROR_TOKEN);
                panic(loopFlag, word, state);
            } else {
                word.push_back(ch);
                state = 13;
            }
            break;

        case 13:
            if (ch == '\'') {
                word.push_back(ch);
                tokens.emplace_back(word, Token::CONST_CHAR);
                word.clear();
                state = 0;
            } else {
                std::string errorMsg = "Lexical error: illegal constant of char at line ";
                errorMsg.append(std::to_string(line));
                tokens.emplace_back(errorMsg, Token::ERROR_TOKEN);
                panic(loopFlag, word, state);
            }
            break;
        }
    }
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

void Lexer::panic(bool& loopFlag, std::string& word, int& state) {
    inputStream.unget();
    word.clear();
    state = 0;
    errorCount++;

    char pch;
    while (loopFlag) {
        if (pch == '\n') {
            inputStream.unget();
            break;
        } else if (pch == EOF) {
            loopFlag = false;
            break;
        }
    }
}
