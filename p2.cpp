#include <iostream>
#include <functional>
#include "optimizer.h"

using namespace std;

// interval [-2, 2]
int ND1D(std::function<double(double,double)> func, double epsilon, double &x1, double &x2)
{
    int k = 0;
    double x1_=x1, x2_=x2;
    while(1) {
        if(k%2==0) {
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
        k++;
    }
    return k+1;
}

int Powell(std::function<double(double,double)> func, double epsilon, double &x1, double &x2) {
    auto wrapper = [func] (double x1, double x2, double dx1, double dx2, double lambda) {
        return func(x1+lambda*dx1, x2+lambda*dx2);
    };
    double z[2];
    double s[2][2] = {{1, 0}, {0, 1}};
    const int n = 2;

    // find lambda* to minimize f(X1+lambdaSn)
    auto update = [&x1, &x2, &s, wrapper](int idx) -> double {
        auto func_ = std::bind(wrapper, x1, x2, s[idx][0], s[idx][1], std::placeholders::_1);
        double lambda = opt::GoldenSearch(func_, 1e-5, -2, 2);
        x1 = x1+lambda*s[idx][0];
        x2 = x2+lambda*s[idx][1];
        return lambda*s[idx][0]*lambda*s[idx][0]+lambda*s[idx][1]*lambda*s[idx][1];
    };

    update(1);
    int i = 1;
    z[0] = x1; z[1] = x2;
    int k=1;

    while(1) {
        if(i<n+1) {
            if(update(i-1)<1e-5)
                break;
        } else {
            for(int i=0; i<n-1; i++) {
                s[i][0] = s[i+1][0];
                s[i][1] = s[i+1][1];
            }
            s[n-1][0] = x1 - z[0];
            s[n-1][1] = x2 - z[1];
            if(update(n-1)<1e-5)
                break;
            z[0] = x1; z[1] = x2;
            i=1;
        }
        k++;
        i++;
    }
    return k;
}

int main(int argc, char *argv[])
{
    opt::Variable x1 = opt::AddVariable("x1");
    opt::Variable x2 = opt::AddVariable("x2");
    opt::Expression expr = (x1^4)+(x2^2)*4.0-(x1^2)-x1*x2*4.0;
    auto f = opt::MakeFunction(expr, x1, x2);

    {
        double x1=-0.5, x2=-0.5;
        int steps = ND1D(f, 1e-5, x1, x2);
        cout<<x1<<" "<<x2<<endl;
        cout<<steps<<endl;
    }

    {
        double x1=-0.5, x2=-0.5;
        int steps = Powell(f, 1e-5, x1, x2);
        cout<<x1<<" "<<x2<<endl;
        cout<<steps<<endl;
    }
    

    return 0;
}