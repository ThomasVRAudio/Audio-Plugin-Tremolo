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
    mSpeedSlider.setBounds(0, 0, 100, 100);
    mSpeedSlider.setRange(speedParameter->range.start, speedParameter->range.end);
    mSpeedSlider.setValue(speedParameter->get());
    mSpeedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    mSpeedSlider.setSliderStyle(Slider::LinearVertical);
    addAndMakeVisible(mSpeedSlider);

    mSpeedSlider.onValueChange = [this, speedParameter] { *speedParameter = mSpeedSlider.getValue(); };
    mSpeedSlider.onDragStart = [this, speedParameter] { speedParameter->beginChangeGesture(); };
    mSpeedSlider.onDragEnd = [this, speedParameter] { speedParameter->endChangeGesture(); };
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
