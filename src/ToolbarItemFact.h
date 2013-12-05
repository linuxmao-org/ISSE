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

#ifndef __ISSTOOLBARITEMFACTORY_H_9BB7297B__
#define __ISSTOOLBARITEMFACTORY_H_9BB7297B__


#include "JuceHeader.h"


class ProgramSettings;
class MainToolbar;

class ToolbarItemFact : public ToolbarItemFactory
{
public:
    
    ToolbarItemFact( double & progress );
    
    // 	Must return a list of the IDs for all the item types that this factory can create.
    virtual void getAllToolbarItemIds (Array< int > &ids);
    
    //Must return the set of items that should be added to a toolbar as its default set.
    virtual void getDefaultItemSet (Array< int > &ids);
 	
    virtual ToolbarItemComponent * 	createItem (int itemId);

private:
    
    void getButtonInfo(int itemId, char* & icon_png, int & icon_pngSize, String & labelText );
    
    double & progress;
//    MainToolbar * toolbar;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarItemFact);

};

#endif  // __ISSTOOLBARITEMFACTORY_H_9BB7297B__




