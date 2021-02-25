#include <vector>
#include "node.hpp"

void Node::addChild(Node *kid) {
    children.push_back(kid);
}
