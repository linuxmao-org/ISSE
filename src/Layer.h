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

#ifndef __LAYER_H_AB47B5CB__
#define __LAYER_H_AB47B5CB__

#include "JuceHeader.h"


class Layer : public Component
{
public:
    
    Layer() : rY(0,0), rX(0,0), maxY(0), maxX(0)
    { }
    
    virtual ~Layer(){}
    
//    virtual void screen2ImageSpace(int x, int y, int & xImagespace, int & yImagespace)=0;
    
    //! Set y-axis range, limits are (0,maxY)
    void setYRange( Range<double> rY, double maxY);

    //! Set x-axis range, limits are (0, maxX)
    void setXRange( Range<double> rX, double maxX);
    
//    void setPixelSize( size_t width, size_t height);

    //! Get y-axis range
    Range<double> getYRange();
    
    //! Get x-axis range 
    Range<double> getXRange();
    
    double getMaxX();
    double getMaxY();
    virtual void paint(Graphics &g){}
    
private:
        
    Range<double> rY;
    Range<double> rX;
    double maxY;
    double maxX;
};



#endif  // __LAYER_H_AB47B5CB__
