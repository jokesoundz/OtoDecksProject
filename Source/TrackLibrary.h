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
        void removeTrack(int index);

        void saveToDisk();
        void loadFromDisk();

        const std::vector<TrackInfo>& getTracks() const;

    private:
        ValueTree libraryTree;
        std::vector<TrackInfo> trackInfos;

        void rebuildTreeFromVector();
        void rebuildVectorFromTree();
};