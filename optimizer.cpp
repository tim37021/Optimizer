#include "optimizer.h"

// Nodes implementation here
namespace opt
{
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

    class OpSubNodeImpl: public IGraphNodeImpl {
    public:
        OpSubNodeImpl(GraphNode lhs, GraphNode rhs)
        : lhs(lhs), rhs(rhs) {}
        virtual double eval() const
        { return lhs.eval() - rhs.eval(); }
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
}

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

    GraphNode GraphNode::operator-(GraphNode rhs) const
    {
        return GraphNode("SUB", new OpSubNodeImpl(*this, rhs));
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

    // (sqrt(5)-1)/2
    constexpr double alpha=(2.2360679775-1)/2; 

    // uncertainty l
    // perform search within [a, b]
    double GoldenSearch(std::function<double(double)> func, double l, double a, double b)
    {
        double lambda = a+(1-alpha)*(b-a);
        double mu = a+alpha*(b-a);
        double k = 1;
        double flambda = func(lambda);
        double fmu = func(mu);
        while(b-a>=l) {
            if(flambda>fmu) {
                a = lambda;
                lambda = mu;
                mu = a+alpha*(b-a);
                flambda = fmu;
                fmu = func(mu);
            } else {
                b = mu;
                mu = lambda;
                lambda = a+(1-alpha)*(b-a);
                fmu = flambda;
                flambda = func(lambda);
            }
            k++;
        }
        return (a+b)/2.0;
    }

    int fib(int n)
    {
        static int dp[10000]={0};
        if(n<=2)
            return 1;
        if(dp[n]!=0)
            return dp[n];
        return dp[n]=(fib(n-1)+fib(n-2));
    }

    double FibonacciSearch(std::function<double(double)> func, double l, double epsilon, double a, double b)
    {
        // choose a suitable n
        int n;
        for(n=1;fib(n)<=(b-a)/l; n++);

        int k=1;
        double lambda = a+(static_cast<double>(fib(n-2))/fib(n))*(b-a);
        double mu = a+(static_cast<double>(fib(n-1))/fib(n))*(b-a);

        double flambda=func(lambda);
        double fmu=func(mu);

        while(1) {
            if(flambda>fmu) {
                a = lambda;
                lambda = mu;
                mu = a+(static_cast<double>(fib(n-k-1))/fib(n-k))*(b-a);
                flambda = fmu;
                fmu = func(mu);
            } else {
                b = mu;
                mu = lambda;
                lambda = a+(static_cast<double>(fib(n-k-2))/fib(n-k))*(b-a);
                fmu = flambda;
                flambda = func(lambda);
            }
            if(k==n-2) {
                mu = lambda+epsilon;
                fmu = func(mu);
                if(flambda>fmu)
                    a = lambda;
                else
                    b = mu;
                break;
            }
            k++;
        }

        return (a+b)/2.0;
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