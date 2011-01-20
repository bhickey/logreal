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
            _value = 0;
            _sign = 0;
        }

        LogReal<T>(T v)
        { 
            if(v) {
                _value = log(std::fabs(v)); 
                _sign = v > 0 ? POSITIVE : NEGATIVE;
            } else {
                _value = 0;
                _sign = ZERO;
            }
        }
    
        template <class S>
        explicit LogReal<T>(LogReal<S> v)
        {
            _value = (T) v.getValue();
            _sign  = v.getSign();
        }

//        operator T() const { return (_sign * exp(_value)); }
        T toBase() const { return (_sign * exp(_value)); }
        
        LogReal<T> operator*(const LogReal<T> &rhs) const
        { 
            LogReal<T> ld = *this;
            ld._sign  = ld._sign * rhs.getSign();
            ld._value = ld._value + rhs.getValue();
            return ld;
        }

        LogReal<T> operator/(const LogReal<T> &rhs) const
        { 
            if( !rhs.getSign() ) {
              throw "Divide by zero exception";
            }
            LogReal<T> ld = *this;
            ld._sign  = ld._sign * rhs.getSign();
            ld._value = ld._value - rhs.getValue();
            return ld;
        }

        T operator+(const T &rhs) const
        { return (*this).toBase() + rhs; }

        T operator-(const T &rhs) const
        { return ((T) *this) - rhs; }

        LogReal<T> operator-(){ 
            LogReal<T> ld = *this;
            ld._sign *= -1;
            return ld;
        }

        bool operator<(const LogReal<T> &rhs) const
        { 
            if( _sign != rhs.getSign() ) {
                return _sign < rhs.getSign();
            }
            return _value < rhs.getValue();
        }

        bool operator<(const T &rhs) const
        { return (*this) < ((LogReal<T>) rhs); }

        bool operator==(const LogReal<T> &rhs) const
        { return _sign == rhs._sign && _value == rhs._value; }

        bool operator==(const T &rhs) const
        { 
            if( rhs * _sign < 0 )
                return false;
            return *this == (LogReal<T>) rhs;
        }

        template <class S>
        bool operator!=(const LogReal<S> &rhs) const
        { return _sign != rhs._sign || _value != rhs._value; }

        bool operator!=(const T &rhs) const
        { 
            if( rhs * _sign < 0 )
                return true;
            return *this != (LogReal<T>) rhs;
        }

        friend std::ostream& operator<<(std::ostream& out, const LogReal<T>& d)
        {
            out << (T) d;
            return out;
        }

        friend std::istream& operator>>(std::istream& in, LogReal<T>& ld)
        {
            T v;
            in >> v;
            ld = (LogReal<T>) v;
            return in;
        }

        static T sum(LogReal<T> *reals, int sz)
        {
            T s = 0.0;
            for(int ii = 0; ii < sz; ++ii){ s += reals[ii].toBase(); }
            return s;
        }

        static LogReal<T> product(LogReal<T> *reals, int sz)
        {
            LogReal<T> p = reals[0];
            for(int ii = 1; ii < sz; ++ii){ 
                p._value += reals[ii]._value; 
                p._sign  *= reals[ii]._sign;
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

        T   getValue() const{ return _value; }
        int getSign()  const{ return _sign; }

    private:

        T    _value;
        char _sign;
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
