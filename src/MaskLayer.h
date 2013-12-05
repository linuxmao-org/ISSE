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

#ifndef __MaskLAYER_H_BA1C853F__
#define __MaskLAYER_H_BA1C853F__


#include "JuceHeader.h"
#include "Layer.h"
#include "SeparationEngine.h"
#include "XMLSerializable.h"

class ISSMask;


class MaskLayer :   public Layer,
                    public SeparationEngine::Listener,
                    public XMLSerializable
{
public:
  
    MaskLayer(int trackNum, int source);
    virtual ~MaskLayer();
    
    
    Matrix & getMatrix();
    virtual Image * getImage();
    void dirty();
//    void clearMask();
    void clearImage();
    void addToMask();
    
    void setEditable( bool isEditable );
    
    unsigned int getSource(){return source;}
    
    virtual void engineNewPlugin( ISSPluginBase * plugin, Msg msg  );
    virtual void engineNewOutputs( File file, int source ){};
    virtual void engineTrainingChanged(){};
    
    void resizeImage(size_t width, size_t height);
    
    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets );
    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  );
    
    virtual void paint(Graphics &g);
    
    
    virtual int getImageHeight();
    virtual int getImageWidth();
    
    virtual void mouseDown (const MouseEvent& e);
    virtual void mouseMove(const MouseEvent& e);
    virtual void mouseDrag (const MouseEvent& e);
    virtual void mouseUp(const MouseEvent & e);
  
    
private:
    void screen2ImageSpace(int x, int y, double & xImagespace, double & yImagespace);
    void image2ScreenSpace(double xImagespace, double yImagespace, int &x, int &y);
    void quantizeScreenSpace(int & x, int & y);
    
    Point<int> getLeftLower( bool inImageSpace );
    Point<int> getRightUpper( bool inImageSpace );
    
    void applybrushSelector(int xx, int yy, bool erase, bool first);
    
    void createBoxAction(MaskLayer * mask, int xmin, int xmax,
                         int ymin, int ymax, float strength, bool erase, bool accumulate);
    
    bool saveAsBinary( File & file );
    bool loadAsBinary( File & file );
    
    bool loadedFromXML;
    
    ISSMask * mask;

    Image image;
    int trackNum;
    const int source;
    Matrix empty;
    

    bool isMouseDown;
    bool isEditable;
    Point< int >  mouseDownPoint, mouseDragPoint;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MaskLayer)

};




#endif  // __MaskLAYER_H_BA1C853F__


