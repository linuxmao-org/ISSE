/*
 Copyright 2013 Stanford University
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

#include "ToolbarProgress.h"


class  MyProgressBar  : public Component, public SettableTooltipClient,
private Timer
{
public:
    
    MyProgressBar (double& progress_)
    : progress (progress_),
    displayPercentage (true),
    lastCallbackTime (0)
    {
        currentValue = jlimit (0.0, 1.0, progress);
    }
    
    ~MyProgressBar()
    {
    }
    
    void setPercentageDisplay (const bool shouldDisplayPercentage)
    {
        displayPercentage = shouldDisplayPercentage;
        repaint();
    }
    
    void setTextToDisplay (const String& text)
    {
        displayPercentage = false;
        displayedMessage = text;
    }
    
    enum ColourIds
    {
        backgroundColourId              = 0x1001900,    /**< The background colour, behind the bar. */
        foregroundColourId              = 0x1001a00,    /**< The colour to use to draw the bar itself. LookAndFeel
                                                         classes will probably use variations on this colour. */
    };
    
protected:
    
    void colourChanged()
    {
        lookAndFeelChanged();
    }
    
    void paint (Graphics& g)
    {
        String text;
        
        if (displayPercentage)
        {
            if (currentValue >= 0 && currentValue <= 1.0)
                text << roundToInt (currentValue * 100.0) << '%';
        }
        else
        {
            text = displayedMessage;
        }
        
        drawProgressBar (g, getWidth(), getHeight(), currentValue, text);
    }
    
    void drawGlassLozenge (Graphics& g,
                           const float x, const float y,
                           const float width, const float height,
                           const Colour& colour,
                           const float outlineThickness,
                           const float cornerSize,
                           const bool flatOnLeft,
                           const bool flatOnRight,
                           const bool flatOnTop,
                           const bool flatOnBottom) noexcept
    {
        if (width <= outlineThickness || height <= outlineThickness)
            return;
        
        const int intX = (int) x;
        const int intY = (int) y;
        const int intW = (int) width;
        const int intH = (int) height;
        
        const float cs = cornerSize < 0 ? jmin (width * 0.5f, height * 0.5f) : cornerSize;
        const float edgeBlurRadius = height * 0.75f + (height - cs * 2.0f);
        const int intEdge = (int) edgeBlurRadius;
        
        Path outline;
        outline.addRoundedRectangle (x, y, width, height, cs, cs,
                                     ! (flatOnLeft || flatOnTop),
                                     ! (flatOnRight || flatOnTop),
                                     ! (flatOnLeft || flatOnBottom),
                                     ! (flatOnRight || flatOnBottom));
        
        {
            ColourGradient cg (colour.darker (0.2f), 0, y,
                               colour.darker (0.2f), 0, y + height, false);
            
            cg.addColour (0.03, colour.withMultipliedAlpha (0.3f));
            cg.addColour (0.4, colour);
            cg.addColour (0.97, colour.withMultipliedAlpha (0.3f));
            
            g.setGradientFill (cg);
            g.fillPath (outline);
        }
        
        ColourGradient cg (Colours::transparentBlack, x + edgeBlurRadius, y + height * 0.5f,
                           colour.darker (0.2f), x, y + height * 0.5f, true);
        
        cg.addColour (jlimit (0.0, 1.0, 1.0 - (cs * 0.5f) / edgeBlurRadius), Colours::transparentBlack);
        cg.addColour (jlimit (0.0, 1.0, 1.0 - (cs * 0.25f) / edgeBlurRadius), colour.darker (0.2f).withMultipliedAlpha (0.3f));
        
        if (! (flatOnLeft || flatOnTop || flatOnBottom))
        {
            g.saveState();
            g.setGradientFill (cg);
            g.reduceClipRegion (intX, intY, intEdge, intH);
            g.fillPath (outline);
            g.restoreState();
        }
        
        if (! (flatOnRight || flatOnTop || flatOnBottom))
        {
            cg.point1.setX (x + width - edgeBlurRadius);
            cg.point2.setX (x + width);
            
            g.saveState();
            g.setGradientFill (cg);
            g.reduceClipRegion (intX + intW - intEdge, intY, 2 + intEdge, intH);
            g.fillPath (outline);
            g.restoreState();
        }
        
        {
            const float leftIndent = flatOnTop || flatOnLeft ? 0.0f : cs * 0.4f;
            const float rightIndent = flatOnTop || flatOnRight ? 0.0f : cs * 0.4f;
            
            Path highlight;
            highlight.addRoundedRectangle (x + leftIndent,
                                           y + cs * 0.1f,
                                           width - (leftIndent + rightIndent),
                                           height * 0.4f,
                                           cs * 0.4f,
                                           cs * 0.4f,
                                           ! (flatOnLeft || flatOnTop),
                                           ! (flatOnRight || flatOnTop),
                                           ! (flatOnLeft || flatOnBottom),
                                           ! (flatOnRight || flatOnBottom));
            
            g.setGradientFill (ColourGradient (colour.brighter (10.0f), 0, y + height * 0.06f,
                                               Colours::transparentWhite, 0, y + height * 0.4f, false));
            g.fillPath (highlight);
        }
        
        g.setColour (colour.darker().withMultipliedAlpha (1.5f));
        g.strokePath (outline, PathStrokeType (outlineThickness));
    }
    
    void drawProgressBar(Graphics& g,int width, int height,
                         double progress, const String& textToShow)
    {
        const Colour background (Colours::darkgrey);
        const Colour foreground (Colours::grey);
        
        g.fillAll (background);
        
        if (progress >= 0.0f && progress <= 1.0f)
        {
            drawGlassLozenge (g, 1.0f, 1.0f,
                              (float) jlimit (0.0, width - 2.0, progress * (width - 2.0)),
                              (float) (height - 2),
                              foreground,
                              0.5f, 0.0f,
                              true, true, true, true);
        }
        
        if (textToShow.isNotEmpty())
        {
            g.setColour (Colours::white);
            g.setFont (height * 0.6f);
            g.drawText (textToShow, 0, 0, width, height, Justification::centred, false);
        }
    }
    
    
    void lookAndFeelChanged()
    {
        setOpaque (findColour (backgroundColourId).isOpaque());
    }
    
    void visibilityChanged()
    {
        if (isVisible())
            startTimer (30);
        else
            stopTimer();
    }
    
private:
    double& progress;
    double currentValue;
    bool displayPercentage;
    String displayedMessage, currentMessage;
    uint32 lastCallbackTime;
    
    void timerCallback()
    {
        double newProgress = progress;
        
        const uint32 now = Time::getMillisecondCounter();
        const int timeSinceLastCallback = (int) (now - lastCallbackTime);
        lastCallbackTime = now;
        
        if (currentValue != newProgress
            || newProgress < 0 || newProgress >= 1.0
            || currentMessage != displayedMessage)
        {
            if (currentValue < newProgress
                && newProgress >= 0 && newProgress < 1.0
                && currentValue >= 0 && currentValue < 1.0)
            {
                newProgress = jmin (currentValue + 0.0008 * timeSinceLastCallback,
                                    newProgress);
            }
            
            currentValue = newProgress;
            currentMessage = displayedMessage;
            repaint();
        }
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyProgressBar)
};





ToolbarProgress::ToolbarProgress( int itemID, const String & labelText, double & progress )
:   ToolbarItemComponent(itemID, labelText, false),
            progressbar(new MyProgressBar(progress))
{
    
    addAndMakeVisible(progressbar);
}

//This method must return the size criteria for this item, based on a given toolbar size and orientation.
bool ToolbarProgress::getToolbarItemSizes (int toolbarThickness, bool isToolbarVertical,
                                  int &preferredSize, int &minSize, int &maxSize)
{
    int w = 100;
    preferredSize = w;
    minSize = 50;
    maxSize = 100;
    return true;
}

//Your subclass should use this method to draw its content area.
void ToolbarProgress::paintButtonArea (Graphics &g, int width, int height, bool isMouseOver, bool isMouseDown)
{}

//Callback to indicate that the content area of this item has changed.
void 	ToolbarProgress::contentAreaChanged (const Rectangle< int > &newBounds)
{
    Rectangle<int> b(newBounds);
    
    b.setY(b.getY()+3);
    b.setHeight( b.getHeight()-6);
    progressbar->setBounds(b);
    
}
