#include <iostream>
#include <optional>
#include "parser.hpp"
#include "node.hpp"
#include "token.hpp"

Node *Parser::program() {
    if (current == tokens.end()) {
        return nullptr;
    }
    auto programNode = new Node(Node::Program);
    auto extDefListNode = extDefList();
    programNode->addChild(extDefListNode);
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
    root->addChild(child1);
    if (current != tokens.end()) {
        auto child2 = extDefList();
        if (child2 == nullptr) {
            delete root;
            return nullptr;
        }
        root->addChild(child2);
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
        root->addChild(funDefNode);
        return root;
    } else {
        auto extVarDefNode = extVarDef();
        if (extVarDefNode == nullptr) {
            delete root;
            return nullptr;
        }
        root->addChild(extVarDefNode);
        return root;
    }
}

bool Parser::isTypeSpec(iterator t) {
    return t->type == Token::INT
           || t->type == Token::CHAR
           || t->type == Token::SHORT
           || t->type == Token::LONG
           || t->type == Token::DOUBLE
           || t->type == Token::FLOAT;
}

bool Parser::isFunTypeSpec(iterator t) {
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
        root->addChild(varDefNode);
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
        auto typeSpecNode = new Node(Node::TypeSpec, *current);
        root->addChild(typeSpecNode);
        current++;
        if (current == tokens.end()) {
            logError("Incomplete variable definition", tokens.end() - 1);
            delete root;
            return nullptr;
        }
        if (current->type == Token::CONST) {
            auto constNode = new Node(Node::Const);
            root->addChild(constNode);
            current++;
        }
        if (current == tokens.end()) {
            logError("Incomplete variable definition", tokens.end() - 1);
            delete root;
            return nullptr;
        }
    } else if (current->type == Token::CONST) {
        auto constNode = new Node(Node::Const);
        root->addChild(constNode);
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
        auto typeSpecNode = new Node(Node::TypeSpec, *current);
        root->addChild(typeSpecNode);
        current++;
    }
    auto varInitSeqNode = varInitSeq();
    if (varInitSeqNode == nullptr) {
        delete root;
        return nullptr;
    } else {
        root->addChild(varInitSeqNode);
    }
    auto checkRes = checkTerminal(current, Token::SEMICOL);
    if (checkRes.has_value()) {
        logError("Require semicolon at the end of variable definition.", checkRes.value());
        delete root;
        return nullptr;
    }
    current++;
    return root;
}

void Parser::logError(const std::string &reason, iterator pos) {
    if (pos >= tokens.end()) {
        pos = tokens.end() - 1;
    }
    out << "Parsing error at line " << pos->line
        << " in file " << pos->fileName << ": " << reason
        << std::endl;
}

std::optional<Parser::iterator> Parser::checkTerminal(iterator t, Token::TokenType type) {
    if (t >= tokens.end()) {
        return tokens.end() - 1;
    } else {
        if (t->type != type) {
            return t;
        } else {
            return {};
        }
    }
}

Node *Parser::varInitSeq() {
    auto root = new Node(Node::VarInitSeq);
    auto child1 = varInit();
    if (child1 == nullptr) {
        delete root;
        return nullptr;
    }
    root->addChild(child1);
    auto result = checkTerminal(current, Token::COMMA);
    if (!result.has_value()) {
        current++;
        auto varInitSeqNode = varInitSeq();
        if (varInitSeqNode == nullptr) {
            delete root;
            return nullptr;
        }
        root->addChild(varInitSeqNode);
    }
    return root;
}

Node *Parser::varInit() {
    auto root = new Node(Node::VarInit);
    auto result = checkTerminal(current, Token::IDENTIFIER);
    if (result.has_value()) {
        delete root;
        logError("Requires an identifier.", result.value());
        return nullptr;
    }
    auto identifierNode = new Node(Node::Identifier, *current);
    root->addChild(identifierNode);
    current++;
    if (!checkTerminal(current, Token::ASSIGN).has_value()) {
        current++;
        auto exp = expression();
        if (exp == nullptr) {
            delete root;
            return nullptr;
        } else {
            root->addChild(exp);
        }
    } else if (!checkTerminal(current, Token::OPEN_BRACKET).has_value()) {
        current++;
        auto isConstInt = checkTerminal(current, Token::CONST_INT);
        if (isConstInt.has_value()) {
            logError("Requires CONST_INT token for array declaration.", isConstInt.value());
            delete root;
            return nullptr;
        }
        auto constIntNode = new Node(Node::ConstNumber, *current);
        current++;
        root->addChild(constIntNode);
        auto isCloseBracket = checkTerminal(current, Token::CLOSE_BRACKET);
        if (isCloseBracket.has_value()) {
            logError("Requires a close bracket for array declaration.", isCloseBracket.value());
            delete root;
            return nullptr;
        }
        current++;
    }
    return root;
}

Node *Parser::funDef() {
    auto root = new Node(Node::FunDef);
    if (current >= tokens.end()) {
        delete root;
        return nullptr;
    }
    if (isFunTypeSpec(current)) {
        auto funTypeSpecNode = new Node(Node::FunTypeSpec, *current);
        current++;
        root->addChild(funTypeSpecNode);
    } else {
        logError("Requires a function return type specifier.", current);
        delete root;
        return nullptr;
    }
    auto isIdentifier = checkTerminal(current, Token::IDENTIFIER);
    if (!isIdentifier.has_value()) {
        auto identifierNode = new Node(Node::Identifier, *current);
        current++;
        root->addChild(identifierNode);
    } else {
        logError("Requires an identifier for function definition.", isIdentifier.value());
        delete root;
        return nullptr;
    }
    auto isOpenParen = checkTerminal(current, Token::OPEN_PAREN);
    if (!isOpenParen.has_value()) {
        current++;
    } else {
        logError("Missing OPEN_PAREN for function definition", isOpenParen.value());
        delete root;
        return nullptr;
    }
    int productionType = 0;
    for (auto it = current; it != tokens.end(); it++) {
        if (it->type == Token::CLOSE_PAREN && it + 1 != tokens.end()) {
            if ((it + 1)->type == Token::OPEN_BRACE) {
                productionType = 1;
            } else if ((it + 1)->type == Token::SEMICOL) {
                productionType = 2;
            }
            break;
        }
    }
    if (productionType == 0) {
        logError("incomplete function definition.", current);
        delete root;
        return nullptr;
    } else if (productionType == 1) {
        if (current->type != Token::CLOSE_PAREN) {
            auto parameterListNode = parameterList();
            if (parameterListNode == nullptr) {
                delete root;
                return nullptr;
            }
            root->addChild(parameterListNode);
        }

        auto isCloseParen = checkTerminal(current, Token::CLOSE_PAREN);
        if (!isCloseParen.has_value()) {
            current++;
        } else {
            logError("Function definition requires CLOSE_PAREN.", isCloseParen.value());
            delete root;
            return nullptr;
        }
        auto compoundStatNode = compoundStatements();
        if (compoundStatNode == nullptr) {
            delete root;
            return nullptr;
        } else {
            root->addChild(compoundStatNode);
        }
    } else {
        if (current->type != Token::CLOSE_PAREN) {
            auto parameterTypeListNode = parameterTypeList();
            if (parameterTypeListNode == nullptr) {
                delete root;
                return nullptr;
            }
            root->addChild(parameterTypeListNode);
        }

        auto isCloseParen = checkTerminal(current, Token::CLOSE_PAREN);
        if (!isCloseParen.has_value()) {
            current++;
        } else {
            logError("Function definition requires CLOSE_PAREN.", isCloseParen.value());
            delete root;
            return nullptr;
        }
        auto isSemi = checkTerminal(current, Token::SEMICOL);
        if (isSemi.has_value()) {
            logError("expect a semicolon for function declaration", isSemi.value());
            delete root;
            return nullptr;
        } else {
            current++;
        }
    }
    return root;
}

Node *Parser::parameterList() {
    auto root = new Node(Node::ParameterList);
    auto parameterNode = parameter();
    if (parameterNode == nullptr) {
        delete root;
        return nullptr;
    } else {
        root->addChild(parameterNode);
    }
    if (!checkTerminal(current, Token::COMMA).has_value()) {
        current++;
        auto parameterListNode = parameterList();
        if (parameterListNode == nullptr) {
            delete root;
            return nullptr;
        } else {
            root->addChild(parameterListNode);
        }
    }
    return root;
}

Node *Parser::parameter() {
    auto root = new Node(Node::Parameter);
    if (current >= tokens.end()) {
        logError("Parameter requires a type specifier.", tokens.end() - 1);
        delete root;
        return nullptr;
    }
    if (!isTypeSpec(current)) {
        logError("Parameter requires a type specifier.", current);
        delete root;
        return nullptr;
    } else {
        auto typeSpecNode = new Node(Node::TypeSpec, *current);
        current++;
        root->addChild(typeSpecNode);
    }
    auto isIdentifier = checkTerminal(current, Token::IDENTIFIER);
    if (isIdentifier.has_value()) {
        logError("Parameter requires an identifier.", isIdentifier.value());
        delete root;
        return nullptr;
    } else {
        auto identifierNode = new Node(Node::Identifier, *current);
        current++;
        root->addChild(identifierNode);
    }
    return root;
}

Node *Parser::parameterTypeList() {
    auto root = new Node(Node::ParameterTypeList);
    auto parameterTypeNode = parameterType();
    if (parameterTypeNode == nullptr) {
        delete root;
        return nullptr;
    } else {
        root->addChild(parameterTypeNode);
    }
    if (!checkTerminal(current, Token::COMMA).has_value()) {
        current++;
        auto parameterTypeListNode = parameterTypeList();
        if (parameterTypeListNode == nullptr) {
            delete root;
            return nullptr;
        } else {
            root->addChild(parameterTypeListNode);
        }
    }
    return root;
}

Node *Parser::parameterType() {
    auto root = new Node(Node::ParameterType);
    if (current >= tokens.end()) {
        logError("type specifier required.", tokens.end() - 1);
        delete root;
        return nullptr;
    }
    if (!isTypeSpec(current)) {
        logError("type specifier required.", current);
        delete root;
        return nullptr;
    } else {
        auto typeSpecNode = new Node(Node::TypeSpec, *current);
        current++;
        root->addChild(typeSpecNode);
    }
    auto isIdentifier = checkTerminal(current, Token::IDENTIFIER);
    if (!isIdentifier.has_value()) {
        auto identifierNode = new Node(Node::Identifier, *current);
        current++;
        root->addChild(identifierNode);
    }
    return root;
}

Node *Parser::compoundStatements() {
    auto root = new Node(Node::CompoundStatements);
    auto isOpenBrace = checkTerminal(current, Token::OPEN_BRACE);
    if (isOpenBrace.has_value()) {
        logError("Compound statements require a {", isOpenBrace.value());
        delete root;
        return nullptr;
    } else {
        current++;
    }
    if (!checkTerminal(current, Token::CLOSE_BRACE).has_value()) {
        current++;
        return root;
    }
    auto statementsNode = statements();
    if (statementsNode == nullptr) {
        delete root;
        return nullptr;
    } else {
        root->addChild(statementsNode);
    }
    auto isCloseBrace = checkTerminal(current, Token::CLOSE_BRACE);
    if (isCloseBrace.has_value()) {
        logError("Compound statements require a }", isCloseBrace.value());
        delete root;
        return nullptr;
    } else {
        current++;
    }
    return root;
}

Node *Parser::statements() {
    auto root = new Node(Node::Statements);
    auto statNode = statement();
    if (statNode == nullptr) {
        delete root;
        return nullptr;
    } else {
        root->addChild(statNode);
    }
    if (current < tokens.end()) {
        if (isFirstOfExpression(current)
            || current->type == Token::OPEN_BRACE
            || current->type == Token::IF
            || current->type == Token::WHILE
            || current->type == Token::FOR
            || current->type == Token::RETURN
            || current->type == Token::BREAK
            || current->type == Token::CONTINUE
            || isFirstOfLocalVarDef(current)
            || current->type == Token::SEMICOL) {
            auto statementsNode = statements();
            if (statementsNode == nullptr) {
                delete root;
                return nullptr;
            } else {
                root->addChild(statementsNode);
            }
        }
    }
    return root;
}

bool Parser::isConstNumber(Parser::iterator t) {
    auto type = t->type;
    return type == Token::CONST_INT
           || type == Token::CONST_CHAR
           || type == Token::CONST_FLOAT
           || type == Token::CONST_LONG;
}

bool Parser::isFirstOfExpression(Parser::iterator t) {
    return t->type == Token::OPEN_PAREN
           || t->type == Token::IDENTIFIER
           || isConstNumber(t);
}

bool Parser::isFirstOfLocalVarDef(Parser::iterator t) {
    return t->type == Token::CONST
           || isTypeSpec(t);
}

Node *Parser::statement() {
    auto root = new Node(Node::Statement);
    Node *child = nullptr;
    if (current >= tokens.end()) {
        logError("A statement is expected.", tokens.end() - 1);
        delete root;
        return nullptr;
    }
    if (isFirstOfExpression(current)) {
        child = expression();
    } else if (current->type == Token::OPEN_BRACE) {
        child = compoundStatements();
    } else if (current->type == Token::IF) {
        child = ifStatement();
    } else if (current->type == Token::WHILE) {
        child = whileStatement();
    } else if (current->type == Token::FOR) {
        child = forStatement();
    } else if (current->type == Token::RETURN) {
        child = returnStatement();
    } else if (current->type == Token::BREAK) {
        child = breakStatement();
    } else if (current->type == Token::CONTINUE) {
        child = continueStatement();
    } else if (isFirstOfLocalVarDef(current)) {
        child = localVarDef();
    } else if (current->type == Token::SEMICOL) {
        current++;
        return root;
    } else {
        logError("Illegal statement!", current);
        delete root;
        return nullptr;
    }
    if (child == nullptr) {
        delete root;
        return nullptr;
    } else {
        root->addChild(child);
    }
    return root;
}
