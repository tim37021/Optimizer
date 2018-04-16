#include <iostream>
#include "optimizer.h"
#ifdef PLOT
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
#endif
using namespace std;

static constexpr double a = -1;
static constexpr double b = 10;



int main(int argc, char *argv[])
{

    opt::Variable x = opt::AddVariable("x");
    opt::Constant e = 2.71828_const;
    opt::Expression expr = (x^4)*(e^-x);

    // build f(x)
    auto f = opt::MakeFunction(expr, x);
#ifdef PLOT
{
    std::vector<double> x(101);
    for(int i=0; i<101; i++)
        x[i] = a+i*(b-a)/100;
    plt::plot(x, f, "b");
    plt::show();
}
#endif

    cout<<opt::GoldenSearch(f, 1e-5, -1, 10)<<endl;
    cout<<opt::FibonacciSearch(f, 1e-5, 0.5e-5, -1, 10)<<endl;
    return 0;
}
