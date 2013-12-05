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
#ifndef __TOOLBARPROGRESS_H_4924D001__
#define __TOOLBARPROGRESS_H_4924D001__

#include "JuceHeader.h"


class MyProgressBar;

class  ToolbarProgress : public  ToolbarItemComponent
{
public:
    
    ToolbarProgress( int itemID, const String & labelText, double & progress );
    
    //This method must return the size criteria for this item, based on a given toolbar size and orientation.
    virtual bool getToolbarItemSizes (int toolbarThickness, bool isToolbarVertical,
                                      int &preferredSize, int &minSize, int &maxSize);
    //Your subclass should use this method to draw its content area.
    virtual void paintButtonArea (Graphics &g, int width, int height, bool isMouseOver, bool isMouseDown);
    
    //Callback to indicate that the content area of this item has changed.
    virtual void 	contentAreaChanged (const Rectangle< int > &newBounds);
    
private:
    ScopedPointer<MyProgressBar> progressbar;
};


#endif  // __TOOLBARPROGRESS_H_4924D001__
