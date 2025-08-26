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

void TrackInfo::setTitle(const String& newTitle)
{
    title = newTitle;
}

void TrackInfo::setArtist(const String& newArtist)
{
    artist = newArtist;
}

const File& TrackInfo::getFile() const
{
    return file;
}

void TrackInfo::parseFilename()
{
    String filename = file.getFileNameWithoutExtension();
    StringArray tokens;
    tokens.addTokens(filename, "-.", "");
    tokens.trim();

    String firstToken = tokens[0].trim();
    bool startsWithNumber = firstToken.retainCharacters("0123456789").length() == firstToken.length();

    // typical filenames include:
    // "# - artist - title" || "# - title" || "#. artist - title" || #. title" || "artist - title" || "title"
    // tokenizer attempts to guess 'artist' and 'title' or write 'title' twice but avoid writing numbers
    // NOTE: tokenizer cannot split on "emptyspace" or notice "-" in "artist-name" so some errors will occur on file import
    if (startsWithNumber && tokens.size() >= 2)
    {
        title = tokens[tokens.size() - 1].trim();
        artist = tokens[1];
    }
    else
    {
        title = tokens[tokens.size() - 1].trim();
        artist = tokens[0];
    }

}