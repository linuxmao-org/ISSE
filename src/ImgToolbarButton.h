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

#ifndef __MYTOOLBARBUTTON_H_6F3F1357__
#define __MYTOOLBARBUTTON_H_6F3F1357__

#include "JuceHeader.h"

using namespace juce;


class ImgToolbarButton : public ToolbarButton  
{
public:
    
    ImgToolbarButton(int itemId, String labelText, DrawableImage *normalImage,
                     DrawableImage *toggledOnImage, Colour selectedColor );
    
    // Inherited methods
    virtual void paintButtonArea (Graphics &g, int width, int height,
                                  bool isMouseOver, bool isMouseDown);

    virtual String 	getTooltip ();
    
    
    // Added methods
    void setOverlayColour( Colour c);
    Colour getOverlayColour();

private:
    
    DrawableImage *normalImage;
    DrawableImage *toggledOnImage;
    Colour backgroundColour;
    String labelText;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImgToolbarButton);
};




#endif  // __MYTOOLBARBUTTON_H_6F3F1357__
