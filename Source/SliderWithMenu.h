/*
  ==============================================================================

    SliderWithMenu.h
    Created: 15 Feb 2023 9:21:54pm
    Author:  thoma

  ==============================================================================
*/
#include <JuceHeader.h>
#include "PluginProcessor.h"

#pragma once
class SliderWithMenu : public juce::Slider {
public:
    SliderWithMenu();
    virtual void mouseDown(const MouseEvent& e) override;
    void setupMouseEvent(AudioProcessorEditor& editor, TVRATremoloAudioProcessor& processor, int index);

private:
    AudioProcessorEditor *editor;
    TVRATremoloAudioProcessor *audioProcessor;
    int index;
    std::unique_ptr<HostProvidedContextMenu> mParamMenu;
};