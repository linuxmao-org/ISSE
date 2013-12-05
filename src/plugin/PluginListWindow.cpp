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
// Authors: Nicholas J. Bryan, Original Author: Julian Storer
#include "PluginListWindow.h"
#include "../Main.h"

PluginListWindow::PluginListWindow (DocumentWindowClosedListener & owner_,
                                    AudioPluginFormatManager& formatManager)
        :           DocumentWindow ("Available Plugins", Colours::white,
                    DocumentWindow::minimiseButton | DocumentWindow::closeButton),
                    owner(owner_)
{
    const File deadMansPedalFile (MAIN_APP->appProperties->getUserSettings()
                                  ->getFile().getSiblingFile ("RecentlyCrashedPluginsList"));
    
    setContentOwned (new PluginListComponent (  formatManager,
                                                MAIN_APP->knownPluginList,
                                                deadMansPedalFile,
                                                MAIN_APP->appProperties->getUserSettings()),
                                                true);
    
    setResizable (true, false);
    setResizeLimits (300, 400, 800, 1500);
    setTopLeftPosition (60, 60);
    
    restoreWindowStateFromString (MAIN_APP->appProperties->getUserSettings()->getValue ("listWindowPos"));
    setVisible (true);
}

PluginListWindow::~PluginListWindow()
{
    MAIN_APP->appProperties->getUserSettings()->setValue ("listWindowPos", getWindowStateAsString());
    clearContentComponent();
}

void PluginListWindow::closeButtonPressed()
{
    
    setVisible(false);
    owner.documentWindowClosed(this);
}