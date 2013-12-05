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
#ifndef __PLUGINLISTWINDOW_H_6C66BAD7__
#define __PLUGINLISTWINDOW_H_6C66BAD7__

#include "JuceHeader.h"

class DocumentWindowClosedListener
{
public:
    
    virtual void documentWindowClosed(DocumentWindow * window)=0;
    virtual ~DocumentWindowClosedListener(){}
};

//==============================================================================
class PluginListWindow  : public DocumentWindow
{
public:
    PluginListWindow (DocumentWindowClosedListener & owner_,
                      AudioPluginFormatManager& formatManager);
    
    ~PluginListWindow();
    
    void closeButtonPressed();
    
private:
    
    DocumentWindowClosedListener & owner;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginListWindow);
};


#endif  // __PLUGINLISTWINDOW_H_6C66BAD7__


