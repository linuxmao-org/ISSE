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

#ifndef __TOOLBARSTATE_H_2085A390__
#define __TOOLBARSTATE_H_2085A390__

#include "AppCommandIDs.h"

class ToolbarState
{
public:
    
    enum Selector {
        ArrowSelector       = AppCommandIDs::ArrowSelector,
        BrushSelector       = AppCommandIDs::BrushSelector,
        TimeSelector        = AppCommandIDs::TimeSelector,
        FreqSelector        = AppCommandIDs::FreqSelector,
        BoxSelector         = AppCommandIDs::BoxSelector,
        TrainingSelector    = AppCommandIDs::TrainingSelector,
//        DrawLineSelector    = AppCommandIDs::DrawLineSelector,
        SoundPenSelector    = AppCommandIDs::SoundPenSelector,
    };
    
    static Selector selector;
    static bool autoSolo;
    static bool paint;
    static bool erase;
    static bool isPlaying;
    
};



#endif  // __TOOLBARSTATE_H_2085A390__
