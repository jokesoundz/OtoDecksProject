/*
  ==============================================================================

    TrackInfoDisplay.h
    Created: 25 Aug 2025 4:33:40pm
    Author:  PC

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackInfo.h"

//==============================================================================
/*
*/
class TrackInfoDisplay  : public juce::Component
{
    public:
        TrackInfoDisplay();
        ~TrackInfoDisplay() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        /** permits User Interaction
        *** take current TrackInfo from LibraryComponent
        *** updates DeckGUI with correct TrackInfoDisplay */
        void setTrackInfo(const TrackInfo* trackInfo);

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackInfoDisplay)

        Label titleLabel;
        Label artistLabel;
};
