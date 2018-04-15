#include "cppplt.h"

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

namespace cppplt
{
    void plot_1d(std::function<double(double)> func, double a, double b, int steps)
    {
        std::vector<double> y(101);
        double step = (b-a)/steps;
        for(int i=0; i<101; i++) {
            y[i] = func(a+step*i);
        }
        plt::plot(y);
        plt::show();
    }
}