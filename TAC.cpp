#ifndef _PL0C_TAC
#define _PL0C_TAC

#include "tokens.cpp"

class TAC{
public:
    string op;      // 运算符
    string y;       // 运算数1
    string z;       // 运算数2
    string x;       // 运算结果
    TAC(string o, string y, string z, string x): op(o), y(y), z(z), x(x) {};
};

#endif
