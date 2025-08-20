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
class TimeDisplay  : public Component,
                     public ChangeListener
{
public:
    TimeDisplay(AudioFormatManager& formatManagerToUse,
                AudioThumbnailCache& cacheToUse);
    ~TimeDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeDisplay)

    AudioThumbnail audioThumb;
    bool fileLoaded;
};
