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

void TrackLibrary::removeTrack(const TrackInfo& trackToRemove)
{
    for (int i = 0; i < trackInfos.size(); ++i)
    {
        if (trackInfos[i].getFile().getFullPathName() == trackToRemove.getFile().getFullPathName())
        {
            trackInfos.erase(trackInfos.begin() + i); //erase from trackInfos vec

            if (i < libraryTree.getNumChildren())
            {
                libraryTree.removeChild(i, nullptr); //remove node from value tree
            }

            if (i < importedFiles.size())
            {
                importedFiles.erase(importedFiles.begin() + i); //erase from files vec to avoid sync erros
            }

            break;
        }
    }
}

//void TrackLibrary::updateTrackTitle(const TrackInfo& track, const String& newTitle)
void TrackLibrary::updateTrackTitle(TrackInfo& track, const String& newTitle)
{
    for (int i = 0; i < trackInfos.size(); ++i)
    {
        if (trackInfos[i].getFile() == track.getFile())
        {
            trackInfos[i].setTitle(newTitle);

            if (i < libraryTree.getNumChildren())
            {
                auto node = libraryTree.getChild(i);
                node.setProperty("title", newTitle, nullptr);
            }
            break;
        }
    }
}

void TrackLibrary::updateTrackArtist(TrackInfo& track, const String& newArtist)
{
    for (int i = 0; i < trackInfos.size(); ++i)
    {
        if (trackInfos[i].getFile() == track.getFile())
        {
            trackInfos[i].setArtist(newArtist);

            if (i < libraryTree.getNumChildren())
            {
                auto node = libraryTree.getChild(i);
                node.setProperty("artist", newArtist, nullptr);
            }
            break;
        }
    }
}





void TrackLibrary::saveToDisk(const File& file)
{
    Logger::outputDebugString("TrackLibrary::saveToDisk called");
    Logger::outputDebugString("Saving to: " + file.getFullPathName());
    Logger::outputDebugString("Num tracks being saved: " + String(libraryTree.getNumChildren()));

    FileOutputStream stream(file);
    if (stream.openedOk())
    {
        libraryTree.writeToStream(stream);
    }
    else
    {
        Logger::outputDebugString("Failed to open file for writing");
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

        DBG("TrackLibrary::LoadFromDisk called");
        DBG("File exists: " + file.existsAsFile());
        DBG("Loaded tree is valid: " + loadedTree.isValid());
        DBG("Loaded tree type: " << loadedTree.getType().toString());
        DBG("Num children: " << loadedTree.getNumChildren());


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
            importedFiles.push_back(file);

            TrackInfo track(file);

            track.setTitle(node["title"].toString());
            track.setArtist(node["artist"].toString());

            trackInfos.push_back(track);
        }
    }
}


// getter for library (that allows edit)
std::vector<TrackInfo>& TrackLibrary::getTracksMutable()
{
    return trackInfos;
}

// getter for library that preserves 
const std::vector<TrackInfo>& TrackLibrary::getTracks() const
{
    return trackInfos;
}

const std::vector<File>& TrackLibrary::getFilepaths() const
{
    return importedFiles;
}