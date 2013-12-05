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
#ifndef __ISS_Defs_H__
#define __ISS_Defs_H__

#include <utility>
#include <Eigen/Dense>


inline double round(double val)
{    
    return floor(val + 0.5);
}

#if defined __WINDOWS__

#ifndef isnan
inline bool isnan(double x) {
    return x != x;
}
#endif

#endif


#ifdef __SEPARATION_FLOAT_PRECISION__

    //! \typedef Definition of a MCFloat, so it can change to 32-bit or 64-bit
    typedef float ISSFloat;
    
#else

    //! \typedef Definition of a MCFloat, so it can change to 32-bit or 64-bit
    typedef double ISSFloat;

#endif

typedef Eigen::Matrix<ISSFloat, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> Matrix;

#ifndef ISSEPS
#define ISSEPS (1.4013e-12)
#endif

class ISSException : public std::exception
{
public:
    ISSException(std::string ss) : s(ss+std::string("\n")){}
    
    virtual ~ISSException() throw(){};
    
    virtual const char* what() const throw()
    {
        return s.c_str();
    }
    std::string s;
};



#endif
