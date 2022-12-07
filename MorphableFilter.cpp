/*
  ==============================================================================

    MorphableFilter.cpp
    Created: 8 Oct 2021 4:20:46pm
    Author:  Eoin Roe

  ==============================================================================
*/

#include "MorphableFilter.h"
#include <cmath>

MorphableFilter::MorphableFilter()
{
    // Create four first-order filters and add them to the vector
    for (int i = 0; i < 4; ++i)
        filters.push_back({a1, b0, b1, g});
    
    filter_weights[two_pole_low_pass]   = { {0,  0,  1,  0, 0} };
    filter_weights[two_pole_band_pass]  = { {0,  2, -2,  0, 0} };
    filter_weights[two_pole_high_pass]  = { {1, -2,  1,  0, 0} };
    
    filter_weights[four_pole_low_pass]  = { {0,  0,  0,  0, 1} };
    filter_weights[four_pole_band_pass] = { {0,  0,  4, -8, 4} };
    filter_weights[four_pole_high_pass] = { {1, -4,  6, -4, 1} };
}

void MorphableFilter::set_filter_type(int type)
{
    // Convert integer to the enumeration type
    filter_t = static_cast<filter_type>(type);
}

void MorphableFilter::calculate_coefficients(float sampleRate, float frequencyHz, float resonance)
{
    a1 = 1.0f;
    b0 = 1.0f / 1.3f;
    b1 = 0.3f / 1.3f;
    
    // Angular frequency
    float w =  2.0f * M_PI * frequencyHz / sampleRate;
    
    // Polynomial model which improves the fit
    g = 0.9892f*w - 0.4342f*powf(w, 2.0f) + 0.1381f*powf(w, 3.0f) - 0.0202f*powf(w, 4.0f);
    
    // Calculate a cubic polynomial to correct for the error
    gRes = resonance * (1.0029f + 0.0526f*w - 0.0926f*powf(w, 2.0f) + 0.0218f*powf(w, 3.0f));
}

float MorphableFilter::process(float input)
{
    float feedback_loop = input - ( (delayed_filter_output - (input * gComp)) * gRes * 4.0f );
    
    // Use the hyperbolic tangent function to approximate the nonlinearity of an analog circuit
    float nonlinearity = tanhf (feedback_loop);

    float filter_out {};
    float summing_junction {};
    
    // Use filter_type to index into the two dimensional array
    for (unsigned int i = 0; i < filters.size(); ++i) {
        if (i == 0) {
            summing_junction += nonlinearity * filter_weights[filter_t][i];
            filter_out = filters[i].process(nonlinearity);
        }
        else {
            summing_junction += filter_out * filter_weights[filter_t][i];
            filter_out = filters[i].process(filter_out);
        }
    }
    
    // This happens just before the multiplication by the coefficient E
    delayed_filter_output = filter_out;
    
    // Add the final weighting of the output outside of the for loop
    summing_junction += filter_out * filter_weights[filter_t][4];
    
    return summing_junction;
}
