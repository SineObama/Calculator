#pragma once

#include <stdexcept>
#include <string>

namespace Sine {
namespace Calculator {

class CalculationError : public std::runtime_error {
public:
    CalculationError(const std::string &s)
        : std::runtime_error(s) {
    }
};

class EmptyContent : public CalculationError {
public:
    EmptyContent(const std::string &s)
        : CalculationError("syntax error: " + s) {
    }
};

class SyntaxError : public CalculationError {
public:
    SyntaxError(const std::string &s)
        : CalculationError("syntax error: " + s) {
    }
};

class UnknownCharactor : public SyntaxError {
public:
    UnknownCharactor(const std::string &s)
        : SyntaxError(s) {
    }
};

class MissingOperator : public SyntaxError {
public:
    MissingOperator(const std::string &s)
        : SyntaxError(s) {
    }
};

class MissingValue : public SyntaxError {
public:
    MissingValue(const std::string &s)
        : SyntaxError(s) {
    }
};

class BrackerMismatch : public SyntaxError {
public:
    BrackerMismatch(const std::string &s)
        : SyntaxError(s) {
    }
};

}
}
