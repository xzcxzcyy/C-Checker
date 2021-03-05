//
// Created by xzcxzcyy on 2021/3/5.
//

#include "serializer.hpp"

void Serializer::serialize(Node *root, int indent) {
    switch (root->type) {
        case Node::Program:
            serialize(root->children[0], indent);
            break;
        case Node::ExtDefList:
            for (auto kid : root->children) {
                serialize(kid, indent);
            }
            break;
        case Node::ExtDef: {
            auto comment = root->comments.begin();
            for (; comment != root->comments.end(); comment++) {
                if (std::get<1>(*comment) == 0) {
                    printComment(indent, std::get<0>(*comment).name);
                } else {
                    break;
                }
            }
            serialize(root->children[0], indent);
            break;
        }
        case Node::ExtVarDef:
            serialize(root->children[0], indent);
            break;
        case Node::VarDef:
            break;
        case Node::TypeSpec:
            break;
        case Node::VarInitSeq:
            break;
        case Node::VarInit:
            break;
        case Node::Identifier:
            break;
        case Node::Expression:
            break;
        case Node::ConstNumber:
            break;
        case Node::FunDef:
            break;
        case Node::FunTypeSpec:
            break;
        case Node::ParameterList:
            break;
        case Node::CompoundStatements:
            break;
        case Node::ParameterTypeList:
            break;
        case Node::Parameter:
            break;
        case Node::ParameterType:
            break;
        case Node::Statements:
            break;
        case Node::Statement:
            break;
        case Node::IfStatement:
            break;
        case Node::WhileStatement:
            break;
        case Node::ForStatement:
            break;
        case Node::ReturnStatement:
            break;
        case Node::BreakStatement:
            break;
        case Node::ContinueStatement:
            break;
        case Node::LocalVarDef:
            break;
        case Node::Operator:
            break;
        case Node::Const:
            break;
        case Node::WrappedExpression:
            break;
        case Node::FunctionCall:
            break;
        case Node::ArgumentList:
            break;
        default:
            throw std::logic_error("Got unhandled node type when serializing.");
    }
}

void Serializer::perform(Node *root) {
    serialize(root, 0);
}

void Serializer::printIndent(int indent) {
    for (int i = 0; i < indent; i++) {
        out << " ";
    }
}
