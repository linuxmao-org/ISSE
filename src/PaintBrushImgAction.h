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

#ifndef __PAINTBRUSHIMGACTION_H_81600D8C__
#define __PAINTBRUSHIMGACTION_H_81600D8C__

#include "JuceHeader.h"
#include "XMLSerializable.h"
#include "api/ISSDefs.h"

class MaskLayer;

class PaintBrushImgAction : public UndoableAction
//                         ,public XMLSerializable
{
    
public:
    
    PaintBrushImgAction(MaskLayer * mask_, bool isFirstAction_,
                        int xc_, int yc_,
                        float strength_, float sigmaX2_, float sigmaY2_,
                        bool accumulate);
    
    virtual int getSizeInUnits();
    
//    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets );
//    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  );
    
    virtual bool perform ();

    virtual bool undo ();
    
    virtual String getName(){return "PaintBrush";}
    
private:
    
//    void logEvent();
    
    MaskLayer * mask;
    Matrix addedMatrix;
    
    bool isFirstAction;
    int x1;
    int x2;
    int y1;
    int y2;
    int xc;
    int yc;
    float strength;
    float sigmaX2;
    float sigmaY2;
    bool accumulate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaintBrushImgAction);

    
};





#endif  // __PAINTBRUSHACTION_H_81600D8C__
