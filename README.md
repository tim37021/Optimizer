# Optimizer

A optimizer framework written in my spare time.

# Example
```cpp
opt::Variable x = opt::AddVariable("x");
opt::Constant e = 2.71828_const;
opt::Expression expr = (x^4)*(e^-x);
```

To evaluate expression
```cpp
// it will use the current value of variables
cout<<expr.eval()<<endl;
```

Pack expression into function(aka define function)
```cpp
// build f(x)
auto f = opt::MakeFunction(expr, x);
cout<<f(1.0)<<endl;
```
Constant literal
```cpp
opt::Constant pi = 3.1415926_const;
```

# Build
You must install numpy and matplotlib first to draw graph. For some unknown reason, I cannot get numpy work in python_d. Thus, debug build using VC++ may not run correctly. However, mingw and release mode works perfectly. Note that if you are using mingw, you might need to modify pyconfig.h
```bash
pip install numpy matplotlib

mkdir build && cd build && cmake .. -G "Visual Studio 15 2017 Win64" -DENABLE_PLOT=1
cmake --build . --config Release
```

Remove this line in pyconfig.h
```cpp
#define hypot _hypot
```