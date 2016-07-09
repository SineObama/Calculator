#pragma once

#include "CalculationException.h"
#include "Functions.h"

namespace sine {
namespace calculator {

/**
 * 表示希望的插入数据类型，操作数（数值）或操作符（单双目）
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
 * 保存操作符数据。可保存为单操作数和双操作数。
 * 操作符一般用字符本身作为哈希值标识。保存为整型哈希值。
 * 具体包括：哈希值，优先级（大优先），执行函数指针，是否为双目。
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
 * 保存操作符数据
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
