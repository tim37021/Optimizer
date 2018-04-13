#include <iostream>
#include <functional>
#include <cassert>
#include <vector>
#include "optimizer.h"

using namespace std;

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

int main(int argc, char *argv[])
{
    opt::Variable x = opt::AddVariable("x");
    opt::Expression expr = (x^4)*(e^-x);

    // build f(x)
    auto f = opt::MakeFunction(expr, x);
    cout<<f(1.0)<<endl;

    cout<<GoldenSearch(f, 1e-5, -1, 10)<<endl;
    cout<<FibonacciSearch(f, 1e-5, 0.5e-5, -1, 10)<<endl;
    return 0;
}
