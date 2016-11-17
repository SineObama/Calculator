#pragma once

#include <stack>
#include <stdexcept>
#include "BasicCalculation.h"
#include "CalculationException.h"
#include "CalculationSetting.h"

namespace sine {
namespace calculator {

/**
* 用二叉树实现的计算。在插入时不会进行计算。不接受括号。
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
        Node *insert(Node *node);
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
    Operator op = getSetting().get(hash);
    Node *node = new Node();
    if (op.binary) {
        Node *cur = end->F, *last = end;
        while (cur != NULL
               && getSetting().get(cur->hash).prior > op.prior) {
            last = cur;
            cur = cur->F;
        }
        if (cur == NULL) {
            node->add(root);
            root = end = node;
        }
        else
            end = cur->insert(node);
    }
    else {
        end = end->add(node);
    }
    node->hash = hash;
}

template<class T>
T BasicCalculationTree<T>::calculate() {
    BasicCalculation<T>::calculateCheck();
    return *(new T(*root->getResult(getSetting())));
}

template<class T>
void BasicCalculationTree<T>::clear() {
    BasicCalculation<T>::clearCheck();
    Node::clear(root);
    end = NULL;
}

template<class T>
BasicCalculationTree<T>::Node::Node() {
    F = LC = RC = NULL;
    v = NULL;
    hash = 0;
}

template<class T>
BasicCalculationTree<T>::Node::~Node() {
    delete v;
}

template<class T>
typename BasicCalculationTree<T>::Node *
BasicCalculationTree<T>::Node::add(Node *node) {
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
typename BasicCalculationTree<T>::Node *
BasicCalculationTree<T>::Node::insert(Node *node) {
    if (LC == NULL)
        throw;
    F->RC = node;
    F = node;
    node->LC = this;
    //node->F = this;
    //if (RC == NULL) {
    //    node->LC = LC;
    //    LC = node;
    //}
    //else {
    //    node->LC = RC;
    //    RC = node;
    //}
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
        OpPtr2 func = static_cast<OpPtr2>(op.func);
        v = new T((*func)(*a, *b));
        clear(RC);
    }
    else {
        OpPtr1 func = static_cast<OpPtr1>(op.func);
        v = new T((*func)(*a));
    }
    clear(LC);
    return v;
}

template<class T>
void BasicCalculationTree<T>::Node::clear(Node *&root) {
    if (root != NULL) {
        clear(root->LC);
        clear(root->RC);
        delete root;
        root = NULL;
    }
}

}
}
