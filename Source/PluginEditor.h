/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SliderWithMenu.h"
#include "AtomicLabel.h"

//==============================================================================
/**
*/
class TVRATremoloAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TVRATremoloAudioProcessorEditor (TVRATremoloAudioProcessor&);
    ~TVRATremoloAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TVRATremoloAudioProcessor& audioProcessor;
    void sliderSetup(SliderWithMenu& slider, AudioParameterFloat* param, Label& label, int x = 0, int y = 0, int width = 20, int height = 100);
    void sliderSetup(SliderWithMenu& slider, AudioParameterInt* param, Label& label, int x = 0, int y = 0, int width = 100, int height = 100);
    void comboSetup(ComboBox& box, AudioParameterInt* param);

    SliderWithMenu mSpeedSlider;
    SliderWithMenu mDryWetSlider;
    SliderWithMenu mDepthSlider;

    std::unique_ptr<ToggleButton> mSyncButton;
    Label speedLabel;
    Label dryWetLabel;
    Label depthLabel;
    Label syncLabel;

    int xOffset;
    int yOffset;

    ComboBox mShapeType;

    StringArray mShapeTypeList = {
        "Sine",
        "Square",
        "Triangle"
    };

    //AtomicLabel positionLabel;
    //AtomicLabel quarterNoteLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TVRATremoloAudioProcessorEditor)
};


