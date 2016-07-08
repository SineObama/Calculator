#pragma once

#include "CalculationException.h"
#include "Functions.h"

namespace sine {
namespace calculator {

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

    static CalculationSetting getDefault();

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
CalculationSetting<Value> CalculationSetting<Value>::getDefault() {
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
