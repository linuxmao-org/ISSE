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

#include "MaskLayer.h"
#include "api/ISSMask.h"
#include "ProgramSettings.h"
#include "ToolbarState.h"
#include "Main.h"
#include "AppCommandIDs.h"
#include "Track.h"

// Action/rendering classes
#include "TrainingSelectionAction.h"
#include "PaintBrushImgAction.h"
#include "ClearImageAction.h"
#include "ClearMaskAction.h"
#include "AddToMaskAction.h"
#include "BoxBrushImgAction.h"
#include "SeparationEngine.h"

 



MaskLayer::MaskLayer(int trackNum_, int source_)    :   loadedFromXML(false), mask(NULL),
                                                        trackNum(trackNum_),
                                                        source(source_),
                                                        isMouseDown(false),
                                                         isEditable(true)
{
    SeparationEngine::getInstance()->addListener(this);
    
    image = Image (Image::SingleChannel, 10, 10, true);
    mask = SeparationEngine::getInstance()->getMask(source);
    
    setVisible(true);
}


MaskLayer::~MaskLayer()
{    
    SeparationEngine * engine = SeparationEngine::getInstanceWithoutCreating();
    if(engine) engine->removeListener(this);
}

void MaskLayer::resizeImage(size_t width, size_t height)
{
    image = Image (Image::SingleChannel, width, height, true);
}

// Used to update the mask sizes
void MaskLayer::engineNewPlugin( ISSPluginBase * plugin, Msg msg )
{

    if(msg==SeparationEngine::Listener::PluginLoadedXML)
        return;
    
    
    if(msg==SeparationEngine::Listener::NewPluginLoaded ||
       msg==SeparationEngine::Listener::NewMaskSTFTParams)
    {
    
    
        if(!loadedFromXML)
        {
            size_t w, h;
            std::pair<size_t, size_t> maskSize(plugin->getMaskSize());
            w = maskSize.first;
            h = maskSize.second;
    
            if(w==0 || h==0)
                image = Image (Image::SingleChannel, 10, 10, true);
            else
                image = Image (Image::SingleChannel, w, h, true);
     
        }
    }
    loadedFromXML = false;
    
    
   
}


void MaskLayer::dirty()
{
    if(mask)
        mask->dirty();
}


bool MaskLayer::saveToXML( XmlElement & xml, const File & folderForAssets )
{
    if(mask==NULL)
        return false;

    XmlElement * maskXML = new XmlElement ("maskimage");
    String maskFileName = String(trackNum) + "-" + String(source) + ".png";
    maskXML->setAttribute("filename", maskFileName );
    
    File file = folderForAssets.getFullPathName() + "/" + maskFileName;

    
    bool okay = saveAsBinary(file);
    if(okay)
    {
        xml.addChildElement(maskXML);
        return true;
    }
    else
    {
        deleteAndZero(maskXML);
        return false;
    }
}

bool MaskLayer::loadFromXML( const XmlElement & xml, const File & assetsFolder  )
{
    
    String filename  = xml.getStringAttribute ("filename");
    File maskfile = assetsFolder.getFullPathName() + "/" + filename ;
    bool okay = loadAsBinary(maskfile);
    
    loadedFromXML = true;
    return okay;
}


bool MaskLayer::saveAsBinary( File & file )
{
    if(mask==NULL) return false;

    // Write PNGs
    File imgFile = file;//file.withFileExtension("png");
    imgFile.deleteFile();
    imgFile.create();
    
    FileOutputStream * imgstream = imgFile.createOutputStream();
    PNGImageFormat pngFormat;
    pngFormat.writeImageToStream(image, *imgstream);
    imgstream->flush();
    deleteAndZero(imgstream);

    return true;
}

bool MaskLayer::loadAsBinary( File & file )
{
    if(mask==NULL)
        return false;


    File imgFile = file.withFileExtension("png");
    FileInputStream * pngStream = imgFile.createInputStream();
    if(pngStream)
    {
        PNGImageFormat pngFormat;
        image = pngFormat.decodeImage(*pngStream);
        deleteAndZero(pngStream);
    }
    
    return true;
}



void MaskLayer::clearImage()
{
    // Perform as undoable action
    ClearImageAction * action = new ClearImageAction(this);
    
    // Begin new undo-able action state
    MAIN_APP->undoManager.perform(action, "Clear Image");
    MAIN_APP->undoManager.sendChangeMessage();

}

void MaskLayer::addToMask()
{
    // Perform as undoable action
    AddToMaskAction * action = new AddToMaskAction(this);
    
    // Begin new undo-able action state
    MAIN_APP->undoManager.perform(action, "Add To Mask");
    MAIN_APP->undoManager.sendChangeMessage();
}

void MaskLayer::paint(Graphics &g)
{
    if(getMaxX()==0) return;

    Colour c(ProgramSettings::sourceColors[source]);
    g.setColour(c.withAlpha(1.f));
    int w(getWidth());
    int h(getHeight());

    double sourceX(0), sourceY(0);
    double sourceXEnd(0), sourceYEnd(0);
    screen2ImageSpace(0, 0, sourceX, sourceY);
    screen2ImageSpace(w, h, sourceXEnd, sourceYEnd);

    g.drawImage(image, 0, 0, w, h, sourceX, sourceY,
                sourceXEnd-sourceX, sourceYEnd-sourceY, true);

	

    if( isMouseDown )
    {
	    Point<int> p = getLeftLower( false );
		int xmin = p.getX();
		int ymin = p.getY();
		p = getRightUpper( false );
		int xmax = p.getX();
		int ymax = p.getY();

		//Logger::outputDebugString(String("xmin:") + String(xmin) + String(" xmax:") + String(xmax) );
        
        if( ToolbarState::selector == ToolbarState::TimeSelector  )
        {
            g.setColour(c.withAlpha(ProgramSettings::brushStrength));
            g.fillRect( xmin, 0,  xmax - xmin, h);
            g.setColour(Colours::white);
            g.drawRect( xmin, 0,  xmax - xmin, h, 1);
            
        }
        else if( ToolbarState::selector == ToolbarState::TrainingSelector)
        {
            g.setColour(c.withAlpha(.5f));
            g.fillRect( xmin, 0,  xmax - xmin, h);
            
            g.setColour(Colours::white);
            g.drawRect( xmin, 0,  xmax - xmin, h, 1);
        }
        else if( ToolbarState::selector == ToolbarState::FreqSelector )
        {
            g.setColour(c.withAlpha(ProgramSettings::brushStrength));
            g.fillRect( 0, ymin,  w, ymax - ymin );
            
            g.setColour(Colours::white);
            g.drawRect( 0, ymin,  w, ymax - ymin, 1);
        }
        else if(ToolbarState::selector ==  ToolbarState::BoxSelector )
        {
            g.setColour(c.withAlpha(ProgramSettings::brushStrength));
            g.fillRect( xmin, ymin,  xmax - xmin, ymax - ymin );
            
            g.setColour(Colours::white);
            g.drawRect( xmin, ymin,  xmax - xmin, ymax - ymin, 1);
        }
    }
}

void MaskLayer::quantizeScreenSpace(int & x, int & y)
{
    Range<double> xrange(getXRange());
    Range<double> cr(getYRange());
    double sourceX = xrange.getStart();
    double sourceXEnd =  xrange.getEnd();
    double sourceY =     cr.getStart();
    double sourceYEnd =  cr.getEnd();
    
    double imgWidth(image.getWidth());
    double imgHeight(image.getHeight());
    
    double xs(x/(1.0*getWidth()));
    double ys(y/(1.0*getHeight()));
    
    double xImagespace = imgWidth *(sourceX + (sourceXEnd - sourceX)*xs)/getMaxX();
    double yImagespace = imgHeight*(sourceY + (sourceYEnd - sourceY)*ys)/getMaxY();
    
    xImagespace = round(xImagespace);
    yImagespace = round(yImagespace);
    
    x = getWidth()*((getMaxX()*xImagespace/imgWidth - sourceX)/(sourceXEnd - sourceX));
    y = getHeight()*((getMaxY()*yImagespace/imgHeight - sourceY)/(sourceYEnd - sourceY));
 
    if((sourceXEnd - sourceX)==0) y = 0;
    if((sourceYEnd - sourceY)==0) x = 0;
}

void MaskLayer::image2ScreenSpace(double xImagespace, double yImagespace, int &x, int &y)
{
    Range<double> xrange(getXRange());
    Range<double> cr(getYRange());
    double sourceX = xrange.getStart();
    double sourceXEnd =  xrange.getEnd();
    double sourceY =     cr.getStart();
    double sourceYEnd =  cr.getEnd();
    
    double imgWidth(image.getWidth());
    double imgHeight(image.getHeight());
    
    double xs = (getMaxX()*xImagespace/imgWidth - sourceX)/(sourceXEnd - sourceX);
    double ys = (getMaxY()*yImagespace/imgHeight - sourceY)/(sourceYEnd - sourceY);
    
    x = xs*getWidth();
    y = ys*getHeight();
    
    if((sourceXEnd - sourceX)==0) y = 0;
    if((sourceYEnd - sourceY)==0) x = 0;
}

void MaskLayer::screen2ImageSpace(int x, int y, double & xImagespace, double & yImagespace)
{
    Range<double> xrange(getXRange());
    Range<double> cr(getYRange());
    double sourceX = xrange.getStart();
    double sourceXEnd =  xrange.getEnd();
    double sourceY =     cr.getStart();
    double sourceYEnd =  cr.getEnd();
    
    double imgWidth(image.getWidth());
    double imgHeight(image.getHeight());
    
    double xs(x/(1.0*getWidth()));
    double ys(y/(1.0*getHeight()));
    
    xImagespace = imgWidth *(sourceX + (sourceXEnd - sourceX)*xs)/getMaxX();
    yImagespace = imgHeight*(sourceY + (sourceYEnd - sourceY)*ys)/getMaxY();
}

int MaskLayer::getImageHeight()
{
    return image.getHeight();
}

int MaskLayer::getImageWidth()
{
    return image.getWidth();
}

Image * MaskLayer::getImage()
{
    return &image;
}

void MaskLayer::setEditable( bool isEditable_ )
{
    isEditable = isEditable_;
}
Matrix & MaskLayer::getMatrix()
{
    if(mask)
        return mask->getMask();
    else
        return empty;
}

void MaskLayer::applybrushSelector(int x, int y, bool erase, bool first)
{
    
    double yImagespace, xImagespace;
    screen2ImageSpace(x, y, xImagespace, yImagespace);
    // quantize
    yImagespace = std::min(std::max(0, (int)round(yImagespace-.5)), int(image.getHeight()-1));
    xImagespace = std::min(std::max(0, (int)round(xImagespace-.5)), int(image.getWidth()-1));

    float A = ProgramSettings::brushStrength;
    if(erase) A *= -1;
    
    PaintBrushImgAction * action = new PaintBrushImgAction(this, first, xImagespace, yImagespace, A,
                                                     ProgramSettings::brushWidth,
                                                     ProgramSettings::brushHeight,
                                                           ProgramSettings::accumulate);
    
    MAIN_APP->undoManager.perform(action, "Apply Brush");
    MAIN_APP->undoManager.sendChangeMessage();
}

void MaskLayer::mouseDown (const MouseEvent& e)
{
    Component * comp = getParentComponent();
    if(comp) comp->mouseDown(e);
    
    if( e.mods.isRightButtonDown() )
    {
        PopupMenu menu;
        
        menu.addItem(1, "Open Multi Paint View");
        menu.addItem(2, "Open Single Paint View");
        
        const int result = menu.show();
        
        if (result == 0)
        {
            // user dismissed the menu without picking anything
        }
        else if (result == 1)
        {
            // user picked item 1
            MAIN_APP->commandManager.invokeDirectly(AppCommandIDs::OpenMultiTrack, true);
        }
        else if (result >= 2)
        {
            // user picked item 2
            MAIN_APP->commandManager.invokeDirectly(AppCommandIDs::OpenSingleTrack, true);
        }
    }
    
    if(!isEditable) return;
 
    
    int x = e.getPosition().getX();
    int y = e.getPosition().getY();
    
    isMouseDown = true;
    
    // Begin new undo-able action state
    MAIN_APP->undoManager.beginNewTransaction ();
    
    mouseDownPoint = Point<int>(x, y);
    mouseDragPoint = mouseDownPoint;

    if(ToolbarState::selector == ToolbarState::BrushSelector)
        applybrushSelector(x, y, ToolbarState::erase, true);
    
    repaint();
}
void MaskLayer::mouseMove(const MouseEvent& e)
{}

void MaskLayer::mouseDrag (const MouseEvent& e)
{
    if(!isEditable) return;
    
    int x = e.getPosition().getX();
    int y = e.getPosition().getY();
    
    mouseDragPoint = Point<int>(x, y);
    
    if(ToolbarState::selector == ToolbarState::BrushSelector)
        applybrushSelector(x, y, ToolbarState::erase, false);
    
    repaint();
}

Point<int> MaskLayer::getLeftLower( bool inImageSpace )
{
    int leftPoint  = std::max(std::min(mouseDownPoint.getX(), mouseDragPoint.getX()), 0);
    int downPoint  = std::max(std::min(mouseDownPoint.getY(), mouseDragPoint.getY()), 0);
    
    if(inImageSpace)
    {
        double xmin, ymin;
        screen2ImageSpace(leftPoint, downPoint, xmin, ymin);
        xmin = std::min(std::max(0., round(xmin-.5)), image.getWidth()-1.);
        ymin = std::min(std::max(0., round(ymin-.5)), image.getHeight()-1.);
        return Point<int>(xmin, ymin);
    }
    else
    {
        quantizeScreenSpace(leftPoint, downPoint );
        return Point<int>(leftPoint,downPoint);
    }
}

Point<int> MaskLayer::getRightUpper( bool inImageSpace )
{
    int w(getWidth());
    int h(getHeight());
    int rightPoint = std::min(std::max(mouseDownPoint.getX(), mouseDragPoint.getX()), w);
    int upPoint    = std::min(std::max(mouseDownPoint.getY(), mouseDragPoint.getY()), h);
    
    if(inImageSpace)
    {
        double xmin, ymin;
        screen2ImageSpace(rightPoint, upPoint, xmin, ymin);
        xmin = std::min(std::max(0., round(xmin-.5)), image.getWidth()-1.);
        ymin = std::min(std::max(0., round(ymin-.5)), image.getHeight()-1.);
        return Point<int>(xmin, ymin);
    }
    else
    {
        quantizeScreenSpace(rightPoint, upPoint );
        return Point<int>(rightPoint,upPoint);
    }
}

void MaskLayer::mouseUp(const MouseEvent & e)
{
    if(!isEditable) return;

    int x = e.getPosition().getX();
    int y = e.getPosition().getY();
    
    isMouseDown = false;
    mouseDragPoint = Point<int>(x, y);

 
    Point<int> p = getLeftLower( true );
    double xmin = p.getX();
    double ymin = p.getY();
    p = getRightUpper( true );
    double xmax = p.getX();
    double ymax = p.getY();
    
    if(ToolbarState::selector == ToolbarState::BrushSelector)
    {
        applybrushSelector(x, y, ToolbarState::erase, false);
        return;
    }
    else if(  ToolbarState::selector == ToolbarState::TrainingSelector )
    {
        if(xmax < xmin) // sanity check
        {
            repaint();
            return;
        }
        
        TrainingSelectionAction * action =
        new TrainingSelectionAction( source,
                                    ToolbarState::erase, xmin, xmax);
        
        MAIN_APP->undoManager.perform(action, "Training Action");
        MAIN_APP->undoManager.sendChangeMessage();
    }
    else if(ToolbarState::selector == ToolbarState::BoxSelector)
    {
        if( (xmax < xmin) || (ymax < ymin)) // sanity check
        {
            repaint();
            return;
        }
        
        createBoxAction(this, xmin, xmax, ymin, ymax,
                        ProgramSettings::brushStrength,
                        ToolbarState::erase,
                        ProgramSettings::accumulate
                        );
    }
    else if(ToolbarState::selector == ToolbarState::TimeSelector)
    {
        if(xmax < xmin) // sanity check
        {
            repaint();
            return;
        }
        
        createBoxAction(this, xmin, xmax, 0, image.getHeight()-1,
                            ProgramSettings::brushStrength,
                            ToolbarState::erase,
                            ProgramSettings::accumulate);
    }
    else if(ToolbarState::selector == ToolbarState::FreqSelector)
    {
        if(ymax < ymin) // sanity check
        {
            repaint();
            return;
        }
        
        createBoxAction(this, 0, image.getWidth()-1, ymin, ymax,
                            ProgramSettings::brushStrength,
                            ToolbarState::erase,
                            ProgramSettings::accumulate);
    }
    
    repaint(); 
}

 
void MaskLayer::createBoxAction(MaskLayer * mask,
                                int xmin, int xmax,
                                int ymin, int ymax,
                                float strength, bool erase, bool accumulate)
{
    
    if(erase)
        strength = -1*fabs(strength);
    
    BoxBrushImgAction * action =
        new BoxBrushImgAction(mask, source,
                              xmin, xmax,
                              ymin, ymax,
                              strength, accumulate);
    
    MAIN_APP->undoManager.perform(action, "Box Action");
    MAIN_APP->undoManager.sendChangeMessage();

}




