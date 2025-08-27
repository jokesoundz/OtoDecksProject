/*
  ==============================================================================

    LibraryComponent.h
    Created: 14 Aug 2025 2:06:02pm
    Author:  PC

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackInfo.h"
#include "TrackLibrary.h"

//==============================================================================
/*
*/
class LibraryComponent  : public juce::Component,
                          public TableListBoxModel,
                          public Button::Listener
                          //,public TableHeaderComponent::Listener
{
public:
    LibraryComponent(TrackLibrary* libraryPtr);
    ~LibraryComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;
    
    void paintCell(Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;

    void cellClicked(int rowNumber, int columnId, const MouseEvent&) override;

    Component* refreshComponentForCell(int rowNumb, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    void buttonClicked(juce::Button* button) override;

    /** row = which rack, deck = 1 or 2 */
    //std::function<void(int row, int deck)> onLoadToDeck;
    std::function<void(const TrackInfo&, int deck)> onLoadToDeck;

    //int getNumImportedFiles() const noexcept
    //{
    //    return importedFiles.size();
    //}
    int getNumImportedFiles() const noexcept
    {
        return trackLibrary->getFilepaths().size();
    }

    //File getImportedFileAt(int index) const
    //{
    //    jassert(index >= 0 && index < importedFiles.size());
    //    return importedFiles[index];
    //}
    File getImportedFileAt(int index) const
    {
        jassert(index >= 0 && index < trackLibrary->getFilepaths().size());
        return trackLibrary->getFilepaths()[index];
    }

    //TrackInfo getTrackInfoAt(int index) const;
    const TrackInfo& getTrackInfoAt(int index) const;

    void refreshFromLibrary();

    void deleteSelectedRows();
    void importTracks(); //encapsulating button logic

    //void tableColumnsChanged(TableHeaderComponent* tableHeader) override;
    //void tableColumnsResized(TableHeaderComponent* tableHeader) override;
    //void tableSortOrderChanged(TableHeaderComponent* tableHeader) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LibraryComponent)


    void setupUI();
    void setupTable();
    void setupCallbacks();

    FileChooser fChooser{ "Select a file..." ,
                            File::getSpecialLocation(File::userHomeDirectory),
                            "*.mp3;*.wav;*.aiff",
                            true
                            };

	TableListBox tableComponent;
    //std::vector<std::string> trackTitles;
    Label tableHeaderLabel;
    TextButton importButton; //button for importing files from computer
    TextButton deleteButton; //button for deleting files from library (dependent on which are selected by checkbox)

    //std::vector<File> importedFiles;
    //std::vector<TrackInfo> trackInfos;

    TrackLibrary* trackLibrary = nullptr;
};
