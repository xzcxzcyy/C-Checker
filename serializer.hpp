//
// Created by xzcxzcyy on 2021/3/5.
//

#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <fstream>
#include <string>
#include "node.hpp"

class Serializer {
private:
    std::ostream &out;

    /**
     * Serialize node root with indent.
     * @param root
     * @param indent
     */
    void serialize(Node *root, int indent);

    /**
     * Print spaces with amount indent.
     * @param indent
     */
    void printIndent(int indent);

    /**
     * Serialize an Expression node.
     * @param root
     */
    void handleExpression(Node *root);

    /**
     * Handle a comment.
     * @param indent
     * @param commentString
     */
    void printComment(int indent, const std::string &commentString);

public:
    /**
     * Constructor.
     * @param out Output stream to which serialization results go.
     */
    explicit Serializer(std::ostream &out) : out(out) {}

    /**
     * Perform serialization on root.
     * @param root
     */
    void perform(Node *root);
};


#endif
