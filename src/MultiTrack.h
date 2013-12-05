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
#ifndef __MULTITRACK_H_BC58BE4C__
#define __MULTITRACK_H_BC58BE4C__

#include "JuceHeader.h"
#include "Track.h"
#include "SpectrogramSettings.h"
#include "Timeline.h"
#include "TransportComp.h"



class Cursor;

class MultiTrack :  public Component,
                    public Track::Listener,
                    public ChangeListener,
                    public XMLSerializable,
                    public FileDragAndDropTarget,
                    public Timeline::Listener,
                    public TransportComp::Listener,
                    public Timer,
                    public ScrollBar::Listener,
                    public SpectrogramSettings::Listener
{    
public:
    
    MultiTrack();
    
    ~MultiTrack();
    
    void setNumberOfTracks(unsigned int num);

    void createNewProject( File loadFile );
    void exportAudio( File saveFolder );
    void toggleAutoSolo();
    double getLengthInSeconds();
    
    void setGain( double gain );
    
    void addToMask();
    void clearMasksImages();
    
    
    
    virtual void timelineEvent( Timeline::Event msg );
    
    //Callback to check whether this target is interested in the set of files being offered.
    virtual bool 	isInterestedInFileDrag (const StringArray &files);
 	
    //Callback to indicate that some files are being dragged over this component.
    virtual void 	fileDragEnter (const StringArray &files, int x, int y);
 	
    //Callback to indicate that the user is dragging some files over this component.
    virtual void 	fileDragMove (const StringArray &files, int x, int y);
 	
    //Callback to indicate that the mouse has moved away from this component.
    virtual void 	fileDragExit (const StringArray &files);
    
    virtual void 	filesDropped (const StringArray &files, int x, int y);

    
    // Transport
    bool isPlaying();
    void stop();
    void start();
    void setLoopMarkers(double startInSec, double stopInSec);
    void setLooping( bool isLooping );
    void setPositionInSec( double posInSec );
    double getPositionInSec();
 
    void setSourceToPaint( int source );
    
    void updateXRangeInSec ( Range<double> range );
    void zoomVertical( bool isZoomIn );
    
    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets );
    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  );
    
    virtual void mouseWheelMove	(	const MouseEvent & 	event,
                                 const MouseWheelDetails & 	wheel
                                 );
    
    virtual void changeListenerCallback (ChangeBroadcaster *source);
    virtual void updateVisualization();
    void resetState();

    void resized();
    
    virtual void trackUpdate( Track * t, Track::Listener::Msg msg );
    void updatedTransportControl( TransportComp * transportComp, TransportEvent event );
    
    void zoomInHorizontal();
    void zoomOutHorizontal();
    void zoomIn();
    void zoomOut();
    
    void setSingleTrackMode( bool isTrue );
    
private:
    void repaintCursors();
    
    void pageScroll( float currentPosition );
    
    virtual void timerCallback ();
    
    void postLoad();
    
    virtual void scrollBarMoved (ScrollBar *scrollBarThatHasMoved, double newRangeStart);

private:
 
    
    void setCursorBecauseOfAudio( );
    
    ScopedPointer<Cursor> cursor;
    ScopedPointer<Cursor> loopStartCursor;
    ScopedPointer<Cursor> loopStopCursor;
    ScopedPointer<ScrollBar> xScrollBar;
    ScopedPointer<ScrollBar> yScrollBar;
    ScopedPointer<Timeline> timeline;
    
    OwnedArray<Track> tracks;
//    Array<Rectangle<int>> heights;
    float tempHeight;
    
    Array<float> trueHeights;
    
    unsigned int numVisibleTracks;
    double previousStartTime;
    
    static const int numTracks;
    static const int scrollBarHeight;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiTrack)
    
};


#endif  // __MULTITRACK_H_BC58BE4C__
