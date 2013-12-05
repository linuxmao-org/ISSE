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
#ifndef __WINDOW_H_F5C5212C__
#define __WINDOW_H_F5C5212C__

#include <vector>
#include <string>
#include <math.h>

#include "ISSDefs.h"

//!=============================================================================

 
    

//! Hann Window
inline Matrix hann( unsigned long N, bool periodic = false);

//! Hanning Window
//inline Matrix hanning( unsigned long N, bool periodic = false );

//! Rectangular Window
//inline Matrix rectwin( unsigned long N, bool periodic = false );

//! Hamming Window
inline Matrix hamming( unsigned long N, bool periodic = false );


//! Give it a window of length M, and it will linear-phase zero-pad it.
inline Matrix zerophasewin( const Matrix & m, int N );
    
//! Bartlett Window    
//inline Matrix bartlett( unsigned long N );

//! Triangle Window
//inline Matrix triang( unsigned long N );

 

//!=============================================================================

    // Forward reference
    namespace internal {
       
        
        inline bool isTrivial(unsigned long N)
        {
            if ( N < 2)
                return true;
            else
                return false;
        }
    }

//! Hann Window
inline Matrix hann( unsigned long N, bool periodic )
{
    Matrix window(N,1);
    if(internal::isTrivial(N))
    {
        window.setOnes();
        return window;
    }
    
    
    if(!periodic)
    {
        for(unsigned long i = 0; i < N; i++ )
            window(i) = (ISSFloat)(0.5 * (1.0 - cos(i*2.0*M_PI/(N-1.0))));
    }
    else
    {
        for(unsigned long i = 0; i < N; i++ )
            window(i) = (ISSFloat)(0.5 * (1.0 - cos(i*2.0*M_PI/(N*1.0))));
    }
    
    return window;
}
    

    

 



//! Hamming Window 
inline Matrix hamming( unsigned long N, bool periodic )
{
    Matrix window(N, 1);    
    if(internal::isTrivial(N))
    {
        window.setOnes();
        return window;
    }
    
    double delta = 0.0;
 
    if(periodic)
        delta = 2.0 * M_PI / (N*1.0); 
    else
        delta = 2.0 * M_PI / (N-1.0);    
    
    for(unsigned long i = 0; i < N; i++ )
        window(i) = (ISSFloat)(.54 - .46*cos(delta*i)); 
    
    return window;
}




inline Matrix zerophasewin( const Matrix & m, int N )
{
        
    if ( m.cols() > 1 )
        throw "[zerophasewin]: Input Matrix must be a column vector.\n";
        
    if( N < m.cols() )
        throw "[zerophasewin]: N must be > numcols of m.\n";
           
        
    Matrix temp(N,1); 
    temp.setZero();
        
    //first half and second half in linear-phase
    int half = m.rows()/2;
    temp.block(0, 0, half, 1) = m.block(half, 0, half, 1);
    temp.block(N-half, 0, half, 1) = m.block(0, 0, half, 1);
    
    return temp;
}
    




 



#endif  // __WINDOWS_H_F5C5212C__
