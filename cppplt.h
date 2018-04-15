#ifndef _CPPPLT_H_
#define _CPPPLT_H_

#include <functional>

namespace cppplt
{
    void plot_1d(std::function<double(double)> func, double a, double b, int steps=100);
}


#endif