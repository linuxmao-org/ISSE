/*
 Copyright 2013 Stanford University
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
#ifndef __COLORMAP_H_89EAD0D7__
#define __COLORMAP_H_89EAD0D7__

#include <map>

#include "JuceHeader.h"

class ColorMap
{
public:
    
    enum ColorMapType{  purpleColorMap=1,
                        jetColorMap,
                        blackAndWhiteColorMap,
                        coolColorMap,
                        hotColorMap,
                        hueSaturationColorMap,
                        springColorMap,
                        summerColorMap,
                        autumnColorMap,
                        winterColorMap,
                        boneColorMap,
                        copperColorMap,
                        pinkColorMap,
                        whiteAndBlackColorMap,
    };

public:

    ColorMap();
    
    //! Set an the internal color map used for getColor()
    void setColorMap(ColorMapType type);
    
    //! Returns the currently set color map
    ColorMapType getColorMap(){ return colorMapType;}
    
    // Stores an internal state of the min and max normalization values for rendering an intensity map
    void setMaxAndMin(float max, float min);
    
    inline Colour getColor(float v)
    {
        if(maxValue==minValue)
            return Colour();

        int index = ((v-minValue)/(maxValue-minValue))*(cmLength-1);
        return Colour(COLOR[index*3],
                      COLOR[index*3+1],
                      COLOR[index*3+2]);
    }

    
    
    Colour getColor(float v, Colour c);
    Colour getColor(float v, float vmin, float vmax);
    Colour getColor(float v, float vmin, float vmax, ColorMapType type);
    Colour getColor(float v, float vmin, float vmax, Colour c);
     
public:
    static std::map<ColorMap::ColorMapType, std::string> getAllColorMaps();
    
private:
    
    void colorSwitch(uint8 *&COLOR, ColorMapType colorMapType);
    
    uint8 *COLOR;
    ColorMapType colorMapType;
    float maxValue, minValue;
    
    static const int cmLength;
    static const uint8 PURPLE[192];
    static const uint8 BLACKANDWHITE[192];
    static const uint8 JET[192];
    static const uint8 COOL[192];
    static const uint8 HOT[192];
    static const uint8 HSV[192];
    static const uint8 SPRING[192];
    static const uint8 SUMMER[192];
    static const uint8 AUTUMN[192];
    static const uint8 WINTER[192];
    static const uint8 BONE[192];
    static const uint8 WHITEANDBLACK[192];
    static const uint8 COPPER[192];
    static const uint8 PINK[192];
};






#endif  // __COLORMAP_H_89EAD0D7__
