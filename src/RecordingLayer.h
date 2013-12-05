/*
  ==============================================================================

    RecordingLayer.h
    Created: 19 Oct 2013 6:01:58pm
    Author:  njb

  ==============================================================================
*/

#ifndef RECORDINGLAYER_H_INCLUDED
#define RECORDINGLAYER_H_INCLUDED


 
 
#include "JuceHeader.h"
#include "Layer.h"

class AudioRecorder;
class LiveAudioInputDisplayComp;




class RecordingLayer  : public Layer, public AudioIODeviceCallback, public Timer
{
public:
 
    RecordingLayer (AudioDeviceManager& deviceManager_);
    
    ~RecordingLayer();
    
    virtual void updateLayer();
    
    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
 
    
    void record( bool isOn );

    void timerCallback();
    
    void audioDeviceAboutToStart (AudioIODevice* device);
    
    void audioDeviceStopped();
    
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples);
    
    void screen2ImageSpace(int x, int y, double & xImagespace, double & yImagespace);
    
private:
 
    AudioDeviceManager & deviceManager;
    ScopedPointer<AudioRecorder> recorder;
 
    size_t numSamplesInBuff;
    float * samples;
    float fs;
    int nextSample, subSample;
    float accumulator;
    const int numSubSamples;
    
    Image image;
    
    RecordingLayer (const RecordingLayer&);
    RecordingLayer& operator= (const RecordingLayer&);

};

 


#endif  // RECORDINGLAYER_H_INCLUDED
