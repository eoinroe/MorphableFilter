#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    for (int i = 0; i < 4; ++i)
        labels.add  (new juce::Label());

    labels[0]->setText ("Noise Level L", juce::dontSendNotification);
    labels[1]->setText ("Noise Level R", juce::dontSendNotification);
    labels[2]->setText ("Filter Cutoff", juce::dontSendNotification);
    labels[3]->setText ("Filter Resonance", juce::dontSendNotification);

    for (auto& label : labels)
        addAndMakeVisible(label);

    leftLevelSlider.setRange(0.0, 0.25);
    rightLevelSlider.setRange(0.0, 0.25);
    cutoffLevelSlider.setRange(100.0, 1000.0);
    resonanceLevelSlider.setRange(0.0, 1.1);

    cutoffLevelSlider.setValue(500.0);

    leftLevelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    rightLevelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 50);
    cutoffLevelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 80);
    resonanceLevelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 110);

    addAndMakeVisible(leftLevelSlider);
    addAndMakeVisible(rightLevelSlider);
    addAndMakeVisible(cutoffLevelSlider);
    addAndMakeVisible(resonanceLevelSlider);

    // add items to the combo-box
    addAndMakeVisible (filterMenu);

    filterMenu.addSectionHeading ("Two Pole");
    filterMenu.addItem ("Low Pass",  twoPoleLowPass);
    filterMenu.addItem ("Band Pass", twoPoleBandPass);
    filterMenu.addItem ("High Pass", twoPoleHighPass);
    filterMenu.addSeparator();

    filterMenu.addSectionHeading ("Four Pole");
    filterMenu.addItem ("Low Pass",  fourPoleLowPass);
    filterMenu.addItem ("Band Pass", fourPoleBandPass);
    filterMenu.addItem ("High Pass", fourPoleHighPass);

    filterMenu.onChange = [this] { filterMenuChanged(); };
    filterMenu.setSelectedId (1);

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 200);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

    currentSampleRate = sampleRate;
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    float filterCutoff = cutoffLevelSlider.getValue();
    float filterResonance = resonanceLevelSlider.getValue();

    morphableFilter.calculate_coefficients(currentSampleRate, filterCutoff, filterResonance);

    auto leftLevel = leftLevelSlider.getValue();
    auto rightLevel = rightLevelSlider.getValue();

    auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto noise = random.nextFloat() * 2.0f - 1.0f;
        auto currentSample = morphableFilter.process(noise);

        leftBuffer[sample] = currentSample * leftLevel;
        rightBuffer[sample] = currentSample * rightLevel;
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    filterMenu.setBounds (10, 170, getWidth() - 20, 20);

    labels[0]->setBounds(10, 20, 90, 20);
    labels[1]->setBounds(10, 50, 90, 20);
    labels[2]->setBounds(10, 80, 90, 20);
    labels[3]->setBounds(10, 110, 90, 20);

    leftLevelSlider.setBounds (100, 20, getWidth() -110, 20);
    rightLevelSlider.setBounds (100, 50, getWidth() -110, 20);

    cutoffLevelSlider.setBounds (100, 80, getWidth() -110, 20);
    resonanceLevelSlider.setBounds (100, 110, getWidth() -110, 20);
}

//==============================================================================
void MainComponent::filterMenuChanged()
{
    switch (filterMenu.getSelectedId())
    {
        case twoPoleLowPass:   morphableFilter.set_filter_type(0); break;
        case twoPoleBandPass:  morphableFilter.set_filter_type(1); break;
        case twoPoleHighPass:  morphableFilter.set_filter_type(2); break;
        case fourPoleLowPass:  morphableFilter.set_filter_type(3); break;
        case fourPoleBandPass: morphableFilter.set_filter_type(4); break;
        case fourPoleHighPass: morphableFilter.set_filter_type(5); break;
        default: break;
    }
}
