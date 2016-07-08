#pragma once

#include <iostream>

namespace Sine {
namespace Calculator {

template<class T>
T Sqrt2(T _b, int _n, int t = 50) {
    T src(_b);
    while (t--) {
        T tem(_b);
        for (int j = 0; j < _n - 2; j++)
            tem = tem * _b;
        _b = _b - (tem * _b - src) / (T(_n) * tem);
    }
    return _b;
}

template<class T>
T Sqrt1(T _b, int _n, const T & _m) {
    T src(_b);
    int t = 10000;
    while (t--) {
        T tem(_b);
        for (int j = 0; j < _n - 2; j++)
            tem = tem * _b;
        T tem2 = tem * _b - src;
        _b = _b - tem2 / (T(_n) * tem);
        if (tem2 > -_m && tem2 < _m) {
            std::cout << tem2 << "\n";
            return _b;
        }
    }
    throw;
    return _b;
}

}
}
