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

#ifndef __ADDTOMASKACTION_H_7866A198__
#define __ADDTOMASKACTION_H_7866A198__

#include "MaskLayer.h"
#include "JuceHeader.h"



class AddToMaskAction : public UndoableAction
//                        ,public XMLSerializable
{
    
public:
    
    AddToMaskAction(MaskLayer * mask_);
    
    virtual int getSizeInUnits();

    
//    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets )
//    {
//        return false;
//    }
//    
//    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  )
//    {
//        return false;
//    }
    
    virtual bool perform ();
    
    virtual bool 	undo ();
    
    virtual String getName(){return "Add To Mask Action";}
    
private:
    
    MaskLayer * mask;
    
    Image imageBefore;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AddToMaskAction);
    
};





#endif  // __ADDTOMASKACTION_H_7866A198__
