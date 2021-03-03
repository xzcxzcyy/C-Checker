#ifndef PARSER_HPP
#define PARSER_HPP

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include <stack>
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

    Node *wrappedExpression();

    Node *functionCall();

    Node *ifStatement();

    Node *whileStatement();

    Node *forStatement();

    Node *returnStatement();

    Node *breakStatement();

    Node *continueStatement();

    Node *argumentList();

    void logError(const std::string &reason, iterator pos);

    std::optional<iterator> checkTerminal(iterator t, Token::TokenType type);

    static bool isTypeSpec(iterator t);

    static bool isFunTypeSpec(iterator t);

    static bool isConstNumber(iterator t);

    static bool isFirstOfExpression(iterator t);

    static bool isFirstOfLocalVarDef(iterator t);

    static int precedence(Node *node);

    static bool isLegalExpressionElement(iterator it);

    static bool isBinaryOperator(iterator it);

    static Node *pop(std::stack<Node *> &stack);

    static void clearStack(std::stack<Node *> &stack);

public:
    explicit Parser(std::vector<Token> ts, std::ostream &out) : tokens(std::move(ts)), out(out) {
        current = tokens.begin();
    }

    Node *parse();
};

#endif