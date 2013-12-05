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

#ifndef __AUDIOSOURCE_H_286134A7__
#define __AUDIOSOURCE_H_286134A7__

#include "JuceHeader.h"
#include "api/ISSDefs.h"
#include "api/ISSSource.h"


class FileAudioSource : public ISSAudioSource
{
public:
    
    FileAudioSource( File file );
    
    virtual bool readmono(ISSFloat * buffer,
                          size_t destBufferStartSample,
                          size_t numSamples,
                          size_t readerStartSample );
    
    //! Returns the number of samples in the audio source
    virtual size_t getLengthInSamples() const;
    
    //! Return the sample rate of the audio source
    virtual ISSFloat getSampleRate()const;
    
    //! Return the number channels (mono, stereo, etc.)
    virtual size_t getNumChannels() const;
    
    virtual ~FileAudioSource();
    
private:
    
    InputStream * finput; // Do not delete
    ScopedPointer<AudioFormatReader> freader;
    File file;
    
};





#endif  // __AUDIOSOURCE_H_286134A7__
