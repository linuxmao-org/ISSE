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
#include "MultiTrack.h"
#include "MainContentComp.h"

#include "AudioSink.h"
#include "AudioSource.h"
#include "Main.h"
#include "AppCommandIDs.h"
#include "Cursor.h"
#include "ToolbarState.h"
#include "ProgramSettings.h"


const int MultiTrack::numTracks = 3;
const int MultiTrack::scrollBarHeight = 12;

MultiTrack::MultiTrack( ) :     cursor(new Cursor(Colours::lightyellow)),
                                loopStartCursor(new Cursor(Colours::red)),
                                loopStopCursor(new Cursor(Colours::red)),
                                xScrollBar( new ScrollBar(false) ),
                                yScrollBar( new ScrollBar(true)),
                                timeline( new Timeline(this) ),
                                numVisibleTracks(numTracks),
                                previousStartTime(0)
{
    numVisibleTracks = numTracks;

    for(int i=0; i < numTracks; i++)
    {
        tracks.add(new Track(i));
        tracks[i]->setLoopMarkers(0, 1);
        tracks[i]->setLooping ( true );
        tracks[i]->setBounds(0, 100*i, 100, 100);
        addAndMakeVisible(tracks[i]);
        tracks[i]->addListener(this);
    }
    
    
    // Register change listener to get update messages when the transport starts and stops
    tracks[0]->addChangeListener(this);

    
    // Timeline properties
    timeline->setBounds(0, 0, getWidth(), timeline->getHeight());
    addAndMakeVisible(timeline);
    
    // Add playhead cursor component
    addAndMakeVisible(cursor);
    addAndMakeVisible(loopStartCursor);
    addAndMakeVisible(loopStopCursor);
    
    // Horizontal scrollbar properties
    xScrollBar->setAutoHide(false);
    xScrollBar->setRangeLimits(Range<double>(0, 10));
    xScrollBar->setCurrentRange(Range<double>(0,10));
    xScrollBar->setBounds( 0, getHeight()-scrollBarHeight, getWidth()-scrollBarHeight, scrollBarHeight);
    xScrollBar->addListener(this);
    xScrollBar->setSingleStepSize(1);
    xScrollBar->setColour(ScrollBar::backgroundColourId , ProgramSettings::mainBackgroundColor);
    
    
    yScrollBar->setAutoHide(false);
    yScrollBar->setBounds(getWidth()-scrollBarHeight,0,scrollBarHeight, getHeight());
    yScrollBar->addListener(this);
    yScrollBar->setSingleStepSize(1);
    
 
  
    yScrollBar->setRangeLimits(Range<double>(0,100));
    yScrollBar->setCurrentRange(Range<double>(0.f, 100.0f));
    
   
    
    addAndMakeVisible(xScrollBar);
    addAndMakeVisible(yScrollBar);
    
    setSingleTrackMode( true );
    

    
    startTimer(10);
    
   
}

void MultiTrack::setNumberOfTracks(unsigned int num)
{
    
}


void MultiTrack::repaintCursors()
{
    cursor->setTopLeftPosition(timeline->getCursorInPx(), 2);
    loopStartCursor->setTopLeftPosition(timeline->getStartInPx(), timeline->getHeight());
    loopStopCursor->setTopLeftPosition(timeline->getStopInPx(), timeline->getHeight());
}

void MultiTrack::scrollBarMoved (ScrollBar *scrollBarThatHasMoved, double newRangeStart)
{
    if(scrollBarThatHasMoved==xScrollBar)
    {
    
    if(isPlaying())
        xScrollBar->setCurrentRangeStart(this->getPositionInSec (),
                                         dontSendNotification );
    
    updateXRangeInSec(xScrollBar->getCurrentRange());
    timeline->setRange(xScrollBar->getCurrentRange(), xScrollBar->getRangeLimit());
    
    repaintCursors();
    
//    Range< double > r =	xScrollBar->getCurrentRange ();

    }
    else
    {
        resized();
    }
}

void MultiTrack::pageScroll( float currentPosition )
{
    float scrollEnd = xScrollBar->getCurrentRange().getEnd();
    float scrollStart = xScrollBar->getCurrentRangeStart();
    if(currentPosition > scrollEnd) // Page towards the end
        xScrollBar->setCurrentRangeStart(scrollEnd);
    else if( currentPosition < scrollStart) // Go back to the begining
        xScrollBar->setCurrentRangeStart(currentPosition);
}


void MultiTrack::setCursorBecauseOfAudio( )
{
    float playHead = this->getPositionInSec ();
    timeline->setCursor(playHead);
    
    pageScroll(playHead);
    
    repaintCursors();
}

void MultiTrack::timelineEvent( Timeline::Event msg )
{
    static bool wasPlaying = this->isPlaying();
    
    float playHeadInSecs = timeline->getCursorInSec();
    
    switch(msg)
    {
        case Timeline::MouseDown:
        {
            wasPlaying = this->isPlaying();
            this->stop();
            
            break;
        }
        case Timeline::MouseUp:
        {
            this->setPositionInSec( playHeadInSecs );
            
            if(wasPlaying)
                this->start();
            break;
        }
        case Timeline::LoopMarkersChanged:
        {
            float startInSec = timeline->getStartInSec();
            float stopInSec  = timeline->getStopInSec();
            
            this->setLoopMarkers(startInSec, stopInSec);
            
            
            loopStartCursor->setBounds(timeline->getStartInPx(),
                                       timeline->getHeight(),
                                       1,
                                       getHeight()-scrollBarHeight-timeline->getHeight()-2);
            
            loopStopCursor->setBounds(timeline->getStopInPx(),
                                      timeline->getHeight(),
                                      1,
                                      getHeight()-scrollBarHeight-timeline->getHeight()-2);
            break;
        }
            
            
        default:
            break;
    }
    
    repaintCursors();
}


void MultiTrack::createNewProject(File loadFile)
{
    
    // Delete any old temp files
    File tempDir = File::getSpecialLocation( File::tempDirectory );
    String fileNameNoExt = loadFile.getFileNameWithoutExtension ();
    String ext = loadFile.getFileExtension ();
    
    bool okay = tracks[0]->loadNewFile( loadFile );
    
    if(!okay)
    {
        
        std::cout << "ERROR in loading the new file" << std::endl;
        return;
    }
    
    
    // Register the input file and the tracks
    // Load each file into the transport and create the spectrograms of each
    for(int i=1; i < tracks.size(); ++i)
    {
        tracks[i]->loadNewFile( File(tempDir.getFullPathName() + "/" + fileNameNoExt + "-" + String(i) + ext) );
        tracks[i]->setPosition( getPositionInSec() );
 
    }
    
    postLoad();
}
void MultiTrack::postLoad()
{
    // Update scroll bar
    double lengthInSecs = getLengthInSeconds() ;
    xScrollBar->setRangeLimits(Range<double>(0, lengthInSecs));
    xScrollBar->setCurrentRange(Range<double>(0,lengthInSecs));
    
    // Set timeline loop markers
    timeline->setRange(xScrollBar->getCurrentRange(), xScrollBar->getRangeLimit());
    timeline->setLoopMarkers(0,lengthInSecs);
    
    
    
    // Repaint everything
    repaint();
}


void MultiTrack::toggleAutoSolo()
{
    for(int i=0; i < tracks.size(); i++)
    {
        if( tracks[i] )
        {
            if( tracks[i]->getSelected() )
                tracks[i]->setSolo(true);
        }
    }
}

//void MultiTrack::routeGUIMessage( const XmlElement & xml )
//{
//    int ind = xml.getIntAttribute ("source");
//    if(ind >=0 && ind < tracks.size())
//        tracks[ ind ]->routeGUIMessage( xml );
//}


void MultiTrack::addToMask()
{
    for(int i=0; i < tracks.size(); ++i)
        if( tracks[i]->getSelected() )
            tracks[i]->addToMasks();
}

void MultiTrack::setGain( double gain )
{
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->setGainMultiplier(gain);
    
}

void MultiTrack::clearMasksImages()
{
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->clearImages();
}

double MultiTrack::getLengthInSeconds()
{
    if(tracks.size() > 0)
    {
        double len(tracks[0]->getLenInSec());
            return len;
    }
    else
        return 0;
}


void MultiTrack::exportAudio( File saveFolder )
{
    if(tracks.size() > 0)
    {
        String originalName = tracks[0]->getFile().getFileNameWithoutExtension();
        originalName = originalName.substring (0, originalName.length()) ;
        
        for(int i=0; i < tracks.size(); ++i)
        {
            File f(tracks[i]->getFile());
            if(i==0)
                f.copyFileTo(File(saveFolder.getFullPathName() + "/" + originalName + " (Original Mix).wav"));
            else
                f.copyFileTo(File(saveFolder.getFullPathName() + "/" + originalName + " (Source " + String(i) + ").wav"));
        }
    }
}

bool MultiTrack::isPlaying()
{
    if(tracks[0])
        return tracks[0]->isPlaying();
    else
        return false;
}

void MultiTrack::stop()
{
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->stop();
}

void MultiTrack::start()
{
    previousStartTime = timeline->getCursorInSec();
    
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->start();
}

void MultiTrack::setLooping( bool isLooping )
{
    for(int i=0; i < tracks.size(); i++)
        tracks[i]->setLooping(isLooping);
}

void MultiTrack::setLoopMarkers(double startInSec, double stopInSec)
{
    for(int i=0; i < tracks.size(); i++)
        tracks[i]->setLoopMarkers(startInSec, stopInSec);
}

void MultiTrack::setSourceToPaint( int source )
{
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->setSourceToPaint( source );
}

void MultiTrack::updateXRangeInSec ( Range<double> range )
{
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->updateXRangeInSec( range );
}

void MultiTrack::zoomVertical( bool isZoomIn )
{
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->zoomVertical( isZoomIn );
}


//Callback to check whether this target is interested in the set of files being offered.
bool 	MultiTrack::isInterestedInFileDrag (const StringArray &files)
{
    if(files.size() > 1 ||  files.size()==0)
        return false;
    
    String filename = files[0];
    File file(filename);
    String ext(file.getFileExtension());
     
    if(ext.compare(".wav")==0 && !ProgramSettings::processing && !ProgramSettings::playing)
    {
        return true;
    }
    else if( ext.compare(".isse")==0 && !ProgramSettings::processing && !ProgramSettings::playing)
    {
        return true;
    }

    return false;
}

//Callback to indicate that some files are being dragged over this component.
void 	MultiTrack::fileDragEnter (const StringArray &files, int x, int y)
{}

//Callback to indicate that the user is dragging some files over this component.
void 	MultiTrack::fileDragMove (const StringArray &files, int x, int y)
{}

//Callback to indicate that the mouse has moved away from this component.
void 	MultiTrack::fileDragExit (const StringArray &files)
{}

void 	MultiTrack::filesDropped (const StringArray &files, int x, int y)
{
    if(files.size() > 1 ||  files.size()==0)
        return;

    MainContentComp * comp = (findParentComponentOfClass<MainContentComp>	());
    
    if(comp)
    {
        String filename = files[0];
        File file(filename);
        String ext(file.getFileExtension());
        
        if(ext.compare(".wav")==0 && !ProgramSettings::processing && !ProgramSettings::playing)
        {
            FileBasedDocument::SaveResult res = comp->saveIfNeededAndUserAgrees ();
            if(res==FileBasedDocument::savedOk)
                comp->createNewProject( file, true );
        }
        else if(ext.compare(".isse")==0 && !ProgramSettings::processing && !ProgramSettings::playing)
        {
            FileBasedDocument::SaveResult res = comp->saveIfNeededAndUserAgrees ();
            if(res==FileBasedDocument::savedOk)
                comp->loadDocument(file);
        }
    }
}


bool MultiTrack::saveToXML( XmlElement & xml, const File & folderForAssets )
{
    // First file
    if(tracks.size() < 1) return false;
    
    File firstFile(tracks[0]->getFile());
    String firstFileName(firstFile.getFileNameWithoutExtension());
    for(int i=0; i < tracks.size(); ++i)
    {
        XmlElement* trackXML = new XmlElement ("TRACK");
        String extra;
        if(i > 0)
            extra = " (Source " + String(i) + ")";
        
        File newFile = folderForAssets.getFullPathName() + "/" + firstFileName + extra + ".wav";
        bool okay = tracks[i]->getFile().copyFileTo ( newFile );
        
        
        if(!okay && i==0)
        {
            String title("ERROR");
            String message("Could not save the file. Please save as...");
            String buttonText("Dismiss");
            AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, title,
                                              message, buttonText);
            return false;
            
            
        }
        
        
        
        
        trackXML->setAttribute("filename", newFile.getFileName());
        xml.addChildElement(trackXML);
        
        tracks[i]->saveToXML(*trackXML, folderForAssets );
    }
    return true;
}

bool MultiTrack::loadFromXML( const XmlElement & xml, const File & assetsFolder  )
{
    int trackNum = 0;
    forEachXmlChildElement (xml, e)
    {
        if (e->hasTagName ("TRACK"))
        {
            // Load the file into its track
            bool res = tracks[trackNum]->loadFromXML(*e, assetsFolder);
            
            if(!res)
                return false;
            
            trackNum++;
        }
    }
   
    postLoad();
    return true;
}


void MultiTrack::setPositionInSec( double posInSec )
{
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->setPosition( posInSec );
}


void MultiTrack::changeListenerCallback (ChangeBroadcaster *source)
{
 
    if( tracks.size() > 0 && tracks[0]->hasStreamFinished() ) // If stopped
    {
        MAIN_APP->commandManager.invokeDirectly(AppCommandIDs::Stop, false);
        for(int i=0; i < tracks.size(); ++i)
            tracks[i]->setPosition (0);
    }
}
void MultiTrack::setSingleTrackMode( bool isTrue )
{
    
    if(isTrue)
    {
        if(numVisibleTracks > 0)
            tempHeight = yScrollBar->getCurrentRangeStart();
        
        int selectedTrack(0);
        for(int i=0; i < tracks.size(); i++)
        {
            if(tracks[i]->getSelected())
            {
                selectedTrack = i;
                break;
            }
        }

        float heightForTracks(getHeight() - timeline->getHeight() - xScrollBar->getHeight()*1.0);
        yScrollBar->setRangeLimits(Range<double>(0,heightForTracks*tracks.size()), dontSendNotification);
        yScrollBar->setCurrentRangeStart(selectedTrack*heightForTracks, dontSendNotification);

        numVisibleTracks = 1;
    }
    else
    {
     
        numVisibleTracks = tracks.size();
        float heightForTracks(getHeight() - timeline->getHeight() - xScrollBar->getHeight()*1.0);
        double yrstart = tempHeight;
        int pastHeight = timeline->getBottom() - yrstart;
        float newH(heightForTracks/tracks.size());
        for(int i=0; i < tracks.size(); i++)
        {
            if(newH < Track::minHeight) newH = Track::minHeight;
            
            tracks[i]->setBounds(0, pastHeight, getWidth()-scrollBarHeight, newH);
            tracks[i]->setVisible(true);
            pastHeight = tracks[i]->getBottom();
        }  
        
        int totalHeight(0);
        for(int i=0; i < tracks.size(); ++i)
            totalHeight += tracks[i]->getHeight();
        yScrollBar->setRangeLimits(Range<double>(0,totalHeight), dontSendNotification);
        Range<double> cr(yScrollBar->getCurrentRange());
        cr.setLength(heightForTracks);
        yScrollBar->setCurrentRange(cr, dontSendNotification);
        
    }
    resized();
}


void MultiTrack::resized()
{
    int height = getHeight();
    int width = getWidth();
    
    if(timeline)
        timeline->setBounds(0, 0, width, timeline->getHeight());
    
    jassert(numVisibleTracks<=tracks.size());
    jassert(numVisibleTracks>0);
    
    float heightForTracks(height - timeline->getHeight() - xScrollBar->getHeight()*1.0);
//    float oldHeightForTracks(yScrollBar->getCurrentRange().getLength());
    
    if(numVisibleTracks==1)
    {
        double yrstart = yScrollBar->getCurrentRange().getStart();
        float heightForTracks(getHeight() - timeline->getHeight() - xScrollBar->getHeight()*1.0);
        int startY = timeline->getBottom() - yrstart;
        for(int i=0; i < tracks.size(); i++)
        {
            Rectangle<int> bounds(tracks[i]->getBounds());
            tracks[i]->setBounds(bounds.getX(), startY, width-scrollBarHeight, heightForTracks);
            startY +=heightForTracks;
 
        }
        
        int totalHeight(0);
        for(int i=0; i < tracks.size(); ++i)
            totalHeight += tracks[i]->getHeight();
        yScrollBar->setRangeLimits(Range<double>(0,totalHeight), dontSendNotification);
        Range<double> cr(yScrollBar->getCurrentRange());
        cr.setLength(heightForTracks);
        
        yScrollBar->setCurrentRange(cr, dontSendNotification);

    }
    else
    {
        double yrstart = yScrollBar->getCurrentRange().getStart();
        int pastHeight = timeline->getBottom() - yrstart;
        float newH(heightForTracks/tracks.size());
        for(int i=0; i < tracks.size(); i++)
        {
            if(newH < Track::minHeight) newH = Track::minHeight;
            
            tracks[i]->setBounds(0, pastHeight, width-scrollBarHeight, newH);
            
            pastHeight = pastHeight+newH;
        }

        int totalHeight(0);
        for(int i=0; i < tracks.size(); ++i)
            totalHeight += tracks[i]->getHeight();
        yScrollBar->setRangeLimits(Range<double>(0,totalHeight), dontSendNotification);
        Range<double> cr(yScrollBar->getCurrentRange());
        cr.setLength(heightForTracks);
        yScrollBar->setCurrentRange(cr, dontSendNotification);
    }
    
    

    if( xScrollBar )
        xScrollBar->setBounds( 0, height-scrollBarHeight, width-scrollBarHeight, scrollBarHeight);
    
    if( yScrollBar )
        yScrollBar->setBounds( getWidth()- scrollBarHeight, timeline->getBottom(),
                              scrollBarHeight, height-timeline->getHeight()-scrollBarHeight);
    
    cursor->setBounds(timeline->getCursorInPx(), 2,1, getHeight()-scrollBarHeight-2-3);
    loopStartCursor->setBounds(timeline->getStartInPx(), timeline->getHeight(),
                               1, getHeight()-scrollBarHeight-timeline->getHeight()-2);
    loopStopCursor->setBounds(timeline->getStopInPx(), timeline->getHeight(),
                              1, getHeight()-scrollBarHeight-timeline->getHeight()-2);
    
    repaint();

}

void MultiTrack::zoomInHorizontal()
{
    double currentPos = timeline->getCursorInSec();
    Range<double> r = xScrollBar->getCurrentRange();

    double len = r.getLength()/1.5;
    
    r.setStart(currentPos - len/2.0);
    r.setEnd(currentPos + len/2.0);
    
    xScrollBar->setCurrentRange( r );

    updateXRangeInSec( xScrollBar->getCurrentRange() );
    
}

void MultiTrack::zoomOutHorizontal()
{
    double currentPos = timeline->getCursorInSec();
    Range<double> r = xScrollBar->getCurrentRange();
    
    double len = r.getLength()*1.5;
    
    r.setStart(currentPos - len/2.0);
    r.setEnd(currentPos + len/2.0);
    
    xScrollBar->setCurrentRange( r );
    updateXRangeInSec( xScrollBar->getCurrentRange() );
    
}

void MultiTrack::zoomIn()
{
    zoomInHorizontal();
    this->zoomVertical( true );
    
}

void MultiTrack::zoomOut()
{
    zoomOutHorizontal();
    this->zoomVertical( false );
}

void MultiTrack::updateVisualization()
{
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->updateVisualization();
}

void MultiTrack::mouseWheelMove	( const MouseEvent & event,
                                  const MouseWheelDetails & wheel )
{
    
    ModifierKeys keys = event.mods;
    
    if(wheel.deltaX==0 && wheel.deltaY==0)
    {
        return;
    }
    if( keys.isShiftDown () )
    {
        // Horizontal
        if(fabs(wheel.deltaX)  > fabs(wheel.deltaY) )
        {
        
            if(wheel.deltaX < 0)
            {
                zoomOutHorizontal();
            }
            else
            {
                zoomInHorizontal();
            }
        }
    }
    else
    {
        if(wheel.deltaY < 0)
            xScrollBar->moveScrollbarInSteps(-1);
        else
            xScrollBar->moveScrollbarInSteps(1);
    }
    repaint();
    
}

void MultiTrack::resetState()
{
    trackUpdate( tracks[0], Track::Listener::SetSelected );
    setSourceToPaint(0);

    setCursorBecauseOfAudio( );
    
    // Update scroll bar
    double lengthInSecs = 10;
    if(xScrollBar)
    {
        xScrollBar->setCurrentRangeStart(0);
        xScrollBar->setRangeLimits(Range<double>(0, lengthInSecs));
        xScrollBar->setCurrentRange(Range<double>(0,lengthInSecs));
    }
    
    // Set timeline loop markers
    timeline->setLoopMarkers(0,lengthInSecs);
    timeline->setRange(xScrollBar->getCurrentRange(), xScrollBar->getRangeLimit());
    timeline->setLoopMarkers(0,lengthInSecs);
    
    // Load each file into the transport and create the spectrograms of each
    for(int i=0; i < tracks.size(); ++i)
        tracks[i]->reset();
    
    if(tracks.size() > 0)
    {
        Track * t(tracks[0]);
        for(int i=0; i < tracks.size(); i++)
        {
            if(tracks[i]!=t)
                tracks[i]->setSelected(false);
            else
                tracks[i]->setSelected(true);
        }
    }
    
    int preTrackHeight = (getHeight() - timeline->getHeight() - xScrollBar->getHeight()*1.0)/numVisibleTracks;
    int minTrackHeight(Track::minHeight);
    int trackHeight(preTrackHeight);
    if(trackHeight < minTrackHeight)
        trackHeight = minTrackHeight;
    
    
    float heightForTracks(getHeight() - timeline->getHeight() - xScrollBar->getHeight()*1.0);
    int pastHeight = timeline->getBottom();
    float newH(heightForTracks/tracks.size());
    for(int i=0; i < tracks.size(); i++)
    {
        if(newH < Track::minHeight) newH = Track::minHeight;
        
        tracks[i]->setBounds(0, pastHeight, getWidth()-scrollBarHeight, newH);
        
        pastHeight = tracks[i]->getBottom();
    }
    
    yScrollBar->setRangeLimits(Range<double>(0,preTrackHeight*tracks.size()));
    yScrollBar->setCurrentRange(Range<double>(0.f, 1.0f*getHeight()));
}


void MultiTrack::updatedTransportControl( TransportComp * transportComp, TransportEvent event )
{
    if(tracks.size() < 1) return;
    
    switch(event)
    {
        case Play:
        {
            
            
            bool isP = tracks[0]->isPlaying();
            if(isP)
                setPositionInSec(previousStartTime);
            else
            {
                setPositionInSec(timeline->getCursorInSec());
                start();
            }
            
            ToolbarState::isPlaying = true;
            
            break;
        }
        case Pause:
        {
            if(!transportComp->isPaused() && transportComp->isPlaying())
            {
                setPositionInSec(timeline->getCursorInSec());
                start();
                ToolbarState::isPlaying = true;
            }
            else //if(transportComp->isPaused())
            {
                stop();
                ToolbarState::isPlaying = false;
            }
            break;
        }
        case Stop:
        {
            bool isP = tracks[0]->isPlaying();
                
            stop();
                
            if(!isP)
            {
                setPositionInSec(0);
                setCursorBecauseOfAudio( );
            }
            
            ToolbarState::isPlaying = false;
            
            break;
        }
        case Previous:
        {
            setPositionInSec(0);
            setCursorBecauseOfAudio( );
            break;
        }
        case Next:
        {
            double len(tracks[0]->getLenInSec());
            
            setPositionInSec(len);
            setCursorBecauseOfAudio( );
            
            if(!transportComp->isLooping())
            {
                changeListenerCallback(NULL);
            }
            
            break;
        }
        case Rewind:
        {
            setPositionInSec(timeline->getCursorInSec()-1.0);
            setCursorBecauseOfAudio( );
            break;
        }
        case FastForward:
        {
            setPositionInSec(timeline->getCursorInSec()+1.0);
            setCursorBecauseOfAudio( );
            break;
        }
        case AutoSolo:
        {
            ToolbarState::autoSolo = !ToolbarState::autoSolo;
            if( ToolbarState::autoSolo )
                toggleAutoSolo();
            break;
        }
        case Loop:
        {
            setLooping( transportComp->isLooping() );
            break;
        }
        default:
            break;
    }
}




double MultiTrack::getPositionInSec()
{
    if(tracks[0])
        return tracks[0]->getCurrentPosition ();
    else
        return 0;
}

void MultiTrack::timerCallback ()
{
    if( this->isPlaying() )
        setCursorBecauseOfAudio( );
}

MultiTrack::~MultiTrack()
{
    stopTimer();
    
    for(int i=0; i < tracks.size(); ++i)
    {
        tracks[i]->stop();
        tracks[i]->setSource (nullptr);
    }
    
    if(xScrollBar)
        xScrollBar->removeListener(this);
}

void MultiTrack::trackUpdate( Track * t, Track::Listener::Msg msg )
{
    
    int i;
    switch(msg)
    {
        case Track::Listener::SetSelected:
        {
            for(i=0; i < tracks.size(); i++)
            {
                if(tracks[i]!=t)
                    tracks[i]->setSelected(false);
                else
                    tracks[i]->setSelected(true);
            }
            
            break;
        }
        case Track::Listener::Mute:
        {
            break;
        }
        case Track::Listener::Solo:
        {
            for(i=0; i < tracks.size(); ++i)
                tracks[i]->setSoloMute( !tracks[i]->getSolo() );
            
            int numTracksSolo =0;
            for(i=0; i < tracks.size(); ++i)
                if( tracks[i]->getSolo() )
                    numTracksSolo++;
            
            if(numTracksSolo==0)
                for(i=0; i < tracks.size(); ++i)
                    tracks[i]->setSoloMute( false );
            break;
        }
        case Track::Listener::LoadedNewFile:
        {
            
            updateXRangeInSec(xScrollBar->getCurrentRange());
            
            break;
        }
        case Track::Listener::Resized:
        {
//            if(numVisibleTracks==1)
//            {
//                float heightForTracks(getHeight() - timeline->getHeight() - xScrollBar->getHeight()*1.0);
//                for(int i=0; i < tracks.size(); i++)
//                    tracks[i]->setBounds(0, timeline->getBottom(), getWidth()-scrollBarHeight, heightForTracks);
//                
//            }
//            else
//            {
////                float heightForTracks(getHeight() - timeline->getHeight() - xScrollBar->getHeight()*1.0);
////                int ht = heightForTracks - t->getHeight();
//                
//                bool okay(false);
//                for(int i=0; i < tracks.size(); i++)
//                {
//                    trueHeights.set(i, tracks[i]->getHeight());
//                    if(okay)
//                        tracks[i]->setBounds(0, tracks[i-1]->getBottom(),
//                                        tracks[i]->getWidth(), tracks[i]->getHeight());
//                    if(tracks[i]==t)
//                        okay  = true;
//                }
//            
//                
//                int totalHeight(0);
//                for(int i=0; i < tracks.size(); ++i)
//                    totalHeight += tracks[i]->getHeight();
//            
//                yScrollBar->setRangeLimits(Range<double>(0,totalHeight));                
//                
//            }
        }

        default:
            break;
    }
}



