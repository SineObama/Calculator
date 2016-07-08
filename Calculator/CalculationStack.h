#pragma once

#include <stack>
#include <map>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <iostream>
#include "Calculator.h"
#include "CalculationException.h"
#include "CalculationSetting.h"

namespace Sine {
    namespace Calculator {

        template<class Value>
        class Calculator;

        template<class Value>
        class CalculateStack {

            friend class Calculator<Value>;

        public:

            enum InsertType {
                BinaryOperator,
                ValueOrUnaryOperator
            };

            CalculateStack(const CalculationSetting<Value> &);
            ~CalculateStack();

            InsertType nextInsertType();
            void insertOp(int hash);
            void insertValue(const Value &);

            Value calculate();
            void clear();

        private:

            void calculateOne();

            InsertType nextType;
            std::stack<Value *> ValueStack;
            std::stack<int> OpStack;

            CalculationSetting<Value> set;

        };

        template<class Value>
        CalculateStack<Value>::CalculateStack(const CalculationSetting<Value> &setting)
            : set(setting) {
            nextType = ValueOrUnaryOperator;
        }

        template<class Value>
        CalculateStack<Value>::~CalculateStack() {
            clear();
        }

        template<class Value>
        typename CalculateStack<Value>::InsertType CalculateStack<Value>::nextInsertType() {
            return nextType;
        }

        template<class Value>
        void CalculateStack<Value>::insertOp(int hash) {
            CalculationSetting<Value>::OpDetail detail = set.get(hash);
            if (detail.binary) {
                if (nextType == ValueOrUnaryOperator)
                    throw MissingValue("continual binary operators");
                nextType = ValueOrUnaryOperator;
            }
            else {
                if (nextType == BinaryOperator)
                    throw MissingOperator("continual values(or unary operators");
            }
            while (!OpStack.empty() && set.get(OpStack.top()).prior >= set.get(hash).prior)
                calculateOne();
            OpStack.push(hash);
        }

        template<class Value>
        void CalculateStack<Value>::insertValue(const Value &x) {
            if (nextType == BinaryOperator)
                throw MissingOperator("continual values(or unary operators");
            nextType = BinaryOperator;
            ValueStack.push(new Value(x));
        }

        template<class Value>
        Value CalculateStack<Value>::calculate() {
            if (ValueStack.empty())
                throw EmptyContent("nothing to be calculated.");
            while (ValueStack.size() > 1)
                calculateOne();
            return Value(*ValueStack.top());
        }

        template<class Value>
        void CalculateStack<Value>::clear() {
            while (!ValueStack.empty()) {
                delete ValueStack.top();
                ValueStack.pop();
            }
            while (!OpStack.empty())
                OpStack.pop();
            nextType = ValueOrUnaryOperator;
        }

        template<class Value>
        void CalculateStack<Value>::calculateOne() {
            Value *a = ValueStack.top();
            ValueStack.pop();
            int hash = OpStack.top();
            OpStack.pop();
            CalculationSetting<Value>::OpDetail detail = set.get(hash);
            if (detail.binary) {
                Value *b = ValueStack.top();
                ValueStack.pop();
                ValueStack.push(new Value(((CalculationSetting<Value>::OpPtr2)detail.func)(*b, *a)));
                delete b;
            }
            else {
                ValueStack.push(new Value((*(CalculationSetting<Value>::OpPtr1)detail.func)(*a)));
            }
            delete a;
        }

    }
}
