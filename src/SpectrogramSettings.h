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

#ifndef __JUCE_HEADER_2254AF9A550C4DE0__
#define __JUCE_HEADER_2254AF9A550C4DE0__

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
                             public SliderListener,
                             public ComboBoxListener
{
public:
    class Listener
    {
    public:
        virtual void updateVisualization()=0;
        virtual ~Listener(){}
    };

    
public:
    //==============================================================================
    SpectrogramSettings (SpectrogramSettings::Listener * listener_, ProgramSettings & settings_);
    ~SpectrogramSettings();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    static void openSettingsAsModalDialog( SpectrogramSettings::Listener * listener, ProgramSettings & settings);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SpectrogramSettings::Listener * listener;

    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> clipSlider;
    ScopedPointer<Label> label;
    ScopedPointer<ComboBox> colorMapComboBox;
    ScopedPointer<Label> label2;
    ScopedPointer<ComboBox> processComboBox;
    ScopedPointer<Label> label3;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrogramSettings)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_2254AF9A550C4DE0__
