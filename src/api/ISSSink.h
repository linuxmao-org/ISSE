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
#ifndef isscore_ISSSink_h
#define isscore_ISSSink_h

#include "ISSDefs.h"

class ISSAudioSink
{
public:
    
    // Called to write data samples to internal data structure
    virtual bool writemono( ISSFloat * data, size_t numSamples ) = 0;
    
 
    virtual bool write(	const ISSFloat ** channels, size_t numChannels, size_t numSamples )=0;

                                                     
    virtual bool reset()=0;
    
    virtual bool close()=0;
    
    // Destructor
    virtual ~ISSAudioSink(){};
    
};








#endif
