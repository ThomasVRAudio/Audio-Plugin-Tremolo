/*
  ==============================================================================

    AtomicLabel.h
    Created: 17 Feb 2023 1:50:35pm
    Author:  thoma

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct AtomicLabel : juce::Component, juce::Timer
{
    AtomicLabel(std::atomic<double>& valueToDisplay) : value(valueToDisplay) {
        startTimerHz(60);
        addAndMakeVisible(label);
        label.setBounds(300, 200, 100, 100);
    }

    void resized() override {
        label.setBounds(getLocalBounds().getX() / 2, getLocalBounds().getY() / 2, 100, 100);
    }

    void timerCallback() override {
        label.setText(juce::String(value.load()), dontSendNotification);
    }
    juce::Label label;
    std::atomic<double>& value; // to be displayed
};