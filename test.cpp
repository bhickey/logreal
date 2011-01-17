#include <stdlib.h>
#include <iostream>
#include "logreal.h"

using namespace std;

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
    Probability probs[LENGTH];
    
    for(int ii = 0; ii < LENGTH; ii++){
        probs[ii] = rfloat();
      for(int jj = 0; jj < 725; jj++){
        probs[ii] = probs[ii] * rfloat();
      }
    }

    Probability::normalize(probs, LENGTH);
   

    double sum = 0.0;
    for(int ii = 0; ii < LENGTH; ii++){
        sum += probs[ii];
    }

    cout << "Expected sum 1.0, got " << sum << endl;

}

int main(int argc, char** argv)
{
    test_normalize();
    return 0;
}
