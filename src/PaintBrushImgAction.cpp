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

#include "PaintBrushImgAction.h"

#include "MaskLayer.h"
#include "Main.h"
#include "Track.h"
#include "DocWindow.h"


#include <stdio.h>
#include <math.h>


PaintBrushImgAction::PaintBrushImgAction(MaskLayer * mask_,
                                         bool isFirstAction_,
                                         int xc_, int yc_,
                                         float strength_,
                                         float sigmaX2_,
                                         float sigmaY2_,
                                        bool accumulate_)
        :
    mask(mask_),  isFirstAction(isFirstAction_), xc(xc_), yc(yc_),
    strength(strength_), sigmaX2(sigmaX2_), sigmaY2(sigmaY2_), accumulate(accumulate_)
{

    
    const int lim = 100;
    x1 = std::max(0, xc-lim);
    x2 = std::min(xc+lim, mask->getImageWidth()-1);
    y1 = std::max(0, yc-lim);
    y2 = std::min(yc+lim, mask->getImageHeight()-1);

    addedMatrix = Matrix(x2-x1+1, y2-y1+1);
}


//bool PaintBrushImgAction::saveToXML( XmlElement & xml, const File & folderForAssets )
//{
//    XmlElement * root = &xml;    
//    XmlElement * elem = root->createNewChildElement ("X1");
//    elem->addTextElement(String(x1));
//    
//    elem = root->createNewChildElement ("X2");
//    elem->addTextElement(String(x2));
//    
//    elem = root->createNewChildElement ("Y1");
//    elem->addTextElement(String(y1));
//    
//    elem = root->createNewChildElement ("Y2");
//    elem->addTextElement(String(y2));
//    
//    elem = root->createNewChildElement ("Xc");
//    elem->addTextElement(String(xc));
//    
//    elem = root->createNewChildElement ("Yc");
//    elem->addTextElement(String(yc));
//    
//    elem = root->createNewChildElement ("Strength");
//    elem->addTextElement(String(strength));
//    
//    elem = root->createNewChildElement ("SigmaX2");
//    elem->addTextElement(String(sigmaX2));
//    
//    elem = root->createNewChildElement ("SigmaY2");
//    elem->addTextElement(String(sigmaY2));
//    
//    elem = root->createNewChildElement ("First");
//    elem->addTextElement(String(isFirstAction));
//    
//    return true;
//}
//bool PaintBrushImgAction::loadFromXML( const XmlElement & xml, const File & assetsFolder  )
//{
//    
//    XmlElement * elem = xml.getChildByName ("X1");
//    if(elem)
//        x1 = elem->getAllSubText().getIntValue ();
//    
//    elem = xml.getChildByName ("X2");
//    if(elem)
//        x2 = elem->getAllSubText().getIntValue ();
//    
//    elem = xml.getChildByName ("Y1");
//    if(elem)
//        y1 = elem->getAllSubText().getIntValue ();
//    
//    elem = xml.getChildByName ("Y2");
//    if(elem)
//        y2 = elem->getAllSubText().getIntValue ();
//    
//    elem = xml.getChildByName ("Xc");
//    if(elem)
//        xc = elem->getAllSubText().getIntValue ();
//    
//    elem = xml.getChildByName ("Yc");
//    if(elem)
//        yc = elem->getAllSubText().getIntValue ();
//    
//    elem = xml.getChildByName ("Strength");
//    if(elem)
//        strength = elem->getAllSubText().getFloatValue ();
//    
//    elem = xml.getChildByName ("SigmaX2");
//    if(elem)
//        sigmaX2 = elem->getAllSubText().getFloatValue ();
//    
//    elem = xml.getChildByName ("SigmaY2");
//    if(elem)
//        sigmaY2 = elem->getAllSubText().getFloatValue ();
//    
//    elem = xml.getChildByName ("First");
//    if(elem)
//        isFirstAction = elem->getAllSubText().getIntValue ();
//    addedMatrix = Matrix(x2-x1+1, y2-y1+1);
//    return true;
//}

//void PaintBrushImgAction::logEvent()
//{
//    XmlElement xml("Event");
//    xml.setAttribute("type", "Track");
//    xml.setAttribute("source", (int)mask->getSource());
//    xml.setAttribute("time", ISSTIME);
//    xml.setAttribute("subtype", Track::PaintBrush);
//    saveToXML(xml,File(""));
//    
//    ISSLOG(String(xml.createDocument(String::empty, false,false)));
//}

int PaintBrushImgAction::getSizeInUnits()
{
    return addedMatrix.rows()*addedMatrix.cols()*sizeof(ISSFloat);
}

bool PaintBrushImgAction::perform ()
{
    Image * image = mask->getImage();
    Matrix & M = mask->getMatrix();
    int h = M.rows();
    float clipLimit(0);
    if(accumulate)
    {
        if(strength <= 0)
            clipLimit = 0.f;
        else if( (strength >= 0))
            clipLimit = 1.f;
    }
    else
        clipLimit = fabs(strength);

    {
        for(int x=x1; x <= x2; x++)
        {
            for(int y=y1; y <= y2; y++)
            {
         
                Colour oldC = image->getPixelAt(x, y);
                float alpha = oldC.getFloatAlpha();
                float v = strength*exp(-((x-xc)*(x-xc)/(sigmaX2/5) +
                                        ((y-yc)*(y-yc)/(5*sigmaY2))));
            
                float adding(0);
                
                if(strength >= 0)
                {
                    if(alpha < clipLimit)
                    {
                        adding = std::max(0.f, std::min(alpha+v, clipLimit))-alpha;
                        if(alpha+adding > clipLimit)
                            adding = 0;
                    }
                    else
                        adding = 0;
                }
                else
                {
                   
                    if(alpha < clipLimit)
                         adding = 0;
                    else
                    {
                        double resulting = v+alpha;
                        if(resulting < clipLimit)
                            resulting = clipLimit;
                        
                        adding = resulting - alpha;
                    }
                    
                }
                
                addedMatrix(x-x1, y-y1) = adding;
                image->setPixelAt(x, y, oldC.withAlpha( std::max(alpha + adding, 0.f) ) );
                M(h-1-y,x) += adding;
            }
        }
    }

    mask->dirty();
    mask->repaint();
    MAIN_APP->window->changed();
//    logEvent();
    
    return true;
}

bool PaintBrushImgAction::undo ()
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
            
            // Render into the to internal mask
            M(h-1-y,x) += adding;
        }
    }
    
    mask->repaint();
    mask->dirty();
    MAIN_APP->window->changed();
    
    return true;
}





