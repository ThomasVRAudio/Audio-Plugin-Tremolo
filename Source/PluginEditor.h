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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TVRATremoloAudioProcessor& audioProcessor;

    Slider mSpeedSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TVRATremoloAudioProcessorEditor)
};
