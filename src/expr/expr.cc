
#include "expr.h"
#include <string>
#include <vector>

ExprNode::ExprNode(const ExprNodeType type) : type(type) { }

ExprNode::~ExprNode() {
    for (ExprNode *child : children)
        delete child;
}

ExprNode::ExprNode(const ExprNode &other) : type(other.type),
content(other.content) {
    for (ExprNode *child : other.children)
        children.push_back(new ExprNode(*child));
}

bool ExprNode::operator==(const ExprNode &other) const {
    if (type != other.type || content != other.content)
        return false;
    if (other.children.size() != children.size())
        return false;
    for (size_t i = 0; i < children.size(); i++)
        if (*children[i] != *other.children[i])
            return false;
    return true;
}

bool ExprNode::operator!=(const ExprNode &other) const {
    return !operator==(other);
}

ExprNode &ExprNode::operator=(const ExprNode &other) {
    for (ExprNode *child : children)
        delete child;
    children.clear();
    type = other.type;
    content = other.content;
    for (ExprNode *child : other.children)
        children.push_back(new ExprNode(*child));
}

ExprNode &ExprNode::operator[](size_t index) {
    return *children[index];
}

size_t ExprNode::size() const {
    return children.size();
}