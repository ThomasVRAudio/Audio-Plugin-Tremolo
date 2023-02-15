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

    xOffset = -20;
    yOffset = 0;

    AudioParameterFloat* speedParameter = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* depthParameter = (AudioParameterFloat*)params.getUnchecked(2);
    AudioParameterInt* shapeParameter = (AudioParameterInt*)params.getUnchecked(3);

    sliderSetup(mSpeedSlider, speedParameter, speedLabel, xOffset + getLocalBounds().getWidth() / 4, yOffset);
    sliderSetup(mDryWetSlider, dryWetParameter, dryWetLabel, xOffset + 2.f * (getLocalBounds().getWidth() / 4), yOffset);
    sliderSetup(mDepthSlider, depthParameter, depthLabel, xOffset + 3.f * (getLocalBounds().getWidth() / 4), yOffset);

    mShapeType.addItemList(mShapeTypeList,1);
    comboSetup(mShapeType, shapeParameter, xOffset + getLocalBounds().getWidth() / 4, 200, 100, 20);

}

void TVRATremoloAudioProcessorEditor::sliderSetup(Slider& slider, AudioParameterFloat* param, Label &label, float x, float y, float width, float height) {
    slider.setBounds(x, y, width, height);
    slider.setRange(param->range.start, param->range.end);
    slider.setValue(param->get());
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    slider.setSliderStyle(Slider::LinearVertical);
    addAndMakeVisible(slider);

    slider.onValueChange = [&slider, param] { *param = slider.getValue(); };
    slider.onDragStart = [&slider, param] { param->beginChangeGesture(); };
    slider.onDragEnd = [&slider, param] { param->endChangeGesture(); };

    label.setText(param->getParameterID(), dontSendNotification);
    label.attachToComponent(&slider, true);
    label.setColour(label.textColourId, juce::Colour(242, 243, 241));
    addAndMakeVisible(label);

}

void TVRATremoloAudioProcessorEditor::comboSetup(ComboBox& box, AudioParameterInt* param, float x, float y, float width, float height) {
    
    box.setBounds(xOffset + getLocalBounds().getWidth() / 4, 200, 100, 20);
    box.setSelectedItemIndex(0);
    addAndMakeVisible(box);

    box.onChange = [&box, param] { *param = box.getSelectedItemIndex(); };
}

TVRATremoloAudioProcessorEditor::~TVRATremoloAudioProcessorEditor()
{
}

//==============================================================================
void TVRATremoloAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.setGradientFill(juce::ColourGradient(juce::Colour(80, 89, 96), 0.f, 0.f, 
                                           juce::Colour(124, 125, 114), 
                                           (float)getLocalBounds().getWidth(), 
                                           (float)getLocalBounds().getHeight(), 
                                           false));
    g.fillAll();
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colour(242, 243, 241));
    g.setFont (25.0f);
    g.drawFittedText ("TVRA Tremolo", getLocalBounds(), juce::Justification::centred, 1);
}

void TVRATremoloAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
