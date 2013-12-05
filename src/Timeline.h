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
#ifndef __TIMELINE_H_6CE3EB3B__
#define __TIMELINE_H_6CE3EB3B__


#include "JuceHeader.h"


class Timeline : public Component
{
public:
    
    enum Event{ MouseDown, MouseDrag, MouseUp, LoopMarkersChanged };
    
    class Listener
    {
        public:
        virtual void timelineEvent(Timeline::Event msg )=0;
        virtual ~Listener(){}
    };
    
public:
    
    Timeline( Timeline::Listener * l ) ;
    
    virtual ~Timeline();
    
    virtual void paint (Graphics &g);

    void  setRange( Range<double> currentRange,  Range<double> rangeLimits );
    
    void  setLoopMarkers(float startInSec, float stopInSec);
    void  setCursor(float cursorInSec);
    float getStartInSec();
    float getStopInSec();
    float getCursorInSec();
    float getStartInPx();
    float getStopInPx();
    float getCursorInPx();
    
    static int border;
    
private:
    
    float convertPxToSec(int x);
    int   convertSecToPx( float seconds );
    
    int verifyLoopMarker( int xPos );
    void logMouseEvent( Event event, float xInSec );
    
    virtual void resized();
    virtual void mouseDown (const MouseEvent& e);
    virtual void mouseMove(const MouseEvent& e);
    virtual void mouseDrag (const MouseEvent& e);
    virtual void mouseUp(const MouseEvent & e);
    
    void mouseDownHelper (float xInSec);
    void mouseDragHelper (float xInSec);
    void mouseUpHelper(float xInSec);
    
    void processMouse(float xPtInSec, Event event);
    
    Range<double> currentRange, rangeLimits;
    bool movingStartMarker, movingStopMarker;
    int width;
    int height;
    static int markerWidth;
    
    float startInSec;
    float stopInSec;
    float currentCursorInSec;

    ListenerList <Timeline::Listener> listeners;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Timeline)
};




#endif  // __TIMELINE_H_6CE3EB3B__
