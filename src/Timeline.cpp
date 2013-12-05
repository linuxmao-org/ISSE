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
#include "Timeline.h"
#include "Track.h"
#include "AppCommandIDs.h"
#include "ISSLogger.h"
#include "Main.h"
#include "ProgramSettings.h"

#include <sstream>
#include <string>
#include <iostream>    
#include <iomanip>
#include <math.h>       /* frexp */

int Timeline::border = 1;
int Timeline::markerWidth = 10;


Timeline::Timeline( Timeline::Listener * l )
{
    setSize(100, 30);
    startInSec = 0;
    stopInSec = 11.0;
    currentCursorInSec = 0;
    listeners.add(l);
}

Timeline::~Timeline()
{
    listeners.clear();
}

int Timeline::verifyLoopMarker( int xPos )
{
    if( xPos <= Track::getPanelWidth() )
        xPos = Track::getPanelWidth();
    if( xPos >= getWidth() - Track::getScrollBarWidth() )
        xPos = getWidth() - Track::getScrollBarWidth();
    return xPos;
}

 
template <class T>
int numDigits(T number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

std::string floatToString(float v, float rangeLen)
{
    std::ostringstream os;
    os.setf(std::ios::fixed,std::ios::floatfield);
    os.precision(2);
    os << v;
    
    return os.str();
}

void Timeline::paint (Graphics &g)
{
    
    
    g.setColour( ProgramSettings::mainBackgroundColor);
    g.fillAll();

    g.setColour( Colours::white );
    int w = verifyLoopMarker(convertSecToPx(0));
    
    
    g.drawRect(w, border,  getWidth()-Track::getScrollBarWidth()-w, getHeight()-2*border,1);
 
     
    //Draw the time labels in seconds
    {
        int items2draw = 20;
    
        float startSec = currentRange.getStart();
        float startPx = convertSecToPx(startSec);
        float endSec = currentRange.getEnd();
        float deltaPx = (width-w-2.0*border-Track::getScrollBarWidth())*1.0/(1.0*items2draw);
        float deltaSec = (endSec-startSec)/(1.0*items2draw);
    
        g.drawText( String("Seconds") , 20, 2*border+5, 100, 20, Justification::centredTop, false);
    
        startSec += deltaSec;
        startPx  += deltaPx;

        for(int i=0; i < items2draw-1; i++)
        {
            if( (i+1)%2==0)
            {
                std::string s = floatToString(startSec, currentRange.getLength());
                g.drawText( String(  s.c_str() ), startPx-25, 2*border, 50, 20, Justification::centredTop, false);
                g.drawLine (startPx, .6*height, startPx, height-border);
            }
            else
                g.drawLine (startPx, 3*height/4, startPx, height-border);
        
            startSec+= deltaSec;
            startPx += deltaPx;
        }
    }
    
    // Draw the  markers
    g.setColour(Colours::red);
    
    if(currentRange.getEnd()==0) return;
    
    if( (startInSec >= currentRange.getStart()) && (startInSec < currentRange.getEnd()))
    {
        g.setColour(Colours::red);
        if(movingStartMarker)
            g.setColour(Colours::white);
        
        g.fillRect(convertSecToPx(startInSec)-markerWidth, getHeight()/2+2, markerWidth, getHeight()-border);
    }
    
    
    if( (stopInSec >= currentRange.getStart()) && (stopInSec <= currentRange.getEnd()+.01))
    {
        g.setColour(Colours::red);
        if(movingStopMarker)
            g.setColour(Colours::white);
        
        g.fillRect( convertSecToPx(stopInSec), getHeight()/2+2, markerWidth, getHeight()-border );
    }
}

int Timeline::convertSecToPx( float seconds )
{
    float width = getWidth();
    float percentage = (seconds - currentRange.getStart())/( currentRange.getLength());
    int px = Track::getPanelWidth() + percentage * (width - Track::getScrollBarWidth() - Track::getPanelWidth());
    return px;
}

void Timeline::resized()
{
    width = getWidth();
    height = getHeight();
}

void Timeline::setRange( Range<double> currentRange_,  Range<double> rangeLimits_ )
{
    currentRange = currentRange_;
    rangeLimits = rangeLimits_;

    repaint();
}

void Timeline::processMouse(float ptInSec, Event event)
{

    bool sendLoopMsg(false);
    if(movingStartMarker)
    {
        if( (ptInSec >= 0 ) && (ptInSec < currentRange.getEnd()) && (ptInSec <= stopInSec))
        {
            startInSec = ptInSec;
            sendLoopMsg = true;
        }
        else if(ptInSec <= 0)
        {
            startInSec = 0;
            sendLoopMsg = true;
        }
        else if( (ptInSec > currentRange.getEnd()) || (ptInSec >= stopInSec) )
        {
            startInSec = stopInSec - .01;
            sendLoopMsg = true;
        }
    }
    else if(movingStopMarker)
    {
        if( (ptInSec <= 0) || (ptInSec <= startInSec) )
        {
            stopInSec = startInSec + .01;
            sendLoopMsg = true;
        }
        else if(ptInSec >= currentRange.getEnd())
        {
            stopInSec = currentRange.getEnd();
            sendLoopMsg = true;
        }
        else
        {
            stopInSec = ptInSec;
            sendLoopMsg = true;
        }
    }
    else
    {
        int xPt = convertSecToPx(ptInSec);
        
        // Set the timeline cursor
        if( xPt < Track::getPanelWidth()  )
            xPt = Track::getPanelWidth();
        if( xPt >  getWidth() - Track::getScrollBarWidth()-border )
            xPt = getWidth() - Track::getScrollBarWidth()-border;
 
        currentCursorInSec = convertPxToSec( xPt );
        
        listeners.call (&Listener::timelineEvent, event );
    }
    
    if(sendLoopMsg)
    {
        repaint();
        listeners.call (&Listener::timelineEvent, Timeline::LoopMarkersChanged );
    }
}

 
void Timeline::setLoopMarkers(float startInSec_, float stopInSec_)
{
    startInSec = startInSec_;
    stopInSec = stopInSec_;
    listeners.call (&Listener::timelineEvent, Timeline::LoopMarkersChanged );
    repaint();
}



void Timeline::mouseMove(const MouseEvent& e)
{
    
}



void Timeline::logMouseEvent( Event event, float xInSec )
{
//    ISSLOG("<Event type=\"TimelineEvent\" time=\"" + ISSTIME + "\"><X>" +
//           String(xInSec) + "</X><Event>" + String(event) + "</Event><Timeline></Timeline></Event>");
    
}

void Timeline::mouseDownHelper (float xInSec)
{
    movingStartMarker = false;
    movingStopMarker = false;
    
    int startInPx = convertSecToPx(startInSec);
    int stopInPx = convertSecToPx(stopInSec);
    int xInPx = convertSecToPx(xInSec);
    
    if( (xInPx >= startInPx-markerWidth) && (xInPx <= startInPx ) )
    {
        movingStartMarker = true;
    }
    else  if( (xInPx >= stopInPx) && (xInPx <= stopInPx+markerWidth ) )
    {
        movingStopMarker = true;
    }
    
    processMouse( xInSec, MouseDown);
    
    

    logMouseEvent( MouseDown, xInSec );
}

void Timeline::mouseDragHelper (float xInSec)
{
    processMouse(xInSec, MouseDrag);

    logMouseEvent( MouseDrag, xInSec );

}
void Timeline::mouseUpHelper(float xInSec)
{
    processMouse(xInSec, MouseUp );
    
    movingStartMarker = false;
    movingStopMarker = false;
    
    logMouseEvent( MouseUp, xInSec);
}




void Timeline::mouseDown (const MouseEvent& e)
{
    float posInPos = convertPxToSec(e.getPosition().getX());
    
    if( e.mods.isRightButtonDown() )
    {}
    else
        mouseDownHelper(posInPos);
}

void Timeline::mouseDrag (const MouseEvent& e)
{
    mouseDragHelper(convertPxToSec(e.getPosition().getX()));
}

void Timeline::mouseUp(const MouseEvent & e)
{
    float posInPos = convertPxToSec(e.getPosition().getX());
    if( e.mods.isRightButtonDown() )
    {
        
        PopupMenu menu;
        
        menu.addItem(1, "Set Loop Start");
        menu.addItem(2, "Set Loop Stop");
        menu.addItem(3, "Reset Markers");
        
        const int result = menu.show();
        
        if (result == 0)
        {}
        else if (result == 1)
        {
            movingStartMarker = true;
            mouseUpHelper(posInPos);
        }
        else if (result == 2)
        {
            movingStopMarker = true;
            mouseUpHelper(posInPos);
        }
        else if(result==3)
        {
            movingStartMarker = true;
            mouseUpHelper(0);
            
            movingStopMarker = true;
            
            startInSec = 0;
            stopInSec = rangeLimits.getLength();
            
            listeners.call (&Listener::timelineEvent, Timeline::LoopMarkersChanged );
        }
        
    }
    else mouseUpHelper(posInPos);
}

float Timeline::convertPxToSec(int x)
{
    float width = getWidth();
    float percentage = (x - Track::getPanelWidth())/
                ( width - Track::getScrollBarWidth() - Track::getPanelWidth());
    return currentRange.getStart() + percentage * currentRange.getLength();
}











void  Timeline::setCursor(float cursorInSec)
{
    currentCursorInSec = cursorInSec;
}
float Timeline::getStartInSec(){return startInSec;}
float Timeline::getStopInSec(){return stopInSec;}
float Timeline::getCursorInSec(){return currentCursorInSec;}
float Timeline::getStartInPx()
{
    return verifyLoopMarker(convertSecToPx(getStartInSec()));
}
float Timeline::getStopInPx()
{
    
    return verifyLoopMarker(convertSecToPx(getStopInSec()));
}
float Timeline::getCursorInPx()
{
    return verifyLoopMarker(convertSecToPx(getCursorInSec()));
}









