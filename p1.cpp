#include <iostream>
#include "optimizer.h"

using namespace std;

int main(int argc, char *argv[])
{
    opt::Variable x = opt::AddVariable("x");
    opt::Constant e = 2.71828_const;
    opt::Expression expr = (x^4)*(e^-x);

    // build f(x)
    auto f = opt::MakeFunction(expr, x);

    cout<<opt::GoldenSearch(f, 1e-5, -1, 10)<<endl;
    cout<<opt::FibonacciSearch(f, 1e-5, 0.5e-5, -1, 10)<<endl;
    return 0;
}