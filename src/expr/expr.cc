
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
content(other.content), number(other.number), markerIndex(other.markerIndex) {
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
    if (type == NODE_NUM) {
        if (number != other.number)
            return false;
    } else if (type == NODE_MARKER) {
        if (markerIndex != other.markerIndex)
            return false;
    } else {
        if (content != other.content)
            return false;
    }
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
    markerIndex = other.markerIndex;
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
        case NODE_MARKER:
            return "{" + std::to_string(markerIndex) + "}";
        case NODE_SYMB:
            return content;
        case NODE_NUM:
            return std::to_string(number);
        case NODE_DERIV: {
            std::string out = "d[";
            bool first = true;
            for (const std::string &dim : deriv.dims) {
                if (!first)
                    out.push_back(' ');
                first = false;
                out.append(dim);
            }
            out.append("](" + deriv.var + ")");
            return out;
        }
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

void ExprNode::replaceSymbols(const std::unordered_map<std::string, size_t>
&symbols) {
    if (type == NODE_SYMB && symbols.find(content) != symbols.end()) {
        type = NODE_MARKER;
        markerIndex = symbols.at(content);
        return;
    }
    for (ExprNode *child : children)
        child->replaceSymbols(symbols);
}

void ExprNode::replaceSymbols(const std::unordered_map<std::string, std::string>
&symbols) {
    if (type == NODE_SYMB && symbols.find(content) != symbols.end()) {
        content = symbols.at(content);
        return;
    }
    for (ExprNode *child : children)
        child->replaceSymbols(symbols);
}

void ExprNode::findAllSymbols(std::unordered_set<std::string> &symbols) const {
    if (type == NODE_SYMB) {
        symbols.insert(content);
        return;
    }
    for (ExprNode *child : children)
        child->findAllSymbols(symbols);
}

double ExprNode::eval() const {
    return evalDirect({});
}

double ExprNode::evalDirect(const std::vector<double> &vals) const {
    switch (type) {
        case NODE_MARKER:
            if (markerIndex >= vals.size())
                throw std::runtime_error("Could not evaluate expression, "
                "undefined marker value");
            return vals[markerIndex];
        case NODE_NUM:
            return number;
        case NODE_ADD:
            return evalBinary([](double x, double y) -> double {return x + y;},
            vals);
        case NODE_SUB:
            return evalBinary([](double x, double y) -> double {return x - y;},
            vals);
        case NODE_MUL:
            return evalBinary([](double x, double y) -> double {return x * y;},
            vals);
        case NODE_DIV:
            return evalBinary([](double x, double y) -> double {return x / y;},
            vals);
        case NODE_LT:
            return evalBinary([](double x, double y) -> double {return x < y;},
            vals);
        case NODE_LTE:
            return evalBinary([](double x, double y) -> double {return x <= y;},
            vals);
        case NODE_GT:
            return evalBinary([](double x, double y) -> double {return x > y;},
            vals);
        case NODE_GTE:
            return evalBinary([](double x, double y) -> double {return x >= y;},
            vals);
        case NODE_EQ:
            return evalBinary([](double x, double y) -> double {return x == y;},
            vals);
        case NODE_NEQ:
            return evalBinary([](double x, double y) -> double {return x != y;},
            vals);
        case NODE_AND:
            return evalBinary([](double x, double y) -> double {return x && y;},
            vals);
        case NODE_OR:
            return evalBinary([](double x, double y) -> double {return x || y;},
            vals);
        case NODE_MINUS:
            return -operator[](0).evalDirect(vals);
        default:
            throw std::runtime_error("Could not evaluate expression");
    }
    return 0.0;
}

void ExprNode::replaceDirect(const std::vector<double> &vals) {
    if (type == NODE_MARKER) {
        if (markerIndex >= vals.size())
            throw std::runtime_error("Index of marker too high to evaluate");
        type = NODE_NUM;
        number = vals[markerIndex];
        return;
    }
    for (ExprNode *child : children)
        child->replaceDirect(vals);
}

bool ExprNode::containsNonTimeVars() const {
    if (type == NODE_SYMB && content != "t")
        return true;
    for (ExprNode *child : children)
        if (child->containsNonTimeVars())
            return true;
    return false;
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

double ExprNode::evalBinary(double (*op)(double, double), const
std::vector<double> &vals) const {
    return op(operator[](0).evalDirect(vals), operator[](1).evalDirect(vals));
}