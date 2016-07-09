#pragma once

#include <stack>
#include <stdexcept>
#include "BasicCalculation.h"
#include "CalculationException.h"
#include "CalculationSetting.h"

namespace sine {
namespace calculator {

/**
* 以树为基础实现的。
*/
template<class T>
class BasicCalculationTree : public BasicCalculation<T> {

public:

    BasicCalculationTree(const CalculationSetting<T> &);
    virtual ~BasicCalculationTree();

    virtual void insertValue(const T &);
    virtual void insertOp(int hash);

    virtual T calculate();
    virtual void clear();

private:

    struct Node {
        Node *F, *LC, *RC;
        T *v;
        int hash;
        Node();
        ~Node();
        Node *add(Node *node);
        T *getResult(const CalculationSetting<T> &setting);
        static void clear(Node *&root);
    };

    Node *root, *end;

};

template<class T>
BasicCalculationTree<T>::BasicCalculationTree(
    const CalculationSetting<T> &setting)
    : BasicCalculation<T>(setting) {
    root = end = NULL;
}

template<class T>
BasicCalculationTree<T>::~BasicCalculationTree() {
    clear();
}

template<class T>
void BasicCalculationTree<T>::insertValue(const T &v) {
    BasicCalculation<T>::insertValueCheck();
    Node *node = new Node();
    if (end == NULL)
        root = end = node;
    else
        end = end->add(node);
    node->v = new T(v);
}

template<class T>
void BasicCalculationTree<T>::insertOp(int hash) {
    BasicCalculation<T>::insertOpCheck(hash);

}

template<class T>
T BasicCalculationTree<T>::calculate() {
    BasicCalculation<Value>::calculateCheck();
    return new T(*root->getResult(getSetting()));
}

template<class T>
void BasicCalculationTree<T>::clear() {
    BasicCalculation<Value>::clearCheck();
    Node::clear(root);
    end = NULL;
}

template<class T>
BasicCalculationTree<T>::Node::Node() {
    F = LC = RC = v = NULL;
    hash = 0;
}

template<class T>
BasicCalculationTree<T>::Node::~Node() {
    delete v;
}

template<class T>
Node *BasicCalculationTree<T>::Node::add(Node *node) {
    if (RC != NULL)
        throw;
    if (LC == NULL)
        LC = node;
    else
        RC = node;
    node->F = this;
    return node;
}

template<class T>
T *BasicCalculationTree<T>::Node::getResult(
    const CalculationSetting<T> &setting) {
    if (v != NULL)
        return v;
    T *a, *b;
    if (LC == NULL)
        throw;
    a = LC->getResult(setting);
    Operator op = setting.get(hash);
    if (op.binary) {
        if (RC == NULL)
            throw;
        b = RC->getResult(setting);
        Op2 func = static_cast<Op2>(op.func);
        v = new T((*func)(*b, *a));
        clear(RC);
    }
    else {
        Op1 func = static_cast<Op1>(op.func);
        v = new T((*func)(*a));
    }
    clear(LC);
}

template<class T>
void BasicCalculationTree<T>::Node::clear(Node *&root) {
    if (root != NULL) {
        clear(LC);
        clear(RC);
        delete root;
        root = NULL;
    }
}

}
}
