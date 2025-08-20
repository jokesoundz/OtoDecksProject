/*
  ==============================================================================

    TimeDisplay.h
    Created: 20 Aug 2025 5:42:07pm
    Author:  PC

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TimeDisplay  : public juce::Component
{
public:
    TimeDisplay();
    ~TimeDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeDisplay)
};
