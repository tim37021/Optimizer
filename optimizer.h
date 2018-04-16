#ifndef _OPTIMIZER_H
#define _OPTIMIZER_H

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <array>
#include <functional>

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
        GraphNode operator-(GraphNode rhs) const;
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

    template <size_t N>
    class Function {
    public:
        template <class ...Args>
        Function(GraphNode node, Args&... args)
        : node(node), variables({args...})
        {

        }
        template <class ...Args>
        auto operator()(Args... args) const -> std::enable_if_t<sizeof...(Args) == N, double>
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
        mutable std::array<Variable, N> variables;
        GraphNode node;
    };

    template <class ...Args>
    auto MakeFunction(Expression expr, Args&... args) -> Function<sizeof...(args)>
    {
        return Function<sizeof...(args)>(expr, args...);
    }
    extern const opt::Constant e;
    double GoldenSearch(std::function<double(double)> func, double l, double a, double b);
    double FibonacciSearch(std::function<double(double)> func, double l, double epsilon, double a, double b);
}

std::ostream &operator<<(std::ostream &out, const opt::GraphNode &var);
std::istream &operator>>(std::istream &in, opt::GraphNode &var);
opt::Constant operator "" _const(long double val);

#endif