/*
  ==============================================================================

    TrackLibrary.cpp
    Created: 26 Aug 2025 3:14:23pm
    Author:  PC

  ==============================================================================
*/

#include "TrackLibrary.h"

TrackLibrary::TrackLibrary()
{

}

void TrackLibrary::addTrack(const TrackInfo& track)
{

}

void TrackLibrary::removeTrack(int index)
{

}

void TrackLibrary::saveToDisk()
{

}

void TrackLibrary::loadFromDisk()
{

}





void TrackLibrary::rebuildTreeFromVector()
{
    
}

void TrackLibrary::rebuildVectorFromTree()
{

}


// getter for library

const std::vector<TrackInfo>& TrackLibrary::getTracks() const
{
    return trackInfos;
}