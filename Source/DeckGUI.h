/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew & joe

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "TimeDisplay.h"
#include "TrackInfoDisplay.h"
#include "IndicatorLight.h"

//==============================================================================
/*
*/
class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public MouseListener,
                //public FileDragAndDropTarget, 
                public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
            AudioFormatManager& formatManagerToUse,
            AudioThumbnailCache& cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button * button) override;

    void mouseDown(const MouseEvent& event) override;
    void mouseUp(const MouseEvent& event) override;

    void updatePlayPauseButton();

    /** implement Slider::Listener */
    void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted(Slider* slider) override;
    void sliderDragEnded(Slider* slider) override;

    //bool isInterestedInFileDrag (const StringArray &files) override;
    //void filesDropped (const StringArray &files, int x, int y) override; 
    void loadFile(const File& file, const TrackInfo* trackInfo);

    void timerCallback() override; 


private:
    //juce::FileChooser fChooser{"Select a file..."};

    void setupUI();
    void setupSliders();
    void setupLabels();
    void setupListeners();
    void setupTimer();

    TextButton playPauseButton{"PLAY"}; //initialise to PLAY
    TextButton cueButton{ "CUE" };
    TextButton setCueButton{ "SET CUE" };
    IndicatorLight setCueIndicator;

    bool cuePreviewActive = false; //flag for cue button mousepressed
    //bool suppressCueClick = false; //flag to prevent mousepressed running when mouse is clicked once during setCue mode

    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;
    bool wasPlayingAlready = false; //checks state of player ONLY - and updates accordingly

    Label volLabel; //volume-gain
    Label speedLabel; //speed-tempo-adjust

    WaveformDisplay waveformDisplay;
    TimeDisplay timeDisplay;
    TrackInfoDisplay trackInfoDisplay;

    DJAudioPlayer* player; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
