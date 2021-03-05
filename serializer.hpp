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

    void serialize(Node *root, int indent);

    void printIndent(int indent);

    void printComment(int indent, const std::string &commentString);

public:
    explicit Serializer(std::ostream &out) : out(out) {}

    void perform(Node *root);
};


#endif
