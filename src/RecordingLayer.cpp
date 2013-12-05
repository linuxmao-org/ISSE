/*
  ==============================================================================

    RecordingLayer.cpp
    Created: 19 Oct 2013 6:01:58pm
    Author:  njb

  ==============================================================================
*/

#include "RecordingLayer.h"
#include "AudioRecorder.h"
#include "ProgramSettings.h"


#define BOOST 10.f


void RecordingLayer::screen2ImageSpace(int x, int y, double & xImagespace, double & yImagespace)
{
    double currentLengthInSeconds(getMaxX());
    Range<double> xrange(getXRange());
    Range<double> cr(getYRange());
    double sourceX = xrange.getStart();
    double sourceXEnd =  xrange.getEnd();
    double sourceY =     cr.getStart();
    double sourceYEnd =  cr.getEnd();
    
    double imgWidth(image.getWidth());
    double imgHeight(image.getHeight());
    xImagespace = imgWidth*(sourceX + (sourceXEnd - sourceX)*x/(getWidth()*1.0))/currentLengthInSeconds;
    yImagespace = imgHeight*(1.0f/getMaxY())*(sourceY + (sourceYEnd - sourceY)*(y*1.0)/getHeight());
    
}

void RecordingLayer::paint (Graphics& g)
{
    if(getMaxX()==0) return;
    
    g.setColour(Colours::black);
    g.fillAll();
    
    Colour c(Colours::white);
    g.setColour(c.withAlpha(1.f));
    int w(getWidth());
    int h(getHeight());
    
    double sourceX(0), sourceY(0);
    double sourceXEnd(0), sourceYEnd(0);
    screen2ImageSpace(0, 0, sourceX, sourceY);
    screen2ImageSpace(w, h, sourceXEnd, sourceYEnd);
    
 
    
    g.drawImage(image, 0, 0, w, h, sourceX, sourceY,
                sourceXEnd-sourceX, sourceYEnd-sourceY, true);

    
    
//    RectangleList<float> waveform;
//        
//    const float midY = getHeight() * 0.5f;
//    float deltaX = (1.0*getWidth())/numSamplesInBuff;
//    
//    std::cout << "painting " << numSamplesInBuff << "width: " << getWidth() << " deltaX: " << deltaX << std::endl;
//    float x(0.f);
//    for (int i = 0; i < numSamplesInBuff; i++ )
//    {
//        
//        const float sampleSize = midY * samples [i];
//        waveform.addWithoutMerging (Rectangle<float> (x, midY - sampleSize,
//                                                      deltaX, sampleSize * 2.0f));
//        
//        x += deltaX;
//    }
//    std::cout << x << std::endl;
//    
//    g.fillAll (Colours::black);
//    g.setColour (Colours::white);
//    g.fillRectList (waveform);
    
}

void RecordingLayer::updateLayer()
{

    
    Range<double> range = getXRange();

    
    if(samples)
    {
        delete [] samples;
        samples = nullptr;
    }
    
    
    numSamplesInBuff = range.getLength()*fs/numSubSamples;
    
    samples = new float[numSamplesInBuff];
    zeromem (samples, numSamplesInBuff);
    
    image = Image(Image::SingleChannel, numSamplesInBuff, 100, true);
}


void RecordingLayer::timerCallback()
{
    repaint();
}
    
void RecordingLayer::audioDeviceAboutToStart (AudioIODevice* device)
{
    
    fs = device->getCurrentSampleRate ();
    
    zeromem (samples, numSamplesInBuff);
}

void RecordingLayer::audioDeviceStopped()
{
    zeromem (samples, numSamplesInBuff);
}


void RecordingLayer::audioDeviceIOCallback (const float** inputChannelData,
                                            int numInputChannels,
                                            float** outputChannelData,
                                            int numOutputChannels,
                                            int numSamples)
{
    
    for (int i = 0; i < numSamples; ++i)
    {
        
        for (int chan = 0; chan < numInputChannels; ++chan)
        {
            if (inputChannelData[chan] != 0)
                accumulator += fabsf (inputChannelData[chan][i]);
        }
            
        if (subSample == 0)
        {
            
            float pixSample = accumulator * BOOST / numSubSamples;
            int midY = image.getHeight()/2;
            
            int addedPix = midY*pixSample;
            int lower = midY - abs(addedPix);
            if(lower < 0)
                lower = 0;
            int upper = midY + abs(addedPix);
            if( upper >= image.getHeight())
                upper = image.getHeight()-1;
            
            
            // clear the column
            for(int h=0; h < image.getHeight(); h++)
            {
                
                image.setPixelAt(nextSample, h, Colours::white.withAlpha(0.f));
                
                if( h >= lower && h <= upper)
                    image.setPixelAt(nextSample, h, Colours::white.withAlpha(1.f));
            }
            
            samples[nextSample] = pixSample;
            nextSample = (nextSample + 1) % numSamplesInBuff;
            subSample = numSubSamples;
            accumulator = 0;
            
 
            
        }
        else
        {
            --subSample;
        }

    }
 
    // We need to clear the output buffers, in case they're full of junk..
    for (int i = 0; i < numOutputChannels; ++i)
        if (outputChannelData[i] != 0)
            zeromem (outputChannelData[i], sizeof (float) * (size_t) numSamples);
}
 


 

//==============================================================================
RecordingLayer::RecordingLayer (AudioDeviceManager& deviceManager_)
: deviceManager (deviceManager_),  numSamplesInBuff(2048), numSubSamples(100), fs(44100)
{
    image = Image(Image::SingleChannel, numSamplesInBuff, 100, true);
    samples = new float[numSamplesInBuff];

    nextSample = subSample = 0;
    accumulator = 0;
    zeromem (samples, numSamplesInBuff);
    setOpaque (true);
    
    startTimer (100); // use a timer to keep repainting this component
    
//    setSize (600, 400);

    recorder = new AudioRecorder();
    deviceManager.addAudioCallback (recorder);
    deviceManager.addAudioCallback (this);
  
}

RecordingLayer::~RecordingLayer()
{
    deviceManager.removeAudioCallback (recorder);
    deviceManager.removeAudioCallback (this);
    recorder = 0;
    
    if(samples)
    {
        delete [] samples;
        samples = nullptr;
    }
}

 
void RecordingLayer::resized()
{
}


void RecordingLayer::record( bool isOn )
{
  
    if (recorder->isRecording())
    {
        recorder->stop();
    }
    else
    {
        File file(File::getSpecialLocation (File::userDocumentsDirectory)
                    .getNonexistentChildFile ("Querry", ".wav"));
            
        recorder->startRecording (file);
    }
  
}


 


