#pragma once

#include <JuceHeader.h>
#include "MorphableFilter.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    //==============================================================================
    void filterMenuChanged();

private:
    //==============================================================================
    // Your private member variables go here...
    juce::Random random;
    
    juce::Slider leftLevelSlider;
    juce::Slider rightLevelSlider;
    
    juce::Slider cutoffLevelSlider;
    juce::Slider resonanceLevelSlider;
    
    juce::ComboBox filterMenu;
       
    juce::OwnedArray<juce::Label> labels;
    
    double currentSampleRate {};
    MorphableFilter morphableFilter;
    
    // You can use any integer as an item ID except zero. Zero has a special meaning.
    // It is used to indicate that none of the items are selected.
    enum filterType
    {
        twoPoleLowPass = 1,
        twoPoleBandPass,
        twoPoleHighPass,
        fourPoleLowPass,
        fourPoleBandPass,
        fourPoleHighPass
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
