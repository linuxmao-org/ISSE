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
#include "ISSFft.h"


 
//! Constructor
FFT::FFT( size_t fftsize_, Matrix &fx, FFT_TYPE type_) : fftbuff(fx), fftsize(fftsize_), type(type_)
#ifdef __KISS_FFT__
, freqvec(fftsize/2+1, 1)
#endif
{
    
    const GenericScopedLock<CriticalSection> myScopedLock (mutex);
    
#ifdef __KISS_FFT__
    
    ft.SetFlag(Eigen::FFT<ISSFloat>::HalfSpectrum);
  
#else
   
    
    #ifdef __SEPARATION_FLOAT_PRECISION__
    if( type == FORWARD )
        ft = fftwf_plan_dft_r2c_1d( (int)fftsize, &fx(0), (fftwf_complex*)&fx(0), FFTW_ESTIMATE);
    else
        ft = fftwf_plan_dft_c2r_1d( (int)fftsize, (fftwf_complex*)&fx(0), &fx(0), FFTW_ESTIMATE);
    #else
    if( type == FORWARD )
        ft = fftw_plan_dft_r2c_1d( (int)fftsize, &fx(0), (fftw_complex*)&fx(0), FFTW_ESTIMATE);
    else
        ft = fftw_plan_dft_c2r_1d( (int)fftsize, (fftw_complex*)&fx(0), &fx(0), FFTW_ESTIMATE);
    #endif

    
#endif
}

    //! Destrutor
FFT::~FFT()
{
#ifdef __KISS_FFT__
#else
    #ifdef __SEPARATION_FLOAT_PRECISION__
        fftwf_destroy_plan( ft);
    #else
        fftw_destroy_plan( ft);
    #endif
#endif
}

    
    
    //! Process method
void FFT::execute()
{
    
#ifdef __KISS_FFT__
    if(type==FORWARD)
    {
        ft.fwd( freqvec.data(), fftbuff.data(), fftsize );
        
        int i=0;
        for(; i < fftsize/2; i++)
        {
            fftbuff(i*2) = freqvec(i).real();
            fftbuff(i*2+1) = freqvec(i).imag();
        }
        fftbuff(i*2) = freqvec(i).real();
    }
    else
    {
        int i=0;
        for(; i < fftsize/2; i++)
        {
            freqvec(i).real(fftbuff(i*2));
            freqvec(i).imag(fftbuff(i*2+1));
        }
        freqvec(i).real(fftbuff(i*2));
        
        ft.inv(fftbuff.data(), freqvec.data(), fftsize);
    }
#else

    
#ifdef __SEPARATION_FLOAT_PRECISION__
    fftwf_execute( ft);
#else
    fftw_execute( ft);
#endif
    if(type==REVERSE)
    {
        for( size_t i = 0 ; i < fftbuff.size() ; i++)
            fftbuff(i) /= fftsize;
    }

#endif
}




