/*
  ==============================================================================

    TrackInfoDisplay.cpp
    Created: 25 Aug 2025 4:33:40pm
    Author:  PC

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackInfoDisplay.h"

//==============================================================================
TrackInfoDisplay::TrackInfoDisplay()
{
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(artistLabel);

    titleLabel.setJustificationType(Justification::centred);
    artistLabel.setJustificationType(Justification::centred);

    titleLabel.setFont(Font(16.0f, Font::bold));
    artistLabel.setFont(Font(14.0f));
}

TrackInfoDisplay::~TrackInfoDisplay()
{
}

void TrackInfoDisplay::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    //background of title/ artist info
    g.setColour(juce::Colours::black);
    g.fillRect(getLocalBounds());
}

void TrackInfoDisplay::resized()
{
    titleLabel.setBounds(0, 0, getWidth()/2, getHeight());
    artistLabel.setBounds(getWidth()/2, 0, getWidth()/2, getHeight());
}


void TrackInfoDisplay::setTrackInfo(const TrackInfo* trackInfo)
{
    titleLabel.setText(trackInfo->getTitle(), dontSendNotification);
    artistLabel.setText(trackInfo->getArtist(), dontSendNotification);
}