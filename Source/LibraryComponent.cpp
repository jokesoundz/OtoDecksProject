/*
  ==============================================================================

    LibraryComponent.cpp
    Created: 14 Aug 2025 2:06:02pm
    Author:  PC

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LibraryComponent.h"

//==============================================================================
LibraryComponent::LibraryComponent(TrackLibrary* libraryPtr)
    : trackLibrary(libraryPtr)
{
    setupAssets();
    setupUI();
    setupTable();
    setupCallbacks();
}

LibraryComponent::~LibraryComponent()
{
}

//==============================================================================

void LibraryComponent::setupAssets()
{
    deleteIconNormal = ImageFileFormat::loadFrom(BinaryData::deletebinline_png, BinaryData::deletebinline_pngSize);
    deleteIconHoverPressed = ImageFileFormat::loadFrom(BinaryData::deletebinfill_png, BinaryData::deletebinfill_pngSize);
}

void LibraryComponent::setupUI()
{
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(tableHeaderLabel);
    addAndMakeVisible(importButton);
    addAndMakeVisible(deleteButton);

    tableHeaderLabel.setText("Tracks Library", dontSendNotification);
    tableHeaderLabel.setFont(Font(16.0f, Font::bold));
    tableHeaderLabel.setJustificationType(Justification::centredLeft);

    importButton.setButtonText("Import File(s)...");
    deleteButton.setImages(true, true, true,
                            deleteIconNormal, 1.0f, Colours::darkslategrey,
                            deleteIconHoverPressed, 0.7f, Colours::darkslategrey,
                            deleteIconHoverPressed, 0.5f, Colours::darkslategrey,
                            0.0f);
}

void LibraryComponent::setupTable()
{
    tableComponent.getHeader().addColumn("", 1, 30); //ImageButton sits on top of this space
    tableComponent.getHeader().addColumn("Track Title", 2, 150);
    tableComponent.getHeader().addColumn("Artist", 3, 150);
    tableComponent.getHeader().addColumn("...", 4, 70); //could be used as track length column
    tableComponent.getHeader().addColumn("", 5, 100);
    tableComponent.getHeader().addColumn("", 6, 100);

    tableComponent.setModel(this);
}

void LibraryComponent::setupCallbacks()
{
    importButton.addListener(this);
    //tableComponent.getHeader().addListener(this);
    //deleteButton.onClick = [this]()
    //{
    //    deleteSelectedRows();
    //};
    deleteButton.addListener(this);
}

//==============================================================================


void LibraryComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("LibraryComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void LibraryComponent::resized()
{

    const int headerHeight = 40;

    tableHeaderLabel.setBounds(0, 0, getWidth() - 120, headerHeight);
    importButton.setBounds(getWidth() - 120, 0, 120, headerHeight);
    deleteButton.setBounds(0, headerHeight, 30, 28);

    tableComponent.setBounds(0, headerHeight, getWidth(), getHeight() - headerHeight);
}

int LibraryComponent::getNumRows()
{
    //return trackTitles.size();
    //return static_cast<int>(trackInfos.size());
    return static_cast<int>(trackLibrary->getTracks().size());
}

void LibraryComponent::paintRowBackground(Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    // just highlight selected rows
    if (rowIsSelected)
    {
        g.fillAll(Colours::greenyellow);
    }
    else {
        g.fillAll(Colours::darkgrey);
    }
}

void LibraryComponent::paintCell(Graphics& g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected)
{

    if (rowNumber >= trackLibrary->getTracks().size())
    {
        return;
    }

    if (columnId == 4) //track length column is uneditable
    {
        g.setColour(Colours::white);
        g.setFont(14.0f);
        g.drawText("...", 2, 0, width - 4, height, Justification::centredLeft, true);
    }
}

void LibraryComponent::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
    DBG("LibraryComponent::cellClicked Row: " << rowNumber << ", Column: " << columnId);
}

Component* LibraryComponent::refreshComponentForCell(
    int rowNum,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{

    if (columnId == 1) //delete/ remove track from library column
    {
        ToggleButton* checkbox = dynamic_cast<ToggleButton*>(existingComponentToUpdate);
        if (checkbox == nullptr)
        {
            checkbox = new ToggleButton();
            checkbox->setClickingTogglesState(true);
            checkbox->onClick = [this, checkbox, rowNum]()
            {
                if (rowNum < trackLibrary->getTracks().size())
                {
                    trackLibrary->getTracksMutable()[rowNum].setShouldDelete(checkbox->getToggleState());
                }
            };
        }
        checkbox->setToggleState(trackLibrary->getTracksMutable()[rowNum].getShouldDelete(), dontSendNotification);
        return checkbox;
    }

    //if (columnId == 1) //delete/ remove track from library column
    //{
    //    ToggleButton* checkbox = dynamic_cast<ToggleButton*>(existingComponentToUpdate);
    //    if (checkbox == nullptr)
    //    {
    //        checkbox = new ToggleButton();
    //        checkbox->setClickingTogglesState(true);
    //        checkbox->setComponentID("deleteCheckbox");
    //        addAndMakeVisible(checkbox);
    //    }

    //    checkbox->onClick = [this, checkbox, rowNum]()
    //    {
    //        if (rowNum < trackLibrary->getTracks().size())
    //        {
    //            trackLibrary->getTracksMutable()[rowNum].setShouldDelete(checkbox->getToggleState());
    //        }
    //    };

    //    checkbox->setToggleState(trackLibrary->getTracksMutable()[rowNum].getShouldDelete(), dontSendNotification);
    //    return checkbox;
    //}

    if (columnId == 2 || columnId == 3) //track title and artist name columns are editable
    {
        Label* label = dynamic_cast<Label*>(existingComponentToUpdate);
        if (label == nullptr)
        {
            label = new Label();
            label->setEditable(true, true, false);
            label->setColour(Label::textColourId, Colours::white);
            label->setFont(Font(14.0f));

            label->onEditorHide = [this, label, rowNum, columnId]() //allows user to edit field but revert to previous text if cancel edit
            {
                auto newText = label->getText();
                if (rowNum < trackLibrary->getTracksMutable().size())
                {
                    auto& track = trackLibrary->getTracksMutable()[rowNum];

                    if (columnId == 2)
                    {
                        trackLibrary->updateTrackTitle(track, newText);
                    }
                    else if (columnId == 3)
                    {
                        trackLibrary->updateTrackArtist(track, newText);
                    }
                }
            };
        }

        if (rowNum < trackLibrary->getTracksMutable().size()) //import trackInfo for each track upon file import
        {
            String currentText;

            if (columnId == 2)
            {
                currentText = trackLibrary->getTracksMutable()[rowNum].getTitle();
            }
            else if (columnId == 3)
            {
                currentText = trackLibrary->getTracksMutable()[rowNum].getArtist();
            }

            label->setText(currentText, dontSendNotification);
        }

        return label;
    }

    if (columnId == 5 || columnId == 6) //load to deck columns are buttons
    {
        int deckNum = (columnId == 5 ? 1 : 2);
        
        TextButton* btn = dynamic_cast<TextButton*>(existingComponentToUpdate);

        if (btn == nullptr || btn->getComponentID() != "deckButton" + String(columnId))
        {
            btn = new TextButton("Load to Deck " + String(deckNum));
            btn->setComponentID("deckButton" + String(columnId));

            btn->onClick = [this, rowNum, deckNum]()
            {
                if (rowNum < trackLibrary->getTracksMutable().size())
                {
                    TrackInfo* track = &trackLibrary->getTracksMutable()[rowNum];
                    if (onLoadToDeck)
                    {
                        onLoadToDeck(track, deckNum);
                    }
                }
            };
        }
        return btn;

    }
    return existingComponentToUpdate;
}

void LibraryComponent::buttonClicked(Button* button)
{
    if (button == &importButton)
    {
        importTracks();
    }
    else if (button == &deleteButton)
    {
        deleteSelectedRows();
    }
}

const TrackInfo& LibraryComponent::getTrackInfoAt(int index) const
{
    //return trackInfos[index];
    return trackLibrary->getTracks().at(index);
}

void LibraryComponent::importTracks()
{
    auto fileChooserFlags = FileBrowserComponent::canSelectFiles |
                            FileBrowserComponent::canSelectMultipleItems;
    fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
    {
        for (const auto& file : chooser.getResults())
        {
            if (file.existsAsFile())
            {
                TrackInfo track(file);
                trackLibrary->addTrack(track);
            }
        }
        refreshFromLibrary();
    });
}


void LibraryComponent::deleteSelectedRows()
{
    auto& tracks = trackLibrary->getTracksMutable();
    for (int i = tracks.size() - 1; i >= 0; --i) //go through backwards to ensure vector erased correctly & completely
    {
        if (tracks[i].getShouldDelete())
        {
            trackLibrary->removeTrack(tracks[i]);
        }
    }

    refreshFromLibrary();
}

void LibraryComponent::refreshFromLibrary()
{
    tableComponent.updateContent();
    tableComponent.repaint();
}