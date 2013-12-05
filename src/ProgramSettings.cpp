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
#include "ProgramSettings.h"
#include "ColorMap.h"
#include "Main.h"


//bool ProgramSettings::isInit = init();
Colour ProgramSettings::iconColor(Colours::white);
Colour ProgramSettings::iconColorOver(Colours::lightblue);
Colour ProgramSettings::iconColorDown(Colours::lightblue);
Colour ProgramSettings::mainBackgroundColor(Colours::grey.withBrightness(.3));
Colour ProgramSettings::toolbarBackgroundColor(Colours::grey.withBrightness(.3));
Colour ProgramSettings::selectedTrackColour(Colours::grey.withAlpha(.1f));
Colour ProgramSettings::trackColour(Colours::grey.withBrightness(.3));
Colour ProgramSettings::textColour(Colours::white);
Colour ProgramSettings::selectedTextColour(Colours::white);
Colour ProgramSettings::sourceColors[2];
float ProgramSettings::iconButtonTimeOutMs(500);
float ProgramSettings::brushWidth(50);
float ProgramSettings::brushStrength(.5);
float ProgramSettings::brushHeight(5);
bool  ProgramSettings::accumulate(true);
float ProgramSettings::spectrogramClipLimit(-60);
float ProgramSettings::maxMag(0);
bool ProgramSettings::activeMode(false);
bool ProgramSettings::processing(false);
bool ProgramSettings::playing(false);

ColorMap::ColorMapType ProgramSettings::colorMapType(ColorMap::purpleColorMap);


bool ProgramSettings::init()
{
    reset();
    return true;
    
}

void ProgramSettings::save()
{
    
    XmlElement xml("programsettings");
    
    xml.setAttribute("colormap", colorMapType);
    xml.setAttribute("cliplimit", spectrogramClipLimit);
    
    
    ApplicationProperties* properties(MAIN_APP->appProperties);
    properties->getUserSettings()->setValue ("programsettings", &xml);
    properties->getUserSettings()->saveIfNeeded();
    
}
void ProgramSettings::load()
{
    ApplicationProperties* properties(MAIN_APP->appProperties);
    ScopedPointer<XmlElement> savedState (properties->getUserSettings()->getXmlValue ("programsettings"));
    
    if(savedState)
    {
        colorMapType = (ColorMap::ColorMapType)savedState->getIntAttribute("colormap");
        spectrogramClipLimit = savedState->getDoubleAttribute("cliplimit");
    }
    
//    properties->getUserSettings()->getValue ("programsettings");
    
}

bool ProgramSettings::reset()
{
    // Colour
    iconColor = Colours::white;
    iconColorOver = Colours::lightskyblue;
    iconColorDown = Colours::lightskyblue.withBrightness(.6);
    mainBackgroundColor = Colours::grey.withBrightness(.3);
    toolbarBackgroundColor = Colours::grey.withBrightness(.3);
    
    sourceColors[0] = Colours::blue;
    sourceColors[1] = Colours::red;
 
    selectedTrackColour = Colours::lightgrey.withAlpha(.3f);
    trackColour = Colours::grey.withBrightness(.3);
    textColour = Colours::white;
    selectedTextColour = Colours::white;
    
    // Timing
    iconButtonTimeOutMs = 500;
    
    // Paint Settings
    brushWidth = 50;
    brushStrength = .75;
    brushHeight = 5;
    accumulate = true;
    
    activeMode  = false;
    processing  = false;
    playing = false;
    
    
    
    maxMag = 0;
    
    
    
    
    
    ProgramSettings::spectrogramClipLimit = -60;
    colorMapType = ColorMap::purpleColorMap;
    
//    load();
    
    return true;

}