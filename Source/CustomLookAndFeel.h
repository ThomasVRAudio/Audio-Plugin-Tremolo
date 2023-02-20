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
    void drawComboBox(Graphics&, int width, int height, bool isButtonDown,
        int buttonX, int buttonY, int buttonW, int buttonH,
        ComboBox&) override;
    void drawPopupMenuItem(Graphics&, const Rectangle<int>& area,
        bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
        const String& text, const String& shortcutKeyText,
        const Drawable* icon, const Colour* textColour) override;
    void drawPopupMenuBackgroundWithOptions(Graphics&,
        int width,
        int height,
        const PopupMenu::Options&) override;
    int getMenuWindowFlags() override;

    Font getComboBoxFont(ComboBox& box) override;
    Font getDefaultFont();

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