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

//void TrackLibrary::saveToDisk(const File& file)
//{
//    Logger::outputDebugString("TrackLibrary::saveToDisk called");
//    Logger::outputDebugString("Saving to: " + file.getFullPathName());
//    Logger::outputDebugString("Num tracks being saved: " + String(libraryTree.getNumChildren()));
//
//    if (auto xml = libraryTree.createXml())
//    {
//        xml->writeToFile(file, {});
//    }
//    else
//    {
//        Logger::outputDebugString("Failed to open file for writing");
//    }
//}

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

//void TrackLibrary::loadFromDisk(const File& file)
//{
//    Logger::outputDebugString("TrackLibrary::loadFromDisk called");
//    Logger::outputDebugString("Loading from: " + file.getFullPathName());
//
//    if (!file.existsAsFile())
//    {
//        Logger::outputDebugString("File does not exist");
//        return;
//    }
//
//    auto xml = XmlDocument(file).getDocumentElement();
//    if (xml != nullptr)
//    {
//        auto tree = ValueTree::fromXml(*xml);
//        Logger::outputDebugString("Loaded tree from: " + tree.getType().toString());
//        Logger::outputDebugString("Num children: " + String(tree.getNumChildren()));
//
//        if (tree.isValid() && tree.hasType("Library"))
//        {
//            libraryTree = tree;
//            rebuildVectorFromTree();
//        }
//    }
//}

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