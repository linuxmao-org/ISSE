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
#include "TimelineView.h"
#include "MainContentComp.h"
#include "TransportComp.h"

#include "MultiPaintView.h"


TimelineView::TimelineView( MultiPaintView * multiPaintView_ ) : multiPaintView(multiPaintView_)
{
}



void TimelineView::paint( Graphics & g)
{
 
    g.setColour(Colours::red.withAlpha(.5f));
    g.drawRect(0, 0, 100, getHeight());
    
    Rectangle<int> b1 = getBounds();
    Rectangle<int> b2 = multiPaintView->getBounds();
  
}


void TimelineView::resized()
{
    
    
    
//    multiPaintView->setBounds(getBounds());
    
//    repaint();

}



