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
#ifndef __MAINTOOLBAR_H_FFEAE5BB__
#define __MAINTOOLBAR_H_FFEAE5BB__

#include "JuceHeader.h"


class MainToolbar :
#ifdef __WINDOWS__ 
	public Button::Listener,
#endif
	 public Toolbar
{
public:
    
    MainToolbar( double & progress );
    
#ifdef __WINDOWS__
	virtual void buttonClicked(Button*){}	
#endif
	void addButtonListener( Button::Listener * listener);
	void removeButtonListener( Button::Listener * listener);

    void addSliderListener( Slider::Listener * listener);
    void removeSliderListener( Slider::Listener * listener);
    
    
private:
    
};



#endif  // __MAINTOOLBAR_H_FFEAE5BB__
