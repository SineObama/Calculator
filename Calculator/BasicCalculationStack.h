#pragma once

#include <stack>
#include <map>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <iostream>
#include "CalculationException.h"
#include "CalculationSetting.h"

namespace sine {
namespace calculator {

/**
 * 用栈实现的计算。在插入时可能执行一定的计算。
 *
 */
template<class Value>
class BasicCalculationStack {

public:

    BasicCalculationStack(const CalculationSetting<Value> &);
    ~BasicCalculationStack();

    InsertType nextInsertType();
    void insertValue(const Value &);
    void insertOp(int hash);

    Value calculate();
    void clear();

private:

    void calculateOne();

    InsertType nextType;
    std::stack<Value *> ValueStack;
    std::stack<int> OpStack;

    CalculationSetting<Value> _setting;

    typedef typename CalculationSetting<Value>::OpPtr1 Op1;
    typedef typename CalculationSetting<Value>::OpPtr2 Op2;

};

template<class Value>
BasicCalculationStack<Value>::BasicCalculationStack(
    const CalculationSetting<Value> &setting)
    : _setting(setting) {
    nextType = ValueOrUnaryOperator;
}

template<class Value>
BasicCalculationStack<Value>::~BasicCalculationStack() {
    clear();
}

template<class Value>
InsertType BasicCalculationStack<Value>::nextInsertType() {
    return nextType;
}

template<class Value>
void BasicCalculationStack<Value>::insertValue(const Value &v) {
    if (nextType != ValueOrUnaryOperator)
        throw MissingOperator("continual values(or unary operators");
    nextType = BinaryOperator;
    ValueStack.push(new Value(v));
}

template<class Value>
void BasicCalculationStack<Value>::insertOp(int hash) {
    if (_setting.get(hash).binary) {
        if (nextType != BinaryOperator)
            throw MissingValue("continual operators");
        nextType = ValueOrUnaryOperator;
    }
    else {
        if (nextType != ValueOrUnaryOperator)
            throw MissingOperator();
    }
    while (!OpStack.empty() &&
           _setting.get(OpStack.top()).prior >= _setting.get(hash).prior)
        calculateOne();
    OpStack.push(hash);
}

template<class Value>
Value BasicCalculationStack<Value>::calculate() {
    if (ValueStack.empty())
        throw EmptyContent("nothing to be calculated.");
    if (nextType != BinaryOperator)
        throw MissingValue("at the end");
    while (ValueStack.size() > 1)
        calculateOne();
    return Value(*ValueStack.top());
}

template<class Value>
void BasicCalculationStack<Value>::clear() {
    while (!ValueStack.empty()) {
        delete ValueStack.top();
        ValueStack.pop();
    }
    while (!OpStack.empty())
        OpStack.pop();
    nextType = ValueOrUnaryOperator;
}

template<class Value>
void BasicCalculationStack<Value>::calculateOne() {
    Value *a = ValueStack.top();
    ValueStack.pop();
    int hash = OpStack.top();
    OpStack.pop();
    Operator detail = _setting.get(hash);
    if (detail.binary) {
        Value *b = ValueStack.top();
        ValueStack.pop();
        Op2 func = static_cast<Op2>(detail.func);
        ValueStack.push(new Value((*func)(*b, *a)));
        delete b;
    }
    else {
        Op1 func = static_cast<Op1>(detail.func);
        ValueStack.push(new Value((*func)(*a)));
    }
    delete a;
}

}
}
