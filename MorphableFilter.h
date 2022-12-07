/*
  ==============================================================================

    MorphableFilter.h
    Created: 8 Oct 2021 4:20:46pm
    Author:  Eoin Roe

  ==============================================================================
*/

#pragma once

#include "FirstOrderFilter.h"
#include <array>
#include <vector>

class MorphableFilter {
public:
    MorphableFilter();
    float process(float input);
    void calculate_coefficients(float sampleRate, float frequencyHz, float resonance);
    void set_filter_type(int type);
private:
    // Coefficients
    float a1 {};
    float b0 {};
    float b1 {};
    float g  {};
    
    std::vector<FirstOrderFilter> filters;
    
    // Fixed by design
    const float gComp = 0.5f;
    float gRes = 0.75f;
    
    float delayed_filter_output {};

    enum filter_type
    {
        two_pole_low_pass,
        two_pole_band_pass,
        two_pole_high_pass,
        four_pole_low_pass,
        four_pole_band_pass,
        four_pole_high_pass
    };
                    
    filter_type filter_t = two_pole_low_pass;
    
    std::array<std::array<float, 5>, 6> filter_weights;
};

