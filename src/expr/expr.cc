
#include "expr.h"
#include <string>
#include <unordered_map>
#include <vector>

using namespace expr;

ExprNode::ExprNode(NodeType type) : type(type) { }

ExprNode::ExprNode(NodeType type, const std::vector<ExprNode *> &
children, std::string content) : type(type), children(children),
content(content) { }

ExprNode::ExprNode(NodeType type, const std::vector<ExprNode *> &
children, double number) : type(type), children(children),
number(number) { }

ExprNode::ExprNode(const ExprNode &other) : type(other.type),
content(other.content), number(other.number) {
    for (ExprNode *child : other.children)
        children.push_back(new ExprNode(*child));
}

ExprNode::~ExprNode() {
    for (ExprNode *child : children)
        delete child;
}

bool ExprNode::operator==(const ExprNode &other) const {
    if (type != other.type)
        return false;
    if (type == NODE_NUM && number != other.number)
        return false;
    if (type != NODE_NUM && content != other.content)
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
    for (size_t i = other.children.size(); i < children.size(); i++)
        delete children[i];
    size_t prevSize = children.size();
    children.resize(other.children.size());
    for (size_t i = 0; i < prevSize; i++)
        *children[i] = *other.children[i];
    for (size_t i = prevSize; i < children.size(); i++) 
        children[i] = new ExprNode(*other.children[i]);
    type = other.type;
    content = other.content;
    number = other.number;
    return *this;
}

ExprNode &ExprNode::operator[](size_t index) {
    while (children.size() <= index)
        children.push_back(new ExprNode(NODE_ERR));
    return *children[index];
}

const ExprNode &ExprNode::operator[](size_t index) const {
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
            return content;
        case NODE_NUM:
            return std::to_string(number);
        case NODE_DERIV:
            return "d[" + content.substr(0, content.find(';')) + "](" +
            content.substr(content.find(';') + 1) + ")";
        case NODE_MINUS:
            return "-(" + children[0]->str() + ")";
        case NODE_INTEG:
            return "integ(" + children[0]->str() + ", " + children[1]->str() +
            ")";
        case NODE_LIST: {
            std::string out = "[";
            for (size_t i = 0; i < children.size(); i++) {
                if (i != 0)
                    out.append(", ");
                out.append(children[i]->str());
            }
            out.push_back(']');
            return out;
        }
        default:
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

void ExprNode::replaceSymbol(const std::string &name, double val) {
    if (type == NODE_SYMB && content == name) {
        type = NODE_NUM;
        number = val;
        return;
    }
    for (ExprNode *child : children)
        child->replaceSymbol(name, val);
}

double ExprNode::eval() const {
    return evalDirect({});
}

double ExprNode::evalDirect(const std::unordered_map<std::string, double>
&symbols) const {
    switch (type) {
        case NODE_SYMB:
            if (symbols.find(content) == symbols.end())
                throw std::runtime_error("Could not evaluate expression, "
                "unknown symbol value");
            return symbols.at(content);
        case NODE_NUM:
            return number;
        case NODE_ADD:
            return operator[](0).eval() + operator[](1).eval();
        case NODE_SUB:
            return operator[](0).eval() - operator[](1).eval();
        case NODE_MUL:
            return operator[](0).eval() * operator[](1).eval();
        case NODE_DIV:
            return operator[](0).eval() / operator[](1).eval();
        case NODE_LT:
            return operator[](0).eval() < operator[](1).eval();
        case NODE_LTE:
            return operator[](0).eval() <= operator[](1).eval();
        case NODE_GT:
            return operator[](0).eval() > operator[](1).eval();
        case NODE_GTE:
            return operator[](0).eval() >= operator[](1).eval();
        case NODE_EQ:
            return operator[](0).eval() == operator[](1).eval();
        case NODE_NEQ:
            return operator[](0).eval() != operator[](1).eval();
        case NODE_AND:
            return operator[](0).eval() && operator[](1).eval();
        case NODE_OR:
            return operator[](0).eval() || operator[](1).eval();
        case NODE_MINUS:
            return -operator[](0).eval();
        default:
            throw std::runtime_error("Could not evaluate expression");
    }
    return 0.0;
}

std::string ExprNode::binaryStr() const {
    static const std::unordered_map<NodeType, std::string> typeMap = {
        {NODE_ADD, "+"}, {NODE_SUB, "-"}, {NODE_MUL, "*"}, {NODE_DIV, "/"},
        {NODE_LT, "<"}, {NODE_LTE, "<="}, {NODE_GT, ">"}, {NODE_GTE, ">="},
        {NODE_EQ, "="}, {NODE_NEQ, "!="}, {NODE_AND, " and "},
        {NODE_OR, " or "},
    };
    return "(" + children[0]->str() + typeMap.at(type) + children[1]->str() +
    ")";
}