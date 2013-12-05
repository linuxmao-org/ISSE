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
#include <iostream>

#include "ISSStft.h"
#include "ISSFft.h"
#include "Window.h"
#include "ISSDefs.h"
#include "ISSFft.h"

 
static inline Matrix zeropad( const Matrix & m, int N )
{
    if(m.cols()==1)
    {
        Matrix temp(N,1);
        temp.setZero();
        
        temp.block(0, 0, m.rows(),1) = m;
        
//        int half = m.rows()/2;
//        temp.block(0, 0, half, 1) = m.block(half, 0, half, 1);
//        temp.block(N-half, 0, half, 1) = m.block(0, 0, half, 1);
        
        return temp;
    }
    else
    {
        Matrix temp(1,N);
        temp.setZero();
        
        temp.block(0, 0, 1, m.cols()) = m;
        
//        int half = m.cols()/2;
//        temp.block(0, 0, 1, half) = m.block(0, half, 1, half);
//        temp.block(0, N-half, 1, half) = m.block(0, 0, 1, half);
        
        return temp;
    }
}



bool isPowerOfTwo(unsigned long x)
{
    return ((x & (x - 1)) == 0);
}

 
    
ISSStft::ISSStft(const ISSStftParams & stftParams_) 
    : params(stftParams_)
{
    
    size_t fftsize = params.fftsize;
    size_t hopsize = params.hopsize;
    size_t windowsize = params.windowsize;
    size_t M = windowsize;
    
    fftbuffer.resize( fftsize + 2, 1);
    
    ifft = new FFT( fftsize, fftbuffer, FFT::REVERSE);
    fft  = new FFT( fftsize, fftbuffer, FFT::FORWARD);
    
    // check fftsize is power of two
    if(!isPowerOfTwo(fftsize))
    {
        throw "[ERROR]: fftsize must be power of two.";
    }
    
    // set window size
    window.resize(windowsize, 1);
    
    if((params.windowType==ISSStftParams::HANN) ||
       (params.windowType==ISSStftParams::RECT) ||
       (params.windowType==ISSStftParams::HAMMING))
    {
        if(windowsize%2)//odd
            M = windowsize-1;
        else //even
        {}
        
        // Make sure hopsize is Constant-Overlap-Add (COLA)
        unsigned long remaider = M % hopsize;
        if(remaider!=0)
        {
            throw "[ERROR]: window parameters does not yield constant overlap-add";
        }
        
        switch (params.windowType)
        {
            case ISSStftParams::RECT:
                window.setOnes();
                break;
                
            case ISSStftParams::HANN:
                window.block(0, 0, M, 1) = hann( M, true);
                
                break;
            case ISSStftParams::HAMMING:
                window.block(0, 0, M, 1) = hamming( M, true);
                
                break;
            default:
                break;
        }
    }
    else
    {
        throw  "[ERROR]: Unknown window type";
    }

}

ISSStftParams ISSStft::getParams()
{
    return params;
}

std::pair<size_t,size_t> ISSStft::getDimensions(const ISSStftParams & stftParams, size_t numSamples )
{

    size_t fftsize = stftParams.fftsize;
    size_t hopsize = stftParams.hopsize;
    size_t windowsize = stftParams.windowsize;
    
    size_t hops2fill = (size_t)ceil(windowsize*1.0/hopsize);
    size_t sampsConsumed = hops2fill*hopsize;
    size_t hopsLeft = (size_t)ceil( (numSamples-sampsConsumed*1.0)/hopsize );
    sampsConsumed = sampsConsumed + hopsLeft*hopsize;
    size_t samplesLeftInBuff = windowsize + (numSamples - sampsConsumed);
    size_t buffs2empty = (size_t)ceil(samplesLeftInBuff*1.0/hopsize)-1;
    size_t nframes = hops2fill + hopsLeft + buffs2empty;
    
    return std::pair<size_t, size_t>(nframes, fftsize/2 + 1);

}

void ISSStft::magnitude(  ISSAudioSource& source, Matrix & magnitude )
{
    
    size_t numSamples = source.getLengthInSamples();
    if(numSamples==0)
    {
        std::cerr << "[ERROR]: No input samples to process stft" <<std::endl;
        return;
    }
    
    size_t fftsize = params.fftsize;
    size_t hopsize = params.hopsize;
    size_t windowsize = params.windowsize;
    std::pair<size_t,size_t> maskSize(getDimensions(params, numSamples));
    size_t nframes=maskSize.first;
    
    Matrix inputBuffer( windowsize, 1);
    
    magnitude.resize( fftsize/2 + 1, nframes);
    magnitude.setZero();
    
    inputBuffer.setZero();
    fftbuffer.setZero();
    
    bool okay = true;
    size_t sampleIndex(0);
    
    // Read all data
    for(size_t frameindex=0; frameindex < nframes; ++frameindex)
    {
        // Shift the input buffer by the hopsize
        {
            // Shift old data
            for(size_t i=0; i < (windowsize-hopsize); ++i)
                inputBuffer(i) = inputBuffer(i+hopsize);
            
            // Zero inds for new data
            for(size_t i=(windowsize-hopsize); i < windowsize; ++i)
                inputBuffer(i) = 0;
            
        }
        // read new input
        if(okay)
        {
            okay = source.readmono( &inputBuffer.data()[windowsize-hopsize], 0, hopsize, sampleIndex);
            sampleIndex += hopsize;
        }
        
        // Window the input buffer and store in the fftbuffer
        fftbuffer.block(0, 0, windowsize, 1) = inputBuffer.cwiseProduct(window);
        
        // Zero-pad if the fftbuffer necessary
        fftbuffer.block(windowsize, 0, fftsize+2-windowsize, 1).setZero();
        
        // take the fft
        fft->execute();
        
        // store the stft data
        for(size_t i=0; i < fftsize/2 + 1; ++i)
            magnitude(i,frameindex) = sqrt(  fftbuffer(i*2) * fftbuffer(i*2) + fftbuffer(i*2+1) * fftbuffer(i*2+1) );
    }
}


void ISSStft::forward( ISSAudioSource & source, Matrix & stftdata )
{
    size_t numSamples = source.getLengthInSamples();
    size_t fftsize = params.fftsize;
    size_t hopsize = params.hopsize;
    size_t windowsize = params.windowsize;
    std::pair<size_t,size_t> maskSize(getDimensions(params, numSamples));
    size_t nframes=maskSize.first;

    Matrix inputBuffer( windowsize, 1);
    Matrix fftbuffer( fftsize + 2, 1);
        
    stftdata.resize( fftsize + 2, nframes);

    inputBuffer.setZero();
    fftbuffer.setZero();
    stftdata.setZero();
    
    bool okay = true;
    size_t sampleIndex(0);
        
    // Read all data
    for(size_t frameindex=0; frameindex < nframes; ++frameindex)
    {
        // Shift the input buffer by the hopsize
        {
            for(size_t i=0; i < (windowsize-hopsize); ++i)
                inputBuffer(i) = inputBuffer(i+hopsize);
            
            for(size_t i=(windowsize-hopsize); i < windowsize; ++i)
                inputBuffer(i) = 0;
            
        }
        // read new input
        if(okay)
        {
            okay = source.readmono( &inputBuffer.data()[windowsize-hopsize], 0, hopsize, sampleIndex);
            sampleIndex += hopsize;
        }
               
        // Window the input buffer, linear-phase it, and store in the fftbuffer
        fftbuffer.block(0, 0, fftsize, 1) = zeropad(inputBuffer.cwiseProduct(window), fftsize);
        
        // Zero the last two bins (for fftw)
        fftbuffer.block(fftsize, 0, 2, 1).setZero();
        
        // take the fft
        fft->execute();
        
        // store the stft data
        stftdata.col(frameindex) = fftbuffer;
    
    }
}




//! Streams in the source buffer by buffer and outputs a magnitude and phase stft data matrix
void ISSStft::forward( ISSAudioSource& source, Matrix & magnitude, Matrix & phase )
{
    size_t numSamples = source.getLengthInSamples();
    if(numSamples==0)
    {
        std::cerr << "[ERROR]: No input samples to process stft" <<std::endl;
        return;
    }

    size_t fftsize = params.fftsize;
    size_t hopsize = params.hopsize;
    size_t windowsize = params.windowsize;

    std::pair<size_t,size_t> maskSize(getDimensions(params, numSamples));
    size_t nframes=maskSize.first;

    Matrix inputBuffer( windowsize, 1);
    
    magnitude.resize( fftsize/2 + 1, nframes);
    phase.resize( fftsize/2 + 1, nframes);
    magnitude.setZero();
    phase.setZero();
    
    inputBuffer.setZero();
    fftbuffer.setZero();

    bool okay = true;
    size_t sampleIndex(0);
    
    // Read all data
    for(size_t frameindex=0; frameindex < nframes; ++frameindex)
    {
        // Shift the input buffer by the hopsize
        {
            // Shift old data
            for(size_t i=0; i < (windowsize-hopsize); ++i)
                inputBuffer(i) = inputBuffer(i+hopsize);
            
            // zero inds for new data
            for(size_t i=(windowsize-hopsize); i < windowsize; ++i)
                inputBuffer(i) = 0;
            
        }
        // read new input
        if(okay)
        {
            okay = source.readmono( &inputBuffer.data()[windowsize-hopsize], 0, hopsize, sampleIndex);
            sampleIndex += hopsize;
        }
        
        // Window the input buffer and store in the fftbuffer
        fftbuffer.block(0, 0, fftsize, 1) = zeropad(inputBuffer.cwiseProduct(window), fftsize);
        
        // Zero-pad last two samples
        fftbuffer.block(fftsize, 0, 2, 1).setZero();

        // take the fft
        fft->execute();
        
        // store the stft data
        for(size_t i=0; i < fftsize/2 + 1; ++i)
        {
            magnitude(i,frameindex) = sqrt(  fftbuffer(i*2) * fftbuffer(i*2) + fftbuffer(i*2+1) * fftbuffer(i*2+1) );
            phase(i,frameindex)     = atan2( fftbuffer(i*2+1), fftbuffer(i*2));
        }
    }
}

void ISSStft::reverse(const Matrix & magnitude, const Matrix & phase, Matrix & sink )
{
    if( magnitude.cols() != phase.cols() )
        throw ISSException("size of magnitude and phase must be identical.");
    
    size_t fftsize = params.fftsize;
    size_t hopsize = params.hopsize;
    size_t windowsize = params.windowsize;
    size_t numSamples = (magnitude.cols())*hopsize + fftsize;
    
    Matrix output(numSamples, 1);
    output.setZero();
    
    for(size_t frameindex=0; frameindex < magnitude.cols(); ++frameindex)
    {
        // copy out the stft data
        for(size_t i=0; i < fftsize/2 + 1; ++i)
        {
            fftbuffer(2*i)   = magnitude(i, frameindex) * cos( phase(i,frameindex) );   // Real
            fftbuffer(2*i+1) = magnitude(i, frameindex) * sin( phase(i,frameindex) );   // imag
        }
        
        // perform the fft
        ifft->execute();
        
        // Rescale according to the hopsize and fftsize
        fftbuffer.array() = fftbuffer.array()*((ISSFloat)(hopsize*2.0/windowsize));
        
        // Overlap add
        output.block( frameindex * hopsize, 0, fftsize, 1) += (fftbuffer.block(0, 0, fftsize, 1));
    }
    
    
    size_t frontsamps2chop = (windowsize - hopsize);
    
    sink.resize(numSamples, 1);
    sink.setZero();
    sink.block(0, 0, numSamples-frontsamps2chop, 1) = output.block(frontsamps2chop, 0, numSamples-frontsamps2chop, 1);
}
void ISSStft::reverse(const Matrix & magnitude, const Matrix & phase, ISSAudioSink & sink )
{
    
    if( magnitude.cols() != phase.cols() )
        throw ISSException("size of magnitude and phase must be identical.");
    
    size_t fftsize = params.fftsize;
    size_t hopsize = params.hopsize;
    size_t windowsize = params.windowsize;
    size_t numSamples = (magnitude.cols())*hopsize + fftsize;
    
    Matrix output(numSamples, 1);
    output.setZero();
        
    for(size_t frameindex=0; frameindex < magnitude.cols(); ++frameindex)
    {
        // copy out the stft data
        for(size_t i=0; i < fftsize/2 + 1; ++i)
        {
            fftbuffer(2*i)   = magnitude(i, frameindex) * cos( phase(i,frameindex) );   // Real
            fftbuffer(2*i+1) = magnitude(i,frameindex) * sin( phase(i,frameindex) );   // imag
        }

        // perform the fft
        ifft->execute();
        
        // Rescale according to the hopsize and fftsize
        fftbuffer.array() = fftbuffer.array()*((ISSFloat)(hopsize*2.0/windowsize));
        
        // Overlap add
        output.block( frameindex * hopsize, 0, fftsize, 1) += (fftbuffer.block(0, 0, fftsize, 1));
    }
    
    
    size_t frontsamps2chop = (windowsize - hopsize);
    
    
    
    // for now, write the entire output at once
    bool okay = sink.writemono( output.data()+frontsamps2chop, numSamples);
    

    if(!okay)
        throw ISSException("ERROR: Could not write output sink from STFT.");
      
    
}


ISSStft::~ISSStft()
{
    if(fft)
    {
        delete fft;
        fft = NULL;
    }
    if(ifft)
    {
        delete ifft;
        ifft = NULL;
    }
    
}




 