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

#include "BoxBrushImgAction.h"
#include "MaskLayer.h"
#include "Main.h"
#include "Track.h"
#include "DocWindow.h"

#include <stdio.h>
#include <math.h>

BoxBrushImgAction::BoxBrushImgAction(MaskLayer * mask_, int source_,
                               int x1_, int x2_, int y1_, int y2_,
                               float strength_, bool accumulate_) :
                mask(mask_), source(source_),
                x1(x1_), x2(x2_), y1(y1_), y2(y2_),
                strength(strength_),accumulate(accumulate_)
{
    addedMatrix = Matrix(x2-x1+1, y2-y1+1);
}


bool BoxBrushImgAction::perform ()
{
    Image * image = mask->getImage();
    Matrix & M = mask->getMatrix();
    int h = M.rows();
    
    float v = strength;    
    float clipLimit(0);
    if(accumulate)
    {
        if(v < 0)
            clipLimit = 0.f;
        else
            clipLimit = 1.f;
    }
    else
        clipLimit = fabs(strength);
    
    

    // Render into image
    for(int x = x1; x <= x2; x++)
    {
		//Logger::outputDebugString(String(x));
        for(int y=y1; y <= y2; y++)
        {
            Colour oldC = image->getPixelAt(x, y);
            float alpha(oldC.getFloatAlpha());
            
            float adding(0);
            if(strength >= 0)
            {
                float resulting = v+alpha;
                if(resulting > clipLimit)
                    resulting = clipLimit;
                    
                adding = resulting - alpha;
            }
            else
            {
                float resulting = v+alpha;
                if(resulting < 0)
                    resulting = 0;
                    
                adding = resulting - alpha;
                
            }
            
            addedMatrix(x-x1, y-y1) = adding;
            
            // Render  into the image
            image->setPixelAt(x, y, oldC.withAlpha( std::max(alpha + adding, 0.f) ) );
            
            // Render into the to internal mask
            M(h-1-y,x) += adding;
        }
    }
    
    mask->repaint();
    mask->dirty();
    MAIN_APP->window->changed();
    
    return true;
}

int BoxBrushImgAction::getSizeInUnits()
{
    return addedMatrix.rows()*addedMatrix.cols()*sizeof(ISSFloat);
    
}

bool BoxBrushImgAction::undo ()
{
    
    Image * image = mask->getImage();
    Matrix & M = mask->getMatrix();
    int h = M.rows();
    
    // Render into image
    for(int x = x1; x <= x2; x++)
    {
        for(int y=y1; y <= y2; y++)
        {
            Colour oldC = image->getPixelAt(x, y);
            float alpha = oldC.getFloatAlpha();
            float adding = -1*addedMatrix(x-x1, y-y1);
            
            // Render  into the image
            image->setPixelAt(x, y, oldC.withAlpha( std::max(alpha + adding, 0.f) ) );
            M(h-1-y,x) += adding;
        }
    }
    
    mask->repaint();
    mask->dirty();
    MAIN_APP->window->changed();
    
    return true;
}


