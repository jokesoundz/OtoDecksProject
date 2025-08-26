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
LibraryComponent::LibraryComponent()
{
    setupUI();
    setupTable();
    setupCallbacks();
}

LibraryComponent::~LibraryComponent()
{
}

//==============================================================================

void LibraryComponent::setupUI()
{
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(tableHeaderLabel);
    addAndMakeVisible(importButton);

    tableHeaderLabel.setText("Tracks Library", dontSendNotification);
    tableHeaderLabel.setFont(Font(16.0f, Font::bold));
    tableHeaderLabel.setJustificationType(Justification::centredLeft);

    importButton.setButtonText("Import File(s)...");
}

void LibraryComponent::setupTable()
{
    tableComponent.getHeader().addColumn("del", 1, 30);
    tableComponent.getHeader().addColumn("Track Title", 2, 150);
    tableComponent.getHeader().addColumn("Artist", 3, 150);
    tableComponent.getHeader().addColumn("Length", 4, 70);
    tableComponent.getHeader().addColumn("", 5, 100);
    tableComponent.getHeader().addColumn("", 6, 100);

    tableComponent.setModel(this);
}

void LibraryComponent::setupCallbacks()
{
    importButton.addListener(this);
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

    const int headerArea = 40;

    tableHeaderLabel.setBounds(0, 0, getWidth() - 120, headerArea);
    importButton.setBounds(getWidth() - 120, 0, 120, headerArea);

    tableComponent.setBounds(0, headerArea, getWidth(), getHeight());
}

int LibraryComponent::getNumRows()
{
    //return trackTitles.size();
    return static_cast<int>(trackInfos.size());
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

    if (rowNumber >= trackInfos.size())
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
    //if (columnId == 2)
    //{
    //    if (existingComponentToUpdate == nullptr)
    //    {
    //        TextButton * btn = new TextButton("load to deck 1");
    //        btn->addListener(this);
    //        String id{ std::to_string(rowNumber) };
    //        btn->setComponentID(id);

    //        existingComponentToUpdate = btn;
    //    }
    //}
    //else if (columnId == 3)
    //{
    //    if (existingComponentToUpdate == nullptr)
    //    {
    //        TextButton* btn = new TextButton("load to deck 2");
    //        btn->addListener(this);
    //        String id{ std::to_string(rowNumber) };
    //        btn->setComponentID(id);

    //        existingComponentToUpdate = btn;
    //    }
    //}

    //tableComponent.getHeader().setColumnWidth(1, 25); //disables changes to column 1 (delete checkbox column)


    if (columnId == 1) //delete/ remove track from library column TODO: ticks are sticky, needs fixing.. JUCE is creating new toggle buttons?? multiple times(?)
    {
        auto* checkbox = new ToggleButton();
        checkbox->setClickingTogglesState(true);
        checkbox->setToggleState(false, dontSendNotification);
        return checkbox;

        //ToggleButton* checkbox;

        //if (existingComponentToUpdate == nullptr)
        //{
        //    checkbox = new ToggleButton();
        //    checkbox->setClickingTogglesState(true);
        //    checkbox->onClick = [this, checkbox, rowNum]
        //        {
        //            importedFiles[rowNum].shouldDelete = checkbox->getToggleState();
        //        };
        //}
        //else
        //{
        //    checkbox = static_cast<ToggleButton*>(existingComponentToUpdate);
        //}

        //checkbox->setToggleState(importedFiles[rowNum].shouldDelete, dontSendNotification);
        //return checkbox;

    }

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
                    if (rowNum < trackInfos.size())
                    {
                        if (columnId == 2)
                        {
                            trackInfos[rowNum].setTitle(newText);
                        }
                        if (columnId == 3)
                        {
                            trackInfos[rowNum].setArtist(newText);
                        }
                    }
                };
        }

        if (rowNum < trackInfos.size()) //import trackInfo for each track upon file import
        {
            String currentText;

            if (columnId == 2)
            {
                currentText = trackInfos[rowNum].getTitle();
            }
            else if (columnId == 3)
            {
                currentText = trackInfos[rowNum].getArtist();
            }

            label->setText(currentText, dontSendNotification);
        }

        return label;
    }

    if (columnId == 5 || columnId == 6) //load to deck columns are buttons
    {
        int deckNum = (columnId == 5 ? 1 : 2);
        auto* btn = static_cast<TextButton*>(existingComponentToUpdate);

        if (btn == nullptr)
        {
            btn = new TextButton("Load to Deck " + String(deckNum));
        }

        btn->onClick = [this, rowNum, deckNum]()
        {
            if (onLoadToDeck)
            {
                onLoadToDeck(rowNum, deckNum);
            }
        };
        return btn;
    }
    return existingComponentToUpdate;
}

void LibraryComponent::buttonClicked(Button* button)
{
    if (button == &importButton)
    {
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles |
                                FileBrowserComponent::canSelectMultipleItems;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                //auto chosenFiles = chooser.getResults();
                for (const auto& file : chooser.getResults())
                {
                    if (file.existsAsFile())
                    {
                        //trackTitles.push_back(file.getFileName().toStdString());

                        TrackInfo track(file);
                        trackInfos.push_back(track); //stores track info, e.g. tracktitle, artist, if filename is structured in typical way, using TrackInfo class tokenizer

                        importedFiles.push_back(file); //stores file info for accessing correctly on loadtodeck


                        ////////////////////////////////////////////////////////////////////////////////////////////////////////
                        //TESTING TOKENIZER IN TRACKINFO CLASS TODO: fix everything here to allow title/ artist guess to happen

                        //TrackInfo track(file); //turns file to TrackInfo type
                        //DBG("Imported: " + track.getFileName());
                        //DBG("Parsed Title: " + track.getTitle()); //should give back 'title' using tokeniser
                        //DBG("Parsed Artist: " + track.getArtist()); //should attempt to get 'artist' using tokeniser or return (unknown) if can't guess

                        // these infos should be parsed into table in appropriate columns
                        // later we will also store this info using 'value tree'
                        ////////////////////////////////////////////////////////////////////////////////////////////////////
                }

            }
            
            tableComponent.updateContent();
        });
    }
}

TrackInfo LibraryComponent::getTrackInfoAt(int index) const
{
    return trackInfos[index];
}