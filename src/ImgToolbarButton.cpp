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

#include "ImgToolbarButton.h"



 
ImgToolbarButton::ImgToolbarButton(int itemId, String labelText_, DrawableImage *normalImage_, DrawableImage *toggledOnImage_, Colour selectedColor) :
    ToolbarButton(itemId, labelText_, normalImage_, toggledOnImage_),
    normalImage(normalImage_), toggledOnImage(toggledOnImage_),
    backgroundColour( selectedColor ),
    labelText(labelText_)
{
        
}
    
void ImgToolbarButton::setOverlayColour( Colour c)
{
    normalImage->setOverlayColour(c);
    toggledOnImage->setOverlayColour(c);
}
    
Colour ImgToolbarButton::getOverlayColour()
{
    return normalImage->getOverlayColour();
}
    
void 	ImgToolbarButton::paintButtonArea (Graphics &g, int width, int height, bool isMouseOver, bool isMouseDown)
{
    if(this->getToggleState())
        g.fillAll( backgroundColour );
}
    
    
    
String 	ImgToolbarButton::getTooltip ()
{
    return labelText;
}
    
 
