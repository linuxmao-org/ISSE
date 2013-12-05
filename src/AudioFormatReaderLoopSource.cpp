/*
 ==============================================================================
 
 This file is part of the JUCE library - "Jules' Utility Class Extensions"
 Copyright 2004-9 by Raw Material Software Ltd.
 
 ------------------------------------------------------------------------------
 
 JUCE can be redistributed and/or modified under the terms of the GNU General
 Public License (Version 2), as published by the Free Software Foundation.
 A copy of the license is included in the JUCE distribution, or can be found
 online at www.gnu.org/licenses.
 
 JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses JUCE, commercial licenses are
 available: visit www.rawmaterialsoftware.com/juce for more information.
 
 ==============================================================================
 */
// Authors: Nicholas J. Bryan, Original Author: Julian Storer
#include "AudioFormatReaderLoopSource.h"

AudioFormatReaderLoopSource::AudioFormatReaderLoopSource (AudioFormatReader* const r,
                                                  const bool deleteReaderWhenThisIsDeleted)
    :   reader (r, deleteReaderWhenThisIsDeleted),
        nextPlayPos (0),
        looping (false),
        startMarker(0),
        stopMarker(0)
{
    jassert (reader != nullptr);
    
    
    nextPlayPos = startMarker;
}

AudioFormatReaderLoopSource::~AudioFormatReaderLoopSource()
{

}



int64 AudioFormatReaderLoopSource::getTotalLength() const
{
    return reader->lengthInSamples;
}

void AudioFormatReaderLoopSource::setLoopMarkers( int64 startMarker_, int64 stopMarker_)
{
    startMarker = startMarker_;
    stopMarker = stopMarker_;
}


void AudioFormatReaderLoopSource::setNextReadPosition (int64 newPosition)
{
    
    if(looping)
    {
        if( (newPosition >= startMarker) && (newPosition <= stopMarker) )
            nextPlayPos = newPosition;
        else
        {
            nextPlayPos = startMarker;
        }
    }
    else
        nextPlayPos = newPosition;
}

void AudioFormatReaderLoopSource::setToLoopStart()
{
    nextPlayPos = startMarker;
}


void AudioFormatReaderLoopSource::setLooping (bool shouldLoop)
{
    looping = shouldLoop;
}


int64 AudioFormatReaderLoopSource::getNextReadPosition() const
{
    int64 endPoint(stopMarker);// = //reader->lengthInSamples;
//    endPoint = stopMarker;
    
    if(looping)
    {
        if(nextPlayPos >= endPoint)
            return nextPlayPos - endPoint +  startMarker;
        else
            return nextPlayPos;
    }
    else
        return nextPlayPos;
}

void AudioFormatReaderLoopSource::prepareToPlay (int /*samplesPerBlockExpected*/, double /*sampleRate*/)
{}


void AudioFormatReaderLoopSource::releaseResources()
{}


void AudioFormatReaderLoopSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    if (info.numSamples > 0)
    {
        int64 startT=0;
        int64 stopT=0;
        
        {
            const ScopedLock sl (callbackLock);
            startT = startMarker;
            stopT  = stopMarker;
            
        }
        
        
        const int64 start = nextPlayPos;

        if (looping)
        {
            int64 endPoint(stopT);// = reader->lengthInSamples;
//            endPoint = stopT;

            int newEnd =   (int) ((start + info.numSamples) );

            int newStart = (int) (start);
            if(newStart >= endPoint)
            {
                newStart = (newStart  % endPoint) + startT;
            }
            
            
            if(newEnd >= endPoint)
            { 
                newEnd = (newEnd % endPoint) + startT;
            }

            // If normal
            if (newEnd > newStart)
            {
                reader->read (info.buffer, info.startSample,
                              newEnd - newStart, newStart, true, true);
            }
            else // If looping around
            {
                const int endSamps = (int) endPoint - newStart;

                // First half
                reader->read (info.buffer, info.startSample,
                              endSamps, newStart, true, true);

                // Second half
                reader->read (info.buffer, info.startSample + endSamps,
                              newEnd-startT, startT, true, true);
            }

            nextPlayPos = newEnd;
        }
        else
        {
            reader->read (info.buffer, info.startSample,
                          info.numSamples, start, true, true);
            nextPlayPos += info.numSamples;
        }
    }
}





