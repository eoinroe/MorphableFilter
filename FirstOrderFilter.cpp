/*
  ==============================================================================

    FirstOrderFilter.cpp
    Created: 8 Oct 2021 4:20:20pm
    Author:  Eoin Roe

  ==============================================================================
*/

#include "FirstOrderFilter.h"

float FirstOrderFilter::process(float input)
{
    float output = ((b0*input + b1*previousInput - a1*previousOutput) * g) + a1*previousOutput;

    previousInput = input;
    previousOutput = output;

    return output;
}
