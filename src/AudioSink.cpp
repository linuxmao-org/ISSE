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

#include "AudioSink.h"

 
FileAudioSink::FileAudioSink( const File & file_, double sampleRateToUse, size_t numChannels_, size_t maxSamples_ ) :
    tempFile(file_),
    fwriter(NULL),
    foutput(NULL),
sampleRate(sampleRateToUse),
numChannels(numChannels_),
maxSamples(maxSamples_),
    sampleIndex(0),
    resetPosition(0)
{
    createWriter();
}
    
void FileAudioSink::createWriter()
{
    WavAudioFormat wavFormat;
        
    unsigned int numberOfChannels = numChannels;
    int 	bitsPerSample = 16;
    int 	qualityOptionIndex = 0;
        
    const String description("");
    const String originator("");
    const String originatorRef("");
    const Time dateAndTime(Time::getCurrentTime());
    int64 timeReferenceSamples(0);
    const String codingHistory("");
    StringPairArray metadataValues =
    WavAudioFormat::createBWAVMetadata (description, originator,
                                            originatorRef, dateAndTime, timeReferenceSamples, codingHistory);
        
    foutput = new FileOutputStream( tempFile );
    foutput->setPosition(0);
    
    fwriter = wavFormat.createWriterFor (foutput,
                                         sampleRate,
                                         numberOfChannels,
                                         bitsPerSample,
                                         metadataValues,
                                         qualityOptionIndex);
        
    resetPosition = foutput->getPosition ();
    reset();
}
    
bool FileAudioSink::close()
{
    fwriter = NULL;
    return true;
}
      
bool FileAudioSink::reset()
{
    if(foutput)
    {
        foutput->setPosition(resetPosition);
        sampleIndex = 0;
        return true;
    }
    else
        return false;
}

bool FileAudioSink::write(	const ISSFloat ** 	channelData, size_t numChannels, size_t 	numSamples )
{
    if(!fwriter)
        return false;
    
    numSamples = std::min( maxSamples-sampleIndex, numSamples);
    bool result = fwriter->writeFromFloatArrays (channelData, numChannels, numSamples);
    
    if(result)
        sampleIndex += numSamples;
    else
    {
        std::cout << "[ERROR] Could not write new source data to no where" << std::endl;
        return false;
    }
    
    return true;
}

bool FileAudioSink::writemono( ISSFloat * data, size_t numSamples )
{
    if(!fwriter)
        return false;
        
    ISSFloat * channelData[2];
    channelData[0] = data;
    channelData[1] = nullptr;
        
    numSamples = std::min( (size_t)(maxSamples-sampleIndex), numSamples);
    
    AudioSampleBuffer buff(	channelData, 1, 0, numSamples);
  
    bool result = fwriter->writeFromAudioSampleBuffer (buff, 0, numSamples);
        
    if(result)
        sampleIndex += numSamples;
    else
    {
        std::cout << "[ERROR] Could not write new source data to no where" << std::endl;
        return false;
    }
        
    return true;
}
    

FileAudioSink::~FileAudioSink()
{
	close();
};
    



