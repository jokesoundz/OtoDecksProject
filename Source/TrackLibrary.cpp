/*
  ==============================================================================

    TrackLibrary.cpp
    Created: 26 Aug 2025 3:14:23pm
    Author:  PC

  ==============================================================================
*/

#include "TrackLibrary.h"

TrackLibrary::TrackLibrary()
    : libraryTree("Library")
{

}

void TrackLibrary::addTrack(const TrackInfo& track)
{
    ValueTree trackNode("Track");
    trackNode.setProperty("title", track.getTitle(), nullptr);
    trackNode.setProperty("artist", track.getArtist(), nullptr);
    trackNode.setProperty("filePath", track.getFile().getFullPathName(), nullptr);

    libraryTree.addChild(trackNode, -1, nullptr);
    trackInfos.push_back(track);
}



void TrackLibrary::saveToDisk(const File& file)
{
    FileOutputStream stream(file);
    if (stream.openedOk())
    {
        libraryTree.writeToStream(stream);
    }
}

void TrackLibrary::loadFromDisk(const File& file)
{
    if (!file.existsAsFile())
    {
        return;
    }

    FileInputStream stream(file);
    if (stream.openedOk())
    {
        auto loadedTree = ValueTree::readFromStream(stream);
        if (loadedTree.isValid() && loadedTree.hasType("Library"))
        {
            libraryTree = loadedTree;
            rebuildVectorFromTree();
        }
    }
}

void TrackLibrary::rebuildVectorFromTree()
{
    trackInfos.clear();

    for (int i = 0; i < libraryTree.getNumChildren(); ++i)
    {
        auto node = libraryTree.getChild(i);
        if (node.hasType("Track"))
        {
            File file(node["filePath"].toString());
            TrackInfo track(file);

            track.setTitle(node["title"].toString());
            track.setArtist(node["artist"].toString());

            trackInfos.push_back(track);
        }
    }
}


// getter for library
const std::vector<TrackInfo>& TrackLibrary::getTracks() const
{
    return trackInfos;
}