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
