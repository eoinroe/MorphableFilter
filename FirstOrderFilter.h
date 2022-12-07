/*
  ==============================================================================

    FirstOrderFilter.h
    Created: 8 Oct 2021 4:20:20pm
    Author:  Eoin Roe

  ==============================================================================
*/

#pragma once

#include <functional>

class FirstOrderFilter {
public:
    // Using member intializer list to initialize the reference member variables
    FirstOrderFilter(float& a1, float& b0, float& b1, float& g) : a1(a1), b0(b0), b1(b1), g(g) {}
    float process(float input);
private:
    // Using reference_wrapper since it is more flexible
    // and allows the object to be copied if necessary.
    std::reference_wrapper<float> a1;
    std::reference_wrapper<float> b0;
    std::reference_wrapper<float> b1;
    std::reference_wrapper<float> g;
    
    // Make sure previous input and output are initialized.

    // x[n-1]
    float previousInput {};
    
    // y[n-1]
    float previousOutput {};
};
