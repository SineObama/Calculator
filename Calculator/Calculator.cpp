// Calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <cstdio>
#include <sstream>
#include "Calculator.h"
#include "Functions.h"

using namespace std;
using namespace Sine::Calculator;

typedef double Type;

Type jia(const Type & a, const Type & b) {
    return a + b;
}

Type jian(const Type & a, const Type & b) {
    return a - b;
}

Type cheng(const Type & a, const Type & b) {
    return a * b;
}

Type chu(const Type & a, const Type & b) {
    return a / b;
}

Type mo(const Type & a, const Type & b) {
    return a;
}

int main() {
    //cout << Sqrt1(1000000000000000000.0, 3, 1000000000000.0);
    cout << (-5 % -3);
    getchar();
    getchar();
    return 0;
    CalculationSetting<Type> setting;
    setting.setFunction('+', &jia, 1);
    setting.setFunction('-', &jian, 1);
    setting.setFunction('*', &cheng, 2);
    setting.setFunction('/', &chu, 2);
    setting.setFunction('%', &mo, 2);
    Calculator<Type> t(setting);

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
