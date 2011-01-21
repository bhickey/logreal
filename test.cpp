#include <stdlib.h>
#include <iostream>
#include "logreal.h"

using namespace std;

#define EPSILON 0.0000000001

bool withinEpsilon(double p, double d){
    return abs(p - d) < EPSILON;
}

union intDouble {
    int i;
    double d;
};

double rdouble(){
    intDouble v;
    v.i = (0x3F800000 | (0x7FFFFF & rand()));
    return (v.d - 1.0);
}

void testing(string s){ cout << "TESTING: " << s << " "; }
void result(bool r){ cout << (r ? "PASSED!" : "FAILED") << endl; }

void test_identity()
{
    testing("identity");
    LogDouble d = 0.5;
    result( d == d
         && d == 0.5
         && 0.5 == d );
    return;
}

void test_multiply()
{
    testing("multiply");
    LogDouble a = 0.5;
    LogDouble b = 0.5;
    LogDouble c = a * b;
    result( c == 0.25
         && a * 0.5 == 0.25 
         && 0.5 * b == 0.25);
    return;
}

void test_add()
{
    testing("add");
    LogDouble a = 0.4;
    result( a + 0.1 == 0.5 
         && -0.2 + a == 0.2 );
}


void test_sum()
{
    testing("sum");
    LogDouble arr[5] = { 0.1, 0.2, 0.3, 0.4, 0.5 };
    result( LogDouble::sum(arr, 5) == 1.5 );
}

void test_product()
{
    testing("product");
    LogDouble arr[4] = { 0.1, 0.2, 0.3, 0.4 };
    LogDouble expected = 0.1 * 0.2 * 0.3 * 0.4;
    result( LogDouble::product(arr, 4) == expected );
}

void test_underflow()
{
    testing("underflow safety");
    double MULTIPLIER = 0.01;
    LogDouble ld = MULTIPLIER;
    double rd = MULTIPLIER;
    int ii = 0;

    while( rd != 0.0 ){
        rd *= MULTIPLIER;
        ++ii;
    }

    for( int jj = 0; jj < ii * 10; ++jj ){
        ld = ld * MULTIPLIER;
    }

    result( ld != 0.0 );
}

void test_ostream()
{
    LogDouble zero = 0.0;
    LogDouble half = 0.5;
    LogDouble neg_ten = -10.0;
    cout << zero << " == " << 0.0 << endl;
    cout << half << " == e^-0.693147" << endl; 
    cout << neg_ten << " == -e^2.30259" << endl;
}

int main(int argc, char** argv)
{
    test_identity();
    test_multiply();
    test_add();
    test_sum();
    test_product();
    test_underflow();
    test_ostream();
    return 0;
}
