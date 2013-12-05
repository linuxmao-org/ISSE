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

#include "TrainingSelectionAction.h"
#include "SeparationEngine.h"
#include "Track.h"
#include "Main.h"
#include "DocWindow.h"

#include <stdio.h>
#include <math.h>




TrainingSelectionAction::TrainingSelectionAction(
                        unsigned long source_, bool erase_, int xmin_, int xmax_)
:    source(source_), erase(erase_),
    xmin(xmin_), xmax(xmax_)
{}

//bool TrainingSelectionAction::saveToXML( XmlElement & xml, const File & folderForAssets )
//{
//    XmlElement* elem = xml.createNewChildElement ("Xmin");
//    elem->addTextElement(String(((int)xmin)));
//    
//    
//    elem = xml.createNewChildElement ("Xmax");
//    elem->addTextElement(String(((int)xmax)));
//    
//    
//    elem = xml.createNewChildElement ("Erase");
//    elem->addTextElement(String(erase));
//    
//    return true;
//}
//bool TrainingSelectionAction::loadFromXML( const XmlElement & xml, const File & assetsFolder  )
//{
//    
//    XmlElement * elem = xml.getChildByName ("Xmin");
//    if(elem)
//        xmin = elem->getAllSubText().getIntValue ();
//    
//    elem = xml.getChildByName ("Xmax");
//    if(elem)
//        xmax = elem->getAllSubText().getIntValue ();
//    
//    elem = xml.getChildByName ("Erase");
//    if(elem)
//        erase = elem->getAllSubText().getIntValue ();
//    
// 
//    return true;
//}

int TrainingSelectionAction::getSizeInUnits()
{
    return sizeof(size_t)*5;
}


bool TrainingSelectionAction::perform ()
{ 
    if(erase)
        SeparationEngine::getInstance()->subtractTraining(source, xmin, xmax);
    else
        SeparationEngine::getInstance()->addTraining(source, xmin, xmax);
    MAIN_APP->window->changed();
    return true;
}

void TrainingSelectionAction::logEvent()
{

}

bool TrainingSelectionAction::undo ()
{
    if(erase)
        SeparationEngine::getInstance()->addTraining(source, xmin, xmax);
    else
        SeparationEngine::getInstance()->subtractTraining(source, xmin, xmax);
    
    MAIN_APP->window->changed();
    
    return true;
}



