#include <stdlib.h>
#include <iostream>
#include "logreal.h"

using namespace std;

#define EPSILON 0.0000000001

bool withinEpsilon(double p, double d){
    return abs(p - d) < EPSILON;
}

union intFloat {
    int i;
    float f;
};

float rfloat(){
    intFloat v;
    v.i = (0x3F800000 | (0x7FFFFF & rand()));
    return (v.f - 1.0f);
}

bool test_normalize() 
{
    int LENGTH = 100;  
    LogReal<double> probs[LENGTH];
    
    for(int ii = 0; ii < LENGTH; ++ii){
        probs[ii] = rfloat();
      for(int jj = 0; jj < 725; ++jj){
        probs[ii] = probs[ii] * rfloat();
      }
    }

    LogReal<double>::normalize(probs, LENGTH);
   

    double sum = 0.0;
    for(int ii = 0; ii < LENGTH; ii++){
        sum += probs[ii];
    }

    return withinEpsilon(1, sum);
}

bool test_equality()
{
    LogReal<double> a = 0.1;
    return a == 0.1;
}

bool test_multiply()
{
    bool result = true;
    LogReal<double> a = 0.5;
    LogReal<double> b = 0.5;
    
    result &= a == a * 1.0;  //identity
    result &= a * a == 0.25; //powers
   
   return result;
}

bool test_divide()
{
    bool result = true;
    LogReal<double> a = 0.5;
    LogReal<double> b = 1.0 / 3.0;

    result &= (a / a) == 1.0;   //identity

    result &= (1.0 / a) == 2.0; //reciprocal
    result &= withinEpsilon((1.0 / b), 3.0);
    return result;
}

bool test_overflow_underflow()
{
    LogReal<double> c = 0.5;
    for( int ii = 0 ; ii < 1074 ; ++ii ){
        c = c * 0.5;
    }

    for( int ii = 0 ; ii < 1074 ; ++ii ){
        c = c / 0.5;
    }
  
    return withinEpsilon(c, 0.5);
}

bool test_worthwhile()
{

    double d = 0.25;
    LogReal<double> ld = 0.25;
    LogReal<double> m  = 0.25;
    LogReal<double> zero = 0.0;
    int float_overflow_point = 0;
    int logfloat_overflow_point = 0;
    while( d > zero ) {
        d *= 0.25;
        ++float_overflow_point;
    }

    while( ld > zero ) {
        ld = ld * m;
        ++logfloat_overflow_point;
    }
   
    return logfloat_overflow_point > float_overflow_point;
}


int main(int argc, char** argv)
{
    cout << "TESTING Equality: " << (test_equality() ? "PASSED" : "FAILED!") << endl; 
    cout << "TESTING Multiplication: " << (test_multiply() ? "PASSED" : "FAILED!") << endl; 
    cout << "TESTING Division: " << (test_divide() ? "PASSED" : "FAILED!") << endl; 
    cout << "TESTING Normalization: " << (test_normalize() ? "PASSED" : "FAILED!") << endl; 
    cout << "TESTING Overflow/Underflow: " << (test_overflow_underflow() ? "PASSED" : "FAILED!") << endl; 
    cout << "TESTING Usefulness: " << (test_worthwhile() ? "Hooray!" : "Bugger all, I seem to have made a mistake.") << endl;
    return 0;
}
