#pragma once

#include <stdexcept>
#include <string>

namespace Sine {

    class CalculatorError : public std::runtime_error {
    public:
        CalculatorError(const std::string &s, const std::string &f = "")
            : std::runtime_error(s),
            from(f) {
        }
        virtual const char * function() {
            return from.c_str();
        }
        virtual ~CalculatorError() throw() {}
    protected:
        std::string from;
    };

    class SyntaxError : public CalculatorError {
    public:
        SyntaxError(const std::string &s, const std::string &f = "")
            : CalculatorError("syntax error: " + s, f) {
        }
        virtual ~SyntaxError() throw() {}
    };

}
