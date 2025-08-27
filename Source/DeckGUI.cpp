/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew & joe

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse
) : player(_player),
    waveformDisplay(formatManagerToUse, cacheToUse),
    timeDisplay(formatManagerToUse, cacheToUse)
{
    setupUI();
    setupSliders();
    setupLabels();
    setupListeners();
    setupTimer();
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

//==============================================================================

void DeckGUI::setupUI()
{
    addAndMakeVisible(playPauseButton);
    addAndMakeVisible(cueButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);

    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);

    addAndMakeVisible(timeDisplay);
    addAndMakeVisible(trackInfoDisplay);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(posSlider);

}

void DeckGUI::setupSliders()
{
    posSlider.setRange(0.0, 1.0);
    
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.7); //ensures slider position (for volume) is set to 70% at start of app

    speedSlider.setRange(0.0, 10.0);
    speedSlider.setValue(1.0); //ensures sliderposition is set to default bpm at start of app
    speedSlider.setSkewFactorFromMidPoint(1.0); //makes tempo slider more intuitive
}

void DeckGUI::setupLabels()
{
    volLabel.setText("Gain", dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);

    speedLabel.setText("Speed adjust", dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);
}

void DeckGUI::setupListeners()
{
    playPauseButton.addListener(this);
    cueButton.addListener(this);
    cueButton.addMouseListener(this, false); //for press and hold interaction

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
}

void DeckGUI::setupTimer()
{
    startTimer(50); //TODO: check this doesn't cause lag, was originally 500
}

//==============================================================================

void DeckGUI::paint (Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                Justification::centred, true);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8;

    const int border = 120;
    const int waveformBorder = 10;

    trackInfoDisplay.setBounds(0, 0, getWidth(), rowH);
    waveformDisplay.setBounds(waveformBorder, rowH, getWidth() - waveformBorder * 2, rowH * 2);
    posSlider.setBounds(0, rowH * 2.5, getWidth(), rowH);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    timeDisplay.setBounds(0, rowH * 3, getWidth(), rowH);

    playPauseButton.setBounds(0, rowH *4, getWidth(), rowH);
    cueButton.setBounds(0, rowH *5, getWidth(), rowH);
    volSlider.setBounds(border, rowH * 6, getWidth() - border, rowH);
    speedSlider.setBounds(border, rowH * 7, getWidth() - border, rowH);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playPauseButton)
    {
        std::cout << "Play/Pause button was clicked " << std::endl;
        player->togglePlayPause();
        updatePlayPauseButton();
    }

    if (button == &cueButton && !cuePreviewActive)
    {
        if (player->isPlaying())
        {
            player->setCuePoint(); //sets new cuepoint while playing

            double cueTime = player->getCurrentPosition();
            double trackLength = player->getLengthInSeconds();
            double cueRelative = cueTime / trackLength;

            waveformDisplay.setCuePointRelative(cueRelative);
        }
        else
        {
            player->jumpToCuePoint(); //jumps to current cue point while not playing
        }
    }
}

void DeckGUI::mouseDown(const MouseEvent& event)
{
    if (event.eventComponent == &cueButton)
    {
        wasPlayingAlready = player->isPlaying();
        if (!wasPlayingAlready)
        {
            cuePreviewActive = true;
            player->jumpToCuePoint();
            player->togglePlayPause();
        }
    }
}

void DeckGUI::mouseUp(const MouseEvent& event)
{
    if (event.eventComponent == &cueButton)
    {
        if (!wasPlayingAlready)
        {
            player->togglePlayPause();
        }

        if (cuePreviewActive)
        {
            player->jumpToCuePoint();
        }
        cuePreviewActive = false;
    }
}


void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

void DeckGUI::sliderDragStarted(Slider* slider)
{
    if (slider == &posSlider)
    {
        wasPlayingAlready = player->isPlaying();
        if (wasPlayingAlready)
        {
            player->togglePlayPause();
        }
    }
}

void DeckGUI::sliderDragEnded(Slider* slider)
{
    if (slider == &posSlider && wasPlayingAlready)
    {
        player->togglePlayPause();
    }
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(URL{File{files[0]}});
    }
}

void DeckGUI::timerCallback()
{
    double current = player->getCurrentPosition();
    double total = player->getLengthInSeconds();

    timeDisplay.updateTime(current, total);
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());

    if (!posSlider.isMouseButtonDown())
    {
        posSlider.setValue(player->getPositionRelative(), juce::dontSendNotification);
    }
    
    updatePlayPauseButton(); //ensures play/pause info is correctly displayed, e.g. when track ends
}

void DeckGUI::updatePlayPauseButton()
{
    if (player->isPlaying())
    {
        playPauseButton.setButtonText("PAUSE");
    }
    else
    {
        playPauseButton.setButtonText("PLAY");
    }
}

void DeckGUI::loadFile(const File& file, const TrackInfo* trackInfo)
{
    player->loadURL(URL{file});
    waveformDisplay.loadURL(URL{file});
    timeDisplay.loadURL(URL{ file });

    //TrackInfo* selectedTrack = &trackLibrary.getTrackInfos()[selectedIndex]
    trackInfoDisplay.setTrackInfo(trackInfo);
    updatePlayPauseButton();
}