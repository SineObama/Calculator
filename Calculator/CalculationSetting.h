#pragma once

#include "CalculationException.h"

namespace Sine {
namespace Calculator {

struct Operator {
    Operator(int hash, int p, void *f, bool b) {
        prior = p, func = f, binary = b;
    }
    int hash, prior;
    void *func;
    bool binary;
};

/**
 * 保存操作符对应的操作函数。支持单操作数和双操作数。
 * 操作符一般用字符表示，也可以用整型。以整型保存。
 */
template<class Value>
class CalculationSetting {

public:

    static CalculationSetting default();

    typedef Value(*OpPtr1)(const Value &);
    typedef Value(*OpPtr2)(const Value &, const Value &);

    Operator get(int hash);
    void set(int hash, OpPtr1 func, int prior);
    void set(int hash, OpPtr2 func, int prior);
    void clearFunctions();

private:

    std::map<int, Operator> op;

};

template<class Value>
CalculationSetting<Value> CalculationSetting<Value>::default() {
    static CalculationSetting<Value> def;
    static bool init = false;
    if (!init) {
        init = true;
        def.set('+', plus<Value>, 1);
        def.set('-', subtract<Value>, 1);
        def.set('*', multiply<Value>, 2);
        def.set('/', divide<Value>, 2);
    }
    return def;
}

template<class Value>
Value plus(const Value &a, const Value &b) {
    return a + b;
}

template<class Value>
Value subtract(const Value &a, const Value &b) {
    return a - b;
}

template<class Value>
Value multiply(const Value &a, const Value &b) {
    return a * b;
}

template<class Value>
Value divide(const Value &a, const Value &b) {
    return a / b;
}

template<class Value>
typename Operator CalculationSetting<Value>::get(int hash) {
    std::map<int, Operator>::iterator it = op.find(hash);
    if (it == op.end()) {
        char s[100] = {};
        sprintf_s(s, "operator with hash %d not found.", hash);
        throw UnknownCharactor(s);
    }
    return it->second;
}

template<class Value>
void CalculationSetting<Value>::set(int hash, OpPtr1 func, int prior) {
    op.insert_or_assign(hash, Operator(hash, prior, func, false));
    return;
}

template<class Value>
void CalculationSetting<Value>::set(int hash, OpPtr2 func, int prior) {
    op.insert_or_assign(hash, Operator(hash, prior, func, true));
    return;
}

template<class Value>
void CalculationSetting<Value>::clearFunctions() {
    op.clear();
}

}
}