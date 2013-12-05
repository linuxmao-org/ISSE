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
#ifndef isscore_ISSFFT_h
#define isscore_ISSFFT_h

#ifdef __KISS_FFT__
#else
    #include <fftw3.h>
#endif

#include "ISSDefs.h"
#include "../JuceHeader.h"

#include <unsupported/Eigen/FFT>

class FFT
{ 
public:
    enum FFT_TYPE{ FORWARD, REVERSE };
public:
    
    //! Constructor
    FFT( size_t fftsize_, Matrix &fx, FFT_TYPE dir = FORWARD);
    //! Destrutor
	~FFT();
    
    //! Process method
	void execute();
    
#ifdef __KISS_FFT__
    Eigen::FFT<ISSFloat> ft;

#else
  
#ifdef __SEPARATION_FLOAT_PRECISION__
    fftwf_plan ft;
#else
    fftw_plan ft;
#endif
    
#endif
    
    Matrix & fftbuff;
    size_t fftsize;
    FFT_TYPE type;
    bool forward;
    Eigen::Matrix<std::complex<ISSFloat>, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> freqvec;
    
    
    CriticalSection mutex;
    
};





#endif