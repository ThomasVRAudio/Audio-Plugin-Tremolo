/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
    void sliderSetup(Slider& slider, AudioParameterFloat* param, Label& label, float x = 0.f, float y = 0.f, float width = 20.f, float height = 100.f);
    void comboSetup(ComboBox& box, AudioParameterInt* param, float x = 0.f, float y = 0.f, float widht = 20.f, float height = 100.f);
    Slider mSpeedSlider;
    Slider mDryWetSlider;
    Slider mDepthSlider;

    Label speedLabel;
    Label dryWetLabel;
    Label depthLabel;

    int xOffset;
    int yOffset;

    ComboBox mShapeType;

    StringArray mShapeTypeList = {
        "Sine",
        "Square",
        "Triangle"
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TVRATremoloAudioProcessorEditor)
};
