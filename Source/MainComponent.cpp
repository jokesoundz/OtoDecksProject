/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : libraryComponent(&trackLibrary)
{
    setSize (800, 600);

    setupAudio();
    setupUI();
    setupCallbacks();

    formatManager.registerBasicFormats();

    loadLibraryFromDisk();
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
    addAndMakeVisible(libraryComponent);
}

void MainComponent::setupCallbacks()
{
    //libraryComponent.onLoadToDeck = [this](int row, int deckNum)
    libraryComponent.onLoadToDeck = [this](const TrackInfo& track, int deckNum)
    {
        //if (row < 0 || row >= (int)libraryComponent.getNumImportedFiles())
        //{
        //    return;
        //}

        //auto fileToLoad = libraryComponent.getImportedFileAt(row);
        //auto trackInfo = libraryComponent.getTrackInfoAt(row);
        //if (deckNum == 1)
        //{
        //    deckGUI1.loadFile(fileToLoad, trackInfo);
        //}

        //else if (deckNum == 2)
        //{
        //    deckGUI2.loadFile(fileToLoad, trackInfo);
        //}

        const auto& fileToLoad = track.getFile();

        if (deckNum == 1)
        {
            deckGUI1.loadFile(fileToLoad, track);
        }

        else if (deckNum == 2)
        {
            deckGUI2.loadFile(fileToLoad, track);
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

    libraryComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}

void MainComponent::loadLibraryFromDisk()
{
    File saveFile = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("trackLibrary.xml");

    trackLibrary.loadFromDisk(saveFile);

    //DBG("MainComponent::loadLibraryFromDisk loading from: " << saveFile.getFullPathName());

    libraryComponent.refreshFromLibrary();
}

void MainComponent::saveLibraryToDisk()
{

    File saveFile = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("trackLibrary.xml");

    if (saveFile.existsAsFile())
    {
        saveFile.deleteFile();
    }

    //DBG("MainComponent::saveLibraryToDisk saving to: " << saveFile.getFullPathName());

    trackLibrary.saveToDisk(saveFile);
}