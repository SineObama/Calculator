#pragma once

#include <stack>
#include <stdexcept>
#include "CalculationException.h"
#include "CalculationSetting.h"

namespace sine {
namespace calculator {

/**
* 以树为基础实现的。
*/
template<class T>
class BasicCalculationTree {

public:

    BasicCalculationTree(const CalculationSetting<T> &);
    ~BasicCalculationTree();

    //bool isOpValid(char);
    InsertType nextInsertType();
    void insertValue(const T &);
    void insertOp(int hash);

    T calculate();

    void clear();

private:

    struct CalculationTreeNode {
        CalculationTreeNode *F, *LC, *RC;
        T *v;
        int hash;
        CalculationTreeNode(CalculationTreeNode *f = NULL)
            :F(f) {
            LC = RC = v = NULL;
            hash = 0;
        }
        CalculationTreeNode *add(CalculationTreeNode *node) {
            if (RC != NULL)
                throw;
            if (LC == NULL) {
                LC = node;
            }
            else {
                RC = node;
            }
            node->F = this;
            return node;
        }
    };

    typedef CalculationTreeNode Node;

    InsertType nextType;

    CalculationTreeNode *root, *end;

    CalculationSetting<T> _setting;

    typedef typename CalculationSetting<Value>::OpPtr1 Op1;
    typedef typename CalculationSetting<Value>::OpPtr2 Op2;

};

template<class T>
BasicCalculationTree<T>::BasicCalculationTree(
    const CalculationSetting<T> &setting)
    : _setting(setting) {
    nextType = ValueOrUnaryOperator;
    root = end = NULL;
}

template<class T>
BasicCalculationTree<T>::~BasicCalculationTree() {
    clear();
}

//template<class T>
//bool BasicCalculationTree<T>::isOpValid(char op) {
//	return _setting.isValid(op);
//}

template<class Value>
InsertType BasicCalculationTree<T>::nextInsertType() {
    return nextType;
}

template<class T>
void BasicCalculationTree<T>::insertValue(const T &v) {
    if (nextType == BinaryOperator)
        throw MissingOperator("continual values(or unary operators");
    nextType = BinaryOperator;
    Node *node = new Node();
    if (end == NULL) {
        root = end = node;
    }
    else {
        end->add(node);
    }

    node->v = new T(v);
}

template<class T>
void BasicCalculationTree<T>::insertOp(int x) {
    BasicCalculationTree<T> *tem = stack.top();
    switch (x) {
    case '(':
        if (tem->nextInsertType()
            == BasicCalculationTree<T>::BinaryOperator)
            throw MissingOperator("miss operator before '('.");
        stack.push(new BasicCalculationTree<T>(_setting));
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
T BasicCalculationTree<T>::calculate() {
    if (stack.size() != 1)
        throw BrackerMismatch("miss ')'.");
    T result = stack.top()->calculate();
    stack.top()->clear();
    return result;
}

template<class T>
void BasicCalculationTree<T>::clear() {
    while (stack.size() != 1) {
        delete stack.top();
        stack.pop();
    }
    stack.top()->clear();
}

}
}
