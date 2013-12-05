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
// Authors: Nicholas J. Bryan
#ifndef __TIMELINEVIEW_H_63C221__
#define __TIMELINEVIEW_H_63C221__

#include "JuceHeader.h"
 

class MultiPaintView;
 

class TimelineView : public Component
{
    
public:
    
    TimelineView(MultiPaintView * multiPaintView);
    
    virtual void paint( Graphics & g);
    
    virtual void resized();
    
private:
    
    
    MultiPaintView * multiPaintView;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimelineView)
    
};

#endif  // __TIMELINEVIEW_H_63C221__
