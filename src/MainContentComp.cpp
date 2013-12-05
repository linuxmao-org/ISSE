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

#include "MainContentComp.h"

#include "ProgramSettings.h"
#include "BrushPanel.h"
#include "SpectrogramSettings.h"
#include "Main.h"


#include "AppCommandIDs.h"
#include "ImgToolbarButton.h"


#include "ToolbarState.h"
#include "Timeline.h"
#include "ThreadHelpers.h"
#include "ISSLogger.h"
#include "ToolbarSlider.h"

#include "MultiTrack.h"
#include "AboutPage.h"
#include "TransportComp.h"
#include "MultiPaintView.h"
#include "MainToolbar.h"
#include "api/ISSPluginBase.h"
#include "PluginComponent.h"

#include <iostream>
#include <algorithm>
#include <map>
#include <iostream>
#include <sstream>

class MyTabbedComponent : public TabbedComponent
{
public:
    MyTabbedComponent (MainContentComp * comp_) : TabbedComponent(TabbedButtonBar::TabsAtTop), comp(comp_)
    {}
    
    virtual void currentTabChanged (int newCurrentTabIndex, const String &newCurrentTabName)
    {
        comp->currentTabChanged (newCurrentTabIndex, newCurrentTabName);
    }
    
    virtual void popupMenuClickOnTab (int tabIndex, const String &tabName)
    {
        
        if(tabName.compare("About")==0)
        {
            PopupMenu menu;
            menu.addItem(1, "Close");
            const int result = menu.show();
            if (result == 0)
            {}
            else if (result == 1)
            {
                // TODO: Close
                removeTab(tabIndex);
                setCurrentTabIndex(0);
            }
        }
    }
    MainContentComp * comp;
};


 
MainContentComp::MainContentComp()  :  FileBasedDocument (".isse", "*.isse","Choose a file to open",
                                       "Choose a file to save"),
        currentTab(0),
        loadedFromXML(false),
        xml("trash"),
        toolbar( new MainToolbar(SeparationEngine::getInstance()->getProgress())),
        multiTrack( new MultiTrack() ),
        transportComp( new TransportComp()),
        multiPaintView( new MultiPaintView(toolbar, multiTrack, transportComp)),
        tabbedComponent(new MyTabbedComponent(this)),  aboutPage(new AboutPage())
{
   MAIN_APP->commandManager.registerAllCommandsForTarget (this);
   
    toolbar->addButtonListener(multiPaintView);
    toolbar->addSliderListener(multiPaintView);

#ifdef __MACOSX_CORE__
    
    MenuBarModel::setMacMainMenu (this, NULL);
    tabbedComponent->setBounds(0,0,getWidth(), getHeight());
    
#else // WINDOWS, UNIX, and IOS
    menubar = new MenuBarComponent(this);
    menubar->setBounds(0, 0, getWidth(), 20);
    addAndMakeVisible(menubar);
    tabbedComponent->setBounds(0, menubar->getBottom(),getWidth(), getHeight()-20);
#endif
//    resetState();
    
    
    Colour c(ProgramSettings::mainBackgroundColor);
    addAndMakeVisible(tabbedComponent);
    tabbedComponent->addTab("Multi Paint View", c , multiPaintView, false);
    tabbedComponent->addTab("Single Paint View", c, multiPaintView, false);
    tabbedComponent->setCurrentTabIndex(0);
    
    //MAIN_APP->commandManager.invokeDirectly(AppCommandIDs::OpenMultiTrack, true);
    
    
    SeparationEngine::getInstance()->addListener(this);
    
   
    MAIN_APP->undoManager.addChangeListener(this);
}

// Separation Engine
void MainContentComp::engineNewOutputs( File file, int source )
{
    applicationCommandListChanged();
}
void MainContentComp::engineTrainingChanged()
{
    
}

// Used to load the multipaint view from a file
void MainContentComp::engineNewPlugin( ISSPluginBase * plugin, Msg msg )
{
    if( msg==SeparationEngine::Listener::NewPluginLoaded )
    {
        if(loadedFromXML)
            multiPaintView->loadFromXML(xml, assetsFolder);
        
        StringArray array = tabbedComponent->getTabNames ();
        for(int i=0; i < array.size(); ++i)
        {
            if(array[i].compare("Separation Settings")==0)
            {
                tabbedComponent->removeTab(i);
                break;
            }
        }
        
        Colour c(ProgramSettings::mainBackgroundColor);
        tabbedComponent->addTab("Separation Settings", c, SeparationEngine::getInstance()->createPluginGUI(), true);
    }
    loadedFromXML = false;
    ProgramSettings::activeMode = true;
}

void MainContentComp::currentTabChanged (int newCurrentTabIndex, const String &newCurrentTabName)
{

//    if(newCurrentTabName.compare("Multi Paint View")==0)
    if(newCurrentTabIndex==0)
    {
        multiTrack->setSingleTrackMode(false);
        multiPaintView->addAndMakeVisible(toolbar);
        multiPaintView->addAndMakeVisible(multiTrack);
        multiPaintView->addAndMakeVisible(transportComp);
    }
//    else if(newCurrentTabName.compare("Single Paint View")==0)
    else if(newCurrentTabIndex==1)
    {
        multiTrack->setSingleTrackMode(true);
        multiPaintView->addAndMakeVisible(toolbar);
        multiPaintView->addAndMakeVisible(multiTrack);
        multiPaintView->addAndMakeVisible(transportComp);
    }
    else if(newCurrentTabName.compare("About")==0)
    {
        if(aboutPage)
            aboutPage->reload();
    }
    
    // Update the current tab
    currentTab = newCurrentTabIndex;
}
 


// get the title
String 	MainContentComp::getDocumentTitle ()
{
    return "Project";
}

//  This method should try to load your document from the given file.
Result 	MainContentComp::loadDocument (const File &file)
{
    
//    FileBasedDocument::SaveResult res = saveIfNeededAndUserAgrees ();
//    if(res==FileBasedDocument::savedOk)
    {
        if (!file.hasFileExtension(".isse")) {
             return Result::fail("Incorrect File Extension...");
        }

        // Popup modal view
        LoadThread m(this, file);
        if (m.runThread())
        {
            setFile(file);
            return Result::ok();
        }
        else
            return Result::fail("User Canceled...");
    
        return Result::ok();
    }
    return Result::fail("Canceled Saving...");
    
}

// This method should try to write your document to the given file.
Result 	MainContentComp::saveDocument (const File &file)
{
    // Popup modal view
    File logf;
    
    SaveThread m(this, file, logf);
    if (m.runThread())
        return Result::ok();
    else
        return Result::fail("User canceled");
        
    
    return Result::ok();
}


File MainContentComp::getLastDocumentOpened ()
{
    return lastfileOpen;
}

void MainContentComp::setLastDocumentOpened (const File &file)
{
    lastfileOpen = file;
}


void MainContentComp::logSimpleEvent(String event)
{
//    ISSLOG("<Event type=\"" + event + "\" time=\"" + ISSTIME + "\"></Event>");
}

 
 
void MainContentComp::resetState()
{

	SeparationEngine::getInstance()->reset();

    ProgramSettings::reset();

    // Clear the undo history
    MAIN_APP->undoManager.clearUndoHistory ();
    
    tabbedComponent->setCurrentTabIndex(0);
    
    if(multiPaintView)
        multiPaintView->resetState();
    
    if(multiTrack)
        multiTrack->resetState();
    
    if(transportComp)
        transportComp->reset();
    
    // Repaint everything
    repaint();
    
    
}

MainContentComp::~MainContentComp()
{
    toolbar->removeButtonListener(multiPaintView);
    toolbar->removeSliderListener(multiPaintView);
    
    MAIN_APP->undoManager.removeChangeListener(this);
    
#ifdef __MACOSX_CORE__
    MenuBarModel::setMacMainMenu(nullptr);
#endif

    SeparationEngine * engine(SeparationEngine::getInstanceWithoutCreating());
    if(engine)
        SeparationEngine::getInstance()->removeListener(this);
}
 
 


void MainContentComp::resized()
{
    int height = getHeight();
    int width = getWidth();
    
#ifdef __MACOSX_CORE__
     if(tabbedComponent)
        tabbedComponent->setBounds(0,0,width, height);
#elif __IOS_CORE__
    if(menubar)
        menubar->setBounds(0, 0, width, 20);
	if(tabbedComponent && menubar)
		tabbedComponent->setBounds(0, menubar->getBottom(),width, height-20);
#else
    if(menubar)
        menubar->setBounds(0, 0, width, 20);
    if(tabbedComponent && menubar)
	tabbedComponent->setBounds(0, menubar->getBottom(),getWidth(), height-20);
    else
        tabbedComponent->setBounds(0,0,width, height-20);
#endif
    
    if(aboutPage && multiPaintView)
        aboutPage->setBounds(0, multiPaintView->getY(), getWidth(), getHeight());
    
    
    repaint();
}
 

bool MainContentComp::saveToXML( XmlElement & xml, const File & folderForAssets )
{
    SeparationEngine::getInstance()->saveToXML(xml, folderForAssets);
    multiPaintView->saveToXML(xml, folderForAssets);
    return true;
}
 

void * runErrorMsg( void *userData)
{
    AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "ERROR", "Could not load file...");
    
    return NULL;
}

bool MainContentComp::loadFromXML( const XmlElement & xml_, const File & assetsFolder_  )
{
    // message manager/GUI thread
    const MessageManagerLock mml (Thread::getCurrentThread());
    if (! mml.lockWasGained()) return false;
    
    resetState();
    
    xml = xml_;
    assetsFolder = assetsFolder_;
    
    loadedFromXML = true;
    
    // Load the state of the separation engine
    SeparationEngine::getInstance()->loadFromXML(xml, assetsFolder);
    // Must wait until the separation engine initializes
    return true;
}

bool MainContentComp::createNewProject( File loadFile, bool async )
{
    if(async)
    {
        
        if(ProgramSettings::activeMode)
        {
//            FileBasedDocument::SaveResult res = saveIfNeededAndUserAgrees ();
//            if(res==FileBasedDocument::savedOk)
            {
                setFile(File());
                if(!loadFile.exists())
                    return false;
                CreateNewProjectThread m(this, loadFile);
                m.runThread();
                return true;
            }
        }
        else
        {
            CreateNewProjectThread m(this, loadFile);
            m.runThread();
            return true;
        }
    }
    else
    {
        File tempDir = File::getSpecialLocation( File::tempDirectory );
		if(!tempDir.exists())
			tempDir.createDirectory();
        
        resetState();
        
        // Delete any old temp files
        String fileNameNoExt = loadFile.getFileNameWithoutExtension ();
        String ext = loadFile.getFileExtension ();
        
        // Create files
		File tempInputFile(tempDir.getFullPathName() + "/" + loadFile.getFileName());
        
//		Logger::outputDebugString(tempInputFile.getFullPathName());

		if(tempInputFile.existsAsFile())
		{
			tempInputFile.setReadOnly(false);
            
            bool deletedOkay = tempInputFile.deleteFile();
            
            if(!deletedOkay)
            {
                String title("ERROR");
                String message("Could not load the file. Please restart and try again.");
                String buttonText("Dismiss");
                AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, title,
                                                  message, buttonText);

            }
			 
		}
 
 
        // Copy the input file the all of the outputs
        bool ok = loadFile.copyFileTo ( tempInputFile );
        if(!ok)
		{
            
            String title("ERROR");
            String message("Could not load the file. Please restart and try again.");
            String buttonText("Dismiss");
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, title,
                                              message, buttonText);
            
 
			return false;
		}
        
        SeparationEngine::getInstance()->setupAndLaunchEngine(tempInputFile,
                                                              SeparationEngine::StandardMode);
    
        multiPaintView->createNewProject(tempInputFile);
        ProgramSettings::activeMode  = true;
        changed();
        return true;
    }
    return false;
}

void MainContentComp::modalStateFinished (int returnValue)
{
    if(returnValue==1)
        JUCEApplication::quit();
}
    
void MainContentComp::quit()
{
 
    FileBasedDocument::SaveResult  res = saveIfNeededAndUserAgrees ();
    
    if(res == FileBasedDocument::failedToWriteToFile)
    {
        const String title("WARNING");
        const String message("Could not save project");
        const String button1Text("Quit");
        const String button2Text("Cancel");
        
        AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,  title,
                                      message, button1Text, button2Text, NULL, this);
    }
    else if(res == FileBasedDocument::userCancelledSave)
        return;
    else if(res==FileBasedDocument::savedOk)
    {}
    
    JUCEApplication::quit();
}
 

// This method must return a list of the names of the menus.
StringArray MainContentComp::getMenuBarNames ()
{
    const char* const names[] = { "File", "Edit", "Process", "View", "Transport", "Help", nullptr };
    return StringArray (names);
}

void 	MainContentComp::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{}

 
ApplicationCommandTarget* MainContentComp::getNextCommandTarget()
{
    // this will return the next parent component that is an ApplicationCommandTarget (in this
    // case, there probably isn't one, but it's best to use this method in your own apps).
    return multiPaintView;
}


// This should return the popup menu to display for a given top-level menu.
PopupMenu MainContentComp::getMenuForIndex (int menuIndex, const String &menuName)
{
    ApplicationCommandManager* commandManager = &MAIN_APP->commandManager;
    
    PopupMenu menu;
    
    
    // File
    if (menuIndex == 0)
    {
        menu.addCommandItem (commandManager, AppCommandIDs::NewProject);
        menu.addCommandItem (commandManager, AppCommandIDs::LoadProject);
        menu.addCommandItem (commandManager, AppCommandIDs::SaveProject);
        menu.addCommandItem (commandManager, AppCommandIDs::SaveProjectAs);
        menu.addCommandItem (commandManager, AppCommandIDs::ExportAudio);
        menu.addSeparator();
        menu.addCommandItem (commandManager, AppCommandIDs::OpenAudioSettings);
        //menu.addCommandItem (commandManager, AppCommandIDs::OpenPluginListEditor);
        menu.addSeparator();
        menu.addCommandItem (commandManager, AppCommandIDs::Quit);
    }
    // Edit
    else if (menuIndex == 1)
    {
        menu.addCommandItem (commandManager, AppCommandIDs::Undo);
        menu.addCommandItem (commandManager, AppCommandIDs::Redo);
    }
    // Process
    else if (menuIndex == 2)
    {
        menu.addCommandItem (commandManager, AppCommandIDs::Process);
        menu.addSeparator();
        menu.addCommandItem (commandManager, AppCommandIDs::ClearAllMasks);
        menu.addSeparator();
        menu.addCommandItem (commandManager, AppCommandIDs::OpenSeparationSettings);
    }
    // View
    else if (menuIndex == 3)
    {
        menu.addCommandItem (commandManager, AppCommandIDs::ZoomIn);
        menu.addCommandItem (commandManager, AppCommandIDs::ZoomOut);
        menu.addCommandItem (commandManager, AppCommandIDs::ZoomInHorizontal);
        menu.addCommandItem (commandManager, AppCommandIDs::ZoomOutHorizontal);
        menu.addCommandItem (commandManager, AppCommandIDs::ZoomInVertical);
        menu.addCommandItem (commandManager, AppCommandIDs::ZoomOutVertical);
        menu.addSeparator();
        menu.addCommandItem (commandManager, AppCommandIDs::OpenSpectrogramSettings);
        
    }
    // Transport
    else if (menuIndex == 4)
    {
        menu.addCommandItem (commandManager, AppCommandIDs::Stop);
        menu.addCommandItem (commandManager, AppCommandIDs::Play);
        menu.addCommandItem (commandManager, AppCommandIDs::Pause);
        menu.addCommandItem (commandManager, AppCommandIDs::Previous);
        menu.addCommandItem (commandManager, AppCommandIDs::Rewind);
        menu.addCommandItem (commandManager, AppCommandIDs::FastForward);
        menu.addCommandItem (commandManager, AppCommandIDs::Next);
        menu.addCommandItem (commandManager, AppCommandIDs::Loop);
        menu.addSeparator();
        menu.addCommandItem (commandManager, AppCommandIDs::AutoSolo);
        
    }
    // Help
    else if (menuIndex == 5)
    {
        menu.addCommandItem (commandManager, AppCommandIDs::OpenAboutPage);
    }
    
    return menu;
}

void MainContentComp::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    const String fileCategory("File");
    const String editCategory("Edit");
    const String processCategory("Process");
    const String viewCategory("View");
    const String helpCategory("Help");
    
    switch (commandID)
    {
        case AppCommandIDs::NewProject:
            result.setInfo ("New", "New", fileCategory, 0);
            result.addDefaultKeypress ('n', ModifierKeys::commandModifier);
            result.setActive( !ProgramSettings::processing && !ProgramSettings::playing);
            break;
        case AppCommandIDs::LoadProject:
            result.setInfo ("Open", "Open", fileCategory, 0);
            result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
            result.setActive( !ProgramSettings::processing && !ProgramSettings::playing);
            break;
        case AppCommandIDs::SaveProject:
            result.setInfo ("Save", "Save", fileCategory, 0);
            result.addDefaultKeypress ('s', ModifierKeys::commandModifier);
            result.setActive( ProgramSettings::activeMode && !ProgramSettings::processing && !ProgramSettings::playing );
            break;
//        case AppCommandIDs::CloseProject:
//            result.setInfo ("Close", "Close", fileCategory, 0);
//            result.addDefaultKeypress ('w', ModifierKeys::commandModifier);
//            result.setActive( ProgramSettings::activeMode && !ProgramSettings::processing  && !ProgramSettings::playing);
            break;
        case AppCommandIDs::SaveProjectAs:
            result.setInfo ("Save As", "Save As", fileCategory, 0);
            result.addDefaultKeypress ('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
            result.setActive( ProgramSettings::activeMode && !ProgramSettings::processing && !ProgramSettings::playing);
            break;
        case AppCommandIDs::ExportAudio:
            result.setInfo ("Export Audio", "Export Audio", fileCategory, 0);
            result.setActive( ProgramSettings::activeMode && !ProgramSettings::processing && !ProgramSettings::playing);
            break;
        case AppCommandIDs::OpenAudioSettings:
            result.setInfo ("Audio Settings", "Audio Settings", fileCategory, 0);
            result.setActive(!ProgramSettings::playing);
            break;
        case AppCommandIDs::OpenAboutPage:
        {
            result.setInfo("About", "About", "", 0);

            result.setActive(true);
            break;
        }
//        case AppCommandIDs::OpenPluginListEditor:
//            result.setInfo("Plugins...", "AU/VST Plugin Settings", fileCategory, 0);
//            result.setActive(true);
//            break;
            
        case AppCommandIDs::OpenSpectrogramSettings:
            result.setInfo ("Visualization Settings", "Visualization Settings", viewCategory, 0);
            break;
            
        case AppCommandIDs::OpenSeparationSettings:
            result.setInfo ("Process Settings", "Process  Settings", viewCategory, 0);
            result.setActive( ProgramSettings::activeMode  );
            break;
        case AppCommandIDs::Quit:
            result.setInfo ("Quit", "Quit", fileCategory, 0);
            result.addDefaultKeypress ('q', ModifierKeys::commandModifier);
            result.setActive( !ProgramSettings::processing && !ProgramSettings::playing);
            break;
        case AppCommandIDs::Undo:
            result.setInfo ("Undo", "Undo", editCategory, 0);
            result.addDefaultKeypress ('z', ModifierKeys::commandModifier);
            result.setActive( ProgramSettings::activeMode && MAIN_APP->undoManager.canUndo()  );
            break;            
        case AppCommandIDs::Redo:
            result.setInfo ("Redo", "Redo", editCategory, 0);
            result.addDefaultKeypress ('z', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
            result.setActive( ProgramSettings::activeMode && MAIN_APP->undoManager.canRedo() );
            break;
        case AppCommandIDs::OpenSingleTrack:
        {
            result.setInfo ("Open Single Track View", "Open Single Track View", viewCategory, 0); 
            break;
        }
        case AppCommandIDs::OpenMultiTrack:
        {
            result.setInfo ("Open MultiTrack View", "Open MultiTrack View", viewCategory, 0);
            break;
        }
        default:
            break;
    };
}


void 	MainContentComp::changeListenerCallback (ChangeBroadcaster *source)
{
    
    UndoManager* undoManager(dynamic_cast<UndoManager*>(source));
    if(undoManager)
        applicationCommandListChanged();
    
}

void MainContentComp::getAllCommands (Array <CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] =
    {
        AppCommandIDs::NewProject,
        AppCommandIDs::LoadProject,
        AppCommandIDs::SaveProject,
        AppCommandIDs::SaveProjectAs,
        AppCommandIDs::ExportAudio,
        AppCommandIDs::OpenAudioSettings,
//        AppCommandIDs::OpenPluginListEditor,
//        AppCommandIDs::CloseProject,
        AppCommandIDs::Quit,
        AppCommandIDs::Undo,
        AppCommandIDs::Redo,
        AppCommandIDs::OpenSeparationSettings,
        AppCommandIDs::OpenSpectrogramSettings,
        AppCommandIDs::OpenAboutPage,
        AppCommandIDs::OpenSingleTrack,
        AppCommandIDs::OpenMultiTrack
    };
    int numElems = numElementsInArray (ids);
    
    commands.addArray (ids, numElems);
}


void MainContentComp::exportAudio()
{
    const String dialogBoxTitle("Export Folder");
    const File initialFileOrDirectory=File::nonexistent;
    const String filePatternsAllowed="*.wav";
    bool useOSNativeDialogBox=true;
    
    FileChooser chooser(dialogBoxTitle, initialFileOrDirectory,
                        filePatternsAllowed, useOSNativeDialogBox);
    
    bool okay = chooser.browseForDirectory ();
    if(!okay)
        return;
    
    File saveFolder = chooser.getResult ();
    multiPaintView->exportAudio(saveFolder);
}


void MainContentComp::buttonClickedHelper (Button *button)
{
    ImgToolbarButton * b = dynamic_cast<ImgToolbarButton*>(button);
    
    if(b)
        b->getItemId();
    applicationCommandListChanged ();
}
 

void MainContentComp::buttonClicked (Button *button)
{
    buttonClickedHelper(button);
}


// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool MainContentComp::perform (const InvocationInfo& info)
{
    bool okay = true;
    switch (info.commandID)
    {
        case AppCommandIDs::NewProject:
        {
            FileBasedDocument::SaveResult res(FileBasedDocument::savedOk);
            
            if(ProgramSettings::activeMode)
                res = saveIfNeededAndUserAgrees ();
            
            
            if(res==FileBasedDocument::savedOk)
            {
            
                const String dialogBoxTitle("Open New Audio File");
                const File initialFileOrDirectory = File::nonexistent;
                const String filePatternsAllowed = "*.wav";
                bool useOSNativeDialogBox = true;
                    
                FileChooser chooser(dialogBoxTitle, initialFileOrDirectory,
                                    filePatternsAllowed, useOSNativeDialogBox);
                    
                if(chooser.browseForFileToOpen ())
                {
                    if(multiPaintView && multiPaintView->isProcessing() )
                    MAIN_APP->commandManager.invokeDirectly(AppCommandIDs::Process, false);
                         
                    File loadFile  = chooser.getResult ();
                    
                    if(!loadFile.hasFileExtension(".wav"))
                    {
                        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "ERROR", "File must have a .wav extension...");
                        return true;
                    }
                    else
                        createNewProject( loadFile, true );
                }
            }
            
            break;
        }
        case AppCommandIDs::OpenSingleTrack:
        {
            
            tabbedComponent->setCurrentTabIndex(1);
            multiTrack->setSingleTrackMode(true);
            break;
        }
        case AppCommandIDs::OpenMultiTrack:
        {
            tabbedComponent->setCurrentTabIndex(0);
            multiTrack->setSingleTrackMode(false);
            break;
        }
        case AppCommandIDs::LoadProject:
        {
            FileBasedDocument::SaveResult res = saveIfNeededAndUserAgrees ();
            if(res==FileBasedDocument::savedOk)
            {
                if(multiPaintView && multiPaintView->isProcessing() )
                    MAIN_APP->commandManager.invokeDirectly(AppCommandIDs::Process, false);

                
                loadFromUserSpecifiedFile (true);
            }
           
            break;
        }
        case AppCommandIDs::OpenAboutPage:
        {
            int index(-1);
            bool hasAbout(false);
            StringArray array = tabbedComponent->getTabNames ();
            for(int i=0; i < array.size(); ++i)
            {
                if(array[i].compare("About")==0)
                {
                    index = i;
                    hasAbout = true;
                    
                    break;
                }
            }
            
            if( !hasAbout )
            {
                aboutPage->setVisible(true);
                tabbedComponent->addTab("About", ProgramSettings::mainBackgroundColor, aboutPage, false);
                tabbedComponent->setCurrentTabIndex(tabbedComponent->getNumTabs()-1);
                aboutPage->setBounds(0, multiPaintView->getY(), getWidth(), getHeight());
            }
            else
            {
                tabbedComponent->setCurrentTabIndex(index);
            }
            
            break;
        }
        case AppCommandIDs::SaveProject:
        {
            save (true, true);
            break;
        }
//        case AppCommandIDs::CloseProject:
//        {
//            saveIfNeededAndUserAgrees ();
//            break;
//        }
        case AppCommandIDs::SaveProjectAs:
        {
            saveAsInteractive (true);
            break;
        }
        case AppCommandIDs::OpenSeparationSettings:
        {
            StringArray array = tabbedComponent->getTabNames ();
            for(int i=0; i < array.size(); ++i)
            {
                if(array[i].compare("Separation Settings")==0)
                {
                   tabbedComponent->setCurrentTabIndex(i);
                    break;
                }
            }
            
            break;
        }
        case AppCommandIDs::OpenAudioSettings:
        {
            MAIN_APP->openAudioSettings();
            break;
        }
//        case AppCommandIDs::OpenPluginListEditor:
//        {
//            MAIN_APP->openPluginListEditor();            
//            break;
//        }
        case AppCommandIDs::Quit:
        {
            quit();
            break;
        }
        case AppCommandIDs::Undo:
        {
//            std::cout << "Undo Space:" << MAIN_APP->undoManager.getNumberOfUnitsTakenUpByStoredCommands() << std::endl;
            
            MAIN_APP->undoManager.undo();
//            logSimpleEvent("Undo");
            
            break;
        }
        case AppCommandIDs::Redo:
        {
//            std::cout << "Redo Space:" << MAIN_APP->undoManager.getNumberOfUnitsTakenUpByStoredCommands() << std::endl;
            MAIN_APP->undoManager.redo();
//            logSimpleEvent("Redo");
            
            break;
        }
        case AppCommandIDs::ExportAudio:
        {
            exportAudio();
//            logSimpleEvent("ExportAudio");
            break;
        }
        case AppCommandIDs::OpenSpectrogramSettings:
        {
            ProgramSettings settings;
            SpectrogramSettings::openSettingsAsModalDialog(multiTrack, settings);
            break;
        }
        default:
            okay = false;
    };
    
    
    if(okay)
        applicationCommandListChanged();
 
    
    return okay;
}

