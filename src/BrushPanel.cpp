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

 

#include "BrushPanel.h"

#include "ProgramSettings.h"
 
 
BrushPanel::BrushPanel (float width, float height)
    : widthSlider ( new Slider("widthSlider")),
      heightSlider ( new Slider("heightSlider")),
      opacitySlider( new Slider("opacitySlider")),
      widthLabel (new Label("width label", "Width")),
      heightLabel (new Label("height label", "Height")),
      opacityLabel(new Label("Opacity", "Opacity")),
      accumulateToggle( new ToggleButton("Accumulate Paint"))
{
    widthSlider->setRange (0.5, 500, 0.5);
    widthSlider->setSliderStyle (Slider::LinearHorizontal);
    widthSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    widthSlider->addListener (this);
    widthSlider->setSkewFactor (0.5);

    opacitySlider->setRange (.001, 1, 0.01);
    opacitySlider->setSliderStyle (Slider::LinearHorizontal);
    opacitySlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    opacitySlider->addListener (this);
    opacitySlider->setSkewFactor (1);
    opacitySlider->setValue(1);
    
    heightSlider->setRange (0.5, 500, 0.5);
    heightSlider->setSliderStyle (Slider::LinearHorizontal);
    heightSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    heightSlider->addListener (this);
    heightSlider->setSkewFactor (0.5);
    
    widthLabel->setFont (Font (15.0000f, Font::plain));
    widthLabel->setJustificationType (Justification::centredLeft);
    widthLabel->setEditable (false, false, false);
    widthLabel->setColour (Label::textColourId, ProgramSettings::textColour);
    widthLabel->setColour (Label::backgroundColourId, Colour (0x0));

    heightLabel->setFont (Font (15.0000f, Font::plain));
    heightLabel->setJustificationType (Justification::centredLeft);
    heightLabel->setEditable (false, false, false);
    heightLabel->setColour (Label::textColourId, ProgramSettings::textColour);
    heightLabel->setColour (Label::backgroundColourId, Colour (0x0));
 
    opacityLabel->setColour(Label::textColourId, ProgramSettings::textColour);

    widthSlider->setTooltip("Brush Width");
    heightSlider->setTooltip("Brush Height");
    opacitySlider->setTooltip("Brush Opacity");
    accumulateToggle->setTooltip("Accumulate Paint");
    widthSlider->setValue(width);
    heightSlider->setValue(height);
    
    widthSlider->setValue(ProgramSettings::brushWidth);
    heightSlider->setValue(ProgramSettings::brushHeight);
    opacitySlider->setValue(ProgramSettings::brushStrength);
    
    accumulateToggle->setToggleState(ProgramSettings::accumulate, dontSendNotification);
    accumulateToggle->addListener(this);
    accumulateToggle->setColour(ToggleButton::textColourId, ProgramSettings::textColour);
    
    
    addAndMakeVisible(opacitySlider);
    addAndMakeVisible(widthSlider);
    addAndMakeVisible(heightSlider);
    addAndMakeVisible(widthLabel);
    addAndMakeVisible(heightLabel);
    addAndMakeVisible(opacityLabel);
    addAndMakeVisible(accumulateToggle);

    setSize (275, 160);

 
}

BrushPanel::~BrushPanel()
{
}

void BrushPanel::buttonClicked (Button *b)
{
    
    if(b==accumulateToggle)
    {
        ProgramSettings::accumulate = accumulateToggle->getToggleState();
    }
}

//==============================================================================
void BrushPanel::paint (Graphics& g)
{
 
    g.fillAll (ProgramSettings::mainBackgroundColor);
 
}

void BrushPanel::resized()
{
    opacitySlider->setBounds(16,  22, 184, 24);
    widthSlider->setBounds  (16,  55, 184, 24);
    heightSlider->setBounds (16,  88, 184, 24);
    opacityLabel->setBounds (200, 22, 55,  24);
    widthLabel->setBounds   (200, 55, 56,  24);
    heightLabel->setBounds  (200, 88, 56,  24);
    accumulateToggle->setBounds(16, 88+33, 180, 24);
 
}

void BrushPanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
 
    if (sliderThatWasMoved == widthSlider)
    {
        ProgramSettings::brushWidth = widthSlider->getValue();
    }
    else if (sliderThatWasMoved == heightSlider)
    {
        ProgramSettings::brushHeight = heightSlider->getValue();
    }
    else if(sliderThatWasMoved == opacitySlider)
    {
        ProgramSettings::brushStrength = opacitySlider->getValue();
    }
 
}

 
float BrushPanel::getBrushWidth()
{
    return widthSlider->getValue();
}
float BrushPanel::getBrushHeight()
{
    return heightSlider->getValue();
}
 
 