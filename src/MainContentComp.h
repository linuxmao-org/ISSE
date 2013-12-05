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

#ifndef __MAINCONTENTCOMP_H_D16AC8A9__
#define __MAINCONTENTCOMP_H_D16AC8A9__


#include "JuceHeader.h"


//#include "VirtualUser.h"
#include "XMLSerializable.h"
#include "TransportComp.h"
#include "SeparationEngine.h"

#include <map>

class MultiPaintView;
class MultiTrack;
class TransportComp;
class MyTabbedComponent;
class MainToolbar;
class AboutPage;


class MainContentComp  :    public Component,
                            public Button::Listener,
                            public ApplicationCommandTarget,
                            public MenuBarModel,
                            public ModalComponentManager::Callback,
                            public XMLSerializable,
                            public FileBasedDocument,
                            public ChangeListener, 
                            public SeparationEngine::Listener
{
public:
    
    MainContentComp();
    ~MainContentComp();
  
    // Menu Bar Interface
    virtual StringArray getMenuBarNames ();
    virtual void menuItemSelected (int menuItemID, int topLevelMenuIndex);
    virtual PopupMenu 	getMenuForIndex (int menuIndex, const String &menuName);
    
    // Application Command Target interface
    virtual void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    virtual void getAllCommands (Array <CommandID>& commands);
    virtual bool perform (const InvocationInfo& info);
    virtual ApplicationCommandTarget* getNextCommandTarget();
    
    
    // FileBasedDocument
    virtual Result loadDocument (const File &file);
    virtual Result saveDocument (const File &file);
    virtual File   getLastDocumentOpened ();
    virtual void   setLastDocumentOpened (const File &file);
    virtual String getDocumentTitle ();
    
    // Separation Engine
    virtual void engineNewOutputs( File file, int source );
    virtual void engineTrainingChanged();
    virtual void engineNewPlugin( ISSPluginBase * plugin, Msg msg );


    virtual void changeListenerCallback (ChangeBroadcaster *source);
    
    virtual void modalStateFinished (int returnValue);
    
    virtual void buttonClicked (Button *button);
    void buttonClickedHelper (Button *button);
    
//    void routeGUIMessage( const XmlElement & xml );


    // File I/O
    bool createNewProject( File loadFile, bool async );
    
    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets );
    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  );
    
    // Settings
    void quit();
    void exportAudio();
 
 
    
    void currentTabChanged (int newCurrentTabIndex, const String &newCurrentTabName);
    
    void resetState();
    
private:
    
    
    
    // View and Zoom controls
    void resized();
    
    void logSimpleEvent(String event);

private:
    
    unsigned int currentTab;
    bool loadedFromXML;
    

    
    XmlElement xml;
    File assetsFolder;

    File lastfileOpen;
    
    ScopedPointer<MenuBarComponent> menubar;
    ScopedPointer<MainToolbar> toolbar;
    ScopedPointer<MultiTrack> multiTrack;
    ScopedPointer<TransportComp> transportComp;
    ScopedPointer<MultiPaintView> multiPaintView;
//    ScopedPointer<TimelineView> timelineView;
    ScopedPointer<Component> pluginComponent;
    ScopedPointer<MyTabbedComponent> tabbedComponent;
    ScopedPointer<AboutPage> aboutPage;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComp)
    
};



#endif  // __MAINCONTENTCOMP_H_D16AC8A9__
