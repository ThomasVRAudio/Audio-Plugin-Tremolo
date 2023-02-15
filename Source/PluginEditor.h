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
    void sliderSetup(Slider& slider, AudioParameterFloat* param, Label& label, float x = 0, float y = 0, float width = 20, float height = 100);
    Slider mSpeedSlider;
    Slider mDryWetSlider;
    Slider mDepthSlider;

    Label speedLabel;
    Label dryWetLabel;
    Label depthLabel;

    int xOffset;
    int yOffset;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TVRATremoloAudioProcessorEditor)
};
