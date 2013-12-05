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
#ifndef __ISSRANDOM_H_846C4C36__
#define __ISSRANDOM_H_846C4C36__

#if defined __WINDOWS__
	#include <time.h>
#else
	#include <sys/time.h>
#endif

#include "ISSDefs.h"



//! \class ISSRandom Random number generator 
class ISSRandom
{
public:
    
    //! Default constructor
    ISSRandom(ISSFloat min_, ISSFloat max_)
            : min(min_), max(max_)
    {
        srand(time(NULL));
    }
    
    ISSRandom(ISSFloat min_, ISSFloat max_, int seed)
    : min(min_), max(max_)
    {
        srand(seed);
    }
    
    // templated function to actually generate a random number
    template <typename T>
    inline T getRandom()
    {
        ISSFloat x = rand()/static_cast<ISSFloat>(RAND_MAX);
        ISSFloat that = min + x * (max - min) ;
        
        return that;
    }

    
private:
    
    ISSFloat min;
    ISSFloat max;
    

};


#endif  // __ISSRANDOM_H_846C4C36__
