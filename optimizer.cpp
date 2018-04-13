#include "optimizer.h"

namespace opt
{
    const opt::Constant e = 2.71828_const;
    GraphNode::GraphNode(double val)
    : name("CONST"), impl(new ConstantNodeImpl(val))
    {

    }

    GraphNode::GraphNode(IGraphNodeImpl *impl)
        : name("UNNAMED"), impl(impl)
    {

    }
    GraphNode::GraphNode(const std::string &name, IGraphNodeImpl *impl)
        : name(name), impl(impl)
    {

    }

    GraphNode GraphNode::operator+(GraphNode rhs) const
    {
        return GraphNode("ADD", new OpAddNodeImpl(*this, rhs));
    }

    GraphNode GraphNode::operator^(GraphNode rhs) const
    {
        return GraphNode("POW", new OpPowerNodeImpl(*this, rhs));
    }

    GraphNode GraphNode::operator*(GraphNode rhs) const
    {
        return GraphNode("MUL", new OpMulNodeImpl(*this, rhs));
    }

    GraphNode GraphNode::operator-() const
    {
        return GraphNode("NEGATIVE", new OpNegativeNodeImpl(*this));
    }

    Variable AddVariable(const std::string &name)
    {
        return Variable(name, new VariableNodeImpl());
    }

    Constant AddConstant(double val)
    {
        return Constant(val);
    }
}

std::ostream &operator<<(std::ostream &out, const opt::GraphNode &var)
{
    out<<var.eval();
    return out;
}

std::istream &operator>>(std::istream &in, opt::GraphNode &var)
{
    double val;
    in>>val;
    var.assign(val);
    return in;
}

opt::Constant operator "" _const(long double val)
{
    return opt::AddConstant(val);
}