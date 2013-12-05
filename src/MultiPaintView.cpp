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

#include "JuceHeader.h"
#include "MultiPaintView.h"
 
 

#include "ProgramSettings.h"
#include "ToolbarItemFact.h"
#include "BrushPanel.h"
//#include "SeparationSettings.h"
#include "SpectrogramSettings.h"
#include "Main.h"


#include "AppCommandIDs.h"
#include "ImgToolbarButton.h"
#include "ProgramSettings.h"


#include "ToolbarState.h"
#include "Timeline.h"
#include "ThreadHelpers.h"
#include "ISSLogger.h"
#include "ToolbarSlider.h"

#include "MultiTrack.h"
#include "AboutPage.h"
#include "TransportComp.h"
#include "MultiPaintView.h"
#include "MultiTrack.h"


#include <iostream>
#include <algorithm>
#include <map>
#include <iostream>
#include <sstream>



//const int MultiPaintView::toolbarHeight = 30;
const int MultiPaintView::transportHeight = 50;


Colour callOutColorSelector(Colour c, Rectangle<int> r, Component * parent)
{
    ColourSelector colourSelector( (ColourSelector::showColourAtTop |
                                    ColourSelector::showSliders |
                                    ColourSelector::showColourspace) );
    
    colourSelector.setCurrentColour (c);
    colourSelector.setColour (ColourSelector::backgroundColourId,
                              Colours::transparentBlack);
    colourSelector.setSize (300, 400);
    r.setY( r.getY() + 5);
    CallOutBox callOut (colourSelector, r, parent);
    callOut.runModalLoop();
    
    return colourSelector.getCurrentColour();
}





MultiPaintView::MultiPaintView( Toolbar * toolbar_,
                                MultiTrack * multiTrack_,
                                TransportComp * transportComp_)  :
                                toolbar(toolbar_),
                                multiTrack( multiTrack_ ),
                                transportComp( transportComp_)
                                
{
    // Set dimensions

#ifdef __IOS_CORE__
//    setSize(x,y);
#else
    Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    int x = r.getWidth();
    int y = r.getHeight();
    setSize(.75*x,.75*y);
#endif
    
    int numItems = toolbar->getNumItems();
    for(int i=0; i < numItems;++i)
    {
        int itemId = toolbar->getItemId(i);
        ToolbarItemComponent * comp = (toolbar->getItemComponent(i));
        if(comp)
            toolbarItem2IndexMap[itemId] = comp;
    }
    addAndMakeVisible(toolbar);
    
 
    multiTrack->setBounds(0, 0, getWidth(), getHeight() - transportHeight);
    addAndMakeVisible(multiTrack);
    
    transportComp->setBounds(0, getHeight()-transportHeight, getWidth(), transportHeight );
    addAndMakeVisible(transportComp);
    
    transportComp->addListener(multiTrack);
    
    resetState();
    
    MAIN_APP->commandManager.registerAllCommandsForTarget (this);
    
}


void MultiPaintView::exportAudio( File & file )
{
    if(multiTrack)
        multiTrack->exportAudio(file);
    
}

bool MultiPaintView::isProcessing()
{
    return toolbarItem2IndexMap[ AppCommandIDs::Process ]->getToggleState();
}

void MultiPaintView::logSimpleEvent(String event)
{
    ISSLOG("<Event type=\"" + event + "\" time=\"" + ISSTIME + "\"></Event>");
}



void MultiPaintView::resetState()
{
    // Click the arrow selector
    toolbarItem2IndexMap[ AppCommandIDs::ArrowSelector ]->setToggleState(true,  dontSendNotification);
    
    // Set the state of the buttons
    toolbarItem2IndexMap[ AppCommandIDs::PaintMode ]->setToggleState( true,  dontSendNotification );
    toolbarItem2IndexMap[ AppCommandIDs::SourceColor1 ]->setToggleState( true,  dontSendNotification );
    
    ToolbarState::erase = false;
    ToolbarState::autoSolo = true;
    ToolbarState::selector = ToolbarState::ArrowSelector;
    
    toolbarItem2IndexMap[ AppCommandIDs::Process ]->setToggleState( false, sendNotification );
    
//    ToolbarSlider * slider = dynamic_cast<ToolbarSlider*>(toolbarItem2IndexMap[AppCommandIDs::ChangeBrushStrength]);
//    if(slider)
//        slider->setValue(.5);
    
    // Repaint everything
    repaint();
    
}

MultiPaintView::~MultiPaintView()
{}

void MultiPaintView::createNewProject( File file )
{
    resetState();
    if(multiTrack)
        multiTrack->createNewProject( file );
}

void MultiPaintView::sliderValueChanged (Slider *slider)
{
    switch(slider->getName().getIntValue ())
    {
        case AppCommandIDs::ChangeBrushStrength:
        {
            ProgramSettings::brushStrength = slider->getValue();
            
            ISSLOG("<Event type=\"ChangeBrushStrength\" time=\"" + ISSTIME + "\">" +
                   String(ProgramSettings::brushStrength) + "</Event>");
            break;
        }
        case AppCommandIDs::ChangeVolume:
        {
            
            double sv(slider->getValue());
            double value = pow(10.0,sv/20.);
 
            if(multiTrack)
                multiTrack->setGain( value );

        }
            
            break;
        default:
            break;
    }
}

void MultiPaintView::sourceColourButtonHit( int source )
{
    ImgToolbarButton * button = dynamic_cast<ImgToolbarButton *>
    (toolbarItem2IndexMap[AppCommandIDs::SourceColor1]);
    if(source==1)
        button = dynamic_cast<ImgToolbarButton *>
        (toolbarItem2IndexMap[AppCommandIDs::SourceColor2]);
    
    button->setToggleState( true, dontSendNotification  );
    button->repaint();
    
    multiTrack->setSourceToPaint( source );
    
    // Color menu
    if( button->isDown()  &&
       (button->getMillisecondsSinceButtonDown() > ProgramSettings::iconButtonTimeOutMs-200 ) )
    {
        
        Colour c = callOutColorSelector(button->getOverlayColour(), button->getBounds(), this);
        
        button->setOverlayColour(c);
        
        ProgramSettings::sourceColors[source] = c;
        
        repaint();
        
        ISSLOG("<Event type=\"ChangeSourceColor\" time=\"" + ISSTIME +
               "\">\n\t<Source>1</Source>\n\t<Color>" + c.toString() +
               "</Color>\n</Event>");
    }
    else
        ISSLOG("<Event type=\"SourceColor" + String(source+1) + "\" time=\"" + ISSTIME + "\"></Event>");
}



void MultiPaintView::resized()
{
    int height = getHeight();
    int width = getWidth();
    
 
    if(toolbar)
        toolbar->setBounds(0, 0, width, toolbar->getHeight() );
 
    
    
    if(multiTrack && toolbar)
    {
        int mheight = height - transportHeight - toolbar->getHeight();
        multiTrack->setBounds(0,toolbar->getBottom(), width, mheight);
    }
    
    if(transportComp)
        transportComp->setBounds(0, getHeight()-transportHeight, getWidth(), transportHeight );
    
    repaint();
}



void MultiPaintView::zoomInHorizontal()
{
    
    multiTrack->zoomInHorizontal();
    
}

void MultiPaintView::zoomOutHorizontal()
{
    
    multiTrack->zoomOutHorizontal();
    
}

void MultiPaintView::zoomInVertical()
{
    multiTrack->zoomVertical( true );
    
}

void MultiPaintView::zoomOutVertical()
{
    multiTrack->zoomVertical( false );
}

void MultiPaintView::zoomIn()
{
    
    multiTrack->zoomIn();
    
}

void MultiPaintView::zoomOut()
{
    multiTrack->zoomOut();
}

bool MultiPaintView::saveToXML( XmlElement & xml, const File & folderForAssets )
{
//    std::cout << "saving xml:" << folderForAssets.getFullPathName() << std::endl;
    
    multiTrack->saveToXML(xml, folderForAssets);
    return true;
}


bool MultiPaintView::loadFromXML( const XmlElement & xml, const File & assetsFolder  )
{
    
    return multiTrack->loadFromXML(xml, assetsFolder);
}

bool MultiPaintView::createNewProject( File loadFile, bool async )
{
    multiTrack->createNewProject( loadFile );
    return true;
}

void MultiPaintView::modalStateFinished (int returnValue)
{
    if(returnValue==1)
        JUCEApplication::quit();
}
 

 
 

ApplicationCommandTarget* MultiPaintView::getNextCommandTarget()
{
    // this will return the next parent component that is an ApplicationCommandTarget (in this
    // case, there probably isn't one, but it's best to use this method in your own apps).
    return transportComp;
}


 void MultiPaintView::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    const String fileCategory("File");
    const String editCategory("Edit");
    const String processCategory("Process");
    const String viewCategory("View");
    const String transCategory("Transport");
    const String helpCategory("Help");
    
    switch (commandID)
    {
        case AppCommandIDs::ZoomInHorizontal:
            result.setInfo ("Zoom In Horizontral", "Horizontral Zoom In", viewCategory, 0);
            result.addDefaultKeypress ('=', ModifierKeys::noModifiers );
            break;
            
        case AppCommandIDs::ZoomOutHorizontal:
            result.setInfo ("Zoom Out Horizontral", "Zoom Out Horizontral", viewCategory, 0);
            result.addDefaultKeypress ('-', ModifierKeys::noModifiers );
            break;
            
        case AppCommandIDs::ZoomInVertical:
            result.setInfo ("Zoom In Vertical", "Zoom In Vertical", viewCategory, 0);
            result.addDefaultKeypress ('+', ModifierKeys::shiftModifier );
            break;
            
        case AppCommandIDs::ZoomOutVertical:
            result.setInfo ("Zoom Out Vertical", "Zoom Out Vertical", viewCategory, 0);
            result.addDefaultKeypress ('_', ModifierKeys::shiftModifier );
            break;
            
        case AppCommandIDs::ZoomIn:
            result.setInfo ("Zoom In", "Zoom In", viewCategory, 0);
            result.addDefaultKeypress ('=', ModifierKeys::commandModifier );
            break;
            
        case AppCommandIDs::ZoomOut:
            result.setInfo ("Zoom Out", "Zoom Out", viewCategory, 0);
            result.addDefaultKeypress ('-', ModifierKeys::commandModifier );
            break;
            
        case AppCommandIDs::Process:
        {
            result.setInfo ("Process", "Process", processCategory, 0);
            result.addDefaultKeypress ('p', ModifierKeys::commandModifier );
            result.setActive( ProgramSettings::activeMode );
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::Process ]->getToggleState() );
            
            
            break;
        }
        case AppCommandIDs::ArrowSelector:
            result.setInfo ("Arrow Selector", "Arrow Selector", processCategory, 0);
            result.addDefaultKeypress ('1', 0);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::ArrowSelector ]->getToggleState() );
            break;
        case AppCommandIDs::BrushSelector:
            result.setInfo ("Brush Selector", "Brush Selector", processCategory, 0);
            result.addDefaultKeypress ('2', 0);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::BrushSelector ]->getToggleState() );
            break;
        case AppCommandIDs::TimeSelector:
            result.setInfo ("Time Selector", "Time Selector", processCategory, 0);
            result.addDefaultKeypress ('3', 0);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::TimeSelector ]->getToggleState() );
            break;
        case AppCommandIDs::FreqSelector:
            result.setInfo ("Frequency Selector", "Frequency Selector", processCategory, 0);
            result.addDefaultKeypress ('4', 0);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::FreqSelector ]->getToggleState() );
            break;
        case AppCommandIDs::BoxSelector:
            result.setInfo ("Box Selector", "Box Selector", processCategory, 0);
            result.addDefaultKeypress ('5', 0);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::BoxSelector ]->getToggleState() );
            break;
        case AppCommandIDs::TrainingSelector:
            result.setInfo ("Training Selector", "Training Selector", processCategory, 0);
            result.addDefaultKeypress ('6', 0);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::TrainingSelector ]->getToggleState() );
            break;
        case AppCommandIDs::SourceColor1:
            result.setInfo ("Source 1 Select", "Source 1 Select", processCategory, 0);
            result.addDefaultKeypress ('1', ModifierKeys::commandModifier);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::SourceColor1 ]->getToggleState() );
            break;
        case AppCommandIDs::SourceColor2:
            result.setInfo ("Source 2 Select", "Source 2 Select", processCategory, 0);
            result.addDefaultKeypress ('2', ModifierKeys::commandModifier);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::SourceColor2 ]->getToggleState() );
            break;
        case AppCommandIDs::PaintMode:
            result.setInfo ("Paint Mode", "Paint Mode", processCategory, 0);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::PaintMode ]->getToggleState() );
            result.addDefaultKeypress ('e', ModifierKeys::commandModifier);
            if(toolbarItem2IndexMap[ AppCommandIDs::PaintMode ]->getToggleState() )
                result.setActive( false );
            else
                result.setActive( true );
            break;
        case AppCommandIDs::EraseMode:
            result.setInfo ("Erase Mode", "Erase Mode", processCategory, 0);
            result.setTicked( toolbarItem2IndexMap[ AppCommandIDs::EraseMode ]->getToggleState() );
            result.addDefaultKeypress ('e', ModifierKeys::commandModifier);
            
            if(toolbarItem2IndexMap[ AppCommandIDs::EraseMode ]->getToggleState() )
                result.setActive( false );
            else
                result.setActive( true );
            break;
        case AppCommandIDs::ClearAllMasks:
        {
            result.setInfo ("Clear Paintings", "Clear Paintings", processCategory, 0);
            result.setActive( ProgramSettings::activeMode );
            
        }
        default:
            break;
    };
}


void MultiPaintView::getAllCommands (Array <CommandID>& commands)
{
    // this returns the set of all commands that this target can perform..
    const CommandID ids[] =
    {
        AppCommandIDs::ZoomInHorizontal,
        AppCommandIDs::ZoomOutHorizontal,
        AppCommandIDs::ZoomInVertical,
        AppCommandIDs::ZoomOutVertical,
        AppCommandIDs::ZoomIn,
        AppCommandIDs::ZoomOut,
        
        AppCommandIDs::ArrowSelector,
        AppCommandIDs::TrainingSelector,
        AppCommandIDs::BrushSelector,
        AppCommandIDs::TimeSelector,
        AppCommandIDs::FreqSelector,
        AppCommandIDs::BoxSelector,
        
        AppCommandIDs::SourceColor1,
        AppCommandIDs::SourceColor2,
        
        AppCommandIDs::EraseMode,
        AppCommandIDs::PaintMode,
        
        AppCommandIDs::Process,
        AppCommandIDs::ClearAllMasks,
    };
    int numElems = numElementsInArray (ids);
    
    commands.addArray (ids, numElems);
}




void MultiPaintView::buttonClicked (Button *button)
{
    ImgToolbarButton * b = dynamic_cast<ImgToolbarButton*>(button);
    
    if(b)
    {
        int itemId = b->getItemId();
        if( itemId == AppCommandIDs::ArrowSelector )
        {
            ToolbarState::selector = ToolbarState::ArrowSelector;
            b->setToggleState( true, dontSendNotification  );
            logSimpleEvent("ArrowSelector");
        }
        else if( itemId == AppCommandIDs::FreqSelector)
        {
            ToolbarState::selector = ToolbarState::FreqSelector;
            b->setToggleState( true, dontSendNotification  );
            logSimpleEvent("FreqSelector");
        }
        else if( itemId == AppCommandIDs::TimeSelector)
        {
            ToolbarState::selector = ToolbarState::TimeSelector;
            b->setToggleState( true, dontSendNotification  );
            logSimpleEvent("TimeSelector");
        }
        else if( itemId == AppCommandIDs::BoxSelector)
        {
            ToolbarState::selector = ToolbarState::BoxSelector;
            b->setToggleState( true, dontSendNotification  );
            logSimpleEvent("BoxSelector");
        }
        else if( itemId == AppCommandIDs::TrainingSelector)
        {
            ToolbarState::selector = ToolbarState::TrainingSelector;
            b->setToggleState( true, dontSendNotification  );
            logSimpleEvent("TrainingSelector");
        }
        else if( itemId == AppCommandIDs::SoundPenSelector)
        {
            ToolbarState::selector = ToolbarState::SoundPenSelector;
            b->setToggleState( true, dontSendNotification  );
            logSimpleEvent("SoundPenSelector");
        }
        else if( itemId == AppCommandIDs::Process )
        {
            
            if(!ProgramSettings::activeMode)
                return;
            
            
            b->setToggleState( !b->getToggleState(), dontSendNotification );
            
            if(b->getToggleState())
                SeparationEngine::getInstance()->start();
            else
                SeparationEngine::getInstance()->stop();
            
            
            MenuBarModel * model = findParentComponentOfClass<MenuBarModel>();
            if(model)
                model->applicationCommandListChanged();
            MAIN_APP->commandManager.commandStatusChanged();
            
            
            logSimpleEvent("Process");
            
        }
        else if ( itemId == AppCommandIDs::EraseMode )
        {
            b->setToggleState( true, dontSendNotification );
            ToolbarState::paint = toolbarItem2IndexMap[ AppCommandIDs::PaintMode ]->getToggleState();
            ToolbarState::erase = toolbarItem2IndexMap[ AppCommandIDs::EraseMode ]->getToggleState();
            
            logSimpleEvent("EraseMode");
        }
        else if (itemId == AppCommandIDs::PaintMode)
        {
            b->setToggleState( true, dontSendNotification );
            ToolbarState::paint = toolbarItem2IndexMap[ AppCommandIDs::PaintMode ]->getToggleState();
            ToolbarState::erase = toolbarItem2IndexMap[ AppCommandIDs::EraseMode ]->getToggleState();
            logSimpleEvent("PaintMode");
        }
        else if( itemId == AppCommandIDs::ZoomInHorizontal )
        {
            zoomInHorizontal();
            logSimpleEvent("ZoomInHorizontal");
        }
        else if( itemId == AppCommandIDs::ZoomOutHorizontal )
        {
            zoomOutHorizontal();
            logSimpleEvent("ZoomOutHorizontal");
        }
        else if( itemId == AppCommandIDs::ZoomInVertical )
        {
            zoomInVertical();
            logSimpleEvent("ZoomInVertical");
        }
        else if( itemId == AppCommandIDs::ZoomOutVertical )
        {
            zoomOutVertical();
            logSimpleEvent("ZoomOutVertical");
        }
        else if( itemId == AppCommandIDs::ZoomIn )
        {
            zoomIn();
            logSimpleEvent("ZoomIn");
        }
        else if( itemId == AppCommandIDs::ZoomOut )
        {
            zoomOut();
            logSimpleEvent("ZoomOut");
        }
        else if( itemId==AppCommandIDs::SourceColor1 )
        {
            sourceColourButtonHit( 0 );
        }
        else if( itemId==AppCommandIDs::SourceColor2 )
        {
            sourceColourButtonHit( 1 );
        }
        else if( itemId == AppCommandIDs::BrushSelector)
        {
            
            b->setToggleState( true, dontSendNotification  );
            ToolbarState::selector = ToolbarState::BrushSelector;
            
            ISSLOG("<Event type=\"BrushSelector\" time=\"" + ISSTIME + "\"></Event>");
        }
    }
}


// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool MultiPaintView::perform (const InvocationInfo& info)
{
    bool okay = true;
    switch (info.commandID)
    {
        case AppCommandIDs::ClearAllMasks:
        {

            MAIN_APP->undoManager.beginNewTransaction ();
            SeparationEngine::getInstance()->clearMasks();
            multiTrack->clearMasksImages();

            break;
        }
            // Forward all other messages to the button click method
        case AppCommandIDs::ZoomInHorizontal:
        case AppCommandIDs::ZoomOutHorizontal:
        case AppCommandIDs::ZoomInVertical:
        case AppCommandIDs::ZoomOutVertical:
        case AppCommandIDs::ZoomIn:
        case AppCommandIDs::ZoomOut:
        case AppCommandIDs::Process:
        case AppCommandIDs::ArrowSelector:
        case AppCommandIDs::BrushSelector:
        case AppCommandIDs::TimeSelector:
        case AppCommandIDs::FreqSelector:
        case AppCommandIDs::BoxSelector:
        case AppCommandIDs::SoundPenSelector:
        case AppCommandIDs::SourceColor1:
        case AppCommandIDs::SourceColor2:
        case AppCommandIDs::PaintMode:
        case AppCommandIDs::EraseMode:
        case AppCommandIDs::TrainingSelector:
        {
            toolbarItem2IndexMap[info.commandID ]->triggerClick();
            break;
        }
        default:
            okay = false;
    };
    
    
//    if(okay)
//        applicationCommandListChanged();
    
    
    return okay;
}

