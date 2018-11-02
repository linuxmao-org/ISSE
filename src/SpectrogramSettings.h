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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "ProgramSettings.h"
#include "SpectrogramSettings.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SpectrogramSettings  : public Component,
                             public Slider::Listener,
                             public ComboBox::Listener
{
public:
    class Listener
    {
    public:
        virtual void updateVisualization()=0;
        virtual ~Listener(){}
    };

    //==============================================================================
    SpectrogramSettings (SpectrogramSettings::Listener * listener_, ProgramSettings & settings_);
    ~SpectrogramSettings();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    static void openSettingsAsModalDialog( SpectrogramSettings::Listener * listener, ProgramSettings & settings);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SpectrogramSettings::Listener * listener;
    ProgramSettings & settings;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Slider> clipSlider;
    std::unique_ptr<Label> label;
    std::unique_ptr<ComboBox> colorMapComboBox;
    std::unique_ptr<Label> label2;
    std::unique_ptr<ComboBox> processComboBox;
    std::unique_ptr<Label> label3;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrogramSettings)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
