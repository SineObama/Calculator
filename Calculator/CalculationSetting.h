#pragma once

#include "CalculationException.h"
#include "Functions.h"

namespace sine {
namespace calculator {

/**
 * ��ʾϣ���Ĳ����������ͣ�����������ֵ�������������˫Ŀ��
 * state transition:
 * BinaryOperator:
 *	                    * value->throw MissingOperator();
 *	                    * unary->ValueOrUnaryOperator;
 *	                    * binary->ValueOrUnaryOperator;
 * ValueOrUnaryOperator:
 *	                    * value->BinaryOperator;
 *	                    * unary->ValueOrUnaryOperator;
 *	                    * binary->throw MissingValue();
 */
enum InsertType {
    BinaryOperator,
    ValueOrUnaryOperator
};

/**
 * ������������ݡ��ɱ���Ϊ����������˫��������
 * ������һ�����ַ�������Ϊ��ϣֵ��ʶ������Ϊ���͹�ϣֵ��
 * �����������ϣֵ�����ȼ��������ȣ���ִ�к���ָ�룬�Ƿ�Ϊ˫Ŀ��
 */
struct Operator {
    Operator(int h, int p, void *f, bool b) {
        hash = h, prior = p, func = f, binary = b;
    }
    int hash, prior;
    void *func;
    bool binary;
};

/**
 * �������������
 */
template<class Value>
class CalculationSetting {

public:

    static CalculationSetting getDefault();

    typedef Value(*OpPtr1)(const Value &);
    typedef Value(*OpPtr2)(const Value &, const Value &);

    bool isValid(int hash) const;
    Operator get(int hash) const;
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
bool CalculationSetting<Value>::isValid(int hash) const {
    return op.find(hash) != op.end();
}

template<class Value>
typename Operator CalculationSetting<Value>::get(int hash) const {
    std::map<int, Operator>::const_iterator it = op.find(hash);
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
