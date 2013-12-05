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

#include "ClearMaskAction.h"

#include "Main.h"
#include "DocWindow.h"
#include "api/ISSMask.h"

ClearMaskAction::ClearMaskAction(Array<ISSMask*> & masks_)
{
    
    for(int i=0; i < masks_.size(); ++i)
    {
        masks.add(masks_[i]);
        ownedMasks.add( new ISSMask(*masks[i]) );
    }
}

int ClearMaskAction::getSizeInUnits()
{
    int numBytes(0);
    for(int i=0; i < ownedMasks.size(); ++i)
    {
        Matrix & M = masks[i]->getMask();
        numBytes+=M.rows()*M.cols()*sizeof(ISSFloat);
        
    }
    return numBytes;
}

bool ClearMaskAction::perform ()
{
    for(int i=0; i < masks.size(); ++i)
        masks[i]->clearMask();
    
    
    return true;
}

bool 	ClearMaskAction::undo ()
{

    for(int i=0; i < ownedMasks.size(); ++i)
    {
        masks[i]->getMask() = ownedMasks[i]->getMask();
         
    }
    
    return false;
}









