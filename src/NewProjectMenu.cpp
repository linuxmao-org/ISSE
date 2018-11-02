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
// Authors: Nicholas J. Bryan
//[/Headers]

#include "NewProjectMenu.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
NewComponent::NewComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    textEditor.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (textEditor.get());
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (TRANS("Project Name"));

    textEditor->setBounds (8, 8, 440, 24);

    browseButton.reset (new TextButton ("browseButton"));
    addAndMakeVisible (browseButton.get());
    browseButton->setButtonText (TRANS("Browse"));
    browseButton->addListener (this);

    browseButton->setBounds (464, 8, 134, 24);

    groupComponent.reset (new GroupComponent ("new group",
                                              TRANS("Separation Options")));
    addAndMakeVisible (groupComponent.get());
    groupComponent->setColour (GroupComponent::textColourId, Colours::white);

    groupComponent->setBounds (296, 40, 272, 160);

    groupComponent2.reset (new GroupComponent ("new group",
                                               TRANS("Input Channel Options")));
    addAndMakeVisible (groupComponent2.get());
    groupComponent2->setColour (GroupComponent::textColourId, Colours::white);

    groupComponent2->setBounds (8, 40, 272, 160);

    toggleButton.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton.get());
    toggleButton->setButtonText (TRANS("Speech + Background"));
    toggleButton->addListener (this);
    toggleButton->setColour (ToggleButton::textColourId, Colours::white);

    toggleButton->setBounds (312, 112, 208, 24);

    toggleButton2.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton2.get());
    toggleButton2->setButtonText (TRANS("Music (2 Sources)"));
    toggleButton2->addListener (this);
    toggleButton2->setColour (ToggleButton::textColourId, Colours::white);

    toggleButton2->setBounds (312, 64, 150, 24);

    toggleButton3.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton3.get());
    toggleButton3->setButtonText (TRANS("Polyphonic Music (88 Keys)"));
    toggleButton3->addListener (this);
    toggleButton3->setColour (ToggleButton::textColourId, Colours::white);

    toggleButton3->setBounds (312, 160, 200, 24);

    toggleButton4.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton4.get());
    toggleButton4->setButtonText (TRANS("Right"));
    toggleButton4->addListener (this);
    toggleButton4->setColour (ToggleButton::textColourId, Colours::white);

    toggleButton4->setBounds (24, 96, 150, 24);

    toggleButton5.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton5.get());
    toggleButton5->setButtonText (TRANS("Left"));
    toggleButton5->addListener (this);
    toggleButton5->setColour (ToggleButton::textColourId, Colours::white);

    toggleButton5->setBounds (24, 64, 150, 24);

    toggleButton6.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton6.get());
    toggleButton6->setButtonText (TRANS("Mono (L + R)"));
    toggleButton6->addListener (this);
    toggleButton6->setColour (ToggleButton::textColourId, Colours::white);

    toggleButton6->setBounds (24, 128, 200, 24);

    toggleButton7.reset (new ToggleButton ("new toggle button"));
    addAndMakeVisible (toggleButton7.get());
    toggleButton7->setButtonText (TRANS("Stereo"));
    toggleButton7->addListener (this);
    toggleButton7->setColour (ToggleButton::textColourId, Colours::white);

    toggleButton7->setBounds (24, 160, 200, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

NewComponent::~NewComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textEditor = nullptr;
    browseButton = nullptr;
    groupComponent = nullptr;
    groupComponent2 = nullptr;
    toggleButton = nullptr;
    toggleButton2 = nullptr;
    toggleButton3 = nullptr;
    toggleButton4 = nullptr;
    toggleButton5 = nullptr;
    toggleButton6 = nullptr;
    toggleButton7 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void NewComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff404040));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void NewComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void NewComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == browseButton.get())
    {
        //[UserButtonCode_browseButton] -- add your button handler code here..
        //[/UserButtonCode_browseButton]
    }
    else if (buttonThatWasClicked == toggleButton.get())
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == toggleButton2.get())
    {
        //[UserButtonCode_toggleButton2] -- add your button handler code here..
        //[/UserButtonCode_toggleButton2]
    }
    else if (buttonThatWasClicked == toggleButton3.get())
    {
        //[UserButtonCode_toggleButton3] -- add your button handler code here..
        //[/UserButtonCode_toggleButton3]
    }
    else if (buttonThatWasClicked == toggleButton4.get())
    {
        //[UserButtonCode_toggleButton4] -- add your button handler code here..
        //[/UserButtonCode_toggleButton4]
    }
    else if (buttonThatWasClicked == toggleButton5.get())
    {
        //[UserButtonCode_toggleButton5] -- add your button handler code here..
        //[/UserButtonCode_toggleButton5]
    }
    else if (buttonThatWasClicked == toggleButton6.get())
    {
        //[UserButtonCode_toggleButton6] -- add your button handler code here..
        //[/UserButtonCode_toggleButton6]
    }
    else if (buttonThatWasClicked == toggleButton7.get())
    {
        //[UserButtonCode_toggleButton7] -- add your button handler code here..
        //[/UserButtonCode_toggleButton7]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="NewComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff404040"/>
  <TEXTEDITOR name="new text editor" id="b9ac916389d94a06" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="8 8 440 24" initialText="Project Name"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="browseButton" id="2ada7026565240f" memberName="browseButton"
              virtualName="" explicitFocusOrder="0" pos="464 8 134 24" buttonText="Browse"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GROUPCOMPONENT name="new group" id="416943080231b6ea" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="296 40 272 160" textcol="ffffffff"
                  title="Separation Options"/>
  <GROUPCOMPONENT name="new group" id="b8ffdd45b412275f" memberName="groupComponent2"
                  virtualName="" explicitFocusOrder="0" pos="8 40 272 160" textcol="ffffffff"
                  title="Input Channel Options"/>
  <TOGGLEBUTTON name="new toggle button" id="5c7f1a44cb13e4c" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="312 112 208 24" txtcol="ffffffff"
                buttonText="Speech + Background" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="29da2e55bd8fa2e0" memberName="toggleButton2"
                virtualName="" explicitFocusOrder="0" pos="312 64 150 24" txtcol="ffffffff"
                buttonText="Music (2 Sources)" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="503d8f69ca98276e" memberName="toggleButton3"
                virtualName="" explicitFocusOrder="0" pos="312 160 200 24" txtcol="ffffffff"
                buttonText="Polyphonic Music (88 Keys)" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="e71e44f6e3e04664" memberName="toggleButton4"
                virtualName="" explicitFocusOrder="0" pos="24 96 150 24" txtcol="ffffffff"
                buttonText="Right" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="9beb505ba6a07e87" memberName="toggleButton5"
                virtualName="" explicitFocusOrder="0" pos="24 64 150 24" txtcol="ffffffff"
                buttonText="Left" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="59478797339cd9ab" memberName="toggleButton6"
                virtualName="" explicitFocusOrder="0" pos="24 128 200 24" txtcol="ffffffff"
                buttonText="Mono (L + R)" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="new toggle button" id="80953c62ca7d1cdb" memberName="toggleButton7"
                virtualName="" explicitFocusOrder="0" pos="24 160 200 24" txtcol="ffffffff"
                buttonText="Stereo" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
