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
#include "Track.h"

#include "ProgramSettings.h"
#include "ToolbarState.h"

#include "ImgToolbarButton.h"
#include "ImgBinaryData.h"
#include "AudioFormatReaderLoopSource.h"
#include "plugin/MainHostWindow.h"

#include "Main.h"
#include "ISSLogger.h"

#include "MaskLayer.h"
#include "ImageLayer.h"
#include "DocWindow.h"
#include "MultiTrack.h"


#include <math.h>

// Helper function
ImgToolbarButton * createImageButton(String buttonName, char * imageName, int imageSize)
{
    int itemId = 100;
    DrawableImage * img = new DrawableImage();
    DrawableImage * img2 = new DrawableImage();
    img->setImage ( ImageCache::getFromMemory( imageName, imageSize ) );
    img2->setImage ( ImageCache::getFromMemory  (imageName , imageSize ));
    
    ImgToolbarButton * b = new ImgToolbarButton(itemId, buttonName, img, img2,
                                                ProgramSettings::iconColorDown );
    b->setOverlayColour(ProgramSettings::iconColor);
    return b;
}

const int Track::scrollBarWidth = 12;
const int Track::border = 1;
int Track::panelWidth = 150;
const int Track::labelHeight = 20;
const int Track::minHeight(150);


Track::Track(int trackNum_)  :
    trackNum(trackNum_),
    isSelected(false),
    selectedPaintSource(0),
    isMouseDown(false),
    isResizeAble(false),
    isLooping(true),
    isSoloMuted(false),
    gain(1.0),
    mouseFreq(0),
    sampleRate(44100),
    startMarker(0),
    stopMarker(1.0),
    currentLengthInSeconds(0),
    gainMult(1),
    yScrollBar(NULL),
    plusButton(NULL),
    minusButton(NULL),
    xrange(0.f,30.f)
{
    
    // TODO: should this be here
    SeparationEngine::getInstance()->addListener(this);
    
    String trackName;
    
    char * icon_png = NULL;
    int icon_pngSize = 0;
    
    

    if(trackNum==0)
    {
        trackName = String("Mixture");
        icon_png = (char*)ImgBinaryData::lowertriangleicon_png;
        icon_pngSize = (int)ImgBinaryData::lowertriangleicon_pngSize;
        
        imageButton = createImageButton(trackName, icon_png, icon_pngSize);
        imageButton->setOverlayColour(ProgramSettings::sourceColors[0]);
        imageButton->setBounds(2*border, 2*border, labelHeight, labelHeight);
        imageButton->addListener (this);
        addAndMakeVisible (imageButton);
        
        icon_png = (char*)ImgBinaryData::uppertriangleicon_png;
        icon_pngSize = (int)ImgBinaryData::uppertriangleicon_pngSize;
        
        imageButton2 = createImageButton(trackName, icon_png, icon_pngSize);
        imageButton2->setOverlayColour(ProgramSettings::sourceColors[1]);
        imageButton2->setBounds(2*border, 2*border, labelHeight, labelHeight);
        imageButton2->addListener (this);
        addAndMakeVisible (imageButton2);
    }
    else   
    {
        trackName = String("Source ") + String(trackNum);
        icon_png = (char*)ImgBinaryData::solidcoloricon_png;
        icon_pngSize = (int)ImgBinaryData::solidcoloricon_pngSize;
        
        imageButton = createImageButton(trackName, icon_png, icon_pngSize);
        imageButton->setOverlayColour(ProgramSettings::sourceColors[trackNum-1]);
        imageButton->setBounds(2*border, 2*border, labelHeight, labelHeight);
        imageButton->addListener (this);
        addAndMakeVisible (imageButton);
    }
    
    icon_png = (char*)ImgBinaryData::clearicon_png;
    icon_pngSize = (int)ImgBinaryData::clearicon_pngSize;
    clearImageButton = createImageButton("Clear Overlays", icon_png, icon_pngSize);
    clearImageButton->addListener (this);
    clearImageButton->setBounds(panelWidth-border -labelHeight,
                               getHeight()-labelHeight, labelHeight, labelHeight);
    addAndMakeVisible (clearImageButton);
  
    
    icon_png = (char*)ImgBinaryData::plusicon_png;
    icon_pngSize = (int)ImgBinaryData::plusicon_pngSize;
    plusMaskButton = createImageButton("Add to Overlays", icon_png, icon_pngSize);
    plusMaskButton->addListener (this);
    plusMaskButton->setBounds(panelWidth-border -2*labelHeight - scrollBarWidth,
                              getHeight()-labelHeight, labelHeight, labelHeight);
    addAndMakeVisible (plusMaskButton);


    
    addAndMakeVisible (name = new Label ("Name",trackName));
    name->setTooltip("Track Name");
    name->setFont (Font (17.4000f, Font::plain));
    name->setJustificationType (Justification::centredLeft);
    name->setEditable (false, false, false);
    name->setColour (Label::outlineColourId, Colours::black);
    name->setColour (TextEditor::textColourId, Colours::black);
    name->setColour (TextEditor::backgroundColourId, Colours::white);
    name->setColour (Label::backgroundColourId, Colours::white);
    name->setBounds (imageButton->getX() + imageButton->getWidth(),2*border,
                     panelWidth-(imageButton->getX() + imageButton->getWidth())-2*border, labelHeight);
    name->addListener (this);
    

    volumeKnob = new Slider();
    volumeKnob->setSliderStyle(Slider::RotaryHorizontalDrag);
    volumeKnob->setRotaryParameters	( -2*M_PI/3, 2*M_PI/3,true);
    
    volumeKnob->setColour(Slider::rotarySliderFillColourId, ProgramSettings::iconColor);
    volumeKnob->setColour(Slider::textBoxTextColourId, Colours::black);
    volumeKnob->setRange(-100.0, 15.0, .01);
    volumeKnob->setSkewFactorFromMidPoint(0);
    
    addAndMakeVisible(volumeKnob);
    
    
    icon_png = (char*)ImgBinaryData::muteicon_png;
    icon_pngSize = (int)ImgBinaryData::muteicon_pngSize;
    muteButton = createImageButton(String("Mute"), icon_png, icon_pngSize);
    
    icon_png = (char*)ImgBinaryData::soloicon_png;
    icon_pngSize = (int)ImgBinaryData::soloicon_pngSize;
    soloButton = createImageButton(String("Solo"), icon_png, icon_pngSize);
    
    muteButton->setBounds(border, name->getBottom()+1, labelHeight, labelHeight);
    soloButton->setBounds(muteButton->getWidth() + muteButton->getX(),
                          name->getBottom()+1,labelHeight,labelHeight);
    volumeKnob->setBounds(soloButton->getWidth() + soloButton->getX(),
                          name->getBottom()+1, panelWidth-scrollBarWidth-(soloButton->getWidth() +
                                    soloButton->getX())-2*border, labelHeight);
    volumeKnob->setTextBoxStyle(Slider::TextBoxRight, false,
                                volumeKnob->getWidth(), labelHeight);
    volumeKnob->setTooltip("Gain (dB)");
    volumeKnob->addListener(this);
    muteButton->addListener(this);
    soloButton->addListener(this);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(soloButton);
    
    // Layer Visibiliby stuff
    icon_png = (char*)ImgBinaryData::eyeicon_png;
    icon_pngSize = (int)ImgBinaryData::eyeicon_pngSize;
    
    showSpecButton = createImageButton("Show Visualization", icon_png, icon_pngSize);
    showOverlay1Button = createImageButton("Show Overlay 1", icon_png, icon_pngSize);
    showOverlay2Button = createImageButton("Show Overlay 2", icon_png, icon_pngSize);
    
    showSpecButton->addListener(this);
    showOverlay1Button->addListener(this);
    showOverlay2Button->addListener(this);
    
    showSpecButton->setToggleState(true, dontSendNotification );
    showOverlay1Button->setToggleState(true, dontSendNotification );
    showOverlay2Button->setToggleState(true, dontSendNotification );
    
    showSpecButton->setClickingTogglesState(true);
    showOverlay1Button->setClickingTogglesState(true);
    showOverlay2Button->setClickingTogglesState(true);
    
    showSpecButton->setBounds(border,muteButton->getBottom(), labelHeight, labelHeight);
    showOverlay1Button->setBounds(border,showSpecButton->getBottom(), labelHeight, labelHeight);
    showOverlay2Button->setBounds(border,showOverlay1Button->getBottom(), labelHeight, labelHeight);
    
    addAndMakeVisible(showSpecButton);
    addAndMakeVisible(showOverlay1Button);
    addAndMakeVisible(showOverlay2Button);
    
    // Load training button
    icon_png = (char*)ImgBinaryData::openicon_png;
    icon_pngSize = (int)ImgBinaryData::openicon_pngSize;
    loadTrainingButton = createImageButton("TODO Load Example Sounds", icon_png, icon_pngSize);
    loadTrainingButton->addListener(this);
    loadTrainingButton->setBounds(border, getHeight()-labelHeight, labelHeight, labelHeight);

    addAndMakeVisible(loadTrainingButton);
    
    loadTrainingButton->setVisible(false);
    
    
    
    addAndMakeVisible (specLabel = new Label ("Viz","Viz"));
    specLabel->setFont (Font (14.4000f, Font::plain));
    specLabel->setJustificationType (Justification::centredLeft);
    specLabel->setEditable (false, false, false);
    specLabel->setBounds (showSpecButton->getX() + showSpecButton->getWidth(),
                          showSpecButton->getY(),
                          panelWidth-(showSpecButton->getX() + showSpecButton->getWidth())-border,
                          labelHeight);
    
    addAndMakeVisible (overlay1Label = new Label ("Source 1","Source 1"));
    overlay1Label->setFont (Font (14.4000f, Font::plain));
    overlay1Label->setJustificationType (Justification::centredLeft);
    overlay1Label->setEditable (false, false, false);
    overlay1Label->setBounds (showOverlay1Button->getX() + showOverlay1Button->getWidth(),
                              showOverlay1Button->getY(),
                              panelWidth-(showOverlay1Button->getX() +
                                          showOverlay1Button->getWidth())-border,
                              labelHeight);
    
    addAndMakeVisible (overlay2Label = new Label ("Source 2","Source 2"));
    overlay2Label->setFont (Font (14.4000f, Font::plain));
    overlay2Label->setJustificationType (Justification::centredLeft);
    overlay2Label->setEditable (false, false, false);
    overlay2Label->setBounds (showOverlay2Button->getX() + showOverlay2Button->getWidth(),
                              showOverlay2Button->getY(),
                              panelWidth-(showOverlay2Button->getX() +
                                          showOverlay2Button->getWidth())-border,
                              labelHeight);
    
    specLabel->setColour(Label::textColourId,     ProgramSettings::selectedTextColour );
    overlay1Label->setColour(Label::textColourId, ProgramSettings::selectedTextColour );
    overlay2Label->setColour(Label::textColourId, ProgramSettings::selectedTextColour );
    
    
    // Zoom Control
    plusButton  = new ImageButton("Zoom In Vertical");
    minusButton = new ImageButton("Zoom Out Vertical");
    
    float imageOpacityWhenNormal(1.f);
    Colour overlayColourWhenNormal(ProgramSettings::iconColor);
    float imageOpacityWhenOver(1.0f);
    Colour overlayColourWhenOver(ProgramSettings::iconColorOver);
    float imageOpacityWhenDown(1.0f);
    Colour overlayColourWhenDown(ProgramSettings::iconColorDown);
    
    // Plus button
    icon_png = (char*)ImgBinaryData::plusicon_png;
    icon_pngSize = (int)ImgBinaryData::plusicon_pngSize;
    plusButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenNormal,
                           overlayColourWhenNormal,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenOver,
                           overlayColourWhenOver,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenDown,
                           overlayColourWhenDown);
    // Plus button
    icon_png = (char*)ImgBinaryData::minusicon_png;
    icon_pngSize = (int)ImgBinaryData::minusicon_pngSize;
    minusButton->setImages (false, true, true, ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenNormal,
                           overlayColourWhenNormal,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenOver,
                           overlayColourWhenOver,
                           ImageCache::getFromMemory (icon_png, icon_pngSize ),
                           imageOpacityWhenDown,
                           overlayColourWhenDown);
    
    
    plusButton->setTooltip("Zoom In Vertical");
    minusButton->setTooltip("Zoom In Vertical");
    
    plusButton->addListener(this);
    minusButton->addListener(this);
    addAndMakeVisible(plusButton);
    addAndMakeVisible(minusButton);
    
    yScrollBar = new ScrollBar(true);
    yScrollBar->setAutoHide(false);
    yScrollBar->setRangeLimits( Range<double>(0, 1) );
    yScrollBar->setCurrentRange(Range<double>(0, 1) );
    yScrollBar->addListener(this);
    addAndMakeVisible(yScrollBar);


    pluginWindow = new MainHostWindow(this);
//    pluginWindow->setSize(500, 500);
    pluginWindow->setVisible(false);
    
    allLayers.add( new ImageLayer(trackNum) );
    for(int i=0; i < 2; i++)
        allLayers.add( new MaskLayer( trackNum, i ));
    
    for(int i=0; i < allLayers.size();++i)
    {
//        allLayers[i]->setBounds(panelWidth+border, border, specScreenWidth(), specScreenHeight());
        allLayers[i]->setXRange(xrange, currentLengthInSeconds);
        allLayers[i]->setYRange(yScrollBar->getCurrentRange(), 1.0);
        addAndMakeVisible( allLayers[i] );
    }

    setSourceToPaint(selectedPaintSource);
    
    
}

Track::~Track()
{
    plusButton->removeListener(this);
    minusButton->removeListener(this);
    yScrollBar->removeListener(this);
}

//void Track::routeGUIMessage( const XmlElement & xml )
//{
//    MaskLayer * layer = dynamic_cast<MaskLayer *>(allLayers[selectedPaintSource+1]);
//    
//    Event type((Event)xml.getIntAttribute("subtype"));
//    if(type==PaintBrush)
//    {
//        layer->routeGUIMessage(xml);
//    }
//    else if(type==BoxBrush)
//    {
//        layer->routeGUIMessage(xml);
//    }
//    else if(type==LineBrush)
//    {
//        layer->routeGUIMessage(xml);
//    }
//    else if(type==TrainingBrush)
//    {
//        layer->routeGUIMessage(xml);
//    }
//    else if(type==SetSelected)
//    {
//        listeners.call (&Track::Listener::trackUpdate, this, Track::Listener::SetSelected);
//        ISSLOG(String(xml.createDocument(String::empty, false,false)));
//    }
//    else if(type==PlusButton)
//    {
//        plusButton->triggerClick();
//    }
//    else if(type==MinusButton)
//    {
//        minusButton->triggerClick();
//    }
//    else if(type==MuteButton)
//    {
//        muteButton->triggerClick();
//    }
//    else if(type==SoloButton)
//    {
//        soloButton->triggerClick();
//    }
//    else if(type==ShowSpecButton)
//    {
//        showSpecButton->triggerClick();
//    }
//    else if(type==ShowOverlay1Button)
//    {
//        showOverlay1Button->triggerClick();
//    }
//    else if(type==ShowOverlay2Button)
//    {
//        showOverlay2Button->triggerClick();
//    }
//    else if(type==PlusMaskButton)
//    {
//        plusMaskButton->triggerClick();
//    }
//    else if(type==ClearImageButton)
//    {
//        clearImageButton->triggerClick();
//    }
////    else if(type==PluginGraphButton)
////    {
////        pluginGraphButton->triggerClick();
////    }
//    else if(type==GainChanged)
//    {
//        if(volumeKnob)
//        {
//            float g(0);
//            XmlElement * xx = xml.getChildByName ("Gain");
//            if(xx)
//                g = xx->getAllSubText().getFloatValue ();
//            volumeKnob->setValue (g);
//        }
//    }
//    else if(type==ScrollBarMoved)
//    {
//        XmlElement * x1 = xml.getChildByName ("Start");
//        if(x1)
//        {
//            float start = x1->getAllSubText().getFloatValue();
//            x1 = xml.getChildByName ("Stop");
//            if(x1)
//            {
//                float stop = x1->getAllSubText().getFloatValue();
//                Range< double > newRange(start, stop);
//                yScrollBar->setCurrentRange (newRange);
//            }
//        }
//    }
//}

void Track::reset()
{
    file = "";
//    bool p = isPlaying();
//    double pos(0);
//    if(p) pos = this->getCurrentPosition();
    stop();
    setSource (nullptr);
    audioReaderSource = nullptr;
    
    sampleRate = 44100;
    setSource (audioReaderSource, 0, NULL, sampleRate);
    
    ImageLayer * layer(dynamic_cast<ImageLayer*>(allLayers[0]));
    layer->setFile(file);
    
    currentLengthInSeconds = 0;

    for(int i=0; i < allLayers.size();++i)
    {
        allLayers[i]->setXRange(xrange, currentLengthInSeconds);
        allLayers[i]->setYRange(yScrollBar->getCurrentRange(), 1.0);
    }

    setSourceToPaint(selectedPaintSource);

    
    // Reset volume
    if(volumeKnob)
    {
        volumeKnob->setValue (0.0, dontSendNotification);
        setGain( 1.0*gainMult );
    }
    soloButton->setToggleState( false, dontSendNotification  );
    
    if(showSpecButton)
        showSpecButton->setToggleState(true, dontSendNotification );
    if(showOverlay1Button)
        showOverlay1Button->setToggleState(true, dontSendNotification );
    if(showOverlay2Button)
        showOverlay2Button->setToggleState(true, dontSendNotification );
    if(muteButton)
        muteButton->setToggleState(true, dontSendNotification );
    if(soloButton)
        soloButton->setToggleState(true, dontSendNotification );
    
    setSourceToPaint(0);
    
    yScrollBar->setRangeLimits( Range<double>(0, 1) );
    yScrollBar->setCurrentRange( Range<double>(0, 1) );
    
    for(int i=0; i < allLayers.size(); ++i)
        allLayers[i]->setVisible(true);
    
    repaint();
}


void Track::updateVisualization()
{
    ImageLayer * layer(dynamic_cast<ImageLayer*>(allLayers[0]));
    if(layer)
        layer->update();
}


void Track::addToMasks()
{
    for(int i=0; i < allLayers.size(); ++i)
    {
        MaskLayer * layer = dynamic_cast<MaskLayer*>(allLayers[i]);
        if(layer)
            layer->addToMask();
    }
}

void Track::clearImages()
{
    for(int i=0; i < allLayers.size(); ++i)
    {
        MaskLayer * layer = dynamic_cast<MaskLayer*>(allLayers[i]);
        if(layer)
            layer->clearImage();
    }
    
    repaint();
}
//
//void Track::clearMaskImages()
//{
//    for(int i=0; i < allLayers.size(); ++i)
//    {
//        MaskLayer * layer = dynamic_cast<MaskLayer*>(allLayers[i]);
//        if(layer)
//            layer->clearImage();
//    }
//
//    repaint();
//}

void Track::setLoopMarkers(double startInSec, double stopInSec)
{
    startMarker = startInSec;
    stopMarker = stopInSec;
    if(audioReaderSource)
        audioReaderSource->setLoopMarkers( startMarker*sampleRate,
                                           stopMarker*sampleRate );
}

void Track::sliderValueChanged (Slider *slider)
{
    if(slider==volumeKnob)
    {
        gain = getGainFromSlider();
        setGain( gain*gainMult );
    }

//    ISSLOG("<Event type=\"Track\" time=\"" + ISSTIME +  "\" source=\"" + String(trackNum) +
//           "\" subtype=\"" + String(GainChanged) + "\"<Gain>" + String(gain) + "</Gain></Event>" );
}

void Track::addListener( Track::Listener * l)
{
    listeners.add(l);
}

void Track::removeListener( Track::Listener * l )
{
    listeners.remove(l);
}

void Track::setLooping( bool isLooping_ )
{
    isLooping = isLooping_;
    if(audioReaderSource)
        audioReaderSource->setLooping( isLooping );
}

void Track::setSolo(bool isSolo)
{
    soloButton->setToggleState( isSolo, dontSendNotification  );

    if(isSolo)
        setGain( gain*gainMult );
    
    soloButton->repaint();
}

bool Track::getSolo()
{
    return soloButton->getToggleState();
}

void Track::setSourceToPaint( int sourceToDraw )// Index staring
{
    selectedPaintSource = sourceToDraw;
    allLayers[sourceToDraw+1]->toFront(false);
    
    for(int i=0; i < allLayers.size(); ++i)
    {
        MaskLayer * layer = dynamic_cast<MaskLayer*>(allLayers[i]);
        if(layer)
        {
            if(sourceToDraw+1==i)
                layer->setEditable(true);
            else
                layer->setEditable(false);
        }
    }

    
    
    
    repaint();
}

void Track::setSelected( bool selected )
{
    isSelected = selected;
    
    if(ToolbarState::autoSolo )
    {
        if(selected)
        {
            setSolo(true);
            setSoloMute( false );
        }
        else
        {
            setSolo(false);
            setSoloMute( true ); 
        }
    }
    repaint();
}

bool Track::getSelected()
{
    return isSelected;
}

void Track::labelTextChanged (Label *labelThatHasChanged){}

void Track::zoomVertical( bool isZoomIn )
{
    Range<double> r;
    if(isZoomIn)
    {
        r = yScrollBar->getCurrentRange();
        float end = r.getEnd();
        r.setLength(r.getLength()/2.0);
        r = r.movedToEndAt(end);
    }
    else
    {
        r = yScrollBar->getCurrentRange();
        float end = r.getEnd();
        r.setLength(r.getLength()*2.0);
        r = r.movedToEndAt(end);
    }
    yScrollBar->setCurrentRange( r );    
}

float Track::getGainFromSlider()
{
    if(volumeKnob)
        return pow(10.0, (volumeKnob->getValue())/20.0);
    else
        return 1.0;
}

void Track::setSoloMute(bool isMute_)
{
    isSoloMuted = isMute_;
    
    if(isSoloMuted)
        setGain( 0 );
    else if(!isSoloMuted && !muteButton->getToggleState())
        setGain( getGainFromSlider()*gainMult );
}

void Track::buttonClicked (Button *button)
{
//    Event type(None);
    
    if( button == plusButton )
    {
        zoomVertical( true );

//        type = PlusButton;
    }
    else if( button == minusButton )
    {
        zoomVertical( false );

//        type = MinusButton;
    }
    else if(button==muteButton)
    {
        muteButton->setToggleState( !muteButton->getToggleState(), dontSendNotification  );
        if(muteButton->getToggleState())
        {
            soloButton->setToggleState( false, dontSendNotification  );
            setGain( 0 );
        }
        else
            setGain( getGainFromSlider()*gainMult );
        
        listeners.call (&Track::Listener::trackUpdate, this, Track::Listener::Mute);

//        type = MuteButton;
        
    }
    else if(button==soloButton)
    {
        soloButton->setToggleState( !soloButton->getToggleState(), dontSendNotification  );
        if(soloButton->getToggleState())
            muteButton->setToggleState( false, dontSendNotification  );
        
        listeners.call (&Track::Listener::trackUpdate, this, Track::Listener::Solo);

//        type = SoloButton;
    }
    else if(button==showSpecButton)
    {
        if(showSpecButton->getToggleState())
            specLabel->setColour(Label::textColourId, ProgramSettings::selectedTextColour );
        else
            specLabel->setColour(Label::textColourId, ProgramSettings::textColour );
        
        allLayers[0]->setVisible(showSpecButton->getToggleState());
        specLabel->repaint();
    }
    else if(button==showOverlay1Button)
    {
        if(showOverlay1Button->getToggleState())
            overlay1Label->setColour(Label::textColourId, ProgramSettings::selectedTextColour );
        else
            overlay1Label->setColour(Label::textColourId, ProgramSettings::textColour );
        
        allLayers[1]->setVisible(showOverlay1Button->getToggleState());
        overlay1Label->repaint();
        setSourceToPaint(selectedPaintSource);
    }
    else if(button==showOverlay2Button)
    {
        if(showOverlay2Button->getToggleState())
            overlay2Label->setColour(Label::textColourId, ProgramSettings::selectedTextColour );
        else
            overlay2Label->setColour(Label::textColourId, ProgramSettings::textColour );
        overlay2Label->repaint();
        allLayers[2]->setVisible(showOverlay2Button->getToggleState());
        
        setSourceToPaint(selectedPaintSource);
    }
    else if(button==plusMaskButton)
    {
        MAIN_APP->undoManager.beginNewTransaction ();
        for(int i=0; i < allLayers.size(); ++i)
        {
            MaskLayer * layer = dynamic_cast<MaskLayer*>(allLayers[i]);
            if(layer)
                layer->addToMask();
        }
    }
    else if(button==clearImageButton)
    {
        MAIN_APP->undoManager.beginNewTransaction ();
        
        for(int i=0; i < allLayers.size(); ++i)
        {
            MaskLayer * layer = dynamic_cast<MaskLayer*>(allLayers[i]);
            if(layer)
                layer->clearImage();
        }
    }
//    else if(button==pluginGraphButton)
//    {
//
//        if(pluginWindow->isVisible())
//            pluginWindow->setVisible(false);
//        else
//            pluginWindow->setVisible(true);
//
//        type = PluginGraphButton;
//    }
    
    repaint();
//    ISSLOG("<Event type=\"Track\" time=\"" + ISSTIME +  "\" source=\"" + String(trackNum) +
//           "\" subtype=\"" + String(type) + "\"></Event>" );


}

void Track::resized()
{

    
    if(plusButton)
        plusButton->setBounds(panelWidth-scrollBarWidth-2*border, name->getBottom()+1,
                              scrollBarWidth, scrollBarWidth);
    
    if(minusButton)
        minusButton->setBounds(panelWidth-scrollBarWidth-2*border, plusButton->getBottom(),
                               scrollBarWidth, scrollBarWidth);
    
    if(yScrollBar)
        yScrollBar->setBounds(panelWidth-scrollBarWidth-2*border, minusButton->getBottom(),
                              scrollBarWidth, getHeight()-minusButton->getBottom()-border);

    
    if(loadTrainingButton)
       loadTrainingButton->setBounds(border, getHeight()-labelHeight,
                                     labelHeight, labelHeight);
    
    if(clearImageButton)
        clearImageButton->setBounds(border + labelHeight, getHeight()-labelHeight-border,
                                   labelHeight, labelHeight);
    
    if(plusMaskButton)
        plusMaskButton->setBounds(border, getHeight()-labelHeight-border,
                                  labelHeight, labelHeight);

    for(int i=0; i < allLayers.size(); ++i)
        allLayers[i]->setBounds(panelWidth+border+1, border+1,
                                specScreenWidth(), specScreenHeight());
    
}


void Track::paint (Graphics&g)
{
    if(isSelected)
        g.fillAll( ProgramSettings::selectedTrackColour );
    else
        g.fillAll( ProgramSettings::trackColour );
    
    
    if(trackNum==1 || trackNum==2)
        imageButton->setOverlayColour( ProgramSettings::sourceColors[trackNum-1] );
    else
    {
        imageButton->setOverlayColour(ProgramSettings::sourceColors[0]);
        if(imageButton2)
            imageButton2->setOverlayColour(ProgramSettings::sourceColors[1]);
    }
    
    // Draw border
    int lineThickness=1;
    g.setColour(Colours::white);
    g.drawRect (border, border, panelWidth-2*border,
                getHeight()-2*border, lineThickness);
    
    
    g.drawRect (panelWidth+border, border, getWidth()-panelWidth-2*border,
                getHeight()-2*border, lineThickness);
    
}



int Track::specScreenWidth()
{
    return getWidth() -panelWidth-2*border-2;
}

int Track::specScreenHeight()
{
    return getHeight() - border*2-2;
}

 

void Track::updateXRangeInSec ( Range<double> range )
{
    xrange = range;

    for(int i=0; i < allLayers.size(); ++i)
        allLayers[i]->setXRange(xrange, currentLengthInSeconds);

    repaint();
}

bool Track::saveToXML( XmlElement & xml, const File & folderForAssets )
{
    for(int i=0; i < 2; ++i)
    {
        MaskLayer * mask = dynamic_cast<MaskLayer*>(allLayers[i+1]);
        mask->saveToXML(xml, folderForAssets);
    }

    return true;
}

void Track::engineNewOutputs( File file, int source )
{
    if( (source+1) != trackNum)
        return;
    


    loadNewFile( file );
    
    if(ToolbarState::isPlaying)
    {
        MultiTrack * multiTrack(dynamic_cast<MultiTrack*>(getParentComponent()));
        
        if(multiTrack)
            setPosition(multiTrack->getPositionInSec());
        start();
    }
    
    updateXRangeInSec(xrange);
    
    MAIN_APP->window->changed();
}

double Track::getLenInSec()
{
    ScopedPointer<AudioFormatReader> reader(MAIN_APP->formatManager.createReaderFor (file));
    if(reader)
    {
        double lengthInSamples = reader->lengthInSamples;
        double fs = reader->sampleRate;
        double lengthInSecs = lengthInSamples/fs;
        return lengthInSecs;
    }
    return 0.0;
}

bool Track::loadNewFile( File file_ )
{
    file = file_;
    
    bool p = isPlaying();
    double pos(0);
    if(p) pos = getCurrentPosition();
    stop();
    setSource (nullptr);
    audioReaderSource = nullptr;
    
    // Create a file reader
    AudioFormatReader* reader = MAIN_APP->formatManager.createReaderFor (file);
    if(reader==nullptr) return false;
    
    sampleRate = reader->sampleRate;
    audioReaderSource = new AudioFormatReaderLoopSource (reader, true);
    
    if( reader->lengthInSamples==0 )
    {
        #ifdef DEBUG
        std::cout << "[ERROR]: Zero length file: " << file.getFullPathName() << std::endl;
        #endif
        audioReaderSource = NULL;
    }
    
    setSource (audioReaderSource, 0, NULL, reader->sampleRate);
    
    if(audioReaderSource)
    {
        audioReaderSource->setLooping(isLooping); // Always have reader source set to looping
        currentLengthInSeconds = getLengthInSeconds();

        audioReaderSource->setLoopMarkers( startMarker*sampleRate, stopMarker*sampleRate );
        audioReaderSource->setToLoopStart();
        
        ImageLayer * layer(dynamic_cast<ImageLayer*>(allLayers[0]));
        layer->setFile(file);
 

        for(int i=0; i < allLayers.size();++i)
        {
            allLayers[i]->setXRange(xrange, currentLengthInSeconds);
            allLayers[i]->setYRange(yScrollBar->getCurrentRange(), 1.0);
//            allLayers[i]->setBounds(panelWidth+border+1, border+1, specScreenWidth(), specScreenHeight());
        }
        setSourceToPaint(selectedPaintSource);
    }

    if(p)
    {
        setPosition(pos);
        start();
    }
    
    listeners.call (&Track::Listener::trackUpdate, this,
                    Track::Listener::LoadedNewFile);
    
    repaint();
    
    return true;
}

bool Track::loadFromXML( const XmlElement & xml, const File & assetsFolder  )
{
    
    String filename  = xml.getStringAttribute ("filename");
    file = File(assetsFolder.getFullPathName() + "/" + filename);
    
    bool p = isPlaying();
    double pos(0);
    if(p) pos = getCurrentPosition();
    stop();
    setSource (nullptr);
    audioReaderSource = nullptr;
    
    
    // Create a file reader
    AudioFormatReader* reader = MAIN_APP->formatManager.createReaderFor (file);
    if(reader==nullptr) return false;
    
    sampleRate = reader->sampleRate;
    audioReaderSource = new AudioFormatReaderLoopSource (reader, true);
    
    if( reader->lengthInSamples==0 )
    {
        #ifdef DEBUG
        std::cout << "[ERROR]: Zero length file" << std::endl;
        #endif
        audioReaderSource = NULL;
    }
    
    this->setSource (audioReaderSource, 0, NULL, reader->sampleRate);
    
    if(audioReaderSource)
    {
        // Always have reader source set to looping
        audioReaderSource->setLooping(isLooping); 
        currentLengthInSeconds = this->getLengthInSeconds();
        audioReaderSource->setLoopMarkers( startMarker*sampleRate, stopMarker*sampleRate );
        audioReaderSource->setToLoopStart();
        
        ImageLayer * layer(dynamic_cast<ImageLayer*>(allLayers[0]));
        layer->setFile(file);
        
        if(reader)
        {
            int numSourceOverLay=0;
            forEachXmlChildElement (xml, e)
            {
                if (e->hasTagName ("maskimage"))
                {
                    MaskLayer * mask = dynamic_cast<MaskLayer*>(allLayers[numSourceOverLay+1]);
                    if(mask)
                    {
                        bool res = mask->loadFromXML(*e, assetsFolder);
                    
                        if(!res) return false;
                    }
                    
                    allLayers[numSourceOverLay+1]->setXRange(xrange, currentLengthInSeconds);
                    allLayers[numSourceOverLay+1]->setYRange(yScrollBar->getCurrentRange(), 1.0);
//                    allLayers[numSourceOverLay+1]->setBounds(panelWidth+border+1, border+1,
//                                                             specScreenWidth(), specScreenHeight());

                    numSourceOverLay++;
                }
            }
            
            
            resized();
            
        }
    }
    
    if(p)
    {
        setPosition(pos);
        start();
    }

    return true;
}
 

void Track::scrollBarMoved (ScrollBar *scrollBarThatHasMoved, double newRangeStart)
{
    repaint();
    
    Range<double> r = yScrollBar->getCurrentRange();
    
    for(int i=0; i < allLayers.size(); ++i)
        allLayers[i]->setYRange(r, 1.0);
    
//    ISSLOG("<Event type=\"Track\" time=\"" + ISSTIME +
//           "\" source=\"" + String(trackNum) +
//           "\" subtype=\"" + String(ScrollBarMoved) + "\"><Start>" +
//           String(r.getStart()) + "</Start><Stop>" +
//           String(r.getEnd()) + "</Stop></Event>" )
    
}


//void logTrackEvent(Track::Event event, XMLSerializable * action, int source)
//{
//    XmlElement xml("Event");
//    xml.setAttribute("type", "Track");
//    xml.setAttribute("source", source);
//    xml.setAttribute("time", ISSTIME);
//    xml.setAttribute("subtype", event);
//    if(action)
//        action->saveToXML(xml,File(""));
//    
//    ISSLOG(String(xml.createDocument(String::empty, false,false)));
//}


void Track::mouseDown (const MouseEvent& e)
{
    isResizeAble = false;
    isMouseDown = true;
    if(!getSelected())
    {
        listeners.call (&Track::Listener::trackUpdate, this,
                        Track::Listener::SetSelected);
//        logTrackEvent(Track::SetSelected, NULL, trackNum);
    }
    
    int y = e.getPosition().getY();
    
    // Change the size of the tracks
    if(ToolbarState::selector == ToolbarState::ArrowSelector )
    {
        int distFromBottom = abs(y - getHeight() );
        if( distFromBottom < 30)
        {
            mouseDownPoint = e.getPosition();
            isResizeAble = true;
        }

    }
}


void Track::mouseMove(const MouseEvent& e)
{
    static int pastSelectorState(0);
    int y(e.getPosition().getY());
    int h(getHeight());
    
    // Change the size of the tracks
    if(ToolbarState::selector == ToolbarState::ArrowSelector) 
    {
        int distFromBottom = abs(y - h );
        if( distFromBottom < 30)
            setMouseCursor( MouseCursor::BottomEdgeResizeCursor );
        else
            setMouseCursor( MouseCursor::NormalCursor );
    }
    if( (pastSelectorState == ToolbarState::ArrowSelector) &&
       (ToolbarState::selector != ToolbarState::ArrowSelector) )
    {
        setMouseCursor( MouseCursor::NormalCursor );
    }
    
    pastSelectorState = ToolbarState::selector;
}


void Track::mouseDrag (const MouseEvent& e)
{
    if(isResizeAble)
    {
        int y = e.getPosition().getY();
        float delta = (y - mouseDownPoint.getY());
        
        Rectangle<int> bound = getBounds();
        
        
        int newHeight = mouseDownPoint.getY()+delta;
        if(newHeight < 150)
            newHeight = 150;
        
        
//        setBounds(bound.getX(), bound.getY(), bound.getWidth(), newHeight);
//        
//        listeners.call (&Track::Listener::trackUpdate, this, Track::Listener::Resized);

        
    }


}

void Track::mouseUp(const MouseEvent & e)
{
    isMouseDown = false;
}





