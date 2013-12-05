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
#include "FilePlayerProcessor.h"


#include "Track.h"



//Fills-in the appropriate parts of this plugin description object.
void 	FilePlayerProcessor::fillInPluginDescription (PluginDescription &description) const
{
    description.name = "File Player";
    description.descriptiveName = "Plays an audio file";
    description.pluginFormatName = "Internal";
    description.category = "Synth";
    description.manufacturerName = "Stanford";
    description.version = "1.0";
    description.fileOrIdentifier = String(description.name.hashCode());
    description.lastFileModTime = Time::getCurrentTime();
    description.uid =  description.name.hashCode();
    description.isInstrument = true;
    description.numInputChannels = 0;
    description.numOutputChannels = 2;
    
//    std::cout << "Num output channels:" << getNumOutputChannels() << std::endl;
//    std::cout << "description.numOutputChannels:" << description.numOutputChannels << std::endl;
}

//Returns a pointer to some kind of platform-specific data about the plugin.
void * 	FilePlayerProcessor::getPlatformSpecificData ()
{
    return this;
}

//For some formats (currently AudioUnit), this forces a reload of the list of available parameters.
void 	FilePlayerProcessor::refreshParameterList ()
{
    // do nothing.
}










//Returns the name of this processor.
const String 	FilePlayerProcessor::getName () const
{
    return String("File Player");
}

//Called before playback starts, to let the filter prepare itself.
void 	FilePlayerProcessor::prepareToPlay (double sampleRate, int estimatedSamplesPerBlock)
{
    //TODO
//    std::cout << sampleRate << " " << estimatedSamplesPerBlock << std::endl;
    
    
    
    if(source)
        source->prepareToPlay(estimatedSamplesPerBlock, sampleRate);
}

//Called after playback has stopped, to let the filter free up any resources it no longer needs.
void 	FilePlayerProcessor::releaseResources ()
{
    //TODO
}

//Renders the next block.
void 	FilePlayerProcessor::processBlock (AudioSampleBuffer &buffer, MidiBuffer &midiMessages)
{
    AudioSourceChannelInfo chanInf(buffer);
    source->getNextAudioBlock(chanInf);
}


//Returns the name of one of the processor's input channels.
const String 	FilePlayerProcessor::getInputChannelName (int channelIndex) const
{
    return String("");
}

//Returns the name of one of the processor's output channels.
const String 	FilePlayerProcessor::getOutputChannelName (int channelIndex) const
{
    
    return String(channelIndex+1);
}

//Returns true if the specified channel is part of a stereo pair with its neighbour.
bool 	FilePlayerProcessor::isInputChannelStereoPair (int index) const
{
    return false;
}

// Returns true if the specified channel is part of a stereo pair with its neighbour.
bool 	FilePlayerProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

//Returns true if a silent input always produces a silent output.
bool 	FilePlayerProcessor::silenceInProducesSilenceOut () const
{
    return false;
}

//Returns the length of the filter's tail, in seconds.
double 	FilePlayerProcessor::getTailLengthSeconds () const
{
    return 0.0;
}

//Returns true if the processor wants midi messages.
bool 	FilePlayerProcessor::acceptsMidi () const
{
    return false;
}

//Returns true if the processor produces midi messages.
bool 	FilePlayerProcessor::producesMidi () const
{
    return false;
}


////A plugin can override this to be told when it should reset any playing voices.
//void 	FilePlayerProcessor::reset ()
//{
//    // TODO
//}

//Creates the filter's UI.
AudioProcessorEditor* FilePlayerProcessor::createEditor()
{
    return nullptr;
}

//Your filter must override this and return true if it can create an editor component.
bool 	FilePlayerProcessor::hasEditor () const
{
    return false;
}

//This must return the correct value immediately after the object has been created, and mustn't change the number of parameters later.
int 	FilePlayerProcessor::getNumParameters ()
{
    return 0;
}

//Returns the name of a particular parameter.
const String 	FilePlayerProcessor::getParameterName (int parameterIndex)
{
    return String("");
}

//Called by the host to find out the value of one of the filter's parameters.
float 	FilePlayerProcessor::getParameter (int parameterIndex)
{
    return 0.0;
}

//Returns the value of a parameter as a text string.
const String 	FilePlayerProcessor::getParameterText (int parameterIndex)
{
    return String("0.0");
}

//Some plugin types may be able to return a label string for a parameter's units.
String 	FilePlayerProcessor::getParameterLabel (int index) const
{
    return String("");
}

// The host will call this method to change the value of one of the filter's parameters.
void 	FilePlayerProcessor::setParameter (int parameterIndex, float newValue)
{
    
}

//Returns true if the host can automate this parameter.
bool 	FilePlayerProcessor::isParameterAutomatable (int parameterIndex) const
{
    return false;
}

//Should return true if this parameter is a "meta" parameter.
bool 	FilePlayerProcessor::isMetaParameter (int parameterIndex) const
{
    return false;
}

//Returns the number of preset programs the filter supports.
int 	FilePlayerProcessor::getNumPrograms ()
{
    return 1;
}

//Returns the number of the currently active program.
int 	FilePlayerProcessor::getCurrentProgram ()
{
    return 0;
}

//Called by the host to change the current program.
void 	FilePlayerProcessor::setCurrentProgram (int index)
{
    // do nothing
}

//Must return the name of a given program.
const String 	FilePlayerProcessor::getProgramName (int index)
{
    return String("Default");
}

//Called by the host to rename a program.
void 	FilePlayerProcessor::changeProgramName (int index, const String &newName)
{
      // do nothing
}

//The host will call this method when it wants to save the filter's internal state.
void 	FilePlayerProcessor::getStateInformation (juce::MemoryBlock &destData)
{
    // do nothing   
}

////The host will call this method if it wants to save the state of just the filter's current program.
//void 	FilePlayerProcessor::getCurrentProgramStateInformation (juce::MemoryBlock &destData)
//{
//      // do nothing
//}
//
//This must restore the filter's state from a block of data previously created using getStateInformation().
void 	FilePlayerProcessor::setStateInformation (const void *data, int sizeInBytes)
{
      // do nothing
}

////The host will call this method if it wants to restore the state of just the filter's current program.
//void 	FilePlayerProcessor::setCurrentProgramStateInformation (const void *data, int sizeInBytes)
//{
//      // do nothing
//}
//
////This method is called when the number of input or output channels is changed.
void 	FilePlayerProcessor::numChannelsChanged ()
{
    //TODO
//    std::cout << "Number of channels changed!" << std::endl;
}





