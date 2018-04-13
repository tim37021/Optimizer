#ifndef _OPTIMIZER_H
#define _OPTIMIZER_H

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <array>

namespace opt
{
    class Problem;
    class GraphNode;
    using Variable = GraphNode;
    using Constant = GraphNode;
    using Expression = GraphNode;
    struct IGraphNodeImpl {
        virtual double eval() const = 0;
        virtual bool isMutable() const =0;
        virtual void assign(double val)=0;
    };

    Variable AddVariable(const std::string &name);
    Constant AddVariable(double val);
    class GraphNode {
        friend Variable AddVariable(const std::string &name);
        friend Constant AddConstant(double val);
    public:
        GraphNode(double val);
        double eval() const
        { return impl->eval(); }
        bool isMutable() const
        { return impl->isMutable(); }
        void assign(double val)
        { return impl->assign(val); }
        GraphNode operator+(GraphNode rhs) const;
        // power
        GraphNode operator^(GraphNode rhs) const;
        GraphNode operator*(GraphNode rhs) const;
        // negative
        GraphNode operator-() const;
        double operator()() const
        { return eval(); }
        GraphNode &operator=(const GraphNode &rhs)=delete;
    private:
        GraphNode(IGraphNodeImpl *impl);
        GraphNode(const std::string &name, IGraphNodeImpl *impl);
        IGraphNodeImpl *impl;
        std::string name;
    };

    class VariableNodeImpl: public IGraphNodeImpl {
    public:
        virtual double eval() const
        { return val; }
        virtual bool isMutable() const
        { return true;} 
        virtual void assign(double val_)
        { val = val_; }
    private:
        double val;
    };

    class ConstantNodeImpl: public IGraphNodeImpl {
    public:
        ConstantNodeImpl(double val) : val(val){}
        virtual double eval() const
        { return val; }
        virtual bool isMutable() const
        { return false;} 
        virtual void assign(double val_)
        {
            // nothing..
        }
    private:
        double val;
    };

    class OpAddNodeImpl: public IGraphNodeImpl {
    public:
        OpAddNodeImpl(GraphNode lhs, GraphNode rhs)
        : lhs(lhs), rhs(rhs) {}
        virtual double eval() const
        { return lhs.eval() + rhs.eval(); }
        virtual bool isMutable() const
        { return false;} 
        virtual void assign(double val_)
        {
            // nothing..
        }
    private:
        GraphNode lhs, rhs;
    };

    class OpMulNodeImpl: public IGraphNodeImpl {
    public:
        OpMulNodeImpl(GraphNode lhs, GraphNode rhs)
        : lhs(lhs), rhs(rhs) {}
        virtual double eval() const
        { return lhs.eval() * rhs.eval(); }
        virtual bool isMutable() const
        { return false;} 
        virtual void assign(double val_)
        {
            // nothing..
        }
    private:
        GraphNode lhs, rhs;
    };

    class OpPowerNodeImpl: public IGraphNodeImpl {
    public:
        OpPowerNodeImpl(GraphNode lhs, GraphNode rhs)
        : lhs(lhs), rhs(rhs) {}
        virtual double eval() const
        { return std::pow(lhs.eval(), rhs.eval()); }
        virtual bool isMutable() const
        { return false;} 
        virtual void assign(double val_)
        {
            // nothing..
        }
    private:
        GraphNode lhs, rhs;
    };

    class OpNegativeNodeImpl: public IGraphNodeImpl {
    public:
        OpNegativeNodeImpl(GraphNode rhs)
        : rhs(rhs) {}
        virtual double eval() const
        { return -rhs.eval(); }
        virtual bool isMutable() const
        { return false;} 
        virtual void assign(double val_)
        {
            // nothing..
        }
    private:
        GraphNode rhs;
    };

    template <size_t N>
    class Function {
    public:
        template <class ...Args>
        Function(GraphNode node, Args&... args)
        : node(node), variables({args...})
        {

        }
        template <class ...Args>
        auto operator()(const Args&... args) -> std::enable_if_t<sizeof...(Args) == N, double>
        {
            std::array<double, sizeof...(args)> v = {args...};
            for(int i=0; i<v.size(); i++)
                variables[i].assign(v[i]);
            return node.eval();
        }

        GraphNode operator=(GraphNode node_)
        {
            node = node_;
        }
    private:
        std::array<Variable, N> variables;
        GraphNode node;
    };

    template <class ...Args>
    auto MakeFunction(Expression expr, Args&... args) -> Function<sizeof...(args)>
    {
        return Function<sizeof...(args)>(expr, args...);
    }
}

std::ostream &operator<<(std::ostream &out, const opt::GraphNode &var);
std::istream &operator>>(std::istream &in, opt::GraphNode &var);
opt::Constant operator "" _const(long double val);

const opt::Constant e = 2.71828_const;

#endif