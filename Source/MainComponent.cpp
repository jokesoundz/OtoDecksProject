/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (800, 600);

    setupAudio();
    setupUI();
    setupCallbacks();

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================

void MainComponent::setupAudio()
{
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
        && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
            [&](bool granted) { if (granted)  setAudioChannels(2, 2); });
    }
    else
    {
        setAudioChannels(0, 2);
    }
}

void MainComponent::setupUI()
{
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
}

void MainComponent::setupCallbacks()
{
    playlistComponent.onLoadToDeck = [this](int row, int deckNum)
    {
        if (row < 0 || row >= (int)playlistComponent.getNumImportedFiles())
        {
            return;
        }

        auto fileToLoad = playlistComponent.getImportedFileAt(row);
        if (deckNum == 1)
        {
            deckGUI1.loadFile(fileToLoad);
        }

        else if (deckNum == 2)
        {
            deckGUI2.loadFile(fileToLoad);
        }
    };
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth()/2, getHeight()/ 2);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight()/ 2);

    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}

