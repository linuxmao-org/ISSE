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
#ifndef isscore_ISSSource_h
#define isscore_ISSSource_h


#include "ISSDefs.h"


class ISSAudioSource
{
public:

    virtual bool readmono( ISSFloat * buffer,
                           size_t destBufferStartSample,
                           size_t numSamples,
                           size_t readerStartSample)  = 0;
    
    
    //! Returns the number of samples in the audio source
    virtual size_t getLengthInSamples() const = 0 ;
    
    //! Return the number of channels
    virtual size_t getNumChannels() const = 0 ;
    
    // Return the sample rate of the audio source
    virtual ISSFloat getSampleRate()const = 0;
    
    void setChannel( size_t channel_ ){ channel = channel_; }
    size_t getChannel(){return channel;}
    
    virtual ~ISSAudioSource(){};


private:
    
    size_t channel;
    
};



#endif
