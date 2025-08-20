/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "TimeDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    void updatePlayPauseButton();

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 
    void loadFile(const File& file);

    void timerCallback() override; 

private:
    //juce::FileChooser fChooser{"Select a file..."};

    TextButton playPauseButton{"PLAY"}; //initialise to PLAY
    //TextButton stopButton{"STOP"};
    TextButton cueButton{ "CUE" }; //TODO: various 'cue' functionalty
    TextButton loadButton{"LOAD"}; //will load highlighted track from library to relevant deck component
  
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;

    Label volLabel; //volume-gain
    Label speedLabel; //speed-tempo-adjust

    WaveformDisplay waveformDisplay;

    TimeDisplay timeDisplay;

    DJAudioPlayer* player; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
