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
#include "JuceHeader.h"
#include "ProgramSettings.h"
#include "DocWindow.h"
#include "Main.h"
#include "VirtualTestUser.h"
#include "ISSLogger.h"
#include "MainContentComp.h"
#include "SeparationEngine.h"
#include "plugin/PluginListWindow.h"


class ISSLookAndFeel : public LookAndFeel_V2
{
public:
	
    ISSLookAndFeel()
    {
        setUsingNativeAlertWindows(true);
    }
 #ifdef __MACOSX_CORE__
//    virtual void fillResizableWindowBackground (Graphics &g, int w, int h, const BorderSize< int > &, ResizableWindow &window)
//    {
//        g.setColour(ProgramSettings::mainBackgroundColor);
//        g.fillAll();
//    }
    
//    virtual void drawLabel	(Graphics & g, Label & l)
//    {
//        l.setColour(Label::textColourId, ProgramSettings::textColour);
//        LookAndFeel::drawLabel(g, l);
//    }
    

    
    virtual void drawPopupMenuBackground	(	Graphics & g	,
                                                         int 	width,
                                                         int 	height 
                                                         )
    {

        
        g.setColour(ProgramSettings::mainBackgroundColor);
        g.fillAll();
    }
    
    // TODO: Fix color
//    virtual void 	drawPopupMenuItem (Graphics & g, int width, int height,
//                                       bool isSeparator, bool isActive,
//                                       bool isHighlighted, bool isTicked,
//                                       bool hasSubMenu, const String &text,
//                                       const String &shortcutKeyText,
//                                       Image *image, const Colour *const textColour)
//    {
//        
//        Colour c(ProgramSettings::textColour);
//        LookAndFeel::drawPopupMenuItem(g, width, height, isSeparator,
//                                       isActive, isHighlighted, isTicked, hasSubMenu, text,
//                                       shortcutKeyText, image, &c);
//    }
    
#endif
    
};

using namespace juce; 

ISSApplication::ISSApplication() 
{    }

const String ISSApplication::getApplicationName()       { return ProjectInfo::projectName; }
const String ISSApplication::getApplicationVersion()    { return ProjectInfo::versionString; }
bool ISSApplication::moreThanOneInstanceAllowed()       { return true; }


//==============================================================================
void ISSApplication::initialise (const String& commandLine)
{
    lookAndFeel = new ISSLookAndFeel();
    
    LookAndFeel::setDefaultLookAndFeel(lookAndFeel);
    
    PropertiesFile::Options options;
    options.applicationName     = "Juce Audio Plugin Host";
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Preferences";
    
    appProperties = new ApplicationProperties();
    appProperties->setStorageParameters (options);

    
    //Plugin Stuff
    pluginFormatManager.addDefaultFormats();
    ScopedPointer<XmlElement> savedPluginList (appProperties->getUserSettings()->getXmlValue ("pluginList"));
    
    if (savedPluginList != nullptr)
        knownPluginList.recreateFromXml (*savedPluginList);
    
    pluginSortMethod = (KnownPluginList::SortMethod) appProperties->getUserSettings()
                ->getIntValue ("pluginSortMethod", KnownPluginList::sortByManufacturer);
    
    
#ifndef __UNIX__
    // Start Audio I/O
    ScopedPointer<XmlElement> savedAudioState (appProperties->getUserSettings()->getXmlValue ("audioDeviceState"));
    audioDeviceManager.initialise (0, 2, savedAudioState, true);
    
#else
    audioDeviceManager.initialise (0, 2, nullptr, true);
    
#endif
    
    
    
    formatManager.registerBasicFormats();
    
//    toneSource.setAmplitude(0.0);
//    toneSourcePlayer.setSource(&toneSource);
//    MAIN_APP->audioDeviceManager.addAudioCallback (&toneSourcePlayer);
    
    // Add your application's initialisation code here..
    ProgramSettings::init();
    ProgramSettings::load();
    
    
    // Do your application's initialisation code here..
    window = new DocWindow();
    
    
    
}

//void ISSApplication::launchVirtualUser(juce::File logfile, VirtualUserHost * host)
//{
//    
//    if(virtualUser)
//    {
//        virtualUser->stop();
//        virtualUser = NULL;
//    }
//    virtualUser = new VirtualRealTimeUser(host, logfile);
//    virtualUser->startThread();
//}


void ISSApplication::openAudioSettings()
{
    ScopedPointer<AudioDeviceSelectorComponent> audioSettingsComp( new  AudioDeviceSelectorComponent(audioDeviceManager,
                                                    0, 256,
                                                    0, 256,
                                                    true, true, true, false));
    audioSettingsComp->setSize (500, 450);
    
//    audioSettingsComp.setColour(ListBox::textColourId, ProgramSettings::textColour);
    audioSettingsComp->setColour(TextEditor::highlightColourId, Colours::lightgrey);
    
    
    DialogWindow::LaunchOptions o;
    o.content.setOwned (audioSettingsComp.release());
    o.dialogTitle                   = "Audio Settings";
    o.componentToCentreAround       = window;
    o.dialogBackgroundColour        = Colours::lightgrey;
    o.escapeKeyTriggersCloseButton  = true;
    o.useNativeTitleBar             = true;
    o.resizable                     = false;

    o.launchAsync();

}

void ISSApplication::documentWindowClosed(DocumentWindow * w)
{
    pluginListWindow = nullptr;
    
    MenuBarModel * m = dynamic_cast<MenuBarModel *>(window->getContentComponent());
    
#ifdef __MACOSX_CORE__
    MenuBarModel::setMacMainMenu(m);
#endif
    commandManager.commandStatusChanged();
}

void ISSApplication::openPluginListEditor()
{

//    if (pluginListWindow == nullptr)
//        pluginListWindow = new PluginListWindow (*this, pluginFormatManager);
//    
//    pluginListWindow->toFront (true);
//    pluginListWindow->setVisible(true);
    
}

void ISSApplication::shutdown()
{
    ScopedPointer<XmlElement> audioState (audioDeviceManager.createStateXml());
    appProperties->getUserSettings()->setValue ("audioDeviceState", audioState);
    appProperties->getUserSettings()->saveIfNeeded();
    
    
    ProgramSettings::save();
    
    // Add your application's shutdown code here..
    appProperties->closeFiles();
    
    deleteAndZero (appProperties);
    
    
    SeparationEngine::deleteInstance();
    
    File tempDir = File::getSpecialLocation( File::tempDirectory );
    tempDir.deleteRecursively ();
    
    
    
#ifdef __MACOSX_CORE__
    Desktop & desktop(Desktop::getInstance());

    
    for(int i=0; i < desktop.getNumComponents(); ++i)
    {
        Component * comp(desktop.getComponent(i));
            
        if(comp)
        {
            comp->removeFromDesktop();
            if(dynamic_cast<DialogWindow*>(comp))
            {
                deleteAndZero(comp);
            }
        }
    }
#endif
    
    
}

//==============================================================================
void ISSApplication::systemRequestedQuit()
{
    // This is called when the app is being asked to quit: you can ignore this
    // request and let the app carry on running, or call quit() to allow the app to close.
    
    if(window)
        window->closeButtonPressed();
    else
        quit();
}

void ISSApplication::anotherInstanceStarted (const String& commandLine)
{
    // When another instance of the app is launched while this one is running,
    // this method is invoked, and the commandLine parameter tells you what
    // the other instance's command-line arguments were.
    
    String filename(commandLine);
    if(filename[0] =='"')
    {
        filename = commandLine.substring(1,commandLine.length()-1);
    }
    
    File file(filename);
        
    String ext(file.getFileExtension());
   
    MainContentComp * comp = dynamic_cast<MainContentComp*>(window->getContentComponent());
    
    if(ext.compare(".wav")==0)
    {
        if(comp)
            comp->createNewProject( file, true );
    }
    else if(ext.compare(".isse")==0)
    {
        if(comp)
            comp->loadDocument(file);
    }
    

    
    
}
    
//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (ISSApplication)

