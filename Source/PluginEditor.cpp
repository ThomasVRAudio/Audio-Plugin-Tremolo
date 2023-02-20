/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TVRATremoloAudioProcessorEditor::TVRATremoloAudioProcessorEditor (TVRATremoloAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    audioProcessor (p)
    //positionLabel{audioProcessor.currentPlayHeadPosition}, 
    //quarterNoteLabel{audioProcessor.quarterNotePosition}
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 350);

    juce::LookAndFeel::setDefaultLookAndFeel(&mLookAndFeel);

    mTitleBox = { 80, 300,300, 50 };
    mTitleFont.setBold(true);
    mTitleFont.setHeight(15.0f);

    auto params = audioProcessor.getParameters();

    mXOffset = -30;
    mYOffset = 40;
    mKnobOffset = 15;
    float mKnobYOffset = 55;

    AudioParameterFloat* speedParameter = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* dryWetParameter = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* depthParameter = (AudioParameterFloat*)params.getUnchecked(2);
    AudioParameterInt* shapeParameter = (AudioParameterInt*)params.getUnchecked(3);

    AudioParameterFloat* speedLFOParameter = (AudioParameterFloat*)params.getUnchecked(4);
    AudioParameterFloat* dryWetLFOParameter = (AudioParameterFloat*)params.getUnchecked(5);
    AudioParameterFloat* depthLFOParameter = (AudioParameterFloat*)params.getUnchecked(6);

    AudioParameterFloat* speedLFODepthParameter = (AudioParameterFloat*)params.getUnchecked(7);
    AudioParameterFloat* dryWetLFODepthParameter = (AudioParameterFloat*)params.getUnchecked(8);
    AudioParameterFloat* depthLFODepthParameter = (AudioParameterFloat*)params.getUnchecked(9);

    sliderSetup(mSpeedSlider, speedParameter, mXOffset + getLocalBounds().getWidth() / 4, mYOffset);
    sliderSetup(mDryWetSlider, dryWetParameter, mXOffset + 3 * (getLocalBounds().getWidth() / 4), mYOffset);
    sliderSetup(mDepthSlider, depthParameter, mXOffset + 2 * (getLocalBounds().getWidth() / 4), mYOffset);

    labelSetup(mSpeedSlider, speedParameter, mSpeedLabel);
    labelSetup(mDryWetSlider, dryWetParameter, mDryWetLabel, "Wet");
    labelSetup(mDepthSlider, depthParameter, mDepthLabel);
    
    sliderSetup(mSpeedLFOKnob, speedLFOParameter, mXOffset + getLocalBounds().getWidth() / 4 + mKnobOffset, 100 + mKnobYOffset, 30,30, Slider::SliderStyle::RotaryVerticalDrag);
    sliderSetup(mDryWetLFOKnob, speedLFOParameter, mXOffset + 2 * getLocalBounds().getWidth() / 4 + mKnobOffset, 100 + mKnobYOffset, 30, 30, Slider::SliderStyle::RotaryVerticalDrag);
    sliderSetup(mDepthLFOKnob, speedLFOParameter, mXOffset + 3 * getLocalBounds().getWidth() / 4 + mKnobOffset, 100 + mKnobYOffset, 30, 30, Slider::SliderStyle::RotaryVerticalDrag);

    sliderSetup(mSpeedLFODepthKnob, speedLFODepthParameter, mXOffset + getLocalBounds().getWidth() / 4 + mKnobOffset, 150 + mKnobYOffset, 30, 30, Slider::SliderStyle::RotaryVerticalDrag);
    sliderSetup(mDryWetLFODepthKnob, dryWetLFODepthParameter, mXOffset + 2 * getLocalBounds().getWidth() / 4 + mKnobOffset, 150 + mKnobYOffset, 30, 30, Slider::SliderStyle::RotaryVerticalDrag);
    sliderSetup(mDepthLFODepthKnob, depthLFODepthParameter, mXOffset + 3 * getLocalBounds().getWidth() / 4 + mKnobOffset, 150 + mKnobYOffset, 30, 30, Slider::SliderStyle::RotaryVerticalDrag);

    mShapeType.addItemList(mShapeTypeList,1);
    comboSetup(mShapeType, shapeParameter);

    mSyncButton = std::make_unique <ToggleButton>("Sync");
    mSyncButton->setBounds(65, 250, 70, 70);
    mSyncButton->setToggleable(true);
    mSyncButton->setToggleState(audioProcessor.getSync(), dontSendNotification);
    addAndMakeVisible(*mSyncButton);
    audioProcessor.setSyncAmount();

    mSyncButton->onStateChange = [this, speedParameter] {
        
        audioProcessor.setSync( mSyncButton->getToggleState());
        audioProcessor.setSyncAmount();

        if (mSyncButton->getToggleState()) {
            mSpeedSlider.setRange(0.0f, 9.0f, 1.0f);
        }
        else {
            mSpeedSlider.setRange(speedParameter->range.start, speedParameter->range.end);
        }

    mSpeedSlider.onValueChange = [this, speedParameter] {
        *speedParameter = (float)mSpeedSlider.getValue();
        audioProcessor.setSyncAmount();
        };

    };
}

void TVRATremoloAudioProcessorEditor::sliderSetup(SliderWithMenu& slider, AudioParameterFloat* param, int x, int y, int width, int height, Slider::SliderStyle style) {
    slider.setBounds(x, y, width, height);
    slider.setRange(param->range.start, param->range.end);
    slider.setValue(param->get());
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    slider.setSliderStyle(style);
    addAndMakeVisible(slider);
    slider.onValueChange = [&slider, param] { *param = (float)slider.getValue(); };
    slider.onDragStart = [&slider, param] { param->beginChangeGesture(); };
    slider.onDragEnd = [&slider, param] { param->endChangeGesture(); };
    slider.setupMouseEvent(*this, audioProcessor, param->getParameterIndex());
}

void TVRATremoloAudioProcessorEditor::sliderSetup(SliderWithMenu& slider, AudioParameterInt* param, int x, int y, int width, int height) {
    slider.setBounds(x, y, width, height);
    slider.setRange(param->getRange().getStart(), param->getRange().getEnd(), 1);
    slider.setValue(param->get());
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    addAndMakeVisible(slider);

    slider.onValueChange = [&slider, param] { *param = (int)slider.getValue(); };
    slider.onDragStart = [&slider, param] { param->beginChangeGesture(); };
    slider.onDragEnd = [&slider, param] { param->endChangeGesture(); };

    slider.setupMouseEvent(*this, audioProcessor, param->getParameterIndex());
}

void TVRATremoloAudioProcessorEditor::labelSetup(SliderWithMenu& slider, RangedAudioParameter* param, Label& label, std::string name) {
    
    if (name != "0") {
        label.setText(name, dontSendNotification);
    }
    else {
        label.setText(param->getParameterID(), dontSendNotification);
    }
    label.attachToComponent(&slider, false);
    label.setFont(mLookAndFeel.getDefaultFont());
    label.setColour(label.textColourId, juce::Colour(242, 243, 241));
    label.setJustificationType(Justification::centred);
    addAndMakeVisible(label);
}

void TVRATremoloAudioProcessorEditor::comboSetup(ComboBox& box, AudioParameterInt* param) {
    
    box.setBounds(mXOffset + getLocalBounds().getWidth() / 1.5f, 270, 100, 20);
    box.setSelectedItemIndex(param->get());
    addAndMakeVisible(box);

    box.onChange = [&box, param] { *param = box.getSelectedItemIndex(); };
}

TVRATremoloAudioProcessorEditor::~TVRATremoloAudioProcessorEditor()
{
}

//==============================================================================
void TVRATremoloAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.setGradientFill(juce::ColourGradient(juce::Colour(50, 50, 50), 0.f, 0.f, 
                                           juce::Colour(77, 77, 77), 
                                           (float)getLocalBounds().getWidth(), 
                                           (float)getLocalBounds().getHeight(), 
                                           false));
    //g.setColour(juce::Colour(50, 50, 50));
    g.fillAll();

    //g.setGradientFill(juce::ColourGradient(juce::Colour(30, 108, 142), 0.f, 0.f,
    //    juce::Colours::beige,
    //    (float)mTitleBox.getWidth(),
    //    (float)mTitleBox.getHeight(),
    //    false));
        g.setGradientFill(juce::ColourGradient(juce::Colours::darkgrey, 0.f, 0.f,
        juce::Colours::lightgrey,
        (float)mTitleBox.getWidth(),
        (float)mTitleBox.getHeight(),
        false));
    //g.setColour(juce::Colours::grey);
    g.setFont (mTitleFont);
    g.drawFittedText ("Thomas VR Audio :: Tremolo", mTitleBox, juce::Justification::right, 1);

    Rectangle<int> rect = {0, getBounds().getHeight() / 2 - 40, getBounds().getWidth(),20};
    g.setFont(mLookAndFeel.getDefaultFont());
    g.setColour(juce::Colours::grey);
    g.drawFittedText("Fader LFO Speed: ", rect, juce::Justification::centred, 1);

    Rectangle<int> rect2 = { 0, getBounds().getHeight() / 2 + 10, getBounds().getWidth(),20 };

    g.drawFittedText("Fader LFO Depth: ", rect2, juce::Justification::centred, 1);
}

void TVRATremoloAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
