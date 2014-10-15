/*
 * logreal.h
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 */
#ifndef __LOG_REAL_MODULE__
#define __LOG_REAL_MODULE__

#include <cmath>
#include <iostream>

template <class T>
class LogReal {
    
    public:
        LogReal<T>()
        {
            value_ = 0;
            sign_ = ZERO;
        }

        LogReal<T>(T v)
        { 
            if(v) {
                value_ = log(std::fabs(v)); 
                sign_ = v > 0 ? POSITIVE : NEGATIVE;
            } else {
                value_ = 0;
                sign_ = ZERO;
            }
        }
    
        template <class S>
        explicit LogReal<T>(LogReal<S> v)
        {
            value_ = (T) v.getValue();
            sign_  = v.getSign();
        }

        T base() const { return (sign_ * exp(value_)); }
        
        LogReal<T> operator*(const LogReal<T> &rhs) const
        { 
            LogReal<T> ld = *this;
            ld.sign_  = ld.sign_ * rhs.getSign();
            ld.value_ = ld.value_ + rhs.getValue();
            return ld;
        }

        LogReal<T>& operator*=(const LogReal<T> &rhs)
        {
            sign_  = sign_ * rhs.getSign();
            value_ = value_ + rhs.getValue();
            return *this;
        }

        LogReal<T> operator/(const LogReal<T> &rhs) const
        { 
            if( !rhs.getSign() ) {
              throw "Divide by zero exception";
            }
            LogReal<T> ld = *this;
            ld.sign_  = ld.sign_ * rhs.getSign();
            ld.value_ = ld.value_ - rhs.getValue();
            return ld;
        }

        LogReal<T>& operator/=(const LogReal<T> &rhs)
        {
          if( !rhs.getSign() ) {
              throw "Divide by zero exception";
          }
          sign_ = sign_ * rhs.getSign();
          value_ = value_ - rhs.getValue();
          return *this;
        }

        LogReal<T> operator+(const LogReal<T> &rhs) const
        {
          if (rhs.value_ > value_)
          {
            return rhs + (*this);
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

        LogReal<T> operator-(const LogReal<T> &rhs) const
        {
          return (*this) + -rhs;
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

        bool operator<(const LogReal<T> &rhs) const
        { 
            if( sign_ != rhs.getSign() ) {
                return sign_ < rhs.getSign();
            }
            return value_ < rhs.getValue();
        }

        bool operator<(const T &rhs) const
        {
          return (*this) < ((LogReal<T>) rhs);
        }

        bool operator==(const LogReal<T> &rhs) const
        {
          return sign_ == rhs.sign_ && value_ == rhs.value_;
        }

        bool operator==(const T &rhs) const
        { 
            if( rhs * sign_ < 0 )
            {
                return false;
            }
            return *this == (LogReal<T>) rhs;
        }

        template <class S>
        bool operator!=(const LogReal<S> &rhs) const
        {
          return sign_ != rhs.sign_ || value_ != rhs.value_;
        }

        bool operator!=(const T &rhs) const
        { 
            if( rhs * sign_ < 0 )
                return true;
            return *this != (LogReal<T>) rhs;
        }

        friend std::ostream& operator<<(std::ostream& out, const LogReal<T>& d)
        {
            if( !d.sign_ ) { 
                out << 0; 
                return out;
            }
            out << (NEGATIVE == d.sign_ ? "-" : "") << "e^" << d.value_;
            return out;
        }

        static T sum(LogReal<T> *reals, int sz)
        {
            T s = 0.0;
            for(int ii = 0; ii < sz; ++ii){ s += reals[ii].base(); }
            return s;
        }

        static LogReal<T> product(LogReal<T> *reals, int sz)
        {
            LogReal<T> p = reals[0];
            for(int ii = 1; ii < sz; ++ii){ 
                p.value_ += reals[ii].value_; 
                p.sign_  *= reals[ii].sign_;
            }
            return p;
        }

        static void normalize(LogReal<T> *reals, int sz)
        {
            T s = sum(reals, sz);
            for(int ii = 0; ii < sz; ++ii){
                reals[ii] = reals[ii] / s;
            }
        }

        inline T   getValue() const{ return value_; }
        inline int getSign()  const{ return sign_; }

    private:

        T    value_;
        char sign_;
        enum SIGN { NEGATIVE = -1, ZERO = 0, POSITIVE = 1 };
};

template <class T>
LogReal<T> operator*(const T &lhs, const LogReal<T> &rhs)
{ return rhs * lhs; }

template <class T>
LogReal<T> operator/(const T &lhs, const LogReal<T> &rhs)
{ return ((LogReal<T>) lhs) / rhs; }

template <class T>
LogReal<T> operator+(const T &lhs, const LogReal<T> &rhs)
{ return rhs + lhs; }

template <class T>
LogReal<T> operator-(const T &lhs, const LogReal<T> &rhs)
{ return -rhs + lhs; }

template <class T>
bool operator<(const T &lhs, const LogReal<T> &rhs)
{ return LogReal<T>(lhs) < rhs; }

template <class T>
bool operator==(const T &lhs, const LogReal<T> &rhs)
{ return rhs == lhs; }

typedef LogReal<double> LogDouble;
typedef LogReal<float>  LogFloat;
typedef LogReal<double> Probability;

#endif
