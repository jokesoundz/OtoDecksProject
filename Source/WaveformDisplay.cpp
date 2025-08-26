/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew & joe

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0)
                          
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour(juce::Colours::black);
    //g.fillRect(getLocalBounds());

    g.setColour (Colours::orange);
    if(fileLoaded)
    {
        audioThumb.drawChannel(g, 
                                getLocalBounds(), 
                                0, 
                                audioThumb.getTotalLength(), 
                                0, 
                                1.0f
                                );
        
        // current position line
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth() - 1, 0, 2, getHeight());

        // cue point line
        if (cuePosition >= 0.0)
        {
            g.setColour(Colours::blue);
            g.drawRect(cuePosition * getWidth() - 1, 0, 2, getHeight());
        }
    }
    else 
    {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::cout << "wfd: loaded! " << std::endl;
        repaint();
    }
    else {
        std::cout << "wfd: not loaded! " << std::endl;
    }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;

    repaint();

}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}

void WaveformDisplay::setCuePointRelative(double pos)
{
    cuePosition = pos;
    repaint();
}




