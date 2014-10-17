/*
 * logreal.h
 *
 * Copyright (c) 2011-2014, Brendan Hickey
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __LOG_REAL_MODULE__
#define __LOG_REAL_MODULE__

#include <cmath>
#include <iostream>

using namespace std;

template <class T>
class LogReal {

  public:
    LogReal<T>() {
      value_ = 0;
      sign_ = ZERO;
    }

    LogReal<T>(T v) {
      if (v) {
        value_ = log(std::fabs(v));
        sign_ = v > 0 ? POSITIVE : NEGATIVE;
      } else {
        value_ = 0;
        sign_ = ZERO;
      }
    }

    template <class S>
    explicit LogReal<T>(LogReal<S> v) {
        value_ = (T) v.getValue();
        sign_  = v.getSign();
    }

    T base() const {
      return (sign_ * exp(value_));
    }

    LogReal<T> operator*(const LogReal<T> &rhs) const {
      LogReal<T> ld = *this;
      ld.sign_  = ld.sign_ * rhs.getSign();
      ld.value_ = ld.value_ + rhs.getValue();
      return ld;
    }

    LogReal<T>& operator*=(const LogReal<T> &rhs) {
      sign_  = sign_ * rhs.getSign();
      value_ = value_ + rhs.getValue();
      return *this;
    }

    LogReal<T> operator/(const LogReal<T> &rhs) const {
      if (!rhs.getSign()) {
        throw "Divide by zero exception";
      }
      LogReal<T> ld = *this;
      ld.sign_ = ld.sign_ * rhs.getSign();
      ld.value_ = ld.value_ - rhs.getValue();
      return ld;
    }

    LogReal<T>& operator/=(const LogReal<T> &rhs) {
      if (!rhs.getSign()) {
        throw "Divide by zero exception";
      }
      sign_ = sign_ * rhs.getSign();
      value_ = value_ - rhs.getValue();
      return *this;
    }

    LogReal<T> operator+(const LogReal<T> &rhs) const {
      if (rhs.value_ > value_) {
        return rhs + *this;
      }
      LogReal<T> result;
      result.sign_ = sign_;
      if (sign_ == rhs.sign_) {
        result.value_ = value_ + log(1 + exp(rhs.value_ - value_));
      } else {
        result.value_ = value_ + log(1 - exp(rhs.value_ - value_));
      }
      return result;
    }

    LogReal<T>& operator+=(const LogReal<T> &rhs) {
      T a;
      T b;
      bool same_sign = sign_ == rhs.sign_;
      if (rhs.value_ > value_) {
        a = rhs.value_;
        b = value_;
        sign_ = rhs.sign_;
      } else {
        a = value_;
        b = rhs.value_;
        sign_ = sign_;
      }
      if (sign_ == rhs.sign_) {
        value_ = a + log(1 + exp(b - a));
      } else {
        value_ = a + log(1 - exp(b - a));
      }
      return *this;
    }

    LogReal<T> operator-(const LogReal<T> &rhs) const
    {
      return *this + -rhs;
    }

    LogReal<T> operator-=(const LogReal<T> &rhs) {
      return *this += -rhs;
    }

    LogReal<T> operator-() const
    {
      LogReal<T> ld = *this;
      ld.sign_ *= -1;
      return ld;
    }

    LogReal<T>& operator-()
    {
      sign_ *= -1;
      return *this;
    }

    bool operator<(const LogReal<T> &rhs) const {
      if (sign_ != rhs.getSign()) {
        return sign_ < rhs.getSign();
      }
      return value_ < rhs.getValue();
    }

    bool operator<(const T &rhs) const {
      return (*this) < ((LogReal<T>) rhs);
    }

    bool operator==(const LogReal<T> &rhs) const {
      return sign_ == rhs.sign_ && value_ == rhs.value_;
    }

    bool operator==(const T &rhs) const {
      if (rhs * sign_ < 0) {
        return false;
      }
      return *this == (LogReal<T>) rhs;
    }

    template <class S>
    bool operator!=(const LogReal<S> &rhs) const {
      return sign_ != rhs.sign_
        || value_ != rhs.value_;
    }

    bool operator!=(const T &rhs) const {
      if (rhs * sign_ < 0) {
        return true;
      }
      return *this != (LogReal<T>) rhs;
    }

    friend std::ostream& operator<<(std::ostream& out, const LogReal<T>& d) {
      if (!d.sign_) {
        out << 0;
        return out;
      }
      out << (NEGATIVE == d.sign_ ? "-" : "") << "e^" << d.value_;
      return out;
    }

    static LogReal<T> sum(LogReal<T> *reals, int sz) {
      if (!sz) {
        return 0;
      }
      LogReal<T> s = reals[0];
      for (int ii = 1; ii < sz; ++ii) {
        s += reals[ii];
      }
      return s;
    }

    static LogReal<T> product(LogReal<T> *reals, int sz) {
      if (!sz) {
        return 0;
      }
      LogReal<T> p = reals[0];
      for (int ii = 1; ii < sz; ++ii) {
        p.value_ += reals[ii].value_;
        p.sign_  *= reals[ii].sign_;
      }
      return p;
    }

    static void normalize(LogReal<T> *reals, int sz) {
      if (!sz) {
        return;
      }
      LogReal<T> s = sum(reals, sz);
      for (int ii = 0; ii < sz; ++ii) {
        reals[ii] /=  s;
      }
    }

    inline T   getValue() const { return value_; }
    inline int getSign()  const { return sign_; }

  private:

    T    value_;
    char sign_;
    enum SIGN { NEGATIVE = -1, ZERO = 0, POSITIVE = 1 };
};

template <class T>
LogReal<T> operator*(const T &lhs, const LogReal<T> &rhs) {
  return rhs * lhs;
}

template <class T>
LogReal<T> operator/(const T &lhs, const LogReal<T> &rhs) {
  return ((LogReal<T>) lhs) / rhs;
}

template <class T>
LogReal<T> operator+(const T &lhs, const LogReal<T> &rhs) {
  return rhs + LogReal<T>(lhs);
}

template <class T>
LogReal<T> operator-(const T &lhs, const LogReal<T> &rhs) {
  return rhs + LogReal<T>(-lhs);
}

template <class T>
bool operator<(const T &lhs, const LogReal<T> &rhs) {
  return LogReal<T>(lhs) < rhs;
}

template <class T>
bool operator==(const T &lhs, const LogReal<T> &rhs) {
  return rhs == lhs;
}

typedef LogReal<double> LogDouble;
typedef LogReal<float>  LogFloat;

#endif
