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

#ifndef __DRAWLINEACTION_H_41719768__
#define __DRAWLINEACTION_H_41719768__


#include "JuceHeader.h"
#include "XMLSerializable.h"
#include "BrushActions.h"



class MaskLayer;


class DrawLineAction :  public UndoableAction,
                        public XMLSerializable,
                        public BrushAction
{
    
public:
    
    DrawLineAction( MaskLayer * mask_,int source,
                    int x1_, int x2_,
                    int y1_, int y2_,
                    float strength_, bool erase_);
    
    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets );

    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  );
    
    virtual bool perform ();
    
    virtual bool undo ();
    
    virtual String getName(){return "LineBrush";}

    
private:
    
    int ipart(float x);
    
    float fpart(float x);
    
    float rfpart(float x);
    
    void swap(int & x, int & y);

    void swap(float & x, float & y);
    
    void plot(int x, int y, float c);
    
    void drawLine(float x0,float y0,float x1,float y1, float scale);

    
private:
    
    MaskLayer * mask;
    ScopedPointer<Image> oldImage;
    int source;
    int x1;
    int x2;
    int y1;
    int y2;
    float strength;
    bool erase;
    
};




#endif  // __DRAWLINEACTION_H_41719768__
