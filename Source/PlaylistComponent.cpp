/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 14 Aug 2025 2:06:02pm
    Author:  PC

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	addAndMakeVisible(tableComponent);
    addAndMakeVisible(tableHeaderLabel);
    addAndMakeVisible(importButton);

    
    tableComponent.getHeader().addColumn("del", 1, 30);
    //tableComponent.getHeader().setColumnVisible(1, true);
    !tableComponent.getHeader().resizable;

    tableComponent.getHeader().addColumn("Track", 2, 150);
    tableComponent.getHeader().addColumn("Artist", 3, 150);
    tableComponent.getHeader().addColumn("Length", 4, 70);
    tableComponent.getHeader().addColumn("", 5, 100);
    tableComponent.getHeader().addColumn("", 6, 100);


    tableComponent.setModel(this);

    //trackTitles.push_back("Track 1");
    //trackTitles.push_back("Track 2");
    //trackTitles.push_back("Track 3");
    //trackTitles.push_back("Track 4");

    tableHeaderLabel.setText("Tracks Library", dontSendNotification);
    tableHeaderLabel.setFont(Font(16.0f, Font::bold));
    tableHeaderLabel.setJustificationType(Justification::centredLeft);

    importButton.setButtonText("Import File(s)...");
    importButton.addListener(this);

}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    const int headerArea = 40;

    tableHeaderLabel.setBounds(0, 0, getWidth() - 120, headerArea);
    importButton.setBounds(getWidth() - 120, 0, 120, headerArea);

    tableComponent.setBounds(0, headerArea, getWidth(), getHeight());

}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g,
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

void PlaylistComponent::paintCell(Graphics& g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected)
{
    g.drawText(trackTitles[rowNumber],
        2, 0,
        width - 4, height,
        Justification::centredLeft,
        true);
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
    DBG("PlaylistComponent::cellClicked Row: " << rowNumber << ", Column: " << columnId);
}

Component* PlaylistComponent::refreshComponentForCell(
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


    if (columnId == 1)
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

    if (columnId == 5 || columnId == 6)
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

void PlaylistComponent::buttonClicked(Button* button)
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
                    trackTitles.push_back(file.getFileName().toStdString());
                    importedFiles.push_back(file);
                }

            }
            
            tableComponent.updateContent();
        });
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG("PlaylistComponent::buttonClicked " << trackTitles[id]);
    }
}