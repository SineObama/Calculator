#pragma once

#include <iostream>

namespace sine {
namespace calculator {

template<class T>
T plus(const T &a, const T &b) {
    return a + b;
}

template<class T>
T subtract(const T &a, const T &b) {
    return a - b;
}

template<class T>
T multiply(const T &a, const T &b) {
    return a * b;
}

template<class T>
T divide(const T &a, const T &b) {
    return a / b;
}

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
T Sqrt1(T _b, int _n, const T & _m, int max = 1000) {
    T src(_b);
    while (max--) {
        T tem(_b);
        for (int j = 0; j < _n - 2; j++)
            tem = tem * _b;
        T tem2 = tem * _b - src;
        _b = _b - tem2 / (T(_n) * tem);
        if (tem2 > -_m && tem2 < _m)
            return _b;
    }
    return _b;
}

}
}
