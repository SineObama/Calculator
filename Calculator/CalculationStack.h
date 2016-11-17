#pragma once

#include <stack>
#include <stdexcept>
#include "BasicCalculationStack.h"
#include "BasicCalculationTree.h"
#include "BasicCalculation.h"
#include "CalculationException.h"

namespace sine {
namespace calculator {

/**
 * 接受带小括号()的运算。
 * 以栈为基础实现的。
 */
template<class T>
class CalculationStack : public BasicCalculation<T> {

public:

    CalculationStack(const CalculationSetting<T> &);
    virtual ~CalculationStack();

    virtual void insertValue(const T &);
    virtual void insertOp(int);

    virtual T calculate();
    virtual void clear();

private:

    typedef BasicCalculationStack<T> Basic;
    std::stack<BasicCalculation<T>*> stack;

    const CalculationSetting<T> &_setting;

};

template<class T>
CalculationStack<T>::CalculationStack(
    const CalculationSetting<T> &setting)
    : BasicCalculation<T>(setting), _setting(getSetting()) {
    // keep the size >= 1
    stack.push(new Basic(_setting));
}

template<class T>
CalculationStack<T>::~CalculationStack() {
    clear();
}

template<class T>
void CalculationStack<T>::insertValue(const T &v) {
    stack.top()->insertValue(v);
}

template<class T>
void CalculationStack<T>::insertOp(int hash) {
    BasicCalculation *bc = stack.top();
    switch (hash) {
    case '(':
        if (bc->nextInsertType() == BinaryOperator)
            throw MissingOperator("miss operator before '('.");
        stack.push(new Basic(_setting));
        break;
    case ')':
        if (stack.size() <= 1)
            throw BrackerMismatch("more ')' than '('.");
        stack.pop();
        stack.top()->insertValue(bc->calculate());
        delete bc;
        break;
    default:
        bc->insertOp(hash);
    }
}

template<class T>
T CalculationStack<T>::calculate() {
    if (stack.size() != 1)
        throw BrackerMismatch("miss ')'.");
    T result = stack.top()->calculate();
    stack.top()->clear();
    return result;
}

template<class T>
void CalculationStack<T>::clear() {
    while (stack.size() != 1) {
        delete stack.top();
        stack.pop();
    }
    stack.top()->clear();
}

}
}
