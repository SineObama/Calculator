#pragma once

#include <stack>
#include <stdexcept>
#include "BasicCalculationStack.h"
#include "CalculationException.h"

namespace sine {
namespace calculator {

/**
 * 接受带小括号()的运算。
 * 以栈为基础实现的。
 */
template<class T>
class CalculationStack {

public:

    CalculationStack(const CalculationSetting<T> &);
    ~CalculationStack();

    bool isOpValid(char);
    void insertValue(const T &);
    void insertOp(char);

    T calculate();

    void clear();

private:

    std::stack<BasicCalculationStack<T>*> stack;

    CalculationSetting<T> _setting;

};

template<class T>
CalculationStack<T>::CalculationStack(
    const CalculationSetting<T> &setting)
    : _setting(setting) {
    // keep the size >= 1
    stack.push(new BasicCalculationStack<T>(_setting));
}

template<class T>
CalculationStack<T>::~CalculationStack() {
    clear();
}

template<class T>
bool CalculationStack<T>::isOpValid(char op) {
    return _setting.isValid(op);
}

template<class T>
void CalculationStack<T>::insertValue(const T &v) {
    stack.top()->insertValue(v);
}

template<class T>
void CalculationStack<T>::insertOp(char x) {
    BasicCalculationStack<T> *tem = stack.top();
    switch (x) {
    case '(':
        if (tem->nextInsertType() == BinaryOperator)
            throw MissingOperator("miss operator before '('.");
        stack.push(new BasicCalculationStack<T>(_setting));
        break;
    case ')':
        if (stack.size() <= 1)
            throw BrackerMismatch("more ')' than '('.");
        stack.pop();
        stack.top()->insertValue(tem->calculate());
        delete tem;
        break;
    default:
        tem->insertOp(x);
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
