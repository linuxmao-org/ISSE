/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
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

#include "ProgramSettings.h"
//[/Headers]

#include "SpectrogramSettings.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SpectrogramSettings::SpectrogramSettings (SpectrogramSettings::Listener * listener_, ProgramSettings & settings_)
    : listener(listener_), settings(settings_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    clipSlider.reset (new Slider ("clipSlider"));
    addAndMakeVisible (clipSlider.get());
    clipSlider->setRange (-120, -5, 1);
    clipSlider->setSliderStyle (Slider::LinearHorizontal);
    clipSlider->setTextBoxStyle (Slider::TextBoxRight, false, 40, 20);
    clipSlider->addListener (this);
    clipSlider->setSkewFactor (0.5);

    clipSlider->setBounds (16, 16, 160, 24);

    label.reset (new Label ("new label",
                            TRANS("Clip (dB)")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (184, 16, 64, 24);

    colorMapComboBox.reset (new ComboBox ("colorMapComboBox"));
    addAndMakeVisible (colorMapComboBox.get());
    colorMapComboBox->setEditableText (false);
    colorMapComboBox->setJustificationType (Justification::centredLeft);
    colorMapComboBox->setTextWhenNothingSelected (String());
    colorMapComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    colorMapComboBox->addListener (this);

    colorMapComboBox->setBounds (16, 48, 160, 24);

    label2.reset (new Label ("new label",
                             TRANS("Color Map")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (184, 48, 88, 24);

    processComboBox.reset (new ComboBox ("processComboBox"));
    addAndMakeVisible (processComboBox.get());
    processComboBox->setEditableText (false);
    processComboBox->setJustificationType (Justification::centredLeft);
    processComboBox->setTextWhenNothingSelected (String());
    processComboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    processComboBox->addListener (this);

    processComboBox->setBounds (16, 84, 160, 24);

    label3.reset (new Label ("new label",
                             TRANS("Processing")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (184, 84, 88, 24);


    //[UserPreSize]

    clipSlider->setValue( ProgramSettings::spectrogramClipLimit );

    std::map<ColorMap::ColorMapType, std::string> m =  ColorMap::getAllColorMaps();

    for(std::map<ColorMap::ColorMapType, std::string>::iterator iter = m.begin();
        iter!=m.end();
        iter++)
        colorMapComboBox->addItem( iter->second.c_str() , iter->first);

    colorMapComboBox->setSelectedItemIndex ( ProgramSettings::colorMapType-1, sendNotification);
    //[/UserPreSize]

    setSize (280, 200);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SpectrogramSettings::~SpectrogramSettings()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    clipSlider = nullptr;
    label = nullptr;
    colorMapComboBox = nullptr;
    label2 = nullptr;
    processComboBox = nullptr;
    label3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SpectrogramSettings::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SpectrogramSettings::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SpectrogramSettings::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == clipSlider.get())
    {
        //[UserSliderCode_clipSlider] -- add your slider handling code here..
        ProgramSettings::spectrogramClipLimit = clipSlider->getValue();
        //[/UserSliderCode_clipSlider]
    }

    //[UsersliderValueChanged_Post]

    listener->updateVisualization();

    //[/UsersliderValueChanged_Post]
}

void SpectrogramSettings::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == colorMapComboBox.get())
    {
        //[UserComboBoxCode_colorMapComboBox] -- add your combo box handling code here..
        ProgramSettings::colorMapType = (ColorMap::ColorMapType) (colorMapComboBox->getSelectedId());
        //[/UserComboBoxCode_colorMapComboBox]
    }
    else if (comboBoxThatHasChanged == processComboBox.get())
    {
        //[UserComboBoxCode_processComboBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_processComboBox]
    }

    //[UsercomboBoxChanged_Post]
    listener->updateVisualization();
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void SpectrogramSettings::openSettingsAsModalDialog( SpectrogramSettings::Listener * listener, ProgramSettings & settings)
{

    DialogWindow::LaunchOptions o;
    ScopedPointer<SpectrogramSettings> spec(new SpectrogramSettings(listener, settings));

    o.content.set (spec.release(), true);
    o.dialogTitle                   = "Visualization Settings";
    o.componentToCentreAround       = NULL;

#ifdef __WINDOWS__
	o.dialogBackgroundColour        = ProgramSettings::selectedTrackColour.withAlpha(1.f);
#else
	o.dialogBackgroundColour        = ProgramSettings::selectedTrackColour;
#endif


	o.escapeKeyTriggersCloseButton  = true;
    o.useNativeTitleBar             = true;
    o.resizable                     = false;

    o.launchAsync();



}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SpectrogramSettings" componentName=""
                 parentClasses="public Component" constructorParams="SpectrogramSettings::Listener * listener_, ProgramSettings &amp; settings_"
                 variableInitialisers="listener(listener_), settings(settings_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="280" initialHeight="200">
  <BACKGROUND backgroundColour="ffffffff"/>
  <SLIDER name="clipSlider" id="6de36180b25b0dcb" memberName="clipSlider"
          virtualName="" explicitFocusOrder="0" pos="16 16 160 24" min="-120.00000000000000000000"
          max="-5.00000000000000000000" int="1.00000000000000000000" style="LinearHorizontal"
          textBoxPos="TextBoxRight" textBoxEditable="1" textBoxWidth="40"
          textBoxHeight="20" skewFactor="0.50000000000000000000" needsCallback="1"/>
  <LABEL name="new label" id="e4a0da4c3cf2780e" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="184 16 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Clip (dB)" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="colorMapComboBox" id="145241e7094ccec5" memberName="colorMapComboBox"
            virtualName="" explicitFocusOrder="0" pos="16 48 160 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="1248d10b263f9829" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="184 48 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Color Map" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="processComboBox" id="90d5cfab373d90c7" memberName="processComboBox"
            virtualName="" explicitFocusOrder="0" pos="16 84 160 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="9889bccec03ab2ee" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="184 84 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Processing" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
