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

#include "ToolbarItemFact.h"


#include "ImgToolbarButton.h"
#include "ImgBinaryData.h"
#include "DocWindow.h"
#include "AppCommandIDs.h"
#include "ToolbarSlider.h"
#include "ProgramSettings.h"
#include "ToolbarProgress.h"
#include "BrushPanel.h"


#include "MainToolbar.h"
class BrushSettingsComp : public ToolbarItemComponent, public Button::Listener
{
public:
    BrushSettingsComp(int itemId, const String &labelText )
    : ToolbarItemComponent(itemId, labelText, true)
    {
        setTooltip(labelText);
        addListener(this);
    }
    
    virtual void 	buttonClicked (Button * b)
    {
        b->setToggleState(!b->getToggleState(), dontSendNotification);
        
        if(b->getToggleState())
        {
            Rectangle<int> r = this->getScreenBounds();
            r.setY( r.getY() + 5);
            
            CallOutBox::launchAsynchronously (new BrushPanel(5,5), r, nullptr);
        }
    }
    
    virtual bool getToolbarItemSizes (int toolbarThickness, bool isToolbarVertical, int &preferredSize, int &minSize, int &maxSize)
    {
        int w = 25;
        preferredSize = w;
        minSize = 25;
        maxSize = 25;
        
        return true;
    }
    
    virtual void paintButtonArea (Graphics &g, int width, int height, bool isMouseOver, bool isMouseDown)
    {
        g.setColour(Colours::white);
        g.drawRoundedRectangle(2, 4, width-4, height-8, 5, .5);
        
        
        g.setFont( Font(13.0f) );
        g.setColour(ProgramSettings::iconColor);
        
        g.drawText("+", 2, -1, width-4, height, Justification(Justification::centred), true);
        
        
    }
    
    virtual void contentAreaChanged (const Rectangle< int > &newBounds)
    {
    }
private:
    
};






ToolbarItemFact::ToolbarItemFact( double & progress_) : progress(progress_)
{}

// 	Must return a list of the IDs for all the item types of items
void ToolbarItemFact::getAllToolbarItemIds (Array< int > &ids)
{
    ids.add(separatorBarId);
    ids.add( spacerId);
    ids.add(flexibleSpacerId);
    
    ids.add(AppCommandIDs::SourceColor1);
    ids.add(AppCommandIDs::SourceColor2);
    ids.add(AppCommandIDs::ArrowSelector);
    
    ids.add(AppCommandIDs::TrainingSelector);
    ids.add(AppCommandIDs::BrushSelector);
    ids.add(AppCommandIDs::EraseMode);
    ids.add(AppCommandIDs::PaintMode);
    ids.add(AppCommandIDs::TimeSelector);
    ids.add(AppCommandIDs::FreqSelector);
    ids.add(AppCommandIDs::BoxSelector);
    ids.add(AppCommandIDs::ChangeBrushStrength);
    ids.add(AppCommandIDs::SoundPenSelector);
    
    ids.add(AppCommandIDs::ZoomIn);
    ids.add(AppCommandIDs::ZoomOut);
    ids.add(AppCommandIDs::ZoomInHorizontal);
    ids.add(AppCommandIDs::ZoomOutHorizontal);
    ids.add(AppCommandIDs::ZoomInVertical);
    ids.add(AppCommandIDs::ZoomOutVertical);

    ids.add(AppCommandIDs::Process);
    ids.add(AppCommandIDs::ChangeVolume);
    
    ids.add(AppCommandIDs::UpdateSeparationProgress);
    
}

//Must return the set of items that for the default set.
void 	ToolbarItemFact::getDefaultItemSet (Array< int > &ids)
{
    ids.add(-1);
    
    ids.add(AppCommandIDs::SourceColor1);
    ids.add(AppCommandIDs::SourceColor2);
    
    ids.add(-1);
    ids.add(-2);
    ids.add(-2);
    ids.add(-2);
    ids.add(-1);
    
    ids.add(AppCommandIDs::PaintMode);
    ids.add(AppCommandIDs::EraseMode);
    
    ids.add(-1);
    ids.add(-2);
    ids.add(-2);
    ids.add(-2);
    ids.add(-1);
    
    ids.add(AppCommandIDs::ArrowSelector);
    ids.add(AppCommandIDs::BrushSelector);
    ids.add(AppCommandIDs::TimeSelector);
    ids.add(AppCommandIDs::FreqSelector);
    ids.add(AppCommandIDs::BoxSelector);
    ids.add(AppCommandIDs::TrainingSelector);
    ids.add(AppCommandIDs::ChangeBrushStrength);
    
    ids.add(-1);
    ids.add(-2);
    ids.add(-2);
    ids.add(-2);
    ids.add(-1);
    
    ids.add(AppCommandIDs::ZoomIn);
    ids.add(AppCommandIDs::ZoomOut);
    ids.add(AppCommandIDs::ZoomInHorizontal);
    ids.add(AppCommandIDs::ZoomOutHorizontal);
    ids.add(AppCommandIDs::ZoomInVertical);
    ids.add(AppCommandIDs::ZoomOutVertical);
    
    ids.add(-1);
    ids.add(-2);
    ids.add(-2);
    ids.add(-2);
    ids.add(-1);
    

    ids.add(AppCommandIDs::Process);
    ids.add(AppCommandIDs::UpdateSeparationProgress);
    
    ids.add(-1);
    ids.add(-2);
    ids.add(-2);
    ids.add(-2);
    ids.add(-1);
    
    
    ids.add(AppCommandIDs::ChangeVolume);

}


void ToolbarItemFact::getButtonInfo(int itemId, char* & icon_png,
                                    int & icon_pngSize, String & labelText )
{

    if( itemId == AppCommandIDs::SourceColor1 )
    {
        labelText = "Source 1";
        icon_png = (char*)ImgBinaryData::solidcoloricon_png;
        icon_pngSize = (int)ImgBinaryData::solidcoloricon_pngSize;
    }
    else if( itemId == AppCommandIDs::SourceColor2 )
    {
        labelText = "Source 2";
        icon_png = (char*)ImgBinaryData::solidcoloricon_png;
        icon_pngSize = (int)ImgBinaryData::solidcoloricon_pngSize;
    }
    else if( itemId == AppCommandIDs::BrushSelector )
    {
        labelText = "Spray Brush";
        icon_png = (char*)ImgBinaryData::brushicon_png;
        icon_pngSize = (int)ImgBinaryData::brushicon_pngSize;
    }
    else if( itemId == AppCommandIDs::EraseMode )
    {
        labelText = "Erase Mode";
        icon_png = (char*)ImgBinaryData::erasericon_png;
        icon_pngSize = (int)ImgBinaryData::erasericon_pngSize;
    }
    else if( itemId == AppCommandIDs::TimeSelector )
    {
        labelText = "Time Brush";
        icon_png = (char*)ImgBinaryData::timeselectoricon_png;
        icon_pngSize = (int)ImgBinaryData::timeselectoricon_pngSize;
    }
    else if( itemId==AppCommandIDs::FreqSelector )
    {
        labelText = "Frequency Brush";
        icon_png = (char*)ImgBinaryData::freqselectoricon_png;
        icon_pngSize = (int)ImgBinaryData::freqselectoricon_pngSize;
    }
    else if( itemId==AppCommandIDs::BoxSelector )
    {
        labelText = "Box Brush";
        icon_png = (char*)ImgBinaryData::boxselectoricon_png;
        icon_pngSize = (int)ImgBinaryData::boxselectoricon_pngSize;
    }
    else if( itemId == AppCommandIDs::ZoomInHorizontal)
    {
        labelText = "Zoom In Horizontal";
        icon_png = (char*)ImgBinaryData::zoominhorizontalicon_png;
        icon_pngSize = (int)ImgBinaryData::zoominhorizontalicon_pngSize;
    }
    else if( itemId == AppCommandIDs::ZoomOutHorizontal)
    {
        labelText = "Zoom Out Horizontal";
        icon_png = (char*)ImgBinaryData::zoomouthorizontalicon_png;
        icon_pngSize = (int)ImgBinaryData::zoomouthorizontalicon_pngSize;
    }
    else if( itemId == AppCommandIDs::ZoomInVertical)
    {
        labelText = "Zoom In Vertical";
        icon_png = (char*)ImgBinaryData::zoominverticalicon_png;
        icon_pngSize = (int)ImgBinaryData::zoominverticalicon_pngSize;
    }
    else if( itemId == AppCommandIDs::ZoomOutVertical)
    {
        labelText = "Zoom Out Vertical";
        icon_png = (char*)ImgBinaryData::zoomoutverticalicon_png;
        icon_pngSize = (int)ImgBinaryData::zoomoutverticalicon_pngSize;
    }
    else if( itemId == AppCommandIDs::ZoomIn)
    {
        labelText = "Zoom In";
        icon_png = (char*)ImgBinaryData::zoominicon_png;
        icon_pngSize = (int)ImgBinaryData::zoominicon_pngSize;
    }
    else if( itemId == AppCommandIDs::ZoomOut)
    {
        labelText = "Zoom Out";
        icon_png = (char*)ImgBinaryData::zoomouticon_png;
        icon_pngSize = (int)ImgBinaryData::zoomouticon_pngSize;
    }
    else if( itemId == AppCommandIDs::ArrowSelector)
    {
        labelText = "Arrow Select";
        icon_png = (char*)ImgBinaryData::arrowicon_png;
        icon_pngSize = (int)ImgBinaryData::arrowicon_pngSize;
    }
    else if( itemId == AppCommandIDs::PaintMode)
    {
        labelText = "Paint Mode";
        icon_png = (char*)ImgBinaryData::painticon_png;
        icon_pngSize = (int)ImgBinaryData::painticon_pngSize;
    }
    else if( itemId == AppCommandIDs::Process)
    {
        labelText = "Process";
        icon_png = (char*)ImgBinaryData::processicon_png;
        icon_pngSize = (int)ImgBinaryData::processicon_pngSize;
    }
    else if( itemId == AppCommandIDs::TrainingSelector)
    {
        labelText = "Training Brush";
        icon_png = (char*)ImgBinaryData::trainingselectoricon_png;
        icon_pngSize = (int)ImgBinaryData::trainingselectoricon_pngSize;
    }
}




ToolbarItemComponent * 	ToolbarItemFact::createItem (int itemId)
{
    ImgToolbarButton * button = NULL;
    char * icon_png = NULL;
    int icon_pngSize = 0;
    String labelText;
    
    if( itemId == AppCommandIDs::ChangeVolume)
    {
        Range<float> r(-80.0, 15.0);
        ToolbarSlider * slider = new ToolbarSlider(itemId, "Volume", r, false);
        return slider;
    }
    else if(itemId == AppCommandIDs::ChangeBrushStrength)
    {
        return new BrushSettingsComp(itemId, "Brush Settings");
        
    }
    else if(itemId==AppCommandIDs::UpdateSeparationProgress)
    {
        return new ToolbarProgress(itemId, "Separation Progress", progress);
    }
    else
        getButtonInfo(itemId, icon_png, icon_pngSize,  labelText);
    
    
    DrawableImage * img = new DrawableImage();
    img->setImage ( ImageCache::getFromMemory (icon_png, icon_pngSize ) );
    DrawableImage * img2 = new DrawableImage();
    img2->setImage ( ImageCache::getFromMemory  (icon_png , icon_pngSize ));
    button =  new ImgToolbarButton (itemId, labelText, img, img2, ProgramSettings::iconColorDown );
    
    button->setOverlayColour(ProgramSettings::iconColor);
    
    if( (itemId==AppCommandIDs::ArrowSelector) ||
       (itemId==AppCommandIDs::BrushSelector) ||
       (itemId==AppCommandIDs::TimeSelector) ||
       (itemId==AppCommandIDs::FreqSelector) ||
       (itemId==AppCommandIDs::BoxSelector) ||
//       (itemId==AppCommandIDs::DrawLineSelector) ||
       (itemId==AppCommandIDs::TrainingSelector) ||
       (itemId==AppCommandIDs::SoundPenSelector)
       )
    {
        button->setRadioGroupId( 1 );
    }
    else if( (itemId==AppCommandIDs::PaintMode) || (itemId==AppCommandIDs::EraseMode) )
    {
        button->setRadioGroupId( 2 );
    }

    if( itemId == AppCommandIDs::SourceColor1 )
    {
        button->setOverlayColour(ProgramSettings::sourceColors[0]);
        button->setToggleState( true,  dontSendNotification );
        button->setRepeatSpeed(1, ProgramSettings::iconButtonTimeOutMs);
        button->repaint();
        button->setRadioGroupId( 3 );
    }
    else if( itemId == AppCommandIDs::SourceColor2 )
    {
        button->setOverlayColour(ProgramSettings::sourceColors[1]);
        button->setRepeatSpeed(1, ProgramSettings::iconButtonTimeOutMs);
        button->setRadioGroupId( 3 );
    }
    else if( itemId== AppCommandIDs::AutoSolo )// Turn autosolo mode on
    {
        button->setToggleState( true,  dontSendNotification );
        button->repaint();
    }
    else if( itemId == AppCommandIDs::PaintMode)
    {
        button->setToggleState( true,  dontSendNotification );
        button->repaint();
    }
    else if( itemId == AppCommandIDs::BrushSelector )
    {
        button->setRepeatSpeed(1, ProgramSettings::iconButtonTimeOutMs);
    }
    else if( itemId == AppCommandIDs::ArrowSelector )
    {
        button->setToggleState(true,  dontSendNotification);
        
    }

    return button;
}


