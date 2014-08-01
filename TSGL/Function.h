/*
 * Function.h provides functions for drawing with CartesianCanvas
 *
 * Created on: Jun 11, 2014
 * Author: Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/2/2014
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <cmath>

typedef long double Decimal;  // Define the variable type to use

class Function {
 public:
    Function() {}
    virtual ~Function() {}
    virtual Decimal valueAt(Decimal x) const = 0;
};

class PowerFunction : public Function {
 public:
    PowerFunction(Decimal a) {
        a_ = a;
    }
    virtual Decimal valueAt(Decimal x) const {
        return pow(x, a_);
    }
 private:
    Decimal a_;
};

class SquareRootFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return sqrt(x);
    }
};

class SineFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return sin(x);
    }
};

class CosineFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return cos(x);
    }
};

class TangentFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return tan(x);
    }
};

class AbsoluteFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return std::abs(x);
    }
};

class ExponentialFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return exp(x);
    }
};

class NaturalLogFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return log(x);
    }
};

class CommonLogFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return log10(x);
    }
};

class CeilingFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return ceil(x);
    }
};

class FloorFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return floor(x);
    }
};

class RoundFunction : public Function {
 public:
    virtual Decimal valueAt(Decimal x) const {
        return round(x);
    }
};

#endif /* FUNCTION_H_ */