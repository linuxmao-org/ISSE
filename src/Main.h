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
#ifndef __MAIN_H_43BB3997__
#define __MAIN_H_43BB3997__



#include "JuceHeader.h"
#include "ISSLogger.h"

using namespace juce;

class DocWindow;
class Actions;
class PluginListWindow;


#define ISSLOG(x){  { ISSApplication * app = dynamic_cast<ISSApplication*> (JUCEApplication::getInstance()); \
ISSLogger * logger = app->logger; \
if(logger) \
logger->log(x); }\
}



#define MAIN_APP (dynamic_cast<ISSApplication*> (JUCEApplication::getInstance()))


class ISSLookAndFeel;




//==============================================================================
class ISSApplication  : public JUCEApplication //, public DocumentWindowClosedListener
{
public:
    //==============================================================================
    ISSApplication();
    
    const String getApplicationName();
    const String getApplicationVersion();
    bool moreThanOneInstanceAllowed();
    
    //==============================================================================
    void initialise (const String& commandLine);
    void shutdown();
    
//    void launchVirtualUser(File logfile, VirtualUserHost * host);
    
    void openPluginListEditor();
    void documentWindowClosed(DocumentWindow * window);
    
    void openAudioSettings();
    
    //==============================================================================
    void systemRequestedQuit();
    
    void anotherInstanceStarted (const String& commandLine);
    
    //==============================================================================
    
    ApplicationCommandManager commandManager;
    UndoManager undoManager;
    
    ScopedPointer<ISSLogger> logger;
    ApplicationProperties* appProperties;
    
    AudioDeviceManager audioDeviceManager;
    AudioFormatManager formatManager;
    
    ScopedPointer<ISSLookAndFeel> lookAndFeel;
    //ThreadPool threadPool;
 
    
    // Plugin Stuff
    ScopedPointer <PluginListWindow> pluginListWindow;
    AudioPluginFormatManager pluginFormatManager;
    KnownPluginList knownPluginList;
    KnownPluginList::SortMethod pluginSortMethod;
    
    
    // Main document window
    ScopedPointer<DocWindow> window;

};





#endif  // __MAIN_H_43BB3997__
