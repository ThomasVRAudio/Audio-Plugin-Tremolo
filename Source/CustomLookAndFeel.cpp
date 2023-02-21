/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 18 Feb 2023 7:50:26pm
    Author:  thoma

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    setupRotaryKnobPaths();
    setupSyncButtonPaths();
    setupFaderPaths();
    setColour(PopupMenu::backgroundColourId, juce::Colour(0.f, 0.f, 0.f, 0.0f));
    knobOffset = 3.14f;
}

void CustomLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{   
    juce::Rectangle<float> rect(x, y, width, height);
    auto transform = juce::AffineTransform::rotation(juce::jmap(sliderPos, rotaryStartAngle + knobOffset, rotaryEndAngle + knobOffset),
        rect.getCentreX(), rect.getCentreY());

    Path bg;
    bg.addEllipse(rect);

    g.setGradientFill(juce::ColourGradient(juce::Colour(77, 77, 77), rect.getWidth() / 2, 0.f,
    juce::Colour(29,29,29),
    rect.getWidth() /2,
    rect.getHeight(),
    false));
    g.fillPath(bg);

    Path path2;
    path2.addEllipse(rect.reduced(3.f));
    g.setColour(juce::Colour(26, 26, 26));
    g.fillPath(path2);

    Path path3;
    path3.addEllipse(rect.reduced(4.f));

    g.setGradientFill(juce::ColourGradient(juce::Colour(14, 14, 14), rect.getWidth() / 2, rect.getHeight(),
        juce::Colour(51, 51, 51),
        rect.getWidth() / 2,
        rect.getHeight() *1.5f,
        true));
    g.fillPath(path3);

    Path path4;
    path4.addEllipse(rect.reduced(6.f));

    g.setGradientFill(juce::ColourGradient(juce::Colour(114, 114, 114), rect.getWidth() / 2, rect.getHeight() * 0.5f,
        juce::Colour(40, 40, 40),
        rect.getWidth() / 2,
        rect.getHeight() -2.f,
        false));
    g.fillPath(path4);

    Path path5;
    path5.addEllipse(rect.reduced(6.5f));

    g.setGradientFill(juce::ColourGradient(juce::Colour(82, 82, 82), rect.getWidth() / 2, 0.f,
        juce::Colour(37, 37, 37),
        rect.getWidth() / 2,
        rect.getHeight(),
        false));
    g.fillPath(path5);

    Path path6;
    juce::Point<float> newPos(0.f, 9.f);
    rect.setPosition(rect.getPosition() + newPos);
    path6.addEllipse(rect.reduced(17.5f));
    g.setColour(juce::Colour(150, 150, 150));
    g.fillPath(path6, transform);
}

void CustomLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    int x = 0, y = 0, width = 70, height = 70;

    // OUTER RING
    syncKnobPath.scaleToFit(x, y, width, height, true);
    g.setGradientFill(juce::ColourGradient(juce::Colour(0, 0, 0), width / 2, 0.f,
        juce::Colour(70, 70, 70),
        width / 2,
        height,
        false));
    g.fillPath(syncKnobPath);

    // OUTER CIRCLE
    syncKnobPath.scaleToFit(x+3, y+3, width-6, height-6, true);
    g.setGradientFill(juce::ColourGradient(juce::Colour(0, 0, 0), width / 2, 0.f,
        juce::Colour(96, 96, 96),
        width / 2,
        height /2,
        false));
    g.fillPath(syncKnobPath);

    // BLACK RING
    syncKnobPath.scaleToFit(x + 6, y + 6, width - 12, height - 12, true);
    if (button.getToggleState()) {


        g.setGradientFill(juce::ColourGradient(juce::Colour(67, 248, 132), width / 2, 0.f,
            juce::Colour(5, 254, 245),
            width / 2,
            height,
            false));
    }
    else {
        g.setColour(juce::Colour(0, 0, 0));
    }
    g.fillPath(syncKnobPath);

    //MIDDLE RING
    syncKnobPath.scaleToFit(x + 8, y + 8, width - 16, height - 16, true);
    g.setGradientFill(juce::ColourGradient(juce::Colour(15, 15, 15), width / 2,
        height * 1.f,
        juce::Colour(51, 51, 51),
        width / 2,
        height * 0.5f,
        true));
    g.fillPath(syncKnobPath);

    //INNER RING
    syncKnobPath.scaleToFit(x + 12, y + 12, width - 24, height - 24, true);
    g.setGradientFill(juce::ColourGradient(juce::Colour(15, 15, 15), width / 2,
        height * 1.f,
        juce::Colour(87, 87, 87),
        width / 2,
        height * 0.5f,
        true));
    g.fillPath(syncKnobPath);

    //INNER CIRCLE
    syncKnobPath.scaleToFit(x + 13, y + 13, width - 26, height - 26, true);
    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0, 0, 07),
        width / 2,
        0.f,
        juce::Colour(87, 87, 87), width / 2,
        height * 0.7f,
        true));
    g.fillPath(syncKnobPath);
    
    // S
    sPath.scaleToFit(x + 20, y + 20, width - 40, height - 40, true);
    if (button.getToggleState()) {
        g.setGradientFill(juce::ColourGradient(juce::Colour(67, 248, 132), width / 2, 0.f,
            juce::Colour(5, 254, 245),
            width / 2,
            height,
            false));
        g.fillPath(sPath);
    }
    else {
        g.setColour(juce::Colour(102, 102, 102));
    }
    g.fillPath(sPath);

}

void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos,
    float minSliderPos,
    float maxSliderPos,
    const Slider::SliderStyle style, Slider& slider)
{

        auto trackWidth = width * 0.22f;

        Point<float> startPoint((float)x + (float)width * 0.5f, (float)(height + y));
        Point<float> endPoint(startPoint.x, (float)y);

        // BACKGROUND
        Path backgroundTrack;
        backgroundTrack.startNewSubPath(startPoint);
        backgroundTrack.lineTo(endPoint);
        g.setGradientFill(juce::ColourGradient(juce::Colour(0, 0, 16), width / 2, 0.f,
            juce::Colour(70, 70, 70),
            width / 2,
            height,
            false));
        g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        //BLACK
        Path path2;
        endPoint.y += 2.f;
        path2.startNewSubPath(startPoint);
        path2.lineTo(endPoint);
        g.setColour(juce::Colour(0, 0, 0));
        g.strokePath(path2, { trackWidth -2.0f, PathStrokeType::curved, PathStrokeType::rounded });

        // GRADIENT BG
        Path path3;
        endPoint.y += 2.f;
        path3.startNewSubPath(startPoint);
        path3.lineTo(endPoint);
        g.setGradientFill(juce::ColourGradient(juce::Colour(0, 0, 6), width / 2, 0.f,
            juce::Colour(87, 87, 87),
            width / 2,
            height,
            false));
        g.strokePath(path3, { trackWidth -4.f, PathStrokeType::curved, PathStrokeType::rounded });

        Path path4;
        endPoint.y += 2.f;
        path4.startNewSubPath(startPoint);
        path4.lineTo(endPoint);
        g.setColour(juce::Colour(30, 30, 30));
        g.strokePath(path4, { trackWidth - 12.f, PathStrokeType::curved, PathStrokeType::rounded });


        Path valueTrack;
        Point<float> minPoint, maxPoint, thumbPoint;

            auto kx = ((float)x + (float)width * 0.5f);
            auto ky = sliderPos;

            minPoint = startPoint;
            maxPoint = { kx, ky };

        // FILL VALUE COLOUR
        valueTrack.startNewSubPath(minPoint);
        valueTrack.lineTo(maxPoint);
        g.setGradientFill(juce::ColourGradient(juce::Colour(91, 255, 127), width / 2, 0.f,
            juce::Colour(0, 255, 255),
            width / 2,
            height,
            false));
        g.strokePath(valueTrack, { trackWidth -10.5f, PathStrokeType::curved, PathStrokeType::rounded });

        auto thumbWidth = width * 0.6f;
        auto thumbHeight = thumbWidth / 2.5f;

        auto valueWidth = maxPoint.x - thumbWidth / 2;
        auto valueHeight = maxPoint.y - thumbHeight / 2;
        
        // KNOB EDGE
        faderKnobPath.scaleToFit(valueWidth, valueHeight, thumbWidth, thumbHeight, true);
        g.setGradientFill(juce::ColourGradient(juce::Colour(103, 103, 103), valueWidth / 2, valueHeight,
            juce::Colour(0, 0, 0),
            valueWidth / 2,
            valueHeight + thumbHeight,
            false));
        g.fillPath(faderKnobPath);

        // KNOB MAIN BODY
        faderKnobPath.scaleToFit(valueWidth + 0.f, valueHeight + 1.f, thumbWidth - 0.f, thumbHeight - 2.f, true);
        g.setGradientFill(juce::ColourGradient(juce::Colour(71, 71, 71), valueWidth / 2, valueHeight,
            juce::Colour(42, 42, 42),
            valueWidth / 2,
            valueHeight + thumbHeight,
            false));
        g.fillPath(faderKnobPath);

        // KNOB INNER BODY
        faderKnobPath.scaleToFit(valueWidth + 3.f, valueHeight + 3.f, thumbWidth - 6.f, thumbHeight - 6.f, true);
        g.setGradientFill(juce::ColourGradient(juce::Colour(35, 35, 35), valueWidth / 2, valueHeight,
            juce::Colour(90, 90, 90),
            valueWidth / 2,
            valueHeight + thumbHeight /2,
            false));
        g.fillPath(faderKnobPath);


}

void CustomLookAndFeel::drawComboBox(Graphics& g, int width, int height, bool,
    int, int, int, int, ComboBox& box)
{   
    Rectangle<int> boxBounds(0, 0, width, height);

    g.setGradientFill(juce::ColourGradient(juce::Colour(15, 15, 15), width /2,
        0.f,
        juce::Colour(51, 51, 51),
        width /2,
        height * 0.25f,
        false));
    g.fillRoundedRectangle(boxBounds.toFloat(), 1.0f);

    g.setColour(juce::Colour(10,10,10));
    g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), 1.0f, 2.0f);

    Rectangle<int> arrowZone(width - 30, 0, 20, height);
    Path path;
    path.startNewSubPath((float)arrowZone.getX() + 3.0f, (float)arrowZone.getCentreY() - 2.0f);
    path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 3.0f);
    path.lineTo((float)arrowZone.getRight() - 3.0f, (float)arrowZone.getCentreY() - 2.0f);

    g.setColour(box.findColour(ComboBox::arrowColourId).withAlpha((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath(path, PathStrokeType(2.0f));
}

void CustomLookAndFeel::drawPopupMenuItem(Graphics& g, const Rectangle<int>& area,
    const bool isSeparator, const bool isActive,
    const bool isHighlighted, const bool isTicked,
    const bool hasSubMenu, const String& text,
    const String& shortcutKeyText,
    const Drawable* icon, const Colour* const textColourToUse)
{
        auto textColour = (textColourToUse == nullptr ? juce::Colours::white
            : *textColourToUse);

        auto r = area.reduced(1);

        auto font = getPopupMenuFont();
        font.setHeight(20.f);
        font.setBold(true);

        if (isHighlighted && isActive)
        {
            g.setColour(juce::Colour(50,50,50));
            g.fillRect(r);

            g.setColour(findColour(PopupMenu::highlightedTextColourId));
        }
        else
        {
            g.setColour(textColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
        }

        r.reduce(jmin(5, area.getWidth() / 20), 0);



        auto maxFontHeight = (float)r.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight(maxFontHeight);

        g.setFont(font);

        r.removeFromRight(3);
        g.drawFittedText(text, r, Justification::centredLeft, 1);
}

void CustomLookAndFeel::drawPopupMenuBackgroundWithOptions(Graphics& g,
    int width,
    int height,
    const PopupMenu::Options& menu)
{

    g.setColour(juce::Colour(30, 30, 30));
    Rectangle<float>area(width, height);
    g.fillRoundedRectangle(area.reduced(2.f), 3.f);
    
}

int CustomLookAndFeel::getMenuWindowFlags()
{
    return 0;
}

Font CustomLookAndFeel::getComboBoxFont(ComboBox& box)
{
    Font font = jmin(14.0f, (float)box.getHeight() * 0.85f);
    font.setBold(true);
    return font;
}

Font CustomLookAndFeel::getDefaultFont()
{
    Font font = 15.f;
    font.setBold(true);

    return font;
}

void CustomLookAndFeel::setupRotaryKnobPaths()
{
    static const unsigned char pathData[] = { 110,109,158,217,68,65,36,213,58,181,108,158,217,68,65,0,0,128,181,108,253,2,59,65,0,228,123,60,108,167,50,49,65,0,194,123,61,108,229,110,39,65,64,119,13,62,108,245,189,29,65,64,33,123,62,108,11,38,20,65,192,211,195,62,108,75,173,10,65,32,172,12,63,108,
196,89,1,65,176,243,62,63,108,219,98,240,64,96,160,120,63,108,72,116,222,64,168,198,156,63,108,72,243,204,64,128,200,192,63,108,14,235,187,64,160,62,232,63,108,129,102,171,64,228,135,9,64,108,50,112,155,64,84,144,32,64,108,88,18,140,64,212,41,57,64,108,
140,173,122,64,180,68,83,64,108,220,141,94,64,56,208,110,64,108,152,215,67,64,96,221,133,64,108,216,155,42,64,231,248,148,64,108,192,234,18,64,5,177,164,64,108,248,166,249,63,172,251,180,64,108,104,200,208,63,112,206,197,64,108,224,83,171,63,138,30,215,
64,108,96,97,137,63,234,224,232,64,108,64,13,86,63,49,10,251,64,108,32,174,32,63,96,199,6,65,108,0,143,229,62,96,49,16,65,108,160,237,152,62,17,189,25,65,108,0,82,55,62,88,100,35,65,108,0,161,183,61,7,33,45,65,108,0,140,252,60,228,236,54,65,108,0,0,47,
59,170,193,64,65,108,0,0,32,55,153,217,68,65,108,247,111,27,55,153,217,68,65,108,0,0,16,55,151,217,68,65,108,0,76,126,60,56,176,78,65,108,0,234,124,61,133,128,88,65,108,0,229,13,62,56,68,98,65,108,0,178,123,62,18,245,107,65,108,192,45,196,62,222,140,
117,65,108,192,225,12,63,121,5,127,65,108,224,49,63,63,106,44,132,65,108,0,231,120,63,124,192,136,65,108,24,238,156,63,4,60,141,65,108,248,243,192,63,36,156,145,65,108,0,110,232,63,16,222,149,65,108,132,161,9,64,13,255,153,65,108,200,171,32,64,119,252,
157,65,108,20,71,57,64,194,211,161,65,108,160,99,83,64,118,130,165,65,108,188,240,110,64,57,6,169,65,108,102,238,133,64,204,92,172,65,108,164,10,149,64,13,132,175,65,108,111,195,164,64,246,121,178,65,108,183,14,181,64,162,60,181,65,108,17,226,197,64,
78,202,183,65,108,181,50,215,64,88,33,186,65,108,145,245,232,64,62,64,188,65,108,71,31,251,64,168,37,190,65,108,28,210,6,65,92,208,191,65,108,70,60,16,65,76,63,193,65,108,26,200,25,65,140,113,194,65,108,125,111,35,65,88,102,195,65,108,163,25,37,65,27,
138,195,65,108,163,25,37,65,27,138,195,65,108,163,25,37,65,101,230,87,65,108,224,93,96,65,101,230,87,65,108,224,93,96,65,153,219,195,65,108,224,93,96,65,153,219,195,65,108,249,18,106,65,98,12,195,65,108,64,176,115,65,84,255,193,65,108,143,47,125,65,27,
181,192,65,108,105,69,131,65,137,46,191,65,108,6,222,135,65,152,108,189,65,108,174,94,140,65,106,112,187,65,108,126,196,144,65,66,59,185,65,108,168,12,149,65,139,206,182,65,108,110,52,153,65,209,43,180,65,108,37,57,157,65,196,84,177,65,108,62,24,161,
65,54,75,174,65,108,61,207,164,65,24,17,171,65,108,194,91,168,65,123,168,167,65,108,135,187,171,65,140,19,164,65,108,100,236,174,65,152,84,160,65,108,78,236,177,65,2,110,156,65,108,90,185,180,65,75,98,152,65,108,190,81,183,65,9,52,148,65,108,206,179,
185,65,232,229,143,65,108,7,222,187,65,171,122,139,65,108,4,207,189,65,37,245,134,65,108,136,133,191,65,58,88,130,65,108,123,0,193,65,188,77,123,65,108,233,62,194,65,36,200,113,65,108,7,64,195,65,196,37,104,65,108,49,3,196,65,197,108,94,65,108,233,135,
196,65,96,163,84,65,108,219,205,196,65,217,207,74,65,108,158,217,196,65,154,217,68,65,108,158,217,196,65,153,217,68,65,108,158,217,196,65,153,217,68,65,108,33,186,196,65,248,2,59,65,108,189,91,196,65,163,50,49,65,108,175,190,195,65,225,110,39,65,108,
92,227,194,65,241,189,29,65,108,80,202,193,65,8,38,20,65,108,62,116,192,65,72,173,10,65,108,2,226,190,65,194,89,1,65,108,156,20,189,65,216,98,240,64,108,54,13,187,65,70,116,222,64,108,24,205,184,65,71,243,204,64,108,183,85,182,65,15,235,187,64,108,164,
168,179,65,129,102,171,64,108,151,199,176,65,48,112,155,64,108,102,180,173,65,86,18,140,64,108,12,113,170,65,136,173,122,64,108,155,255,166,65,212,141,94,64,108,74,98,163,65,140,215,67,64,108,105,155,159,65,200,155,42,64,108,97,173,155,65,180,234,18,
64,108,184,154,151,65,216,166,249,63,108,6,102,147,65,56,200,208,63,108,0,18,143,65,168,83,171,63,108,104,161,138,65,40,97,137,63,108,22,23,134,65,192,12,86,63,108,241,117,129,65,160,173,32,63,108,226,129,121,65,0,142,229,62,108,49,246,111,65,96,236,
152,62,108,234,78,102,65,64,79,55,62,108,58,146,92,65,0,156,183,61,108,92,198,82,65,0,120,252,60,108,149,241,72,65,0,80,46,59,108,158,217,68,65,0,0,128,181,108,158,217,68,65,36,213,58,181,99,101,0,0 };

    rotaryKnobPath.loadPathFromData(pathData, sizeof(pathData));

    static const unsigned char knobBackgroundPathData[] = { 110,109,159,192,226,65,141,107,220,66,98,145,172,191,65,33,16,223,66,218,142,216,65,163,95,228,66,87,240,179,65,28,168,228,66,98,212,81,143,65,149,240,228,66,240,215,166,65,69,65,223,66,211,45,131,65,179,40,221,66,98,108,7,63,65,33,16,219,66,10,227,84,
  65,200,197,226,66,4,84,20,65,188,112,222,66,98,251,137,167,64,176,27,218,66,218,64,19,65,28,32,216,66,162,186,187,64,41,218,209,66,98,34,231,33,64,54,148,203,66,212,136,228,63,44,159,211,66,72,56,32,191,196,213,203,66,98,142,96,66,192,92,12,196,66,252,
  10,101,63,135,76,198,66,81,126,218,190,132,135,189,66,98,167,196,223,191,129,194,180,66,86,233,140,192,19,251,186,66,232,112,145,192,114,211,177,66,98,122,248,149,192,209,171,168,66,196,13,236,191,88,141,174,66,84,210,75,191,209,162,165,66,98,196,237,
  128,62,74,184,156,66,75,151,102,192,189,115,159,66,88,235,183,191,221,97,151,66,98,196,175,58,63,252,79,143,66,188,60,220,63,120,63,151,66,94,110,155,64,7,147,144,66,98,142,205,255,64,150,230,137,66,76,60,126,64,128,105,136,66,162,180,251,64,236,150,
  131,66,98,142,37,60,65,176,136,125,66,55,36,42,65,115,161,134,66,83,76,112,65,222,252,131,66,98,55,58,155,65,74,88,129,66,238,87,130,65,142,17,120,66,113,246,166,65,156,128,119,66,98,244,148,203,65,170,239,118,66,216,14,180,65,37,39,129,66,245,184,215,
  65,184,63,131,66,98,18,99,251,65,74,88,133,66,67,117,240,65,69,69,123,66,99,94,8,66,175,247,129,66,98,36,130,24,66,188,76,134,66,46,163,8,66,79,72,136,66,16,252,21,66,66,142,142,66,98,242,84,35,66,53,212,148,66,30,79,38,66,63,201,140,66,70,244,47,66,
  166,146,148,66,98,110,153,57,66,14,92,156,66,57,223,41,66,227,27,154,66,97,40,47,66,230,224,162,66,98,138,113,52,66,234,165,171,66,143,16,63,66,87,109,165,66,129,161,63,66,248,148,174,66,98,115,50,64,66,153,188,183,66,210,211,52,66,18,219,177,66,173,
  162,48,66,153,197,186,66,98,136,113,44,66,32,176,195,66,216,220,59,66,173,244,192,66,190,50,51,66,141,6,201,66,98,164,136,42,66,110,24,209,66,125,145,38,66,245,40,201,66,152,5,26,66,99,213,207,66,98,178,121,13,66,212,129,214,66,159,143,29,66,233,254,
  215,66,208,252,13,66,126,209,220,66,98,1,212,252,65,17,164,225,66,87,234,2,66,247,198,217,66,160,192,226,65,140,107,220,66,99,101,0,0 };

    rotaryKnobBackground.loadPathFromData(knobBackgroundPathData, sizeof(knobBackgroundPathData));

    static const unsigned char pathRectData[] = { 110,109,2,162,154,66,244,27,154,66,108,19,184,156,66,244,27,154,66,98,146,66,158,66,244,27,154,66,40,128,159,66,139,89,155,66,40,128,159,66,9,228,156,66,108,40,128,159,66,95,101,201,66,98,40,128,159,66,222,239,202,66,145,66,158,66,117,45,204,66,18,184,
  156,66,117,45,204,66,108,1,162,154,66,117,45,204,66,98,130,23,153,66,117,45,204,66,236,217,151,66,222,239,202,66,236,217,151,66,95,101,201,66,108,236,217,151,66,9,228,156,66,98,236,217,151,66,138,89,155,66,131,23,153,66,244,27,154,66,2,162,154,66,244,
  27,154,66,99,101,0,0 };


    rotaryKnobRect.loadPathFromData(pathRectData, sizeof(pathRectData));
}

void CustomLookAndFeel::setupSyncButtonPaths()
{
    // OUTER RING
    static const unsigned char outerRingPathData[] = { 110,109,172,10,164,66,18,193,149,66,108,172,10,164,66,18,193,149,66,108,75,249,160,66,111,173,149,66,108,224,233,157,66,145,114,149,66,108,98,222,154,66,158,16,149,66,108,194,216,151,66,214,135,148,66,108,239,218,148,66,143,216,147,66,108,212,230,145,
66,58,3,147,66,108,85,254,142,66,94,8,146,66,108,78,35,140,66,158,232,144,66,108,146,87,137,66,176,164,143,66,108,235,156,134,66,100,61,142,66,108,26,245,131,66,161,179,140,66,108,207,97,129,66,97,8,139,66,108,101,201,125,66,182,60,137,66,108,182,254,
120,66,199,81,135,66,108,162,102,116,66,206,72,133,66,108,25,4,112,66,23,35,131,66,108,234,217,107,66,3,226,128,66,108,192,234,103,66,6,14,125,66,108,28,57,100,66,48,39,120,66,108,95,199,96,66,168,18,115,66,108,188,151,93,66,175,211,109,66,108,60,172,
90,66,160,109,104,66,108,192,6,88,66,238,227,98,66,108,246,168,85,66,38,58,93,66,108,100,148,83,66,230,115,87,66,108,94,202,81,66,226,148,81,66,108,10,76,80,66,219,160,75,66,108,90,26,79,66,159,155,69,66,108,22,54,78,66,9,137,63,66,108,204,159,77,66,
252,108,57,66,108,221,87,77,66,96,75,51,66,108,18,81,77,66,220,189,48,66,108,18,81,77,66,221,189,48,66,108,18,81,77,66,220,189,48,66,108,88,120,77,66,27,155,42,66,108,20,238,77,66,70,124,36,66,108,250,177,78,66,73,101,30,66,108,138,195,79,66,9,90,24,
66,108,24,34,81,66,100,94,18,66,108,195,204,82,66,46,118,12,66,108,122,194,84,66,48,165,6,66,108,251,1,87,66,33,239,0,66,108,214,137,89,66,82,175,246,65,108,110,88,92,66,184,196,235,65,108,246,107,95,66,113,37,225,65,108,118,194,98,66,71,216,214,65,108,
203,89,102,66,212,227,204,65,108,170,47,106,66,116,78,195,65,108,157,65,110,66,75,30,186,65,108,10,141,114,66,57,89,177,65,108,51,15,119,66,220,4,169,65,108,52,197,123,66,134,38,161,65,108,4,86,128,66,67,195,153,65,108,72,224,130,66,202,223,146,65,108,
196,127,133,66,133,128,140,65,108,203,50,136,66,135,169,134,65,108,163,247,138,66,142,94,129,65,108,134,204,141,66,250,69,121,65,108,165,175,144,66,178,243,112,65,108,39,159,147,66,154,203,105,65,108,42,153,150,66,72,210,99,65,108,199,155,153,66,140,
11,95,65,108,17,165,156,66,118,122,91,65,108,23,179,159,66,76,33,89,65,108,228,195,162,66,146,1,88,65,108,172,10,164,66,98,230,87,65,108,172,10,164,66,99,230,87,65,108,172,10,164,66,98,230,87,65,108,13,28,167,66,126,131,88,65,108,120,43,170,66,108,90,
90,65,108,247,54,173,66,2,106,93,65,108,151,60,176,66,72,176,97,65,108,105,58,179,66,128,42,103,65,108,132,46,182,66,44,213,109,65,108,4,23,185,66,8,172,117,65,108,12,242,187,66,12,170,126,65,108,200,189,190,66,190,100,132,65,108,110,120,193,66,239,1,
138,65,108,64,32,196,66,254,40,144,65,108,138,179,198,66,254,213,150,65,108,168,48,201,66,170,4,158,65,108,255,149,203,66,103,176,165,65,108,10,226,205,66,79,212,173,65,108,78,19,208,66,42,107,182,65,108,101,40,210,66,124,111,191,65,108,250,31,212,66,
125,219,200,65,108,203,248,213,66,39,169,210,65,108,170,177,215,66,53,210,220,65,108,122,73,217,66,38,80,231,65,108,58,191,218,66,65,28,242,65,108,248,17,220,66,160,47,253,65,108,220,64,221,66,150,65,4,66,108,37,75,222,66,212,7,10,66,108,40,48,223,66,
214,230,15,66,108,82,239,223,66,220,218,21,66,108,41,136,224,66,22,224,27,66,108,76,250,224,66,170,242,33,66,108,113,69,225,66,181,14,40,66,108,104,105,225,66,79,48,46,66,108,206,108,225,66,220,189,48,66,108,206,108,225,66,221,189,48,66,108,206,108,225,
66,221,189,48,66,108,43,89,225,66,159,224,54,66,108,77,30,225,66,116,255,60,66,108,90,188,224,66,113,22,67,66,108,146,51,224,66,177,33,73,66,108,75,132,223,66,86,29,79,66,108,246,174,222,66,140,5,85,66,108,27,180,221,66,138,214,90,66,108,90,148,220,66,
153,140,96,66,108,108,80,219,66,17,36,102,66,108,33,233,217,66,94,153,107,66,108,93,95,216,66,2,233,112,66,108,29,180,214,66,150,15,118,66,108,115,232,212,66,208,9,123,66,108,132,253,210,66,128,212,127,66,108,138,244,208,66,74,54,130,66,108,212,206,206,
66,142,103,132,66,108,192,141,204,66,166,124,134,66,108,192,50,202,66,59,116,136,66,108,85,191,199,66,12,77,138,66,108,18,53,197,66,234,5,140,66,108,150,149,194,66,188,157,141,66,108,144,226,191,66,123,19,143,66,108,184,29,189,66,57,102,144,66,108,213,
72,186,66,30,149,145,66,108,182,101,183,66,103,159,146,66,108,53,118,180,66,106,132,147,66,108,50,124,177,66,148,67,148,66,108,149,121,174,66,108,220,148,66,108,76,112,171,66,143,78,149,66,108,70,98,168,66,180,153,149,66,108,121,81,165,66,172,189,149,
66,108,173,10,164,66,18,193,149,66,108,172,10,164,66,18,193,149,66,99,101,0,0 };

    syncKnobPath.loadPathFromData(outerRingPathData, sizeof(outerRingPathData));

    // S
    static const unsigned char sPathData[] = { 110,109,192,109,133,67,101,151,48,67,108,22,181,135,67,101,219,45,67,113,22,29,138,67,101,183,54,67,107,68,141,67,101,183,54,67,113,192,157,142,67,101,183,54,67,22,205,143,67,15,122,53,67,113,108,252,144,67,100,51,52,67,22,155,145,67,15,22,50,67,113,
193,57,146,67,185,248,47,67,193,57,146,67,15,154,45,67,113,193,57,146,67,100,231,42,67,108,80,145,67,186,80,40,67,113,108,14,144,67,15,190,36,67,108,184,140,67,15,182,31,67,113,193,93,137,67,186,164,26,67,193,139,136,67,15,98,24,67,113,193,31,135,67,
100,151,20,67,193,31,135,67,15,46,16,67,113,193,31,135,67,15,174,12,67,108,246,135,67,186,204,9,67,113,23,205,136,67,101,235,6,67,108,80,138,67,100,71,5,67,113,108,216,139,67,15,154,3,67,193,161,141,67,15,154,3,67,113,22,135,143,67,15,154,3,67,22,43,
145,67,100,127,5,67,113,193,211,146,67,100,91,7,67,22,171,148,67,100,99,12,67,108,22,123,146,67,185,180,15,67,113,193,247,144,67,14,178,11,67,107,228,143,67,100,107,10,67,113,192,213,142,67,185,36,9,67,192,147,141,67,185,36,9,67,113,106,244,139,67,185,
36,9,67,107,234,138,67,185,28,11,67,113,22,229,137,67,185,20,13,67,22,229,137,67,14,246,15,67,113,22,229,137,67,14,182,17,67,107,66,138,67,14,90,19,67,113,192,159,138,67,14,254,20,67,21,151,139,67,185,236,22,67,113,107,30,140,67,14,242,23,67,192,13,143,
67,185,72,28,67,113,22,137,146,67,185,108,33,67,107,212,147,67,100,111,37,67,113,192,31,149,67,15,114,41,67,192,31,149,67,15,126,45,67,113,192,31,149,67,101,83,51,67,107,230,146,67,187,160,55,67,113,192,177,144,67,16,238,59,67,192,133,141,67,16,238,59,
67,113,107,20,139,67,16,238,59,67,192,23,137,67,101,87,57,67,113,21,27,135,67,101,183,54,67,192,109,133,67,100,151,48,67,99,101,0,0 };

    sPath.loadPathFromData(sPathData, sizeof(sPathData));

}

void CustomLookAndFeel::setupFaderPaths()
{
    static const unsigned char faderPathData[] = { 110,109,120,177,105,67,139,68,192,194,98,185,196,108,67,139,68,192,194,129,62,111,67,250,80,187,194,129,62,111,67,120,42,181,194,108,129,62,111,67,137,117,52,66,98,129,62,111,67,141,194,64,66,185,196,108,67,174,169,74,66,120,177,105,67,174,169,74,66,
98,55,158,102,67,174,169,74,66,110,36,100,67,141,194,64,66,110,36,100,67,137,117,52,66,108,110,36,100,67,120,42,181,194,98,110,36,100,67,250,80,187,194,55,158,102,67,139,68,192,194,120,177,105,67,139,68,192,194,99,101,0,0 };

    faderPath.loadPathFromData(faderPathData, sizeof(faderPathData));

    static const unsigned char faderKnobPathData[] = { 110,109,86,77,97,67,85,127,75,194,108,153,21,114,67,85,127,75,194,98,148,132,116,67,85,127,75,194,28,122,118,67,52,169,67,194,28,122,118,67,74,237,57,194,108,28,122,118,67,113,49,41,194,98,28,122,118,67,134,117,31,194,148,132,116,67,102,159,23,194,153,
21,114,67,102,159,23,194,108,86,77,97,67,102,159,23,194,98,91,222,94,67,102,159,23,194,211,232,92,67,134,117,31,194,211,232,92,67,113,49,41,194,108,211,232,92,67,74,237,57,194,98,211,232,92,67,53,169,67,194,91,222,94,67,85,127,75,194,86,77,97,67,85,127,
75,194,99,101,0,0 };

    faderKnobPath.loadPathFromData(faderKnobPathData, sizeof(faderKnobPathData));

}


