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
    Calculator<int> t;
    Calculator<int>::AddFunction('+', &jia, 1);
    Calculator<int>::AddFunction('-', &jian, 1);
    Calculator<int>::AddFunction('*', &cheng, 2);
    Calculator<int>::AddFunction('/', &chu, 2);
    while (1) {
        char c = cin.peek();
        if (c >= '0' && c <= '9') {
            int i;
            cin >> i;
            //            cerr << i << " i\n";
            t.Insert(i);
        }
        else {
            cin >> c;
            if (c == '=')
                break;
            //            cerr << c << " c\n";
            t.Insert(c);
        }
        while (cin.peek() == ' ')
            cin.ignore();
    }
    cout << t.Calculate();
    getchar();
    getchar();
    return 0;
}
