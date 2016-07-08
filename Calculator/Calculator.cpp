// Calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int jia(const int & a, const int & b) {
    return a + b;
}

int jian(const int & a, const int & b) {
    return a - b;
}

int cheng(const int & a, const int & b) {
    return a * b;
}

int chu(const int & a, const int & b) {
    return a / b;
}

#include <iostream>
#include <cstdio>
#include <sstream>
#include "Calculator.h"
#include "Functions.h"

using namespace std;
using namespace Sine;

int main() {
    //cout << Sqrt1(1000000000000000000.0, 3, 1000000000000.0);
    CalculationSetting<int> setting;
    setting.setFunction('+', &jia, 1);
    setting.setFunction('-', &jian, 1);
    setting.setFunction('*', &cheng, 2);
    setting.setFunction('/', &chu, 2);
    Calculator<int> t(setting);
    while (1) {
        char c = cin.peek();
        if (c >= '0' && c <= '9') {
            int i;
            cin >> i;
            //            cerr << i << " i\n";
            t.insert(i);
        }
        else {
            cin >> c;
            if (c == '=')
                break;
            //            cerr << c << " c\n";
            t.insert(c);
        }
        while (cin.peek() == ' ')
            cin.ignore();
    }
    cout << t.calculate();
    getchar();
    getchar();
    return 0;
}
