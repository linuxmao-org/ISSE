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

#include "DrawLineAction.h"
#include "MaskLayer.h"
#include "Main.h"

#include <stdio.h>
#include <math.h>

DrawLineAction::DrawLineAction( MaskLayer * mask_, int source_,
               int x1_, int x2_,
               int y1_, int y2_,
               float strength_, bool erase_) :
mask(mask_),
source(source_),
x1(x1_), x2(x2_),
y1(y1_), y2(y2_),
strength(strength_), erase(erase_)
{
    
}

bool DrawLineAction::saveToXML( XmlElement & xml, const File & folderForAssets )
{
    XmlElement* elem = xml.createNewChildElement ("Track");
    elem->addTextElement(String(source));
    
    elem = xml.createNewChildElement ("X1");
    elem->addTextElement(String(x1));
    
    elem = xml.createNewChildElement ("X2");
    elem->addTextElement(String(x2));
    
    elem = xml.createNewChildElement ("Y1");
    elem->addTextElement(String(y1));
    
    elem = xml.createNewChildElement ("Y2");
    elem->addTextElement(String(y2));
    
    elem = xml.createNewChildElement ("Strength");
    elem->addTextElement(String(strength));
    
    elem = xml.createNewChildElement ("Erase");
    elem->addTextElement(String(erase));
    
    return true;
}

bool DrawLineAction::loadFromXML( const XmlElement & xml, const File & assetsFolder  )
{
    
    XmlElement * elem = xml.getChildByName ("X1");
    if(elem)
        x1 = elem->getAllSubText().getIntValue ();
    
    elem = xml.getChildByName ("X2");
    if(elem)
        x2 = elem->getAllSubText().getIntValue ();
    
    elem = xml.getChildByName ("Y1");
    if(elem)
        y1 = elem->getAllSubText().getIntValue ();
    
    elem = xml.getChildByName ("Y2");
    if(elem)
        y2 = elem->getAllSubText().getIntValue ();
    
    elem = xml.getChildByName ("Strength");
    if(elem)
        strength = elem->getAllSubText().getFloatValue ();
    
    elem = xml.getChildByName ("Erase");
    if(elem)
        erase = elem->getAllSubText().getIntValue ();
    
    return false;
}


bool DrawLineAction::perform ()
{
    // Bounding box limits
    int x1_ = std::min(x1, x2);
    int x2_ = std::max(x1, x2);
    int y1_ = std::min(y1, y2);
    int y2_ = std::max(y1, y2);
    
    
    oldImage = new Image(Image::SingleChannel, x2_-x1_+1, y2_-y1_+1, false);
    Image*image = mask->getImage();
    for(int x = x1_; x <= x2_; x++)
    {
        for(int y=y1_; y <= y2_; y++)
        {
            Colour oldC = image->getPixelAt(x, y);
            oldImage->setPixelAt(x-x1_, y-y1_, oldC);
        }
    }
    
    
    float v = 1;
    for(int i=-v; i <v; ++i )
        drawLine(x1+i, y1+i, x2+i, y2+i, fabs(fabs(i)-fabs(v))/v   );
    
    
    mask->repaint();
    mask->dirty();
    
    MAIN_APP->window->changed();
    
    return true;
}

bool 	DrawLineAction::undo ()
{
    
    int x1_ = std::min(x1, x2);
    int x2_ = std::max(x1, x2);
    int y1_ = std::min(y1, y2);
    int y2_ = std::max(y1, y2);
    Image*image=mask->getImage();
    for(int x = x1_; x <= x2_; x++)
        for(int y=y1_; y <= y2_; y++)
            image->setPixelAt(x, y, oldImage->getPixelAt(x-x1_, y-y1_));
    
    mask->repaint();
    mask->dirty();
    
    MAIN_APP->window->changed();
    
    return true;
}

 
int DrawLineAction::ipart(float x)
{
    return floor(x);
}

float DrawLineAction::fpart(float x)
{
    return x - ipart(x);
}

float DrawLineAction::rfpart(float x)
{
    return 1 - fpart(x);
}

void DrawLineAction::swap(int & x, int & y)
{
    int t = x;
    x = y;
    y = t;
}
void DrawLineAction::swap(float & x, float & y)
{
    int t = x;
    x = y;
    y = t;
}

void DrawLineAction::plot(int x, int y, float c)
{
    Image*image = mask->getImage();
    Colour oldC = image->getPixelAt(x, y);
    float alpha = oldC.getFloatAlpha();
    
    image->setPixelAt(x, y, oldC.withAlpha(  std::max(0.f, std::min(alpha+c, 1.f))    ) );
}

void DrawLineAction::drawLine(float x0,float y0,float x1,float y1, float scale)
{
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    
    if (steep)
    {
        swap(x0, y0);
        swap(x1, y1);
    }
    
    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    
    float dx = x1 - x0;
    float dy = y1 - y0;
    
    if(dx==0)
    {
        for( int y = y0; y < y1; y++)
        {
            plot(x0, y, 1);
        }
        return;
    }
    
    float gradient = dy / dx;
    
    
    // handle first endpoint
    float xend = round(x0);
    
    float yend = y0 + gradient * (xend - x0);
    float xgap = rfpart(x0 + 0.5);
    float xpxl1 = xend;   //this will be used in the main loop
    float ypxl1 = ipart(yend);
    
    if(steep)
    {
        plot(ypxl1,   xpxl1, rfpart(yend) * xgap*scale);
        plot(ypxl1+1, xpxl1,  fpart(yend) * xgap*scale);
    }
    else
    {
        plot(xpxl1, ypxl1, rfpart(yend) * xgap*scale);
        plot(xpxl1, ypxl1+1, fpart(yend) * xgap*scale);
    }
    
    float intery = yend + gradient;
    
    // handle second endpoint
    xend = round(x1);
    yend = y1 + gradient * (xend - x1);
    xgap = fpart(x1 + 0.5);
    float xpxl2 = xend; //this will be used in the main loop
    float ypxl2 = ipart(yend);
    if(steep)
    {
        plot(ypxl2  , xpxl2, rfpart(yend) * xgap*scale);
        plot(ypxl2+1, xpxl2,  fpart(yend) * xgap*scale);
    }
    else
    {
        plot(xpxl2, ypxl2,  rfpart(yend) * xgap*scale);
        plot(xpxl2, ypxl2+1, fpart(yend) * xgap*scale);
    }
    // main loop
    for( int x = xpxl1 + 1; x < xpxl2; x++)
    {
        if(steep)
        {
            plot(ipart(intery)  , x, rfpart(intery)*scale);
            plot(ipart(intery)+1, x,  fpart(intery)*scale);
        }
        else
        {
            plot(x, ipart (intery),  rfpart(intery)*scale);
            plot(x, ipart (intery)+1, fpart(intery)*scale);
        }
        
        intery = intery + gradient;
    }
}


