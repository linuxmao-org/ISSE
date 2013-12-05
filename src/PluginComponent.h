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
#ifndef __JUCER_HEADER_PLUGINCOMPONENT_PLUGINCOMPONENT_44B762E6__
#define __JUCER_HEADER_PLUGINCOMPONENT_PLUGINCOMPONENT_44B762E6__

 
#include "JuceHeader.h"
#include "api/ISSPluginBase.h"
#include "SeparationEngine.h"

class ISSPluginBase;
class ISSEPluginWindow;


 
 
class PluginComponent  : public Component,
                         public Slider::Listener,
                         public Button::Listener,
                         public ComboBox::Listener,
                         public SeparationEngine::Listener
{
public:
  
    PluginComponent (ISSPluginBase * node_);
    ~PluginComponent();

 
    virtual void 	sliderValueChanged (Slider *slider);
    void buttonClicked (Button* buttonThatWasClicked);
    void constructGUI();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
 
    virtual void engineNewPlugin( ISSPluginBase * plugin, Msg msg );
    virtual void engineNewOutputs( File file, int source ){};
    virtual void engineTrainingChanged(){};
    void paint (Graphics& g);
    void resized();

 
    juce_UseDebuggingNewOperator

private:
 
    int height;
    int maxWidth;
    ISSPluginBase * plugin;
    ProgramList programList;
    ParameterList parameterList;
    std::map<std::string, bool> parameterMap;

    ScopedPointer<GroupComponent> optionalGroup;
    ScopedPointer<Label> nameLabel;
    ScopedPointer<Label> descLabel;
    ScopedPointer<Label> makerLabel;
    ScopedPointer<Label> copyrightLabel;
    ScopedPointer<Label> versionLabel;
    ScopedPointer<Label> versionLabel2;
    ScopedPointer<Label> nameLabel2;
    ScopedPointer<Label> descLabel2;
    ScopedPointer<Label> makerLabel2;
    ScopedPointer<Label> copyrightLabel2;
    ScopedPointer<Label> programsLabel;

    OwnedArray<Label> labels;
    OwnedArray<Slider> sliders;
    OwnedArray<ComboBox> comboBoxes;

    ScopedPointer<TextButton> resetButton;
    ScopedPointer<ComboBox> programsComboBox;
    
//    ISSStftParams params;
    bool didChange;
    bool needsNewOverlays;
    
    void updateMaxWidth( int w );
    

    PluginComponent (const PluginComponent&);
    const PluginComponent& operator= (const PluginComponent&);
};


#endif   // __JUCER_HEADER_PLUGINCOMPONENT_PLUGINCOMPONENT_44B762E6__
