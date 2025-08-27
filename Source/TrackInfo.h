/*
  ==============================================================================

    TrackInfo.h
    Created: 22 Aug 2025 6:20:58pm
    Author:  PC

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class TrackInfo
{
    public:
        explicit TrackInfo(const File& file);

        const String& getTitle() const;
        const String& getArtist() const;
        
        /** allows user to edit/ update 'Track Title' field in LibraryComponent UI */
        void setTitle(const String& newTitle);
        /** allows user to edit/ update 'Artist' field in LibraryComponent UI */
        void setArtist(const String& newArtist);
        
        const File& getFile() const;

    private:
        File file;
        String title;
        String artist;

        /** tokenizes filename and attempts to guess 'track title' and 'artist' 
        *** ready for using in LibraryComponent and TrackInfoDisplay
        *** and storing in TrackLibrary */
        void parseFilename();
};