#include <iostream>
#include <functional>
#include "optimizer.h"

using namespace std;

// interval [-2, 2]
void ND1D(std::function<double(double,double)> func, double epsilon, double &x1, double &x2)
{
    int phase = 0;
    double x1_=x1, x2_=x2;
    while(1) {
        if(phase%2==0) {
            auto func_ = std::bind(func, std::placeholders::_1, x2);
            x1 = opt::GoldenSearch(func_, 1e-5, -2, 2);
        } else {
            auto func_ = std::bind(func, x1, std::placeholders::_1);
            x2 = opt::GoldenSearch(func_, 1e-5, -2, 2);
        }
        if((x1-x1_)*(x1-x1_)+(x2-x2_)*(x2-x2_) < epsilon) 
            break;
        x1_ = x1;
        x2_ = x2;
        phase++;
    }
}

int main(int argc, char *argv[])
{
    opt::Variable x1 = opt::AddVariable("x1");
    opt::Variable x2 = opt::AddVariable("x2");
    opt::Expression expr = (x1^4)+(x2^2)*4.0-(x1^2)-x1*x2*4.0;
    auto f = opt::MakeFunction(expr, x1, x2);

    {
        double x1=-0.5, x2=-0.5;
        ND1D(f, 1e-5, x1, x2);
        cout<<x1<<" "<<x2<<endl;
    }

    return 0;
}