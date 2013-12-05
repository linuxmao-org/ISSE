/*
 Copyright 2013 Stanford University
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
#include "../JuceLibraryCode/JuceHeader.h"
#include "TransportComp.h"

#include "ImgBinaryData.h"
#include "ProgramSettings.h"
#include "AppCommandIDs.h"
#include "Main.h"


//==============================================================================
TransportComp::TransportComp() :    playButton(new ImageButton("Play")),
                                    pauseButton(new ImageButton("Pause")),
                                    stopButton(new ImageButton("Stop")),
                                    loopButton(new ImageButton("Loop")),
                                    rewindButton(new ImageButton("Rewind")),
                                    fastForwardButton(new ImageButton("FastForward")),
                                    nextButton(new ImageButton("Next")),
                                    previousButton(new ImageButton("Previous")),
                                    autoSoloButton(new ImageButton("AutoSolo")),
                                    buttonWidth(50), buttonHeight(50)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    String labelText = "Play";
    char* icon_png(nullptr);
    int icon_pngSize(0);
    
 
    float imageOpacityWhenNormal(1.f);
    Colour overlayColourWhenNormal(ProgramSettings::iconColor);
    float imageOpacityWhenOver(1.0f);
    Colour overlayColourWhenOver(ProgramSettings::iconColorOver);
    float imageOpacityWhenDown(1.0f);
    Colour overlayColourWhenDown(ProgramSettings::iconColorDown);
    
    // Stop button
    icon_png = (char*)ImgBinaryData::stopicon_png;
    icon_pngSize = (int)ImgBinaryData::stopicon_pngSize;
    stopButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenNormal,
                           overlayColourWhenNormal,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenOver,
                           overlayColourWhenOver,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenDown,
                           overlayColourWhenDown);
    
    // Play button
    icon_png = (char*)ImgBinaryData::playicon_png;
    icon_pngSize = (int)ImgBinaryData::playicon_pngSize;
    playButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenNormal,
                           overlayColourWhenNormal,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenOver,
                           overlayColourWhenOver,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenDown,
                           overlayColourWhenDown);
    
    
    // Pause button
    icon_png = (char*)ImgBinaryData::pauseicon_png;
    icon_pngSize = (int)ImgBinaryData::pauseicon_pngSize;
    pauseButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenNormal,
                           overlayColourWhenNormal,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenOver,
                           overlayColourWhenOver,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenDown,
                           overlayColourWhenDown);
    
    // previous button
    icon_png = (char*)ImgBinaryData::movepreviousicon_png;
    icon_pngSize = (int)ImgBinaryData::movepreviousicon_pngSize;
    previousButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenNormal,
                           overlayColourWhenNormal,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenOver,
                           overlayColourWhenOver,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenDown,
                           overlayColourWhenDown);
    
    // rewind button
    icon_png = (char*)ImgBinaryData::rewindicon_png;
    icon_pngSize = (int)ImgBinaryData::rewindicon_pngSize;
    rewindButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenNormal,
                               overlayColourWhenNormal,
                               ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenOver,
                               overlayColourWhenOver,
                               ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenDown,
                               overlayColourWhenDown);
    
    // fastforward button
    icon_png = (char*)ImgBinaryData::fastforwardicon_png;
    icon_pngSize = (int)ImgBinaryData::fastforwardicon_pngSize;
    fastForwardButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenNormal,
                               overlayColourWhenNormal,
                               ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenOver,
                               overlayColourWhenOver,
                               ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenDown,
                               overlayColourWhenDown);
    
    // next button
    icon_png = (char*)ImgBinaryData::movenexticon_png;
    icon_pngSize = (int)ImgBinaryData::movenexticon_pngSize;
    nextButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenNormal,
                               overlayColourWhenNormal,
                               ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenOver,
                               overlayColourWhenOver,
                               ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenDown,
                               overlayColourWhenDown);
    
    // loop button
    icon_png = (char*)ImgBinaryData::loopicon_png;
    icon_pngSize = (int)ImgBinaryData::loopicon_pngSize;
    loopButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenNormal,
                               overlayColourWhenNormal,
                               ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenOver,
                               overlayColourWhenOver,
                               ImageCache::getFromMemory (icon_png, icon_pngSize ),
                               imageOpacityWhenDown,
                               overlayColourWhenDown);
    
    // autosolo button
    icon_png = (char*)ImgBinaryData::autosoloicon_png;
    icon_pngSize = (int)ImgBinaryData::autosoloicon_pngSize;
    autoSoloButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenNormal,
                           overlayColourWhenNormal,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenOver,
                           overlayColourWhenOver,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenDown,
                           overlayColourWhenDown);
    
 
    
    
    int startPosition((getWidth() - 8.0*buttonWidth)/2.0);
    
    stopButton->setBounds(startPosition, 0, 50, 50);
    playButton->setBounds(stopButton->getRight(),  0,buttonWidth,buttonHeight);
    pauseButton->setBounds(playButton->getRight(), 0, buttonWidth, buttonHeight);
    previousButton->setBounds(pauseButton->getRight(), 0, buttonWidth, buttonHeight);
    rewindButton->setBounds(previousButton->getRight(), 0, buttonWidth, buttonHeight);
    fastForwardButton->setBounds(rewindButton->getRight(), 0, buttonWidth, buttonHeight);
    nextButton->setBounds(fastForwardButton->getRight(), 0, buttonWidth, buttonHeight);
    loopButton->setBounds(nextButton->getRight(), 0, buttonWidth, buttonHeight);
    autoSoloButton->setBounds(loopButton->getRight()+2*buttonWidth, 0, buttonWidth, buttonHeight);
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(rewindButton);
    addAndMakeVisible(fastForwardButton);
    addAndMakeVisible(nextButton);
    addAndMakeVisible(previousButton);
    addAndMakeVisible(autoSoloButton);
    
    stopButton->addListener(this);
    playButton->addListener(this);
    pauseButton->addListener(this);
    previousButton->addListener(this);
    rewindButton->addListener(this);
    fastForwardButton->addListener(this);
    nextButton->addListener(this);
    loopButton->addListener(this);
    autoSoloButton->addListener(this);
    
    stopButton->setTooltip("Stop");
    playButton->setTooltip("Play");
    pauseButton->setTooltip("Pause");
    previousButton->setTooltip("Previous");
    rewindButton->setTooltip("Rewind");
    fastForwardButton->setTooltip("Fast Forward");
    nextButton->setTooltip("Next");
    loopButton->setTooltip("Loop");
    autoSoloButton->setTooltip("Auto Solo");
    
 
    playButton->addShortcut	(	KeyPress(KeyPress::spaceKey)	);
    previousButton->addShortcut( KeyPress(KeyPress::returnKey));

    
    loopButton->setToggleState( true,  dontSendNotification );
    stopButton->setToggleState( true, dontSendNotification );
    autoSoloButton->setToggleState( true, dontSendNotification);
    
    MAIN_APP->commandManager.registerAllCommandsForTarget (this);

}

TransportComp::~TransportComp()
{
    
}

void TransportComp::logSimpleEvent(String event)
{
    ISSLOG("<Event type=\"" + event + "\" time=\"" + ISSTIME + "\"></Event>");
}

void TransportComp::addListener( TransportComp::Listener * listener )
{
    listeners.add(listener);
}

void TransportComp::removeListener( TransportComp::Listener * listener )
{
    listeners.remove(listener);
}

void TransportComp::buttonClicked (Button * button)
{
    if(button==playButton)
    {
        playButton->setToggleState( true, dontSendNotification  );
        pauseButton->setToggleState(false, dontSendNotification );
        stopButton->setToggleState(false, dontSendNotification );
        
        listeners.call(&TransportComp::Listener::updatedTransportControl, this, Listener::Play);
    }
    else if(button==pauseButton)
    {
        
        pauseButton->setToggleState( !pauseButton->getToggleState(), dontSendNotification  );

        listeners.call(&TransportComp::Listener::updatedTransportControl, this,  Listener::Pause);
    }
    else if(button==stopButton)
    {
        pauseButton->setToggleState(false, dontSendNotification );
        stopButton->setToggleState(true, dontSendNotification );
        playButton->setToggleState(false, dontSendNotification );
        listeners.call(&TransportComp::Listener::updatedTransportControl, this, Listener::Stop);
    }
    else if(button==loopButton)
    {
        loopButton->setToggleState( !loopButton->getToggleState(), dontSendNotification  );
        listeners.call(&TransportComp::Listener::updatedTransportControl, this, Listener::Loop);
        logSimpleEvent("Loop");
    }
    else if(button==rewindButton)
    {
        
        listeners.call(&TransportComp::Listener::updatedTransportControl, this, Listener::Rewind);
    }
    else if(button==fastForwardButton)
    {
        
        listeners.call(&TransportComp::Listener::updatedTransportControl, this, Listener::FastForward);
    }
    else if(button==nextButton)
    {
        
        listeners.call(&TransportComp::Listener::updatedTransportControl, this, Listener::Next);
    }
    else if(button==previousButton)
    {
        listeners.call(&TransportComp::Listener::updatedTransportControl, this, Listener::Previous);
    }
    else if(button==autoSoloButton)
    {
        
        autoSoloButton->setToggleState( !autoSoloButton->getToggleState(), dontSendNotification  );
        listeners.call(&TransportComp::Listener::updatedTransportControl, this, Listener::AutoSolo);
        logSimpleEvent("AutoSolo");
    }
    
    playButton->clearShortcuts();
    pauseButton->clearShortcuts();
    
    if(isPaused() || stopButton->getToggleState())
        playButton->addShortcut	(	KeyPress(KeyPress::spaceKey)	);
    else
        pauseButton->addShortcut	(	KeyPress(KeyPress::spaceKey)	);
    
    ProgramSettings::playing = playButton->getToggleState() && !pauseButton->getToggleState();
    
  
   
    MenuBarModel * model = findParentComponentOfClass<MenuBarModel>	();
    if(model)
        model->applicationCommandListChanged();

}




void TransportComp::paint (Graphics& g)
{
    g.fillAll (ProgramSettings::mainBackgroundColor);   // clear the background
}

void TransportComp::reset()
{
    playButton->setToggleState( false,  dontSendNotification );
    pauseButton->setToggleState( false,  dontSendNotification );
    stopButton->setToggleState( true, dontSendNotification );
    loopButton->setToggleState( true,  dontSendNotification );
    fastForwardButton->setToggleState( false,  dontSendNotification );
    rewindButton->setToggleState( false,  dontSendNotification );
    nextButton->setToggleState( false,  dontSendNotification );
    previousButton->setToggleState( false,  dontSendNotification );
    autoSoloButton->setToggleState( true,  dontSendNotification );
    
   
    ProgramSettings::playing = playButton->getToggleState() && !pauseButton->getToggleState();
    
        
}

void TransportComp::resized()
{    
    int startPosition((getWidth() - 8.0*buttonWidth)/2.0);
    
    stopButton->setBounds(startPosition, 0, 50, 50);
    playButton->setBounds(stopButton->getRight(),  0,buttonWidth,buttonHeight);
    pauseButton->setBounds(playButton->getRight(), 0, buttonWidth, buttonHeight);
    previousButton->setBounds(pauseButton->getRight(), 0, buttonWidth, buttonHeight);
    rewindButton->setBounds(previousButton->getRight(), 0, buttonWidth, buttonHeight);
    fastForwardButton->setBounds(rewindButton->getRight(), 0, buttonWidth, buttonHeight);
    nextButton->setBounds(fastForwardButton->getRight(), 0, buttonWidth, buttonHeight);
    loopButton->setBounds(nextButton->getRight(), 0, buttonWidth, buttonHeight);
    autoSoloButton->setBounds(loopButton->getRight()+2*buttonWidth, 0, buttonWidth, buttonHeight);

}


ApplicationCommandTarget* TransportComp::getNextCommandTarget()
{
    // this will return the next parent component that is an ApplicationCommandTarget (in this
    // case, there probably isn't one, but it's best to use this method in your own apps).
    return findFirstTargetParentComponent();
}


void TransportComp::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    const String fileCategory("File");
    const String editCategory("Edit");
    const String processCategory("Process");
    const String viewCategory("View");
    const String transCategory("Transport");
    const String helpCategory("Help");
    
    switch (commandID)
    {
        case AppCommandIDs::Stop:
        {
            result.setInfo ("Stop", "Stop", transCategory, 0);
            result.setTicked(stopButton->getToggleState());
            
            break;
        } 
        case AppCommandIDs::Play:
        {
            result.setInfo ("Play", "Play", transCategory, 0);
            result.setTicked(playButton->getToggleState());
            break;
        }
        case AppCommandIDs::Pause:
        {
            result.setInfo ("Pause", "Pause", transCategory, 0);
            result.setTicked(pauseButton->getToggleState());
            break;
        }
        case AppCommandIDs::Next:
        {
            result.setInfo ("Next", "Next", transCategory, 0);
            break;
        }
        case AppCommandIDs::Previous:
        {
            result.setInfo ("Previous", "Previous", transCategory, 0);
            break;
        }
        case AppCommandIDs::FastForward:
        {
            result.setInfo ("Fast Forward", "Fast Forward", transCategory, 0);
            break;
        }
        case AppCommandIDs::Rewind:
        {
            result.setInfo ("Rewind", "Rewind", transCategory, 0);
            break;
        }
        case AppCommandIDs::AutoSolo:
        {
            result.setInfo ("Auto Solo", "Auto Solo", transCategory, 0);
            result.setTicked(autoSoloButton->getToggleState());
            break;
        }
        case AppCommandIDs::Loop:
        {
            result.setInfo ("Loop", "Loop", transCategory, 0);
            result.setTicked(loopButton->getToggleState());
            
            break;
        }
  
        default:
            break;
    };
}


void TransportComp::getAllCommands (Array <CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] =
    {
      
        AppCommandIDs::Stop,
        AppCommandIDs::Play,
        AppCommandIDs::Pause,
        AppCommandIDs::Next,
        AppCommandIDs::Previous,
        AppCommandIDs::FastForward,
        AppCommandIDs::Rewind,
        AppCommandIDs::Loop,
        AppCommandIDs::AutoSolo
    };
    int numElems = numElementsInArray (ids);
    
    commands.addArray (ids, numElems);
}


// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool TransportComp::perform (const InvocationInfo& info)
{
    bool okay = true;
    switch (info.commandID)
    {
        case AppCommandIDs::Stop:
        {
            stopButton->triggerClick();
            break;
        }
        case AppCommandIDs::Play:
        {
            playButton->triggerClick();
            break;
        }
        case AppCommandIDs::Pause:
        {
            pauseButton->triggerClick();
            break;
        }
        case AppCommandIDs::Next:
        {
            nextButton->triggerClick();
            break;
        }
        case AppCommandIDs::Previous:
        {
            previousButton->triggerClick();
            break;
        }
        case AppCommandIDs::FastForward:
        {
            fastForwardButton->triggerClick();
            break;
        }
        case AppCommandIDs::Rewind:
        {
            rewindButton->triggerClick();
            break;
        }
        case AppCommandIDs::AutoSolo:
        {
            autoSoloButton->triggerClick();
            break;
        }
        case AppCommandIDs::Loop:
        {
            loopButton->triggerClick();
            break;
        }
        default:
            okay = false;
    };
    
    
  
  
    //    if(okay)
    //        applicationCommandListChanged();
    
    
    return okay;
}



