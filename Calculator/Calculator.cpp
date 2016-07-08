// Calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <cstdio>
#include <sstream>
#include "CalculationStack.h"
#include "Functions.h"

using namespace std;
using namespace Sine::Calculator;

typedef double Type;

int main() {
    cout << Sqrt1(1000000000000000000.0, 3, 1000000000000.0);
    getchar();
    getchar();
    return 0;
    CalculationStack<Type> t(CalculationSetting<Type>::default());

    while (1) {
        while (1) {
            char c = cin.peek();
            if ((c >= '0' && c <= '9') || c == '.') {
                Type i;
                cin >> i;
                t.insert(i);
            }
            else {
                cin >> c;
                if (c == '=')
                    break;
                t.insert(c);
            }
            while (cin.peek() == ' ' || cin.peek() == '\n')
                cin.ignore();
        }
        cout << t.calculate() << endl;
        while (cin.peek() == ' ' || cin.peek() == '\n')
            cin.ignore();
    }

    return 0;
}
