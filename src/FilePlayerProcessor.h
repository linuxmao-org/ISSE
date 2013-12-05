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
#ifndef __FILEPLAYERPROCESSOR_H_F3E4290F__
#define __FILEPLAYERPROCESSOR_H_F3E4290F__

#include "JuceHeader.h"
#include "AudioFormatReaderLoopSource.h"

 

class FilePlayerProcessor : public AudioPluginInstance
{
    
public:
    
    FilePlayerProcessor(AudioSource * source_) : source(source_)
    {
        int numOuts(2);
        setPlayConfigDetails (0,numOuts,getSampleRate(), getBlockSize());
    }
    
    //Fills-in the appropriate parts of this plugin description object.
    virtual void 	fillInPluginDescription (PluginDescription &description) const;
 	
    //Returns a pointer to some kind of platform-specific data about the plugin.
    virtual void * 	getPlatformSpecificData ();
 	
    //For some formats (currently AudioUnit), this forces a reload of the list of available parameters. 
    virtual void 	refreshParameterList ();
    
    //Returns the name of this processor.
    virtual const String 	getName () const;
 	
    //Called before playback starts, to let the filter prepare itself.
    virtual void 	prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
 	
    //Called after playback has stopped, to let the filter free up any resources it no longer needs.
    virtual void 	releaseResources ();
 	
    //Renders the next block.
    virtual void 	processBlock (AudioSampleBuffer &buffer, MidiBuffer &midiMessages);
 	
    //Returns the name of one of the processor's input channels.
    virtual const String 	getInputChannelName (int channelIndex) const;
 	
    //Returns the name of one of the processor's output channels.
    virtual const String 	getOutputChannelName (int channelIndex) const;
 	
    //Returns true if the specified channel is part of a stereo pair with its neighbour.
    virtual bool 	isInputChannelStereoPair (int index) const;
 	
    // Returns true if the specified channel is part of a stereo pair with its neighbour.    
    virtual bool 	isOutputChannelStereoPair (int index) const;
    
    //Returns true if a silent input always produces a silent output.
    virtual bool 	silenceInProducesSilenceOut () const;
 	
    //Returns the length of the filter's tail, in seconds.    
    virtual double 	getTailLengthSeconds () const;
    
    //Returns true if the processor wants midi messages.    
    virtual bool 	acceptsMidi () const;
    
    //Returns true if the processor produces midi messages.        
    virtual bool 	producesMidi () const;
    
//    //A plugin can override this to be told when it should reset any playing voices.
//    virtual void 	reset ();
    
    //Creates the filter's UI.
    virtual AudioProcessorEditor* createEditor();
    
    //Your filter must override this and return true if it can create an editor component.
    virtual bool 	hasEditor () const;
    
    //This must return the correct value immediately after the object has been created, and mustn't change the number of parameters later.
    virtual int 	getNumParameters ();
 	
    //Returns the name of a particular parameter.
    virtual const String 	getParameterName (int parameterIndex);
 	
    //Called by the host to find out the value of one of the filter's parameters.
    virtual float 	getParameter (int parameterIndex);
 	
    //Returns the value of a parameter as a text string.
    virtual const String 	getParameterText (int parameterIndex);
 	
    //Some plugin types may be able to return a label string for a parameter's units.
    virtual String 	getParameterLabel (int index) const;
 	
    // The host will call this method to change the value of one of the filter's parameters.    
    virtual void 	setParameter (int parameterIndex, float newValue);
    
    //Returns true if the host can automate this parameter.
    virtual bool 	isParameterAutomatable (int parameterIndex) const;
 	
    //Should return true if this parameter is a "meta" parameter.    
    virtual bool 	isMetaParameter (int parameterIndex) const;
    
    //Returns the number of preset programs the filter supports.
    virtual int 	getNumPrograms ();
 	
    //Returns the number of the currently active program.
    virtual int 	getCurrentProgram ();
 	
    //Called by the host to change the current program.
    virtual void 	setCurrentProgram (int index);
 	
    //Must return the name of a given program.
    virtual const String 	getProgramName (int index);
 	
    //Called by the host to rename a program.
    virtual void 	changeProgramName (int index, const String &newName);
 	
    //The host will call this method when it wants to save the filter's internal state.
    virtual void 	getStateInformation (juce::MemoryBlock &destData);
 	
//    //The host will call this method if it wants to save the state of just the filter's current program.
//    virtual void 	getCurrentProgramStateInformation (juce::MemoryBlock &destData);
// 	
    //This must restore the filter's state from a block of data previously created using getStateInformation().    
    virtual void 	setStateInformation (const void *data, int sizeInBytes);
    
//    //The host will call this method if it wants to restore the state of just the filter's current program.
//    virtual void 	setCurrentProgramStateInformation (const void *data, int sizeInBytes);
    
    //This method is called when the number of input or output channels is changed.        
    virtual void 	numChannelsChanged ();
    
private:
    
    AudioSource * source;
    
};




#endif  // __FILEPLAYERPROCESSOR_H_F3E4290F__
