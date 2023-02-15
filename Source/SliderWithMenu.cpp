/*
  ==============================================================================

    SliderWithMenu.cpp
    Created: 15 Feb 2023 9:21:54pm
    Author:  thoma

  ==============================================================================
*/

#include "SliderWithMenu.h"

SliderWithMenu::SliderWithMenu()
    : editor{nullptr}, audioProcessor{nullptr}, index{0}
{
}

void SliderWithMenu::mouseDown(const MouseEvent& e)
{
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();

    if (editor != nullptr && modifiers.isPopupMenu()) 
    {
        AudioProcessorEditorHostContext* context = editor->getHostContext();

        if (context != nullptr) {
            mParamMenu = context->getContextMenuForParameter((AudioParameterFloat*)audioProcessor->getParameters().getUnchecked(index));
            
            if (mParamMenu != nullptr)
                mParamMenu->showNativeMenu(getMouseXYRelative());
        }   
    } else
	{
		Slider::mouseDown(e);
	}

}

void SliderWithMenu::setupMouseEvent(AudioProcessorEditor& editor, TVRATremoloAudioProcessor& processor, int index)
{
    this->editor = &editor;
    this->audioProcessor = &processor;
    this->index = index;
}
