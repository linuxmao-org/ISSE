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
#ifndef __TOOLBARSLIDER_H_D5E23F92__
#define __TOOLBARSLIDER_H_D5E23F92__



#include "JuceHeader.h"

using namespace juce;


class ToolbarSlider: public ToolbarItemComponent
{
public:
    
    
    ToolbarSlider(int itemId, const String &labelText_, Range<float> range, bool isBeingUsedAsAButton);
    
    virtual bool getToolbarItemSizes (int toolbarThickness, bool isToolbarVertical, int &preferredSize, int &minSize, int &maxSize);
    
    virtual void paintButtonArea (Graphics &g, int width, int height, bool isMouseOver, bool isMouseDown);
    
    virtual void contentAreaChanged (const Rectangle< int > &newBounds);
    
    void addListener( Slider::Listener * l);
    
    void removeListener( Slider::Listener * l);
    
    void setValue(double v);
    
//    virtual String 	getTooltip ();
    
private:
    
    ScopedPointer<Slider> volumeKnob;
    const String labelText;
    Rectangle< int > bounds;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarSlider);
};





#endif  // __TOOLBARSLIDER_H_D5E23F92__
