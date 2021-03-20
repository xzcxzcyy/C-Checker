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

    /**
     * Tokens to be processed.
     */
    std::vector<Token> tokens;

    /**
     * Iterator pointing to the current token.
     */
    iterator current;

    /**
     * Out stream for error logging.
     */
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

    /**
     * Log when a syntax error occurs.
     * @param reason
     * @param pos Iterator referring to the wrong token.
     */
    void logError(const std::string &reason, iterator pos);

    /**
     * Check whether a given terminal matches a specified type.
     * @param t Iterator to be checked.
     * @param type Type willing to match.
     * @return An iterator on terminal mismatch, otherwise returns nothing.
     */
    std::optional<iterator> checkTerminal(iterator t, Token::TokenType type);

    /**
     * Consume comment(s).
     * @param root Comment is attached to this node.
     * @param pos 0 if comment appears before the sentence, 1 otherwise.
     */
    void consumeComment(Node *&root, int pos);

    static bool isTypeSpec(iterator t);

    static bool isFunTypeSpec(iterator t);

    static bool isConstNumber(iterator t);

    static bool isFirstOfExpression(iterator t);

    static bool isFirstOfLocalVarDef(iterator t);

    /**
     * Precedence of an operator.
     * @param node
     * @return A larger number for a higher precedence.
     */
    static int precedence(Node *node);

    /**
     * Whether it points to a legal start symbol of Expression element, i.e.:
     * open parenthesis, identifier, constant, or binary operator.
     * @param it
     * @return Boolean result.
     */
    static bool isLegalExpressionElement(iterator it);

    static bool isBinaryOperator(iterator it);

    /**
     * Pop an element out and return it.
     * @param stack
     * @return Top element of stack if not empty, otherwise nullptr.
     */
    static Node *pop(std::stack<Node *> &stack);

    /**
     * Clear a stack of operand or operator, releasing the correspond memory in the meantime.
     * @param stack
     */
    static void clearStack(std::stack<Node *> &stack);

public:
    /**
     * Constructor for Parser.
     * @param ts Tokens.
     * @param out Out stream.
     */
    explicit Parser(std::vector<Token> ts, std::ostream &out) : tokens(std::move(ts)), out(out) {
        current = tokens.begin();
    }

    /**
     * Generate AST.
     * @return Null pointer on failure; root of AST on success.
     */
    Node *parse();
};

#endif