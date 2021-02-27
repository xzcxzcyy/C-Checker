#ifndef NODE_HPP
#define NODE_HPP
#include <vector>
#include <string>
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
        Operand,
        Operator,
        Epsilon,
    };

    NodeType type;
    Token *info{};
    std::vector<Node *> children;

    explicit Node(NodeType type) : type(type) {
        children.clear();
    }

    void addChild(Node *kid);
};

#endif