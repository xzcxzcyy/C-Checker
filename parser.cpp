#include <iostream>
#include "parser.hpp"
#include "node.hpp"
#include "token.hpp"

Node *Parser::program() {
    if (current == tokens.end()) {
        return nullptr;
    }
    auto programNode = new Node(Node::Program);
    auto extDefListNode = extDefList();
    programNode->children.push_back(extDefListNode);
    return programNode;
}

Node *Parser::extDefList() {
    if (current == tokens.end()) {
        return nullptr;
    }
    auto child1 = extDef();
    if (child1 == nullptr) {
        return nullptr;
    }
    auto root = new Node(Node::ExtDefList);
    root->children.push_back(child1);
    if (current != tokens.end()) {
        auto child2 = extDefList();
        if (child2 == nullptr) {
            delete root;
            return nullptr;
        }
        root->children.push_back(child2);
    }
    return root;
}

Node *Parser::extDef() {
    if (tokens.end() - current < 3) {
        return nullptr;
    }
    auto root = new Node(Node::ExtDef);
    if (isFunTypeSpec(current)
        && (current + 1)->type == Token::IDENTIFIER
        && (current + 2)->type == Token::OPEN_PAREN) {
        auto funDefNode = funDef();
        if (funDefNode == nullptr) {
            delete root;
            return nullptr;
        }
        root->children.push_back(funDefNode);
        return root;
    } else {
        auto extVarDefNode = extVarDef();
        if (extVarDefNode == nullptr) {
            delete root;
            return nullptr;
        }
        root->children.push_back(extVarDefNode);
        return root;
    }
}

bool Parser::isTypeSpec(std::vector<Token>::iterator t) {
    return t->type == Token::INT
           || t->type == Token::CHAR
           || t->type == Token::SHORT
           || t->type == Token::LONG
           || t->type == Token::DOUBLE
           || t->type == Token::FLOAT;
}

bool Parser::isFunTypeSpec(std::vector<Token>::iterator t) {
    return t->type == Token::VOID
           || t->type == Token::INT
           || t->type == Token::CHAR
           || t->type == Token::SHORT
           || t->type == Token::LONG
           || t->type == Token::DOUBLE
           || t->type == Token::FLOAT;
}

Node *Parser::extVarDef() {
    auto varDefNode = varDef();
    if (varDefNode != nullptr) {
        auto root = new Node(Node::ExtVarDef);
        root->children.push_back(varDefNode);
        return root;
    } else {
        return nullptr;
    }
}

Node *Parser::varDef() {
    if (current >= tokens.end()) {
        if (!tokens.empty()) {
            logError("Illegal variable definition", tokens.end() - 1);
        }
        return nullptr;
    }
    auto root = new Node(Node::VarDef);
    if (isTypeSpec(current)) {
        auto typeSpecNode = new Node(Node::TypeSpec);
        typeSpecNode->info = &*current;
        root->children.push_back(typeSpecNode);
        current++;
        if (current == tokens.end()) {
            logError("Incomplete variable definition", tokens.end() - 1);
            delete root;
            return nullptr;
        }
        if (current->type == Token::CONST) {
            auto constNode = new Node(Node::Const);
            root->children.push_back(constNode);
            current++;
        }
        if (current == tokens.end()) {
            logError("Incomplete variable definition", tokens.end() - 1);
            delete root;
            return nullptr;
        }
    } else if (current->type == Token::CONST) {
        auto constNode = new Node(Node::Const);
        root->children.push_back(constNode);
        current++;
        if (current == tokens.end()) {
            logError("Incomplete variable definition", tokens.end() - 1);
            delete root;
            return nullptr;
        }
        if (!isTypeSpec(current)) {
            logError("Illegal variable definition", current);
            delete root;
            return nullptr;
        }
        auto typeSpecNode = new Node(Node::TypeSpec);
        typeSpecNode->info = &*current;
        root->children.push_back(typeSpecNode);
    }
    auto varInitSeqNode = varInitSeq();
    if (varInitSeqNode == nullptr) {
        delete root;
        return nullptr;
    } else {
        root->children.push_back(varInitSeqNode);
    }
    if (current >= tokens.end()) {
        logError("Require semicolon at the end of variable definition", tokens.end() - 1);
        delete root;
        return nullptr;
    }
    if (current->type != Token::SEMICOL) {
        logError("Require semicolon at the end of variable definition", current);
        delete root;
        return nullptr;
    }
    current++;
    return root;
}

void Parser::logError(const std::string &reason, std::vector<Token>::iterator pos) {
    out << "Parsing error at line " << pos->line
        << " " << reason
        << std::endl;
}
