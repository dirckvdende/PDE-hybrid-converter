
#include "expr.h"
#include <string>
#include <unordered_map>
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
    return *this;
}

ExprNode &ExprNode::operator[](size_t index) {
    return *children[index];
}

size_t ExprNode::size() const {
    return children.size();
}

std::string ExprNode::str() const {
    switch (type) {
        case NODE_ERR:
            return "[ERR]";
        case NODE_SYMB:
        case NODE_NUM:
            return content;
        case NODE_DERIV:
            return "d" + content + "(" + children[0]->str() + ")";
        case NODE_ADD:
        case NODE_SUB:
        case NODE_MUL:
        case NODE_DIV:
        case NODE_LT:
        case NODE_LTE:
        case NODE_GT:
        case NODE_GTE:
        case NODE_EQ:
        case NODE_NEQ:
            return binaryStr();
    }
    return "[ERR]";
}

void ExprNode::find(const ExprNode &other, std::vector<ExprNode *> &occ) {
    if (*this == other) {
        occ.push_back(this);
        return;
    }
    for (ExprNode *child : children)
        child->find(other, occ);
}

void ExprNode::replace(const ExprNode &search, const ExprNode &repl) {
    std::vector<ExprNode *> occ;
    find(search, occ);
    for (ExprNode *node : occ)
        *node = repl;
}

std::string ExprNode::binaryStr() const {
    static const std::unordered_map<ExprNodeType, std::string> typeMap = {
        {NODE_ADD, "+"}, {NODE_SUB, "-"}, {NODE_MUL, "*"}, {NODE_DIV, "/"},
        {NODE_LT, "<"}, {NODE_LTE, "<="}, {NODE_GT, ">"}, {NODE_GTE, ">="},
        {NODE_EQ, "=="}, {NODE_NEQ, "!="},
    };
    return "(" + children[0]->str() + typeMap.at(type) + children[1]->str() +
    ")";
}