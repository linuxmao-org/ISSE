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
#ifndef __ImageLayer_H_2CFEDD90__
#define __ImageLayer_H_2CFEDD90__

#include "JuceHeader.h"

#include "Layer.h"
#include "VizRenderThread.h"
#include "SeparationEngine.h"


class ImageLayer :  public Layer,
                    public VizRenderThread::Listener,
                    public SeparationEngine::Listener,
                    public Timer
{
public:
    
    ImageLayer( int trackNum );
    
    ~ImageLayer();

    void setFile( File file_ );

    void update();
    
    virtual void 	timerCallback ();
    
    virtual void paint(Graphics &g);

    virtual void vizRenderThreadDone( ScopedPointer<Image> & spectrogram );
    
    virtual void engineNewPlugin( ISSPluginBase * plugin, Msg msg );
    virtual void engineNewOutputs( File file, int source ){};
    virtual void engineTrainingChanged();
    
private:

    void screen2ImageSpace(int x, int y, double & xImagespace, double & yImagespace);

    
    ScopedPointer<ISSStft> stft;
    ScopedPointer<Image> image;

    File file;
    
    int trackNum;
    
    
    
    ScopedPointer<VizRenderThread> vizRenderThread;
    bool dirty;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageLayer)
};




#endif  // __ImageLayer_H_2CFEDD90__
