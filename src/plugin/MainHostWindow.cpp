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
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainHostWindow.h"
#include "InternalFilters.h"
#include "../Main.h"



//==============================================================================
MainHostWindow::MainHostWindow( AudioSource * source )
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
{
    setResizable (true, false);
    setResizeLimits (500, 400, 10000, 10000);
    centreWithSize (400, 600);

    
    // Store the graph document component as the content component
    setContentOwned (new GraphDocumentComponent (MAIN_APP->pluginFormatManager,
                                                 &MAIN_APP->audioDeviceManager, source), false);
    
    MAIN_APP->commandManager.registerAllCommandsForTarget(this);
    MAIN_APP->knownPluginList.addChangeListener (this);

    Process::setPriority (Process::HighPriority);

    setMenuBar (this);
}



MainHostWindow::~MainHostWindow()
{
    setMenuBar (nullptr);
    MAIN_APP->knownPluginList.removeChangeListener (this);
    clearContentComponent();
}

void MainHostWindow::closeButtonPressed()
{
    setVisible(false);
}

bool MainHostWindow::tryToQuitApplication()
{
    PluginWindow::closeAllCurrentlyOpenWindows();

    if (getGraphEditor() == nullptr
         || getGraphEditor()->graph.saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
    {
        JUCEApplication::quit();
        return true;
    }

    return false;
}

void MainHostWindow::changeListenerCallback (ChangeBroadcaster*)
{
    menuItemsChanged();
    // save the plugin list every time it gets chnaged, so that if we're scanning
    // and it crashes, we've still saved the previous ones
    ScopedPointer<XmlElement> savedPluginList (MAIN_APP->knownPluginList.createXml());

    if (savedPluginList != nullptr)
    {
        MAIN_APP->appProperties->getUserSettings()->setValue ("pluginList", savedPluginList);
        MAIN_APP->appProperties->saveIfNeeded();
    }
}

StringArray MainHostWindow::getMenuBarNames()
{
    const char* const names[] = { "File", "Plugins", "Options", nullptr };

    return StringArray (names);
}

PopupMenu MainHostWindow::getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/)
{
    PopupMenu menu;

    if (topLevelMenuIndex == 0)
    {
        // "File" menu
        menu.addCommandItem (&MAIN_APP->commandManager, CommandIDs::open);

        RecentlyOpenedFilesList recentFiles;
        recentFiles.restoreFromString (MAIN_APP->appProperties->getUserSettings()
                                            ->getValue ("recentFilterGraphFiles"));

        PopupMenu recentFilesMenu;
        recentFiles.createPopupMenuItems (recentFilesMenu, 100, true, true);
        menu.addSubMenu ("Open recent file", recentFilesMenu);

        menu.addCommandItem (&MAIN_APP->commandManager, CommandIDs::save);
        menu.addCommandItem (&MAIN_APP->commandManager, CommandIDs::saveAs);
        menu.addSeparator();
        menu.addCommandItem (&MAIN_APP->commandManager, StandardApplicationCommandIDs::quit);
    }
    else if (topLevelMenuIndex == 1)
    {
        // "Plugins" menu
        PopupMenu pluginsMenu;
        addPluginsToMenu (pluginsMenu);
        menu.addSubMenu ("Create plugin", pluginsMenu);
        menu.addSeparator();
        menu.addItem (250, "Delete all plugins");
    }
    else if (topLevelMenuIndex == 2)
    {
        PopupMenu sortTypeMenu;
        sortTypeMenu.addItem (200, "List plugins in default order",      true,
                              MAIN_APP->pluginSortMethod == KnownPluginList::defaultOrder);
        sortTypeMenu.addItem (201, "List plugins in alphabetical order", true,
                              MAIN_APP->pluginSortMethod == KnownPluginList::sortAlphabetically);
        sortTypeMenu.addItem (202, "List plugins by category",           true,
                              MAIN_APP->pluginSortMethod == KnownPluginList::sortByCategory);
        sortTypeMenu.addItem (203, "List plugins by manufacturer",       true,
                              MAIN_APP->pluginSortMethod == KnownPluginList::sortByManufacturer);
        sortTypeMenu.addItem (204, "List plugins based on the directory structure", true,
                              MAIN_APP->pluginSortMethod == KnownPluginList::sortByFileSystemLocation);
        menu.addSubMenu ("Plugin menu type", sortTypeMenu);
    }

    return menu;
}

void MainHostWindow::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
    GraphDocumentComponent* const graphEditor = getGraphEditor();
    
    if (menuItemID == 250)
    {
        if (graphEditor != nullptr)
            graphEditor->graph.clear();
    }
    else if (menuItemID >= 100 && menuItemID < 200)
    {
        RecentlyOpenedFilesList recentFiles;
        recentFiles.restoreFromString (MAIN_APP->appProperties->getUserSettings()
                                            ->getValue ("recentFilterGraphFiles"));

        if (graphEditor != nullptr &&
            graphEditor->graph.saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
            graphEditor->graph.loadFrom (recentFiles.getFile (menuItemID - 100), true);
    }
    else if (menuItemID >= 200 && menuItemID < 210)
    {
             if (menuItemID == 200)     MAIN_APP->pluginSortMethod = KnownPluginList::defaultOrder;
        else if (menuItemID == 201)     MAIN_APP->pluginSortMethod = KnownPluginList::sortAlphabetically;
        else if (menuItemID == 202)     MAIN_APP->pluginSortMethod = KnownPluginList::sortByCategory;
        else if (menuItemID == 203)     MAIN_APP->pluginSortMethod = KnownPluginList::sortByManufacturer;
        else if (menuItemID == 204)     MAIN_APP->pluginSortMethod = KnownPluginList::sortByFileSystemLocation;

        MAIN_APP->appProperties->getUserSettings()->setValue ("pluginSortMethod", (int) MAIN_APP->pluginSortMethod);

        menuItemsChanged();
    }
    else
    {
        createPlugin (getChosenType (menuItemID),
                      proportionOfWidth  (0.3f + Random::getSystemRandom().nextFloat() * 0.6f),
                      proportionOfHeight (0.3f + Random::getSystemRandom().nextFloat() * 0.6f));
    }
}

void MainHostWindow::createPlugin (const PluginDescription* desc, int x, int y)
{
    GraphDocumentComponent* const graphEditor = getGraphEditor();

    if (graphEditor != nullptr)
        graphEditor->createNewPlugin (desc, x, y);
}

void MainHostWindow::addPluginsToMenu (PopupMenu& m) const
{
    MAIN_APP->knownPluginList.addToMenu (m, MAIN_APP->pluginSortMethod);
}

const PluginDescription* MainHostWindow::getChosenType (const int menuID) const
{
//    if (menuID >= 1 && menuID < 1 + internalTypes.size())
//        return internalTypes [menuID - 1];
    return MAIN_APP->knownPluginList.getType (MAIN_APP->knownPluginList.getIndexChosenByMenu (menuID));
}

//==============================================================================
ApplicationCommandTarget* MainHostWindow::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}
 
void MainHostWindow::getAllCommands (Array <CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] = { CommandIDs::open,
                              CommandIDs::save,
                              CommandIDs::saveAs};

    commands.addArray (ids, numElementsInArray (ids));
}

void MainHostWindow::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
    const String category ("General");

    switch (commandID)
    {
    case CommandIDs::open:
        result.setInfo ("Open...",
                        "Opens a filter graph file",
                        category, 0);
//        result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
        break;

    case CommandIDs::save:
        result.setInfo ("Save",
                        "Saves the current graph to a file",
                        category, 0);
        break;

    case CommandIDs::saveAs:
        result.setInfo ("Save As...",
                        "Saves a copy of the current graph to a file",
                        category, 0);
        break;
    default:
        break;
    }
}

bool MainHostWindow::perform (const InvocationInfo& info)
{
    GraphDocumentComponent* const graphEditor = getGraphEditor();

    switch (info.commandID)
    {
    case CommandIDs::open:
        if (graphEditor != nullptr &&
            graphEditor->graph.saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
            graphEditor->graph.loadFromUserSpecifiedFile (true);

        break;

    case CommandIDs::save:
        if (graphEditor != nullptr)
            graphEditor->graph.save (true, true);
        break;

    case CommandIDs::saveAs:
        if (graphEditor != nullptr)
            graphEditor->graph.saveAs (File::nonexistent, true, true, true);
        break;

    default:
        return false;
    }

    return true;
}



GraphDocumentComponent* MainHostWindow::getGraphEditor() const
{
    return dynamic_cast <GraphDocumentComponent*> (getContentComponent());
}





