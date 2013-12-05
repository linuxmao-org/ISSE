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
#include "ImageLayer.h"
#include "ProgramSettings.h"
#include "SeparationEngine.h"
 

ImageLayer::ImageLayer(int trackNum_ )
:  image ( new Image(Image::RGB, 100, 100, true)), trackNum(trackNum_), vizRenderThread(NULL), dirty(false)
{
    setVisible(true);

//    stft = new ISSStft(SeparationEngine::getInstance()->getSTFTParams());
    stft = NULL;
    
    SeparationEngine::getInstance()->addListener(this);
    
    startTimer(100);
}

ImageLayer::~ImageLayer()
{
    stopTimer();
    
    if(vizRenderThread)
        vizRenderThread->stopThread (100);
    
  
    
    SeparationEngine * engine = SeparationEngine::getInstanceWithoutCreating();
    if(engine) engine->removeListener(this);
}



void ImageLayer::update()
{
    dirty = true;
   
}


void ImageLayer::timerCallback ()
{
    if(vizRenderThread && vizRenderThread->isThreadRunning ())
        return; // Do nothing
    
    
    if(dirty && file.exists() && stft)
    {
        ProgramSettings s;
        Colour c1(s.sourceColors[0]);
        Colour c2(s.sourceColors[1]);
        float clipLimit(s.spectrogramClipLimit);
        ColorMap::ColorMapType type(s.colorMapType);
        
        vizRenderThread = new VizRenderThread(this, &file, stft,
                                              SeparationEngine::getInstance()->getTraining(),
                                              c1, c2, clipLimit, type, trackNum);
        vizRenderThread->startThread();
        dirty = false;
    }
    
}


void ImageLayer::vizRenderThreadDone( ScopedPointer<Image> & spectrogram )
{
    const MessageManagerLock mml (Thread::getCurrentThread());
    if (! mml.lockWasGained())
        return;
    
    // copy
    image = spectrogram.release();
    repaint();
}


void ImageLayer::paint(Graphics &g)
{
    if(0==getMaxX()) return;
    
    int w(getWidth());
    int h(getHeight());
    
    double sourceX(0), sourceY(0);
    double sourceXEnd(0), sourceYEnd(0);
    screen2ImageSpace(0, 0, sourceX, sourceY);
    screen2ImageSpace(w, h, sourceXEnd, sourceYEnd);
    
    g.drawImage(*image, 0, 0, w, h,
                sourceX, sourceY, sourceXEnd-sourceX, sourceYEnd-sourceY);

}
void ImageLayer::screen2ImageSpace(int x, int y, double & xImagespace, double & yImagespace)
{
    double currentLengthInSeconds(getMaxX());
    Range<double> xrange(getXRange());
    Range<double> cr(getYRange());
    double sourceX = xrange.getStart();
    double sourceXEnd =  xrange.getEnd();
    double sourceY =     cr.getStart();
    double sourceYEnd =  cr.getEnd();
    
    double imgWidth(image->getWidth());
    double imgHeight(image->getHeight());
    xImagespace = imgWidth*(sourceX + (sourceXEnd - sourceX)*x/(getWidth()*1.0))/currentLengthInSeconds;
    yImagespace = imgHeight*(1.0f/getMaxY())*(sourceY + (sourceYEnd - sourceY)*(y*1.0)/getHeight());
    
}


void ImageLayer::setFile( File file_ )
{
    file = file_;
    
    if(file.exists())
    {
        update();
    }
    else
    {
        
        const MessageManagerLock mml (Thread::getCurrentThread());
        if (! mml.lockWasGained())
            return;
        image =  new Image (Image::RGB, 100, 100, true); // default engine
    }
}



void ImageLayer::engineTrainingChanged()
{
    update();
}

// used to reset the stft parameters and re-render
void ImageLayer::engineNewPlugin( ISSPluginBase * plugin, Msg msg )
{
    // Update viz for all messages
    
    if(vizRenderThread)
    {
        vizRenderThread->signalThreadShouldExit ();
        vizRenderThread->stopThread(1000);
    }
    
    stft = new ISSStft( plugin->getSTFTParams() );
    update();
}

 

