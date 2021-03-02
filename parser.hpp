#ifndef PARSER_HPP
#define PARSER_HPP

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include "token.hpp"
#include "node.hpp"

class Parser {
private:
    typedef std::vector<Token>::iterator iterator;

    std::vector<Token> tokens;
    iterator current;
    std::ostream &out;

    Node *program();

    Node *extDefList();

    Node *extDef();

    Node *extVarDef();

    Node *varDef();

    Node *varInitSeq();

    Node *varInit();

    Node *funDef();

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

    void logError(const std::string &reason, iterator pos);

    std::optional<iterator> checkTerminal(iterator t, Token::TokenType type);

    static bool isTypeSpec(iterator t);

    static bool isFunTypeSpec(iterator t);

    static bool isConstNumber(iterator t);

    static bool isFirstOfExpression(iterator t);

    static bool isFirstOfLocalVarDef(iterator t);

public:
    explicit Parser(std::vector<Token> ts, std::ostream &out) : tokens(std::move(ts)), out(out) {
        current = tokens.begin();
    }
};

#endif