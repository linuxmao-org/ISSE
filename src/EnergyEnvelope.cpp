/*
  ==============================================================================

    EnergyEnvelope.cpp
    Created: 29 Aug 2013 12:10:50pm
    Author:  njb

  ==============================================================================
*/
// Authors: Nicholas J. Bryan
//
//#include "EnergyEnvelope.h"
//
//#include <iostream>
//
//#include "EnergyEnvelope.h"
//#include "api/ISSDefs.h"
//#include "api/ISSFft.h"
//
//
//EnergyEnvelope::EnergyEnvelope()
//{
//    params.fftsize = 1024;
//    params.windowsize = 1024;
//    params.hopsize = 256;
//}
//
//
//
//void EnergyEnvelope::getDimensions(const ISSStftParams & stftParams, size_t numSamples, size_t & width, size_t & height)
//{
//
//    size_t fftsize = stftParams.fftsize;
//    size_t hopsize = stftParams.hopsize;
//    size_t windowsize = stftParams.windowsize;
//    
//    size_t hops2fill = ceil(windowsize*1.0/hopsize);
//    size_t sampsConsumed = hops2fill*hopsize;
//    size_t hopsLeft = ceil( (numSamples-sampsConsumed)*1.0/hopsize );
//    sampsConsumed = sampsConsumed + hopsLeft*hopsize;
//    size_t samplesLeftInBuff = windowsize + (numSamples - sampsConsumed);
//    size_t buffs2empty = ceil(samplesLeftInBuff*1.0/hopsize)-1;
//    size_t nframes = hops2fill + hopsLeft + buffs2empty;
//    
//    
//    height = fftsize/2 + 1;
//    width = nframes;
//
//}
//
//void EnergyEnvelope::magnitude(  ISSAudioSource& source, Matrix & magnitude )
//{
//    size_t hopsize = params.hopsize;
//    size_t windowsize = params.windowsize;
//    size_t  nframes=0;
//    size_t height=0;
//    
//    size_t numSamples = source.getLengthInSamples();
//    if(numSamples <= windowsize)
//    {
//        //std::cerr << "[ERROR]: No input samples to process stft" <<std::endl;
//        return;
//    }
//
//
//    getDimensions(params, numSamples, nframes, height);
//    
//    Matrix inputBuffer( windowsize, 1);
//    
//    magnitude.resize( nframes, 1);
//    magnitude.setZero();
//    
//    inputBuffer.setZero();
//    
//    bool okay = true;
//    
//    // Read all data
//    for(int frameindex=0; frameindex < nframes; ++frameindex)
//    {
//        // Shift the input buffer by the hopsize
//        {
//            // Shift old data
//            for(int i=0; i < (windowsize-hopsize); ++i)
//                inputBuffer(i) = inputBuffer(i+hopsize);
//            
//            // Zero inds for new data
//            for(unsigned long i=(windowsize-hopsize); i < windowsize; ++i)
//                inputBuffer(i) = 0;
//            
//        }
//        // read new input
//        if(okay)
//            okay = source.read( &inputBuffer.data()[windowsize-hopsize], hopsize);
//        
//        magnitude(frameindex, 0) = inputBuffer.squaredNorm();
//    }
//}
