// Calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <cstdio>
#include <sstream>
#include "CalculationStack.h"
#include "Functions.h"

using namespace std;
using namespace sine::calculator;

typedef double Type;

int &test(int *a) {
    return *a;
}

int main() {
//     cout << Sqrt1(100000000000000000.0, 5, 10.0);

//     int a = 2;
//     int &b = test(&a);
//     b = 1;
//     cout << a;
//     getchar();
//     getchar();
//     return 0;
    CalculationStack<Type> t(CalculationSetting<Type>::getDefault());

    while (1) {
        try {
            Type sign = 1;
            while (1) {
                while (cin.peek() == ' ' || cin.peek() == '\n')
                    cin.ignore();
                char c = cin.peek();
                if (t.nextInsertType() == InsertType::ValueOrUnaryOperator) {
                    if (c == '-') {
                        cin.ignore();
                        sign *= -1;
                        continue;
                    }
                }
                if ((c >= '0' && c <= '9') || c == '.') {
                    Type i;
                    cin >> i;
                    i *= sign;
                    sign = 1;
                    t.insertValue(i);
                }
                else {
                    cin >> c;
                    if (c == '=')
                        break;
                    t.insertOp(c);
                }
            }
            cout << t.calculate() << endl;
        }
        catch (CalculationError e) {
            t.clear();
            cout << e.what() << endl;
            while (cin.peek() != '\n') {
                cin.ignore();
            }
            cin.ignore();
        }
    }

    return 0;
}
