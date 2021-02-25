#ifndef NODE_HPP
#define NODE_HPP
#include <vector>

class Node {
public:
    enum NodeType {

    };

    NodeType type;
    std::vector<Node *> children;

    Node(NodeType type) : type(type) {
        children.clear();
    }

    void addChild(Node *kid);
};

#endif