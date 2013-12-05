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

#ifndef __TRAININGSELECTIONACTION_H_E871D962__
#define __TRAININGSELECTIONACTION_H_E871D962__

#include "JuceHeader.h"
#include "XMLSerializable.h"

#include <map>

class TrainingSelectionAction : public UndoableAction
//                                ,public XMLSerializable
{
    
public:
    
    TrainingSelectionAction(unsigned long source_,
                            bool erase_,
                            int xmin_,
                            int xmax_);
    
    virtual int getSizeInUnits();
    
//    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets );
//    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  );
    
    virtual bool perform ();

    virtual bool undo ();
    
    virtual String getName(){return "TrainingBrush";}

private:
    
    void logEvent();

    size_t source;
    bool erase;
    size_t xmin;
    size_t xmax;
//    std::map<unsigned long, unsigned long> training;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainingSelectionAction);

    
};



#endif  // __TRAININGSELECTIONACTION_H_E871D962__
