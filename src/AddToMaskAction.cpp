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

#include "AddToMaskAction.h"
#include "Main.h"
#include "DocWindow.h"

AddToMaskAction::AddToMaskAction(MaskLayer * mask_) : mask(mask_)
{
    Image * image = mask->getImage();
    imageBefore = image->createCopy();

}

int AddToMaskAction::getSizeInUnits()
{
    
    return imageBefore.getWidth()*imageBefore.getHeight();
}

bool AddToMaskAction::perform ()
{
    if(mask==NULL)
        return false;
    
    Matrix & M = mask->getMatrix();
    int h = M.rows();
    int w = M.cols();
    
    Image * image = mask->getImage();

    // Render into image
    for(int x = 0; x < w; x++)
        for(int y = 0; y < h; y++)
            M(h-1-y,x) += image->getPixelAt(x, y).getFloatAlpha();
        
  
    
    mask->repaint();
    mask->dirty();
    MAIN_APP->window->changed();
    
    return true;
}

bool 	AddToMaskAction::undo ()
{
    if(mask==NULL)
        return false;
    
    Matrix & M = mask->getMatrix();
    int h = M.rows();
    int w = M.cols();
    
    Image * image = mask->getImage();
    
    // Render into image
    for(int x = 0; x < w; x++)
        for(int y = 0; y < h; y++)
            M(h-1-y,x) -= image->getPixelAt(x, y).getFloatAlpha();
    
    
    
    mask->repaint();
    mask->dirty();
    MAIN_APP->window->changed();
    
    return true;
}










