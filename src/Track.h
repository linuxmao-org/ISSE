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

#ifndef __TRACK_H_C2851CE1__
#define __TRACK_H_C2851CE1__

#include "JuceHeader.h"
#include "XMLSerializable.h"
#include "SeparationEngine.h"


class ImgToolbarButton;
class AudioFormatReaderLoopSource;
class Layer;
class MainHostWindow;



class Track :   public Component,
                public ScrollBar::Listener,
                public Button::Listener,
                public Label::Listener,
                public Slider::Listener,
                public AudioTransportSource,
                public XMLSerializable,
                public SeparationEngine::Listener
{
public:
    
    class Listener
    {
    public:
        
        enum Msg{ SetSelected, Mute, Solo, Paint, MouseState, LoadedNewFile, Resized };
        virtual void trackUpdate( Track * track, Listener::Msg msg )=0;
        virtual ~Listener(){}
    };
    

public:
    
    // TODO: Move to AppCommandIDs
//    enum Event{ None = -1,
//                MouseDown = 0,
//                MouseDrag = 2,
//                MouseUp = 3,
//                PlusButton = 4,
//                MinusButton = 5,
//                MuteButton = 6,
//                SoloButton = 7,
//                ShowSpecButton = 8,
//                ShowOverlay1Button = 9,
//                ShowOverlay2Button = 10,
//                PlusMaskButton = 11,
//                ClearImageButton = 12,
//                PluginGraphButton = 13,
//                GainChanged = 14,
//                ScrollBarMoved = 15,
//                PaintBrush = 16,
//                BoxBrush = 17,
//                LineBrush = 18,
//                TrainingBrush = 19,
//                SetSelected,
//                LoadedNewFile,
//                Resized,
//    };
    
    Track(int trackNum_);
    
    ~Track();
    
    
    bool loadNewFile( File file );
    void reset();
    double getLenInSec();
    
    
    virtual void engineNewOutputs( File file, int source );
    virtual void engineTrainingChanged(){}
    virtual void engineNewPlugin( ISSPluginBase * plugin, Msg msg ){}
    
 
    void setLoopMarkers(double startInSec, double stopInSec);
    
    void updateVisualization();
    
    void addToMasks();
    void clearImages();
 
    void addListener( Track::Listener * l);
    void removeListener( Track::Listener * l );

    void setSolo( bool isSolo );
    bool getSolo();
    
    void setSoloMute( bool isMute );
    void setLooping( bool isLooping );
    
    void setSelected( bool selected );
    bool getSelected();
    
    int getSource(){ return trackNum; }
    void setSourceToPaint( int sourceToPaint );// Index staring at 0
    
    virtual void zoomVertical( bool isZoomIn );

    void updateXRangeInSec ( Range<double> range );
    
    static int getPanelWidth(){return panelWidth+border;}
    static int getScrollBarWidth(){ return scrollBarWidth;}
    
    void setGainMultiplier( double gainMult_ )
    {
        gainMult = gainMult_;
        if(!isSoloMuted)
            setGain( getGainFromSlider()*gainMult );
    }
    
    File getFile()
    {
        return file;
    }
    
        static const int minHeight;

public:
    
    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets );
    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  );
    
private:
    
    virtual void mouseDown (const MouseEvent& e);
    virtual void mouseMove(const MouseEvent& e);
    virtual void mouseDrag (const MouseEvent& e);
    virtual void mouseUp(const MouseEvent & e);
    virtual void scrollBarMoved (ScrollBar *scrollBarThatHasMoved, double newRangeStart);
    virtual void sliderValueChanged (Slider *slider);
    virtual void labelTextChanged (Label *labelThatHasChanged);
    virtual void buttonClicked (Button *button);
    void paint (Graphics&g);
    void resized();
 
 
    int   specScreenWidth();
    int   specScreenHeight();
    float getGainFromSlider();
    

    
private:
    
    int trackNum;
    bool isSelected;
    unsigned long selectedPaintSource;
    bool isMouseDown;
    bool isResizeAble;
    bool isLooping;
    bool isSoloMuted;
    float gain;
    float mouseFreq;
    float sampleRate;
    double startMarker, stopMarker;
    double currentLengthInSeconds;
    double gainMult;
    
    
    OwnedArray<Layer> allLayers;// First layers is the audio viz, the second two are the Masks

 
    ScopedPointer<AudioFormatReaderLoopSource> audioReaderSource;
    File file;

    // UI
    ScopedPointer<ImgToolbarButton> muteButton, soloButton;
    ScopedPointer<Slider> volumeKnob;
    ScopedPointer<Label> name, specLabel, overlay1Label, overlay2Label;
    ScopedPointer<ImgToolbarButton> imageButton, imageButton2;
    ScopedPointer<ImgToolbarButton> showSpecButton, showOverlay1Button, showOverlay2Button;
    ScopedPointer<ImgToolbarButton> loadTrainingButton;
    ScopedPointer<ImgToolbarButton> clearImageButton, plusMaskButton;
    ScopedPointer<ScrollBar> yScrollBar;
    ScopedPointer<ImageButton> plusButton, minusButton;
    Range<double> xrange;
    
    Point< int >  mouseDownPoint, mouseDragPoint;
    
    ListenerList <Track::Listener> listeners;
    
    ScopedPointer<MainHostWindow> pluginWindow;
    
    static const int border;
    static const int scrollBarWidth;
    static int panelWidth;
    static const int labelHeight;
  
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Track)
    
};




#endif  // __TRACK_H_C2851CE1__



