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

#include "ClearImageAction.h"
#include "DocWindow.h"


ClearImageAction::ClearImageAction(MaskLayer * mask_) : mask(mask_)
{
    Image * image = mask->getImage();
    imageBefore = image->createCopy ();
}

int ClearImageAction::getSizeInUnits()
{
    return imageBefore.getWidth()*imageBefore.getHeight();
}


bool ClearImageAction::perform ()
{
    
    Image * image = mask->getImage();
    
    int w = image->getWidth();
    int h = image->getHeight();
    if(w==0 && h==0)
        return false;
    
    
    
    *image = Image (Image::SingleChannel, w, h, true);
    
    mask->repaint();
    
    MAIN_APP->window->changed();
    
    return true;
}

bool 	ClearImageAction::undo ()
{
    Image * image = mask->getImage();
    *image = imageBefore.createCopy ();
    
    mask->repaint();
    MAIN_APP->window->changed();
    return true;
}

