#pragma once

#include "CalculationSetting.h"

namespace sine {
namespace calculator {

/**
 * “插入型”计算的接口。接受单双目操作符。
 * 4个主要接口：插入数值，插入运算符，计算结果，清空。
 * 构造时设置运算符数据。
 */
template<class Value>
class BasicCalculation {

public:

    BasicCalculation(const CalculationSetting<Value> &);
    virtual ~BasicCalculation();

    const CalculationSetting<Value> &getSetting();
    bool isOpValid(int hash);
    
    InsertType nextInsertType();

    virtual void insertValue(const Value &) = 0;
    virtual void insertOp(int hash) = 0;

    virtual Value calculate() = 0;
    virtual void clear() = 0;

protected:

    void insertValueCheck();
    void insertOpCheck(int hash);

    void calculateCheck();
    void clearCheck();

    typedef typename CalculationSetting<Value>::OpPtr1 Op1;
    typedef typename CalculationSetting<Value>::OpPtr2 Op2;

private:

    CalculationSetting<Value> _setting;

    InsertType _nextType;

};

template<class Value>
BasicCalculation<Value>::BasicCalculation(
    const CalculationSetting<Value> &setting)
    : _setting(setting) {
    _nextType = ValueOrUnaryOperator;
}

template<class Value>
BasicCalculation<Value>::~BasicCalculation() {}

template<class Value>
const CalculationSetting<Value> &BasicCalculation<Value>::getSetting() {
    return _setting;
}

template<class T>
bool BasicCalculation<T>::isOpValid(int hash) {
    return _setting.isValid(hash);
}

template<class Value>
InsertType BasicCalculation<Value>::nextInsertType() {
    return _nextType;
}

template<class Value>
void BasicCalculation<Value>::insertValueCheck() {
    if (_nextType != ValueOrUnaryOperator)
        throw MissingOperator("continual values(or unary operators");
    _nextType = BinaryOperator;
}

template<class Value>
void BasicCalculation<Value>::insertOpCheck(int hash) {
    if (_setting.get(hash).binary) {
        if (_nextType != BinaryOperator)
            throw MissingValue("continual operators");
        _nextType = ValueOrUnaryOperator;
    }
    else {
        if (_nextType != ValueOrUnaryOperator)
            throw MissingOperator();
    }
}

template<class Value>
void BasicCalculation<Value>::calculateCheck() {
    if (_nextType != BinaryOperator)
        throw MissingValue("at the end");
}

template<class Value>
void BasicCalculation<Value>::clearCheck() {
    _nextType = ValueOrUnaryOperator;
}

}
}
