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
#ifndef __DOCWINDOW_H_B132959E__
#define __DOCWINDOW_H_B132959E__


#include "JuceHeader.h"


class MainContentComp;

class DocWindow : public DocumentWindow
{
public:
    
    DocWindow();
    
    ~DocWindow();
    
    virtual void closeButtonPressed();
    
    void changed();


private:
 
    MainContentComp * mainContentComp;
    TooltipWindow tooltipWindow;// Just create and let it do it's things
    ComponentBoundsConstrainer newConstrainer;
    
};


#endif  // __DOCWINDOW_H_B132959E__


