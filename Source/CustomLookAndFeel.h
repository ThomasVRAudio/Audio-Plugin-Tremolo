/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 18 Feb 2023 7:50:26pm
    Author:  thoma

  ==============================================================================
*/
#include <JuceHeader.h>

#pragma once
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    void drawRotarySlider(Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, Slider&) override;
    void drawToggleButton(Graphics&, ToggleButton&,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;


private:
    void setupRotaryKnobPaths();
    void setupSyncButtonPaths();

    Path rotaryKnobPath;
    Path rotaryKnobBackground;
    Path rotaryKnobRect;

public:
    Path syncKnobPath;
    Path sPath;
private:
    float knobOffset;


};