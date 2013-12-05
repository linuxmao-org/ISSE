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

#include "ToolbarSlider.h"


 
    
ToolbarSlider::ToolbarSlider(int itemId, const String &labelText_, Range<float> range, bool isBeingUsedAsAButton) :
    ToolbarItemComponent(itemId, labelText_, isBeingUsedAsAButton), labelText(labelText_)
{
    volumeKnob = new Slider(String(itemId));
    volumeKnob->setSliderStyle( Slider::LinearHorizontal );
        
    volumeKnob->setBounds(bounds);
    volumeKnob->setColour(Slider::textBoxTextColourId, Colours::black);
    volumeKnob->setRange(range.getStart(), range.getEnd(), .01);
//    volumeKnob->setRange(-80.0, 15.0, .01);
    volumeKnob->setSkewFactor(1);
    volumeKnob->setTextBoxStyle (Slider::TextBoxRight, false, 50, volumeKnob->getTextBoxHeight());
    volumeKnob->setTooltip(labelText);
        
    addAndMakeVisible(volumeKnob);
        
}
    
bool ToolbarSlider::getToolbarItemSizes (int toolbarThickness, bool isToolbarVertical, int &preferredSize, int &minSize, int &maxSize)
{
    int w = 150;
    preferredSize = w;
    minSize = 50;
    maxSize = 150;
        
    return true;
}

void ToolbarSlider::setValue(double v)
{
    if(volumeKnob)
        volumeKnob->setValue(v);
}
    
void ToolbarSlider::paintButtonArea (Graphics &g, int width, int height, bool isMouseOver, bool isMouseDown)
{ }
    
    
void ToolbarSlider::contentAreaChanged (const Rectangle< int > &newBounds)
{
    bounds = newBounds;
    if(volumeKnob)
        volumeKnob->setBounds(bounds);
}
    
void ToolbarSlider::addListener( Slider::Listener * l)
{
    if(volumeKnob)
        volumeKnob->addListener(l);
}
    
void ToolbarSlider::removeListener( Slider::Listener * l)
{
    if(volumeKnob)
        volumeKnob->removeListener(l);
}

//String 	ToolbarSlider::getTooltip ()
//{
//    return labelText;
//}
// 
