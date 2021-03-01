#ifndef PARSER_HPP
#define PARSER_HPP

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include "token.hpp"
#include "node.hpp"

class Parser {
private:
    std::vector<Token> tokens;
    std::vector<Token>::iterator current;
    std::ostream &out;

    Node *program();

    Node *extDefList();

    Node *extDef();

    Node *extVarDef();

    Node *varDef();

    Node *varInitSeq();

    Node *varInit();

    Node *constNumber();

    Node *funDef();

    Node *funTypeSpec();

    Node *parameterList();

    Node *parameter();

    Node *parameterTypeList();

    Node *parameterType();

    Node *compoundStatements();

    Node *statements();

    Node *statement();

    Node *localVarDef();

    Node *expression();

    Node *ifStatement();

    Node *whileStatement();

    Node *forStatement();

    void logError(const std::string &reason, std::vector<Token>::iterator pos);

    void logError(const std::string &reason, int line);

    int checkTerminal(std::vector<Token>::iterator t, Token::TokenType type);

    static bool isTypeSpec(std::vector<Token>::iterator t);

    static bool isFunTypeSpec(std::vector<Token>::iterator t);

public:
    explicit Parser(std::vector<Token> ts, std::ostream &out) : tokens(std::move(ts)), out(out) {
        current = tokens.begin();
    }
};

#endif