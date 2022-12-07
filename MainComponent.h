#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
// #include <JuceHeader.h>
// #include <juce_gui_extra/juce_gui_extra.h>

#include <juce_audio_utils/juce_audio_utils.h>
#include "MorphableFilter.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public juce::AudioAppComponent
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
    void paint (juce::Graphics&) override;
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
