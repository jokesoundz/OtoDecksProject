/*
  ==============================================================================

    IndicatorLight.h
    Created: 28 Aug 2025 6:59:48pm
    Author:  PC

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class IndicatorLight  : public juce::Component
{
public:
    IndicatorLight();
    ~IndicatorLight() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setActive(bool shouldBeActive);
    bool isActive() const;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IndicatorLight)

    bool active = false;
};
