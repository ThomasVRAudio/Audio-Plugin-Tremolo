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
#include "CustomLookAndFeel.h"

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
    void sliderSetup(SliderWithMenu& slider, AudioParameterFloat* param, 
                                                                  int x = 0, 
                                                                  int y = 0, 
                                                                  int width = 60, 
                                                                  int height = 100, 
                                                                  Slider::SliderStyle style = Slider::SliderStyle::LinearVertical);
    void sliderSetup(SliderWithMenu& slider, AudioParameterInt* param, int x = 0, int y = 0, int width = 100, int height = 100);
    void comboSetup(ComboBox& box, AudioParameterInt* param);
    void labelSetup(SliderWithMenu& slider, RangedAudioParameter* param, Label& label, std::string name = "0");

    SliderWithMenu mSpeedSlider;
    SliderWithMenu mDryWetSlider;
    SliderWithMenu mDepthSlider;

    SliderWithMenu mSpeedLFOKnob;
    SliderWithMenu mDryWetLFOKnob;
    SliderWithMenu mDepthLFOKnob;

    SliderWithMenu mSpeedLFODepthKnob;
    SliderWithMenu mDryWetLFODepthKnob;
    SliderWithMenu mDepthLFODepthKnob;

    CustomLookAndFeel mLookAndFeel;

    std::unique_ptr<ToggleButton> mSyncButton;
    Label mSpeedLabel;
    Label mDryWetLabel;
    Label mDepthLabel;
    Label mEmptyLabel;

    int mXOffset;
    int mYOffset;
    int mKnobOffset;
    juce::Rectangle<int> mTitleBox;
    juce::Font mTitleFont;

    ComboBox mShapeType;

    StringArray mShapeTypeList = {
        "Sine",
        "Square",
        "Pulse",
        "Triangle"
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TVRATremoloAudioProcessorEditor)
};


