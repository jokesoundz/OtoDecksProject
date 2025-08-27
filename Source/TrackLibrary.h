/*
  ==============================================================================

    TrackLibrary.h
    Created: 26 Aug 2025 3:14:23pm
    Author:  PC

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackInfo.h"

class TrackLibrary
{
    public:
        TrackLibrary();

        void addTrack(const TrackInfo& track);
        void removeTrack(const TrackInfo& trackToRemove);

        void saveToDisk(const File& file);
        void loadFromDisk(const File& file);

        const std::vector<TrackInfo>& getTracks() const;
        const std::vector<File>& getFilepaths() const;

    private:
        ValueTree libraryTree;
        std::vector<File> importedFiles;
        std::vector<TrackInfo> trackInfos;


        void rebuildVectorFromTree();
};