/*
  ==============================================================================

    TimeDisplay.cpp
    Created: 20 Aug 2025 5:42:07pm
    Author:  PC

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TimeDisplay.h"

//==============================================================================
TimeDisplay::TimeDisplay(AudioFormatManager& formatManagerToUse,
                         AudioThumbnailCache& cacheToUse) :
                         audioThumb(1000, formatManagerToUse, cacheToUse),
                         fileLoaded(false),
{

    audioThumb.addChangeListener(this);

}

TimeDisplay::~TimeDisplay()
{
}

void TimeDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    g.setColour(juce::Colours::black);
    g.fillRect(getLocalBounds());

    g.setColour (juce::Colours::darkorange);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("Time Remaining = -0:00 || Elapsed Time = 0:00", getLocalBounds(),
                juce::Justification::centred, true);
}

void TimeDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void TimeDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::cout << "TimeDisplay: loaded! " << std::endl;
        repaint();
    }
    else {
        std::cout << "TimeDisplay: not loaded! " << std::endl;
    }

}

void TimeDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    std::cout << "TimeDisplay: change received! " << std::endl;

    repaint();

}