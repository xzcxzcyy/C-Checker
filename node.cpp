#include <iostream>
#include <vector>
#include <string>
#include "node.hpp"

void Node::addChild(Node *kid) {
    children.push_back(kid);
}

void Node::display(const std::string& prefix) const {
    std::cout << prefix << "-- ";
    std::cout << nodeTypeString() << "  ";
    if (info.has_value()) {
        std::cout << info->name << " ";
    }
    for (const auto &c : comments) {
        printComments(std::get<0>(c).name);
    }
    std::cout << std::endl;
    for (auto kid : children) {
        kid->display(prefix + "  |");
    }
}

std::string Node::nodeTypeString() const {
    std::string name;
    switch (type) {
        case Program:
            name = "Program";
            break;
        case ExtDefList:
            name = "ExtDefList";
            break;
        case ExtDef:
            name = "ExtDef";
            break;
        case ExtVarDef:
            name = "ExtVarDef";
            break;
        case VarDef:
            name = "VarDef";
            break;
        case TypeSpec:
            name = "TypeSpec";
            break;
        case VarInitSeq:
            name = "VarInitSeq";
            break;
        case VarInit:
            name = "VarInit";
            break;
        case Identifier:
            name = "Identifier";
            break;
        case Expression:
            name = "Expression";
            break;
        case ConstNumber:
            name = "ConstNumber";
            break;
        case FunDef:
            name = "FunDef";
            break;
        case FunTypeSpec:
            name = "FunTypeSpec";
            break;
        case ParameterList:
            name = "ParameterList";
            break;
        case CompoundStatements:
            name = "CompoundStatements";
            break;
        case ParameterTypeList:
            name = "ParameterTypeList";
            break;
        case Parameter:
            name = "Parameter";
            break;
        case ParameterType:
            name = "ParameterType";
            break;
        case Statements:
            name = "Statements";
            break;
        case Statement:
            name = "Statement";
            break;
        case IfStatement:
            name = "IfStatement";
            break;
        case WhileStatement:
            name = "WhileStatement";
            break;
        case ForStatement:
            name = "ForStatement";
            break;
        case ReturnStatement:
            name = "ReturnStatement";
            break;
        case BreakStatement:
            name = "BreakStatement";
            break;
        case ContinueStatement:
            name = "ContinueStatement";
            break;
        case LocalVarDef:
            name = "LocalVarDef";
            break;
        case Operator:
            name = "Operator";
            break;
        case Epsilon:
            name = "Epsilon";
            break;
        case Const:
            name = "Const";
            break;
        case WrappedExpression:
            name = "WrappedExpression";
            break;
        case FunctionCall:
            name = "FunctionCall";
            break;
        case ArgumentList:
            name = "ArgumentList";
            break;
    }
    return name;
}

void Node::addComment(const std::tuple<Token, int>& comment) {
    comments.push_back(comment);
}

void Node::printComments(const std::string &comments) {
    for (auto c : comments) {
        if (c == '\r' || c == '\n') {
            c = ' ';
        }
        std::cout << c;
    }
    std::cout << "  ";
}
