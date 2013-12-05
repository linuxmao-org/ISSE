/*
 Copyright 2013 Adobe Systems Incorporated
 Distributed under the terms of the Gnu General Public License
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// Authors: Nicholas J. Bryan

#ifndef __AUDIOSINKL_H_1FF4FC61__
#define __AUDIOSINKL_H_1FF4FC61__


#include "JuceHeader.h"
#include "api/ISSDefs.h"
#include "api/ISSSink.h"


class FileAudioSink : public ISSAudioSink
{
    
public:
    
    FileAudioSink( const File & file_,
                   double sampleRateToUse,
                   size_t numChannels,
                   size_t maxSamples_ );
    
    virtual bool close();
    virtual bool reset();
    virtual bool writemono( ISSFloat * data, size_t numSamples );
    virtual bool write(	const ISSFloat ** 	channels, size_t numChannels, size_t numSamples );
    
    virtual ~FileAudioSink();
    
private:
    
    void createWriter();

    File tempFile;
    ScopedPointer<AudioFormatWriter> fwriter;
    FileOutputStream * foutput; // Do not delete (will be deleted by fwriter
    double sampleRate;
    
    size_t numChannels;
    size_t maxSamples;
    size_t sampleIndex;
    size_t resetPosition;
    
};




#endif  // __AUDIOSINKL_H_1FF4FC61__
