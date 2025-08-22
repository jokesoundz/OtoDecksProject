/*
  ==============================================================================

    TrackInfo.cpp
    Created: 22 Aug 2025 6:20:58pm
    Author:  PC

  ==============================================================================
*/

#include "TrackInfo.h"

TrackInfo::TrackInfo(const File& f)
    : file(f)
{
    parseFilename();
}


const String& TrackInfo::getTitle() const
{
    return title;
}

const String& TrackInfo::getArtist() const
{
    return artist;
}

const File& TrackInfo::getFile() const
{
    return file;
}

void TrackInfo::parseFilename()
{
    String filename = file.getFileNameWithoutExtension();
    StringArray tokens;
    tokens.addTokens(filename, "-", "");

    if (tokens.size() >= 2)
    {
        title = tokens[tokens.size() - 1].trim();
        artist = tokens[0].trim();
    }
    else
    {
        title = filename;
        artist = "(unknown)";
    }
}