//
// Created by xzcxzcyy on 2021/3/5.
//

#include "serializer.hpp"

void Serializer::serialize(Node *root, int indent) {
    switch (root->type) {
        case Node::Program: {
            serialize(root->children[0], indent);
            break;
        }
        case Node::ExtDefList: {
            for (auto kid : root->children) {
                serialize(kid, indent);
            }
            break;
        }
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
        case Node::ExtVarDef: {
            serialize(root->children[0], indent);
            out << std::endl;
            break;
        }
        case Node::VarDef: {
            printIndent(indent);
            for (auto child : root->children) {
                if (child->type == Node::TypeSpec) {
                    out << child->info->name << " ";
                } else if (child->type == Node::Const) {
                    out << "const ";
                } else if (child->type == Node::VarInitSeq) {
                    serialize(child, 0);
                    out << ";";
                } else {
                    throw std::logic_error("illegal type in serializer");
                }
            }
            break;
        }
        case Node::VarInitSeq: {
            for (auto child : root->children) {
                if (child->type == Node::VarInitSeq) {
                    out << ", ";
                }
                serialize(child, 0);
            }
            break;
        }
        case Node::VarInit: {
            out << root->children[0]->info->name;
            if (root->children.size() == 2) {
                if (root->children[1]->type == Node::Expression) {
                    out << " = ";
                    serialize(root->children[1], 0);
                } else if (root->children[1]->type == Node::ConstNumber) {
                    out << "[" << root->children[1]->info->name << "]";
                }
            }
            break;
        }
        case Node::Expression: {
            handleExpression(root);
            break;
        }
        case Node::FunDef: {
            auto child = root->children.begin();
            printIndent(indent);
            out << (*child)->info->name;
            child++;
            out << " " << (*child)->info->name
                << "(";
            child++;
            if (child == root->children.end()) {
                out << ");";
            } else if ((*child)->type == Node::ParameterList) {
                serialize(*child, 0);
                child++;
                out << ") ";
                serialize(*child, indent);
            } else if ((*child)->type == Node::ParameterTypeList) {
                serialize(*child, 0);
                out << ");\n";
            } else if ((*child)->type == Node::CompoundStatements) {
                out << ") ";
                serialize(*child, indent);
            }
            break;
        }
        case Node::ParameterList: {
            serialize(root->children[0], 0);
            if (root->children.size() == 2) {
                out << ", ";
                serialize(root->children[1], 0);
            }
            break;
        }
        case Node::CompoundStatements: {
            out << "{\n";
            if (root->children.size() == 1) {
                serialize(root->children[0], indent + 4);
            }
            printIndent(indent);
            out << "}";
            break;
        }
        case Node::ParameterTypeList: {
            serialize(root->children[0], 0);
            if (root->children.size() == 2) {
                out << ", ";
                serialize(root->children[1], 0);
            }
            break;
        }
        case Node::Parameter: {
            out << root->children[0]->info->name << " "
                << root->children[1]->info->name;
            break;
        }
        case Node::ParameterType: {
            out << root->children[0]->info->name;
            if (root->children.size() == 2) {
                out << " " << root->children[1]->info->name;
            }
            break;
        }
        case Node::Statements: {
            serialize(root->children[0], indent);
            if (root->children.size() == 2) {
                serialize(root->children[1], indent);
            }
            break;
        }
        case Node::Statement: {

            break;
        }
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

void Serializer::handleExpression(Node *root) {
    if (root->type == Node::Expression) {
        handleExpression(root->children[0]);
    } else if (root->type == Node::WrappedExpression) {
        out << "(";
        handleExpression(root->children[0]);
        out << ")";
    } else if (root->type == Node::Operator) {
        handleExpression(root->children[0]);
        out << " " << root->info->name << " ";
        handleExpression(root->children[1]);
    } else if (root->type == Node::ConstNumber
               || root->type == Node::Identifier) {
        out << root->info->name;
    }
}
