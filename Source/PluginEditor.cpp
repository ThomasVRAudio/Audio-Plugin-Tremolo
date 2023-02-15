/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TVRATremoloAudioProcessorEditor::TVRATremoloAudioProcessorEditor (TVRATremoloAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    auto params = audioProcessor.getParameters();
    
    AudioParameterFloat* speedParameter = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* depthParameter = (AudioParameterFloat*)params.getUnchecked(2);
    sliderSetup(mSpeedSlider, speedParameter, 0,0);
    sliderSetup(mDryWetSlider, dryWetParameter, 100, 0);
    sliderSetup(mDepthSlider, depthParameter, 200, 0);
}

void TVRATremoloAudioProcessorEditor::sliderSetup(Slider& slider, AudioParameterFloat* param, float x, float y, float width, float height) {
    slider.setBounds(x, y, width, height);
    slider.setRange(param->range.start, param->range.end);
    slider.setValue(param->get());
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    slider.setSliderStyle(Slider::LinearVertical);
    addAndMakeVisible(slider);

    slider.onValueChange = [&slider, param] { *param = slider.getValue(); };
    slider.onDragStart = [&slider, param] { param->beginChangeGesture(); };
    slider.onDragEnd = [&slider, param] { param->endChangeGesture(); };
}

TVRATremoloAudioProcessorEditor::~TVRATremoloAudioProcessorEditor()
{
}

//==============================================================================
void TVRATremoloAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("TVRA Tremolo", getLocalBounds(), juce::Justification::centred, 1);
}

void TVRATremoloAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
