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

#ifndef __CLEARMASKACTION_H_C3F10769__
#define __CLEARMASKACTION_H_C3F10769__

#include "MaskLayer.h"
#include "JuceHeader.h"


class ClearMaskAction : public UndoableAction
//                        ,public XMLSerializable
{
    
public:
    
    ClearMaskAction(Array<ISSMask*> & masks);
    
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
    
    virtual String getName(){return "Clear Mask Action";}
    
private:
    
    Array<ISSMask*> masks;
    OwnedArray<ISSMask> ownedMasks;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClearMaskAction);
    
};







#endif  // __CLEARMASKACTION_H_C3F10769__
