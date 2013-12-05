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
#ifndef __JUCER_HEADER_BRUSHPANEL_BRUSHPANEL_E199E739__
#define __JUCER_HEADER_BRUSHPANEL_BRUSHPANEL_E199E739__

 
#include "JuceHeader.h"
 
 
class BrushPanel  : public Component,
                    public SliderListener,
                    public Button::Listener
{
public:
    //==============================================================================
    BrushPanel (float width, float height);
    ~BrushPanel();
 
    float getBrushWidth();
    float getBrushHeight();
 

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    virtual void 	buttonClicked (Button *);

private:
 
    
    ScopedPointer<Slider> widthSlider;
    ScopedPointer<Slider> heightSlider;
    ScopedPointer<Slider> opacitySlider;
    ScopedPointer<Label> widthLabel;
    ScopedPointer<Label> heightLabel;
    ScopedPointer<Label> opacityLabel;
    ScopedPointer<ToggleButton> accumulateToggle;
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BrushPanel);
};

 

#endif   // __JUCER_HEADER_BRUSHPANEL_BRUSHPANEL_E199E739__
