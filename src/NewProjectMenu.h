/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_5E53D049AFEB23AF__
#define __JUCE_HEADER_5E53D049AFEB23AF__

//[Headers]     -- You can add your own extra header files here --
// Authors: Nicholas J. Bryan
#include "JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class NewComponent  : public Component,
                      public ButtonListener
{
public:
    //==============================================================================
    NewComponent ();
    ~NewComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextEditor> textEditor;
    ScopedPointer<TextButton> browseButton;
    ScopedPointer<GroupComponent> groupComponent;
    ScopedPointer<GroupComponent> groupComponent2;
    ScopedPointer<ToggleButton> toggleButton;
    ScopedPointer<ToggleButton> toggleButton2;
    ScopedPointer<ToggleButton> toggleButton3;
    ScopedPointer<ToggleButton> toggleButton4;
    ScopedPointer<ToggleButton> toggleButton5;
    ScopedPointer<ToggleButton> toggleButton6;
    ScopedPointer<ToggleButton> toggleButton7;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_5E53D049AFEB23AF__
