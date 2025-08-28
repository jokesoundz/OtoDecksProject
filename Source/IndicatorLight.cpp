/*
  ==============================================================================

    IndicatorLight.cpp
    Created: 28 Aug 2025 6:59:48pm
    Author:  PC

  ==============================================================================
*/

#include <JuceHeader.h>
#include "IndicatorLight.h"

//==============================================================================
IndicatorLight::IndicatorLight()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(20, 20);
}

IndicatorLight::~IndicatorLight()
{
}

void IndicatorLight::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float diameterOuter = jmin(bounds.getWidth(), bounds.getHeight()) * 0.6f;
    float diameterInner = jmin(bounds.getWidth(), bounds.getHeight()) * 0.2f;
    auto lightBulbBorder = bounds.withSizeKeepingCentre(diameterOuter, diameterOuter);
    auto lightBulb = bounds.withSizeKeepingCentre(diameterInner, diameterInner);

    g.setColour(Colours::darkgrey);
    g.fillEllipse(lightBulbBorder);

    g.setColour(active ? Colours::limegreen : Colours::darkorange);
    g.fillEllipse(lightBulb);
}

void IndicatorLight::resized()
{

}

void IndicatorLight::setActive(bool shouldBeActive)
{
    if (active != shouldBeActive)
    {
        active = shouldBeActive;
        repaint();
    }
}

bool IndicatorLight::isActive() const
{
    return active;
}