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
#ifndef __MULTIPAINTVIEW_H_CA3FDD45__
#define __MULTIPAINTVIEW_H_CA3FDD45__


#include "JuceHeader.h"

#include "Timeline.h"
#include "VirtualUser.h"
#include "XMLSerializable.h"
#include "TransportComp.h"

#include <map>

 
class MultiTrack;
  
class MultiPaintView  :     public Component,
                            public Button::Listener,
                            public ApplicationCommandTarget,
                            public Slider::Listener,
                            public ModalComponentManager::Callback,
                            public XMLSerializable
//                            public VirtualUserHost
{
public:
    
    MultiPaintView(Toolbar * toolbar_,
                   MultiTrack * multiTrack_,
                   TransportComp * transportComp_);
    ~MultiPaintView();
    
    
    // Application Command Target interface
    virtual void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    virtual void getAllCommands (Array <CommandID>& commands);
    virtual bool perform (const InvocationInfo& info);
    virtual ApplicationCommandTarget* getNextCommandTarget();
    
    virtual void modalStateFinished (int returnValue);
    
    virtual void buttonClicked (Button *button);
    
    // File I/O
    bool createNewProject( File loadFile, bool async );
    
    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets );
    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  );
    
 
    void exportAudio( File & file );
    
    void createNewProject( File file );
    
    void sourceColourButtonHit( int source );
    void setCursorBecauseOfAudio( );
    
    
    bool isProcessing();
    
    virtual void sliderValueChanged (Slider *slider);
    
     void resetState();

    
private:
    
//    void changeBrushSettings( float w, float h );

   
    
    // View and Zoom controls
    void resized();
    void zoomInHorizontal();
    void zoomOutHorizontal();
    void zoomInVertical();
    void zoomOutVertical();
    void zoomIn();
    void zoomOut();

    
    void logSimpleEvent(String event);
    
private:

        
    Toolbar * toolbar;
    MultiTrack * multiTrack;
    TransportComp * transportComp;
    static const int transportHeight;
    std::map<int, ToolbarItemComponent*> toolbarItem2IndexMap;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiPaintView)
    
};

#endif  // __MULTIPAINTVIEW_H_CA3FDD45__
