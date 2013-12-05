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

#ifndef __ProgramSettings_H_81DEC29C__
#define __ProgramSettings_H_81DEC29C__

#include "JuceHeader.h"

#include "ColorMap.h"

using namespace juce;

class ProgramSettings
{
public:
    
    static bool init();
    static bool reset();
    
    static bool activeMode;
    static bool processing;
    static bool playing;
    
    static Colour iconColor;
    static Colour iconColorOver;
    static Colour iconColorDown;
    static Colour mainBackgroundColor;
    static Colour toolbarBackgroundColor;
    static Colour sourceColors[2];
    static Colour selectedTrackColour;
    static Colour trackColour;
    static Colour textColour;
    static Colour selectedTextColour;
    
    static float iconButtonTimeOutMs;
    
    
    static float brushWidth;
    static float brushStrength;
    static float brushHeight;
    static bool  accumulate;
    
    static float maxMag;
    
    static ColorMap::ColorMapType colorMapType;
    static float spectrogramClipLimit;
    
    static void save();
    static void load();

private:
    static bool isInit;
    
};




#endif  // __ProgramSettings_H_81DEC29C__
