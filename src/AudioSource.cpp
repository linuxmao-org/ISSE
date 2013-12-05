/*
 Copyright 2013 Stanford University
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

#include "AudioSource.h"

 
    
FileAudioSource::FileAudioSource( File file_) : file(file_)
{
//    std::cout << file.getFullPathName() << std::endl;
    WavAudioFormat wavFormat;
    finput = new FileInputStream( file );
    freader = wavFormat.createReaderFor ( finput, true );
}

size_t FileAudioSource::getNumChannels() const
{
    return freader->numChannels;
}

bool FileAudioSource::readmono(ISSFloat * buffer,
                      size_t destBufferStartSample,
                      size_t numSamples,
                      size_t readerStartSample )
{
    if(freader)
    {
        float* channelData[2];
        channelData[0] = buffer;
        channelData[1] = nullptr;
        
        AudioSampleBuffer buff(	channelData, 1, numSamples);
        
        bool 	useReaderLeftChan = true;
        bool 	useReaderRightChan = false;
        
        if(getChannel()==1)
        {
         	useReaderLeftChan = false;
         	useReaderRightChan = true;
        }
        
        freader->read( &buff, destBufferStartSample, numSamples, readerStartSample, useReaderLeftChan, useReaderRightChan );
        
        return true;
    }
    return false;
}

//! Returns the number of samples in the audio source
size_t FileAudioSource::getLengthInSamples() const
{
    if(freader)
        return freader->lengthInSamples;
    else
        return 0;
}
    
    // Return the sample rate of the audio source
float FileAudioSource::getSampleRate()const
{
        
    if(freader)
        return freader->sampleRate;
    else
        return 0;        
}
    
    
FileAudioSource::~FileAudioSource()
{};


     