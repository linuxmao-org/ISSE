/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-9 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/
// Authors: Nicholas J. Bryan, Original Author: Julian Storer
#ifndef __JUCE_MAINHOSTWINDOW_JUCEHEADER__
#define __JUCE_MAINHOSTWINDOW_JUCEHEADER__

#include "FilterGraph.h"
#include "GraphEditorPanel.h"


//==============================================================================
namespace CommandIDs
{
    static const int open                   = 0x30000;
    static const int save                   = 0x30001;
    static const int saveAs                 = 0x30002;
}


//==============================================================================
class MainHostWindow    : public DocumentWindow,
                          public MenuBarModel,
                          public ApplicationCommandTarget,
                          public ChangeListener
{
public:
    //==============================================================================
    MainHostWindow(AudioSource * source);
    ~MainHostWindow();

    //==============================================================================
    void closeButtonPressed();
    void changeListenerCallback (ChangeBroadcaster*);


    StringArray getMenuBarNames();
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
    void menuItemSelected (int menuItemID, int topLevelMenuIndex);
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);

    bool tryToQuitApplication();

    void createPlugin (const PluginDescription* desc, int x, int y);

    void addPluginsToMenu (PopupMenu& m) const;
    const PluginDescription* getChosenType (const int menuID) const;

    GraphDocumentComponent* getGraphEditor() const;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainHostWindow);
};


#endif



