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
#ifndef __TRANSPORTCOMP_H_A19DE6D1__
#define __TRANSPORTCOMP_H_A19DE6D1__

#include "JuceHeader.h"

//==============================================================================
/*
*/
class TransportComp    :    public Component,
                            public Button::Listener,
                            public ApplicationCommandTarget
{
public:
    
    class Listener
    {
    public:
        enum TransportEvent{Play,
                            Pause,
                            Stop,
                            Next,
                            Previous,
                            Rewind,
                            FastForward,
                            Loop,
                            AutoSolo,
                            Update};
        
        Listener(){}
        
        virtual ~Listener(){}
        
        virtual void updatedTransportControl( TransportComp * transportComp, TransportEvent event )=0;
        
    };
    
    
public:
    
    TransportComp();
    
    ~TransportComp();
    
    void addListener( TransportComp::Listener * listener );
    
    void removeListener( TransportComp::Listener * listener );
    
    bool isPaused()
    {
        return pauseButton->getToggleState();
    }
    
    bool isPlaying()
    {
        return playButton->getToggleState();
    }
    
    bool isLooping()
    {
        return loopButton->getToggleState();
    }
    
    virtual void 	buttonClicked (Button *);
    
    // Application Command Target interface
    virtual void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    virtual void getAllCommands (Array <CommandID>& commands);
    virtual bool perform (const InvocationInfo& info);
    virtual ApplicationCommandTarget* getNextCommandTarget();
    
    void reset();

    void paint (Graphics&);
    void resized();

private:
    
    ListenerList<TransportComp::Listener> listeners;
    
    void logSimpleEvent(String event);
    

    
    ScopedPointer<ImageButton> playButton;
    ScopedPointer<ImageButton> pauseButton;
    ScopedPointer<ImageButton> stopButton;
    ScopedPointer<ImageButton> loopButton;
    ScopedPointer<ImageButton> rewindButton;
    ScopedPointer<ImageButton> fastForwardButton;
    ScopedPointer<ImageButton> nextButton;
    ScopedPointer<ImageButton> previousButton;
    ScopedPointer<ImageButton> autoSoloButton;
    
    int buttonWidth;
    int buttonHeight;


    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportComp)
};


#endif  // __TRANSPORTCOMP_H_A19DE6D1__
