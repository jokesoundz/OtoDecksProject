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
        
        void setTitle(const String& newTitle);
        void setArtist(const String& newArtist);
        
        const File& getFile() const;



    private:
        File file;
        String title;
        String artist;

        void parseFilename();

};