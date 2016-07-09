#pragma once

#include <stack>
#include <map>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "BasicCalculation.h"
#include "CalculationException.h"
#include "CalculationSetting.h"

namespace sine {
namespace calculator {

/**
 * 用栈实现的计算。在插入时可能执行一定的计算。不接受括号。
 */
template<class Value>
class BasicCalculationStack : public BasicCalculation<Value> {

public:

    BasicCalculationStack(const CalculationSetting<Value> &);
    virtual ~BasicCalculationStack();

    virtual void insertValue(const Value &);
    virtual void insertOp(int hash);

    virtual Value calculate();
    virtual void clear();

private:

    void calculateOne();

    std::stack<Value *> valueStack;
    std::stack<int> opStack;

};

template<class Value>
BasicCalculationStack<Value>::BasicCalculationStack(
    const CalculationSetting<Value> &setting)
    : BasicCalculation<Value>(setting) {
    
}

template<class Value>
BasicCalculationStack<Value>::~BasicCalculationStack() {
    clear();
}

template<class Value>
void BasicCalculationStack<Value>::insertValue(const Value &v) {
    BasicCalculation<Value>::insertValueCheck();
    valueStack.push(new Value(v));
}

template<class Value>
void BasicCalculationStack<Value>::insertOp(int hash) {
    BasicCalculation<Value>::insertOpCheck(hash);
    while (!opStack.empty() &&
           getSetting().get(opStack.top()).prior
           >= getSetting().get(hash).prior)
        calculateOne();
    opStack.push(hash);
}

template<class Value>
Value BasicCalculationStack<Value>::calculate() {
    BasicCalculation<Value>::calculateCheck();
    if (valueStack.empty())
        throw EmptyContent("nothing to be calculated.");
    while (valueStack.size() > 1)
        calculateOne();
    return Value(*valueStack.top());
}

template<class Value>
void BasicCalculationStack<Value>::clear() {
    BasicCalculation<Value>::clearCheck();
    while (!valueStack.empty()) {
        delete valueStack.top();
        valueStack.pop();
    }
    while (!opStack.empty())
        opStack.pop();
}

template<class Value>
void BasicCalculationStack<Value>::calculateOne() {
    Value *a = valueStack.top();
    valueStack.pop();
    int hash = opStack.top();
    opStack.pop();
    Operator detail = getSetting().get(hash);
    if (detail.binary) {
        Value *b = valueStack.top();
        valueStack.pop();
        OpPtr2 func = static_cast<OpPtr2>(detail.func);
        valueStack.push(new Value((*func)(*b, *a)));
        delete b;
    }
    else {
        OpPtr1 func = static_cast<OpPtr1>(detail.func);
        valueStack.push(new Value((*func)(*a)));
    }
    delete a;
}

}
}
