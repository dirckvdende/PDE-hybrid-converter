
#include "dbg/dbg.h"
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
content(other.content), number(other.number), index(other.index),
deriv(other.deriv) {
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
    } else if (type == NODE_DIM) {
        if (index != other.index)
            return false;
    } else if (type == NODE_DERIV) {
        if (deriv.dims != other.deriv.dims || deriv.count != other.deriv.count
        || deriv.var != other.deriv.var)
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
    index = other.index;
    deriv = other.deriv;
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
        case NODE_DERIV: {
            std::string out = "d[";
            if (deriv.count.empty()) {
                bool first = true;
                for (const std::string &dim : deriv.dims) {
                    if (!first)
                        out.push_back(' ');
                    first = false;
                    out.append(dim);
                }
            } else {
                bool first = true;
                for (const size_t &amt : deriv.count) {
                    if (!first)
                        out.append(", ");
                    first = false;
                    out.append(std::to_string(amt));
                }
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
        case NODE_DIM:
            return "DIM{" + std::to_string(index) + "}";
        case NODE_VAR_MARKER:
            return "VAR{" + std::to_string(index) + "}";
        case NODE_FUNC:
            return content + "(" + children[0]->str() + ")";
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
    const ExprNode *cur = this;
    return evalDirect([&](const ExprNode &node) -> double {
        (void)node;
        dbg::error("Could not evaluate expression \"" + cur->str() + "\"");
        return 0.0;
    });
}

double ExprNode::evalDirect(std::function<double(const ExprNode &)> fallback)
const {
    switch (type) {
        case NODE_NUM:
            return number;
        case NODE_ADD:
            return children[0]->evalDirect(fallback) +
            children[1]->evalDirect(fallback);
        case NODE_SUB:
            return children[0]->evalDirect(fallback) -
            children[1]->evalDirect(fallback);
        case NODE_MUL:
            return children[0]->evalDirect(fallback) *
            children[1]->evalDirect(fallback);
        case NODE_DIV:
            return children[0]->evalDirect(fallback) /
            children[1]->evalDirect(fallback);
        case NODE_LT:
            return children[0]->evalDirect(fallback) <
            children[1]->evalDirect(fallback);
        case NODE_LTE:
            return children[0]->evalDirect(fallback) <=
            children[1]->evalDirect(fallback);
        case NODE_GT:
            return children[0]->evalDirect(fallback) >
            children[1]->evalDirect(fallback);
        case NODE_GTE:
            return children[0]->evalDirect(fallback) >=
            children[1]->evalDirect(fallback);
        case NODE_EQ:
            return children[0]->evalDirect(fallback) ==
            children[1]->evalDirect(fallback);
        case NODE_NEQ:
            return children[0]->evalDirect(fallback) !=
            children[1]->evalDirect(fallback);
        case NODE_AND:
            return children[0]->evalDirect(fallback) &&
            children[1]->evalDirect(fallback);
        case NODE_OR:
            return children[0]->evalDirect(fallback) ||
            children[1]->evalDirect(fallback);
        case NODE_MINUS:
            return -children[0]->evalDirect(fallback);
        case NODE_FUNC:
            return funcs.at(content).eval(children[0]->evalDirect(fallback));
        default:
            return fallback(*this);
    }
    return 0.0;
}

void ExprNode::walk(std::function<void(ExprNode &)> func) {
    func(*this);
    for (ExprNode *child : children)
        child->walk(func);
}

void ExprNode::walk(std::function<void(const ExprNode &)> func) const {
    func(*this);
    for (const ExprNode *child : children)
        child->walk(func);
}

double ExprNode::evalDims(const std::vector<double> &vals) const {
    const ExprNode *cur = this;
    return evalDirect([&](const ExprNode &node) -> double {
        if (node.type == NODE_DIM && node.index < vals.size())
            return vals[node.index];
        dbg::error("Could not evaluate node \"" + cur->str() + "\"");
        return 0.0;
    });
}

void ExprNode::replaceDims(const std::unordered_map<std::string, size_t>
&dimMap) {
    if (type == NODE_SYMB && dimMap.find(content) != dimMap.end()) {
        type = NODE_DIM;
        index = dimMap.at(content);
        return;
    }
    for (ExprNode *child : children)
        child->replaceDims(dimMap);
}

void ExprNode::replaceDims(const std::vector<double> &val) {
    if (type == NODE_DIM) {
        type = NODE_NUM;
        number = val[index];
        return;
    }
    for (ExprNode *child : children)
        child->replaceDims(val);
}

void ExprNode::replaceVars(const std::unordered_map<std::string, size_t>
&nameMap) {
    if (type == NODE_SYMB && nameMap.find(content) != nameMap.end()) {
        type = NODE_VAR_MARKER;
        index = nameMap.at(content);
        return;
    }
    for (ExprNode *child : children)
        child->replaceVars(nameMap);
}

double ExprNode::evalVars(const std::vector<double> &vals) const {
    const ExprNode *cur = this;
    return evalDirect([&](const ExprNode &node) -> double {
        if (node.type == NODE_VAR_MARKER && node.index < vals.size())
            return vals[node.index];
        dbg::error("Could not evaluate node \"" + cur->str() + "\"");
        return 0.0;
    });
}

bool ExprNode::containsNonTimeVars() const {
    if (type == NODE_SYMB && content != "t")
        return true;
    for (ExprNode *child : children)
        if (child->containsNonTimeVars())
            return true;
    return false;
}

ExprNode ExprNode::diff() const {
    ExprNode *d = diffAlloc();
    ExprNode c = *d;
    delete d;
    return c;
}

ExprNode *ExprNode::diffAlloc() const {
    switch (type) {
        case NODE_NUM:
            return new ExprNode(NODE_NUM, {}, 0.0);
        case NODE_ADD:
            return new ExprNode(NODE_ADD, {
                children[0]->diffAlloc(),
                children[1]->diffAlloc()
            });
        case NODE_SUB:
            return new ExprNode(NODE_SUB, {
                children[0]->diffAlloc(),
                children[1]->diffAlloc()
            });
        case NODE_MUL:
            return new ExprNode(NODE_ADD, {
                new ExprNode(NODE_MUL, {
                    children[0]->diffAlloc(),
                    children[1]->copy(),
                }),
                new ExprNode(NODE_MUL, {
                    children[0]->copy(),
                    children[1]->diffAlloc(),
                }),
            });
        case NODE_DIV:
            return new ExprNode(NODE_DIV, {
                new ExprNode(NODE_SUB, {
                    new ExprNode(NODE_MUL, {
                        children[0]->diffAlloc(),
                        children[1]->copy(),
                    }),
                    new ExprNode(NODE_MUL, {
                        children[0]->copy(),
                        children[1]->diffAlloc(),
                    }),
                }),
                new ExprNode(NODE_MUL, {
                    children[1]->copy(),
                    children[1]->copy(),
                }),
            });
        case NODE_MINUS:
            return new ExprNode(NODE_MINUS, {
                children[0]->diffAlloc(),
            });
        case NODE_FUNC:
            return diffFunc();
        default:
            dbg::error("Could not differentiate expression \"" + str() + "\"");
    }
    return nullptr;
}

ExprNode *ExprNode::copy() const {
    ExprNode *out = new ExprNode;
    *out = *this;
    return out;
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

ExprNode *ExprNode::diffFunc() const {
    ExprNode *out = funcs.at(content).diff(*this);
    if (out == nullptr)
        dbg::error("Could not differentiate expression \"" + str() + "\"");
    return out;
}