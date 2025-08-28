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
    volSlider.setSliderStyle(Slider::LinearVertical);
    volSlider.setBounds(getLocalBounds().reduced(10));
    volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    speedSlider.setRange(0.1, 10.0);
    speedSlider.setValue(1.0); //ensures sliderposition is set to default bpm at start of app
    speedSlider.setSkewFactorFromMidPoint(1.0); //makes tempo slider more intuitive
    speedSlider.setSliderStyle(Slider::LinearVertical);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    speedSlider.setColour(Slider::trackColourId, Colours::transparentBlack); //remove the 'track' i.e. amount of slider value information
}

void DeckGUI::setupLabels()
{
    volLabel.setText("Gain", dontSendNotification);
    volLabel.setJustificationType(Justification::centred);
    //volLabel.attachToComponent(&volSlider, true);

    speedLabel.setText("Tempo", dontSendNotification); //even though not technically a tempo adjust, this word fits better
    speedLabel.setJustificationType(Justification::centred);
    //speedLabel.attachToComponent(&speedSlider, true);
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

    //////////////////////////////////////////////////////////////////////////////
    //component border
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);


    //////////////////////////////////////////////////////////////////////////////////////
    //draw ticks around vol 'gain' slider
    auto minV = volSlider.getMinimum();
    auto maxV = volSlider.getMaximum();
    int numTicksV = 10;

    int tickLengthV = 5;
    double stepV = (maxV - minV) / (numTicksV - 1);

    g.setColour(Colours::dimgrey);

    for (int i = 0; i < numTicksV; ++i)
    {
        double value = minV + i * stepV;
        int y = volSlider.getY() + volSlider.getPositionOfValue(value);

        //ticks on left
        g.drawLine((float)(volSlider.getX() + tickLengthV*2), (float)y,
                   (float)(volSlider.getX() + tickLengthV*3), (float)y, 1.0f);

        //ticks on right
        g.drawLine((float)(volSlider.getRight() - tickLengthV*3), (float)y,
                   (float)(volSlider.getRight() - tickLengthV*2), (float)y, 1.0f);
    }
    
    ////////////////////////////////////////////////////////////////////////////////////
    //draw ticks around speed 'tempo' slider
    int tickLengthS = 10;

    int midY = speedSlider.getY() + speedSlider.getHeight() / 2;

    int leftX1 = speedSlider.getX() + tickLengthS * 2;
    int leftX2 = speedSlider.getX() + tickLengthS * 3;

    int rightX1 = speedSlider.getRight() - tickLengthS * 2;
    int rightX2 = speedSlider.getRight() - tickLengthS * 3;

    g.setColour(Colours::dimgrey);

    //tick on left
    g.drawLine(leftX1, midY, leftX2, midY, 1.0f);
    
    //tick on right
    g.drawLine(rightX1, midY, rightX2, midY, 1.0f);

    int topY = speedSlider.getY() + 15;
    int bottomY = speedSlider.getBottom() - 30;

    g.setColour(Colours::ghostwhite);

    g.drawText("+", speedSlider.getX() + 5, topY - 10, 20, 20, Justification::centred);
    g.drawText("-", speedSlider.getX() + 5, bottomY + 5, 20, 20, Justification::centred);

}

//NEW LAYOUT
void DeckGUI::resized()
{
    double rowH = getHeight() / 8;
    double colW = getWidth() / 6;

    const int border = 120;
    const int waveformBorder = 10;

    //Display TRACK TITLE & ARTIST above WAVEFORM above TRACK POSITION/SCRUBBER above TIMEDISPLAY
    trackInfoDisplay.setBounds(0, 0, getWidth(), rowH);
    waveformDisplay.setBounds(waveformBorder, rowH, getWidth() - waveformBorder * 2, rowH * 2);
    posSlider.setBounds(-1, rowH * 2.5, getWidth() + 2, rowH);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    timeDisplay.setBounds(0, rowH * 3, getWidth(), rowH);

    //ORIGINAL BUTTON/ DIAL LAYOUT
    //playPauseButton.setBounds(0, rowH * 4, getWidth(), rowH);
    //cueButton.setBounds(0, rowH * 5, getWidth(), rowH);
    //volSlider.setBounds(border, rowH * 6, getWidth() - border, rowH);
    //speedSlider.setBounds(border, rowH * 7, getWidth() - border, rowH);

    //NEW BUTTON/ DIAL LAYOUT
    cueButton.setBounds(0, rowH * 4, colW*2, rowH * 2);
    playPauseButton.setBounds(0, rowH * 6, colW*2, rowH*2);

    volSlider.setBounds(colW * 5, rowH * 4, colW, rowH * 3.5);
    volLabel.setBounds(colW * 5, rowH * 7.5, colW, rowH * 0.5);

    speedSlider.setBounds(colW * 3, rowH * 4, colW, rowH * 3.5);
    speedLabel.setBounds(colW * 3, rowH * 7.5, colW, rowH * 0.5);
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

//bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
//{
//    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
//    return true; 
//}

//void DeckGUI::filesDropped (const StringArray &files, int x, int y)
//{
//    std::cout << "DeckGUI::filesDropped" << std::endl;
//    if (files.size() == 1)
//    {
//        player->loadURL(URL{File{files[0]}});
//    }
//}

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