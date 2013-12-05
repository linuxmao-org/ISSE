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
#ifndef __SPECRENDERTHREAD_H_84162BF9__
#define __SPECRENDERTHREAD_H_84162BF9__


#include "JuceHeader.h"
#include "ColorMap.h"
#include "EnergyEnvelope.h"

#include <map>

class ISSStft;


class VizRenderThread : public Thread
{
public:

    class Listener
    {
    public:
        virtual ~Listener(){}
        virtual void vizRenderThreadDone( ScopedPointer<Image> & spectrogram  )=0;
    };
    
public:
    
    VizRenderThread ( VizRenderThread::Listener * listener_,
                      File * file_,
                      ISSStft * stft_,
                      std::map<size_t, size_t>  training,
                      Colour c1,
                      Colour c2,
                      float clipLimit,
                      ColorMap::ColorMapType type,
                      int trackNum);

    
    virtual void run ();
    
private:
    
    VizRenderThread::Listener * listener;
    
    File * file;
    ISSStft * stft;
    std::map<size_t, size_t> training;
    Colour c1;
    Colour c2;
    float clipLimit;
    ColorMap::ColorMapType ct;

    
    
    int trackNum;
    Colour color;

};



#endif  // __SPECRENDERTHREAD_H_84162BF9__


