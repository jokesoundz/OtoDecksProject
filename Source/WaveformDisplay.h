/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew & joe

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    ~WaveformDisplay();

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

    void setCuePointRelative(double pos);
    void setCueModeActive(bool shouldBeActive);


private:
    AudioThumbnail audioThumb;
    bool fileLoaded; 
    double position;
    double cuePosition = -1.0; //initialise to impossible value

    bool setCueActive = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
