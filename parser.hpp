#ifndef PARSER_HPP
#define PARSER_HPP

#include <utility>
#include <vector>
#include "token.hpp"
#include "node.hpp"

class Parser {
private:
    std::vector<Token> tokens;
    std::vector<Token>::iterator current;

    Node *program();

    Node *extDefList();

    Node *extDef();

    Node *extVarDef();

    Node *varDef();

    Node *typeSpec();

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

    static bool isTypeSpec(std::vector<Token>::iterator t);

    static bool isFunTypeSpec(std::vector<Token>::iterator t);

public:
    explicit Parser(std::vector<Token> ts) : tokens(std::move(ts)) {
        current = tokens.begin();
    }
};

#endif