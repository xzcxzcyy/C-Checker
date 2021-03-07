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
            auto cit = root->comments.begin();
            for (; cit != root->comments.end(); cit++) {
                int pos = std::get<1>(*cit);
                if (pos == 0) {
                    printComment(indent, std::get<0>(*cit).name);
                } else {
                    break;
                }
            }
            if (root->children.empty()) {
                printIndent(indent);
                out << ";\n";
            } else if (root->children[0]->type == Node::Expression) {
                printIndent(indent);
                serialize(root->children[0], 0);
                out << ";\n";
            } else {
                serialize(root->children[0], indent);
            }
            for (; cit != root->comments.end(); cit++) {
                printComment(indent, std::get<0>(*cit).name);
            }
            break;
        }
        case Node::IfStatement: {
            printIndent(indent);
            out << "if" << " (";
            serialize(root->children[0], 0);
            out << ")";
            bool thenBodyCompound = !root->children[1]->children.empty() &&
                                    root->children[1]->children[0]->type == Node::CompoundStatements;
            if (thenBodyCompound) {
                out << " ";
                serialize(root->children[1], indent);
            } else {
                out << std::endl;
                serialize(root->children[1], indent + 4);
            }
            if (root->children.size() == 3) {
                if (!thenBodyCompound) {
                    out << std::endl;
                    printIndent(indent);
                    out << "else";
                } else {
                    out << " else";
                }
                bool elseBodyCompound = !root->children[2]->children.empty() &&
                                        root->children[2]->children[0]->type == Node::CompoundStatements;
                if (elseBodyCompound) {
                    out << " ";
                    serialize(root->children[2], indent);
                } else {
                    out << std::endl;
                    serialize(root->children[2], indent + 4);
                }
            }
            break;
        }
        case Node::WhileStatement: {
            printIndent(indent);
            out << "while (";
            serialize(root->children[0], 0);
            out << ")";
            bool whileBodyCompound = !root->children[1]->children.empty() &&
                                     root->children[1]->children[0]->type == Node::CompoundStatements;
            if (whileBodyCompound) {
                out << " ";
                serialize(root->children[1], indent);
            } else {
                out << std::endl;
                serialize(root->children[1], indent + 4);
            }
            break;
        }
        case Node::ForStatement: {
            printIndent(indent);
            out << "for (";
            serialize(root->children[0], 0);
            if (root->children[0]->type == Node::Expression) {
                out << ";";
            }
            out << " ";
            serialize(root->children[1], 0);
            out << "; ";
            serialize(root->children[2], 0);
            out << ")";
            bool loopBodyCompound = !root->children[3]->children.empty() &&
                                    root->children[3]->children[0]->type == Node::CompoundStatements;
            if (loopBodyCompound) {
                out << " ";
                serialize(root->children[3], indent);
            } else {
                out << std::endl;
                serialize(root->children[3], indent + 4);
            }
            break;
        }
        case Node::ReturnStatement: {
            printIndent(indent);
            out << "return";
            if (!root->children.empty()) {
                out << " ";
                serialize(root->children[0], 0);
            }
            out << ";";
            break;
        }
        case Node::BreakStatement: {
            printIndent(indent);
            out << "break;";
            break;
        }
        case Node::ContinueStatement: {
            printIndent(indent);
            out << "continue;";
            break;
        }
        case Node::LocalVarDef: {
            serialize(root->children[0], indent);
            break;
        }
        case Node::FunctionCall: {
            out << root->children[0]->info->name
                << "(";
            if (root->children.size() == 2) {
                serialize(root->children[1], 0);
            }
            out << ")";
            break;
        }
        case Node::ArgumentList: {
            serialize(root->children[0], 0);
            if (root->children.size() == 2) {
                out << ", ";
                serialize(root->children[1], 0);
            }
            break;
        }
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
    } else if (root->type == Node::FunctionCall) {
        serialize(root, 0);
    }
}

void Serializer::printComment(int indent, const std::string &commentString) {
    printIndent(indent);
    out << commentString
        << std::endl;
}
