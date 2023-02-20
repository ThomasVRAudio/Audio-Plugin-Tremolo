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
    void drawLinearSlider(Graphics&, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle, Slider&) override;


private:
    void setupRotaryKnobPaths();
    void setupSyncButtonPaths();
    void setupFaderPaths();

    Path rotaryKnobPath;
    Path rotaryKnobBackground;
    Path rotaryKnobRect;

    Path syncKnobPath;
    Path sPath;

    Path faderPath;
    Path faderKnobPath;


    float knobOffset;


};