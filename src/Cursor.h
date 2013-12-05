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

#ifndef __CURSOR_H_2D940910__
#define __CURSOR_H_2D940910__

#include "JuceHeader.h"

class Cursor : public Component
{
public:
    
    Cursor( Colour c ) : color(c)
    {}
    
    virtual void 	paint (Graphics &g)
    {
        g.setColour( color );
        g.fillAll();
    }
    
    
    virtual void resized()
    {
        repaint(getBounds());
        

    }

    
private:
    Colour color;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Cursor);
};






#endif  // __CURSOR_H_2D940910__
