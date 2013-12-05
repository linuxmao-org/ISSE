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

#include "DocWindow.h"

#include "MainContentComp.h"
#include "Main.h"
#include "AppCommandIDs.h"
#include "ProgramSettings.h"




DocWindow::DocWindow() : DocumentWindow("ISSE",Colours::lightgrey,
                                        DocumentWindow::TitleBarButtons::allButtons, true)
                                         
{
    Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    int x = r.getWidth();
    int y = r.getHeight();
#ifdef __IOS_CORE__
#else
    x *= .75;
    y *= .75;
#endif
    
    
    mainContentComp = new MainContentComp();
    mainContentComp->setSize(x,y);
    mainContentComp->resetState();
    
    // point command manager to use keypresses that arrive in our window
    addKeyListener (MAIN_APP->commandManager.getKeyMappings());
    
   
    setUsingNativeTitleBar(true);
   
    
#ifdef __IOS_CORE__
    setResizable(false, false);
//    Desktop::getInstance().setOrientationsEnabled(Desktop::rotatedClockwise | Desktop::rotatedAntiClockwise);

#else
    
   
    setResizable(true, true);
    newConstrainer.setMinimumWidth(500);
    newConstrainer.setMinimumHeight(500);
    setConstrainer (&newConstrainer);
//    Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    setBounds(100, 100, x,y);
    mainContentComp->setSize(x,y);
#endif
    

    MAIN_APP->commandManager.setFirstCommandTarget(mainContentComp);
    mainContentComp->applicationCommandListChanged	();
 
    setContentOwned(mainContentComp, true);
    setVisible(mainContentComp);
    
#ifdef __IOS_CORE__
    Desktop::getInstance().setKioskModeComponent (this, false);
#endif

}

 

void DocWindow::changed()
{
    if(mainContentComp)
        mainContentComp->changed();
}

void DocWindow::closeButtonPressed()
{
    
    if(mainContentComp && !ProgramSettings::processing && !ProgramSettings::playing )
        mainContentComp->quit();
    else
    {
        String title("ERROR");
        String message("Processing must be off and completed...");
        String buttonText("Dismiss");
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, title,
                                          message, buttonText);
    }
    
}

DocWindow::~DocWindow()
{ }





