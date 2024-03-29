#ifndef NODE_HPP
#define NODE_HPP

#include <utility>
#include <vector>
#include <optional>
#include <tuple>
#include <string>
#include <iostream>
#include "token.hpp"

class Node {
public:
    enum NodeType {
        Program,
        ExtDefList,
        ExtDef,
        ExtVarDef,
        VarDef,
        TypeSpec,
        VarInitSeq,
        VarInit,
        Identifier,
        Expression,
        ConstNumber,
        FunDef,
        FunTypeSpec,
        ParameterList,
        CompoundStatements,
        ParameterTypeList,
        Parameter,
        ParameterType,
        Statements,
        Statement,
        IfStatement,
        WhileStatement,
        ForStatement,
        ReturnStatement,
        BreakStatement,
        ContinueStatement,
        LocalVarDef,
        Operator,
        Epsilon,
        Const,
        WrappedExpression,
        FunctionCall,
        ArgumentList,
    };

    NodeType type;
    std::optional<Token> info;
    std::vector<Node *> children;
    std::vector<std::tuple<Token, int>> comments;

    explicit Node(NodeType type) : type(type), info({}) {
        children.clear();
    }

    Node(NodeType type, Token token) : type(type), info(std::move(token)) {
        children.clear();
    }

    ~Node() {
        for (auto child : children) {
            delete child;
        }
    }

    void addChild(Node *kid);

    void addComment(const std::tuple<Token, int> &c);

    void display(const std::string &prefix) const;

    [[nodiscard]] std::string nodeTypeString() const;

private:
    static void printComments(const std::string &comments);

};

#endif