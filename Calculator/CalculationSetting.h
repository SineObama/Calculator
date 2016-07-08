#pragma once

#include "CalculationException.h"

namespace Sine {
namespace Calculator {

template<class Value>
class CalculationSetting {

public:

    typedef Value(*OpPtr1)(const Value &);
    typedef Value(*OpPtr2)(const Value &, const Value &);

    struct OpDetail {
        OpDetail(int p, void *f, bool b) {
            prior = p, func = f, binary = b;
        }
        int prior;
        void *func;
        bool binary;
    };

    OpDetail get(int hash);
    void setFunction(int hash, OpPtr1 func, int prior);
    void setFunction(int hash, OpPtr2 func, int prior);
    void clearFunctions();

private:

    std::map<int, OpDetail> op;

};

template<class Value>
typename CalculationSetting<Value>::OpDetail CalculationSetting<Value>::get(int hash) {
    std::map<int, OpDetail>::iterator it = op.find(hash);
    if (it == op.end()) {
        char s[100] = {};
        sprintf_s(s, "operator with hash %d not found.", hash);
        throw UnknownCharactor(s);
    }
    return it->second;
}

template<class Value>
void CalculationSetting<Value>::setFunction(int hash, OpPtr1 func, int prior) {
    op.insert_or_assign(hash, OpDetail(prior, func, false));
    return;
}

template<class Value>
void CalculationSetting<Value>::setFunction(int hash, OpPtr2 func, int prior) {
    op.insert_or_assign(hash, OpDetail(prior, func, true));
    return;
}

template<class Value>
void CalculationSetting<Value>::clearFunctions() {
    op.clear();
}

}
}