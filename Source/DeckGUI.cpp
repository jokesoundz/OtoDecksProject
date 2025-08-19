/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playPauseButton);
    addAndMakeVisible(cueButton);
    addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(speedLabel);
    addAndMakeVisible(volLabel);

    playPauseButton.addListener(this);
    cueButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);


    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);

    volSlider.setValue(0.7); //ensures slider position (for volume) is set to 70% at start of app
    speedSlider.setValue(1.0); //ensures sliderposition is set to default bpm at start of app

    speedSlider.setSkewFactorFromMidPoint(1.0); //makes tempo slider more intuitive

    volLabel.setText("Gain", dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);

    speedLabel.setText("Speed adjust", dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);

    startTimer(20); //TODO: check this doesn't cause lag, was originally 500


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8;

    const int border = 120;
    const int waveformBorder = 10;

    waveformDisplay.setBounds(waveformBorder, 0, getWidth() - waveformBorder * 2, rowH * 2);
    posSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    playPauseButton.setBounds(0, rowH *3, getWidth(), rowH);
    cueButton.setBounds(0, rowH *4, getWidth(), rowH);  
    volSlider.setBounds(border, rowH * 5, getWidth() - border, rowH);
    speedSlider.setBounds(border, rowH * 6, getWidth() - border, rowH);
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playPauseButton)
    {
        std::cout << "Play/Pause button was clicked " << std::endl;
        player->togglePlayPause();
        //player->start();
        updatePlayPauseButton();
    }
    // if (button == &stopButton)
    //{
    //    std::cout << "Stop button was clicked " << std::endl;
    //    player->stop();

    //}
    if (button == &loadButton)
    {
       auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            if (chosenFile.exists()){
                player->loadURL(URL{chooser.getResult()});
                waveformDisplay.loadURL(URL{chooser.getResult()});
            }
        });
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