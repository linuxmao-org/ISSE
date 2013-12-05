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
#include "MainToolbar.h"

#include "ToolbarItemFact.h"
#include "ProgramSettings.h"
#include "ToolbarSlider.h"


MainToolbar::MainToolbar( double & progress )
{
    // Setup Toolbar
    ToolbarItemFact toolbarFactory(progress);
    int toolbarHeight(30);
    setVertical(false);
    setBounds(0, 0, getWidth(), toolbarHeight );
    setBounds(0, 0, getWidth(), toolbarHeight );
    setColour(Toolbar::backgroundColourId,ProgramSettings::toolbarBackgroundColor);
    
    
    addDefaultItems(toolbarFactory);
}


void MainToolbar::removeButtonListener( Button::Listener * listener)
{
    for(int i=0; i < getNumItems(); ++i)
    {
        Component * comp = getItemComponent (i);
        
        Button * b = dynamic_cast<Button*>(comp);
        if(b)
            b->removeListener(listener);
    }
}




void MainToolbar::addButtonListener( Button::Listener * listener)
{
    for(int i=0; i < getNumItems(); ++i)
    {
        Component * comp = getItemComponent (i);
        
        Button * b = dynamic_cast<Button*>(comp);
		Button::Listener * l = (Button::Listener*)listener;
        if(b)
            b->addListener(l);
    }
}

 
void MainToolbar::addSliderListener( Slider::Listener * listener)
{
    for(int i=0; i < getNumItems(); ++i)
    {
        Component * comp = getItemComponent (i);
        
        ToolbarSlider * b = dynamic_cast<ToolbarSlider*>(comp);
        if(b)
            b->addListener(listener);
    }
}

void MainToolbar::removeSliderListener( Slider::Listener * listener)
{
    for(int i=0; i < getNumItems(); ++i)
    {
        Component * comp = getItemComponent (i);
        
        ToolbarSlider * b = dynamic_cast<ToolbarSlider*>(comp);
        if(b)
            b->removeListener(listener);
    }
}