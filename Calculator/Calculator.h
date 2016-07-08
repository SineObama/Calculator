#pragma once

// only accept '(', ')'

#include <stack>
#include <stdexcept>
#include "CalculationStack.h"
#include "CalculationException.h"

namespace Sine {
    namespace Calculator {

        template<class T>
        class Calculator {

        public:

            Calculator(const CalculationSetting<T> &);
            ~Calculator();

            void insert(const T &);
            void insert(char);

            T calculate();

            void clear();

        private:

            std::stack<CalculateStack<T>*> stack;

            CalculationSetting<T> set;

        };

        template<class T>
        Calculator<T>::Calculator(const CalculationSetting<T> &setting)
            : set(setting) {
            stack.push(new CalculateStack<T>(set));  // keep the size >= 1
        }

        template<class T>
        Calculator<T>::~Calculator() {
            clear();
        }

        template<class T>
        void Calculator<T>::insert(const T &x) {
            stack.top()->insertValue(x);
            return;
        }

        template<class T>
        void Calculator<T>::insert(char x) {
            CalculateStack<T> *tem = stack.top();
            switch (x) {
            case '(':
                if (tem->nextInsertType() == CalculateStack<T>::BinaryOperator)
                    throw MissingOperator("miss operator before '('.");
                stack.push(new CalculateStack<T>(set));
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
        T Calculator<T>::calculate() {
            if (stack.size() != 1)
                throw BrackerMismatch("miss ')'.");
            T result = stack.top()->calculate();
            stack.top()->clear();
            return result;
        }

        template<class T>
        void Calculator<T>::clear() {
            while (stack.size() != 1) {
                delete stack.top();
                stack.pop();
            }
            stack.top()->clear();
        }

    }
}
