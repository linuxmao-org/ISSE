/*
  ==============================================================================

    AudioRecorder.h
    Created: 20 Oct 2013 9:11:41pm
    Author:  njb

  ==============================================================================
*/

#ifndef AUDIORECORDER_H_INCLUDED
#define AUDIORECORDER_H_INCLUDED

#include "JuceHeader.h"



//==============================================================================
/** A simple class that acts as an AudioIODeviceCallback and writes the
 incoming audio data to a WAV file.
 */
class AudioRecorder  : public AudioIODeviceCallback
{
public:
    AudioRecorder() : backgroundThread ("Audio Recorder Thread"),
    sampleRate (0), activeWriter (0)
    {
        backgroundThread.startThread();
    }
    
    ~AudioRecorder()
    {
        stop();
    }
    
    //==============================================================================
    void startRecording (const File& file)
    {
        stop();
        
        if (sampleRate > 0)
        {
            // Create an OutputStream to write to our destination file...
            file.deleteFile();
            ScopedPointer<FileOutputStream> fileStream (file.createOutputStream());
            
            if (fileStream != 0)
            {
                // Now create a WAV writer object that writes to our output stream...
                WavAudioFormat wavFormat;
                AudioFormatWriter* writer =
                    wavFormat.createWriterFor (fileStream, sampleRate, 1, 16, StringPairArray(), 0);
                
                if (writer != 0)
                {
                    fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)
                    
                    // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                    // write the data to disk on our background thread.
                    threadedWriter = new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768);
                    
                    // And now, swap over our active writer pointer so that the audio callback will start using it..
                    const ScopedLock sl (writerLock);
                    activeWriter = threadedWriter;
                }
            }
        }
    }
    
    void stop()
    {
        // First, clear this pointer to stop the audio callback from using our writer object..
        {
            const ScopedLock sl (writerLock);
            activeWriter = 0;
        }
        
        // Now we can delete the writer object. It's done in this order because the deletion could
        // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
        // the audio callback while this happens.
        threadedWriter = 0;
    }
    
    bool isRecording() const
    {
        return activeWriter != 0;
    }
    
    //==============================================================================
    void audioDeviceAboutToStart (AudioIODevice* device)
    {
        sampleRate = device->getCurrentSampleRate();
    }
    
    void audioDeviceStopped()
    {
        sampleRate = 0;
    }
    
    void audioDeviceIOCallback (const float** inputChannelData, int /*numInputChannels*/,
                                float** outputChannelData, int numOutputChannels,
                                int numSamples)
    {
        const ScopedLock sl (writerLock);
        
        if (activeWriter != 0)
            activeWriter->write (inputChannelData, numSamples);
        
        // We need to clear the output buffers, in case they're full of junk..
        for (int i = 0; i < numOutputChannels; ++i)
            if (outputChannelData[i] != 0)
                zeromem (outputChannelData[i], sizeof (float) * (size_t) numSamples);
    }
    
private:
    TimeSliceThread backgroundThread; // the thread that will write our audio data to disk
    ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    double sampleRate;
    
    CriticalSection writerLock;
    AudioFormatWriter::ThreadedWriter* volatile activeWriter;
};

#endif  // AUDIORECORDER_H_INCLUDED
