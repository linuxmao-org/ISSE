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
#include "api/ISSPluginBase.h"
#include "ProgramSettings.h"
 
#include "SeparationEngine.h"
#include "PluginComponent.h"
#include "PluginThread.h"

 
 
PluginComponent::PluginComponent (ISSPluginBase * plugin_)   :   plugin(plugin_)
{
    if(!plugin)
        throw std::exception();

    height = 500;
    maxWidth = 200;

    addAndMakeVisible ( optionalGroup = new GroupComponent());
    optionalGroup->setText("Settings");
    optionalGroup->setColour( GroupComponent::outlineColourId, Colours::white);
    optionalGroup->setColour( GroupComponent::textColourId, Colours::white);
 
    constructGUI();
 
}

PluginComponent::~PluginComponent()
{
}


// Used to rerender the GUI
void PluginComponent::engineNewPlugin( ISSPluginBase * plugin_, Msg msg )
{
    plugin = plugin_;
    constructGUI();
}



//==============================================================================
void PluginComponent::paint (Graphics& g)
{
 
    g.setColour(ProgramSettings::mainBackgroundColor);
 
}

void PluginComponent::resized()
{
    repaint();
}

 
void PluginComponent::updateMaxWidth( int w )
{

    if(w > maxWidth)
        maxWidth = w;
}

void PluginComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == programsComboBox)
    {
        if(programList.size()==0)
            return;
        
        int id = programsComboBox->getSelectedId();
        if(id > 0)
        {
            std::string prog = programList[id-1];
            
            ISSEPluginEvent event(ISSEPluginEvent::ProgramChange);
            event.name = prog;
            SeparationEngine::getInstance()->postEvent( event );
            
            // reconstruct the GUI
            constructGUI();
            
            programsComboBox->setSelectedId(id, sendNotificationAsync);

        }
        else
            return;
    }
    else
    {
        for(int i=0; i < comboBoxes.size(); ++i)
        {
            if(comboBoxes[i]==comboBoxThatHasChanged)
            {
                bool okay(true);
                String identifier = comboBoxThatHasChanged->getName();
                std::string id(identifier.toUTF8());
                int v = comboBoxThatHasChanged->getSelectedItemIndex();
                if(parameterMap[id])
                {
                    String title("WARNING");
                    String message("Changing this parameter will clear your paintings. Continue?");
                    String button1Text("OK");
                    String button2Text("Cancel");
                    okay = AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
                                                  title,message,button1Text,button2Text,
                                                  this,nullptr);
                }
                
                if(okay)
                {
                    ISSEPluginEvent event(ISSEPluginEvent::Normal);
                    event.name = id;
                    event.value = v;
                
                    SeparationEngine::getInstance()->postEvent(event);
                }
                else
                    comboBoxThatHasChanged->setSelectedId(plugin->getParameter(id)+1, dontSendNotification);

                break;
            }
        }
    }
    didChange = true;
}


void PluginComponent::buttonClicked (Button* buttonThatWasClicked)
{
}



void PluginComponent::sliderValueChanged (Slider *slider)
{
    String identifier = slider->getName();
    std::string id = std::string(identifier.toUTF8());
    bool okay(true);
    
    if(parameterMap[id])
    {
        String title("WARNING");
        String message("Changing this parameter will clear your paintings. Continue?");
        String button1Text("OK");
        String button2Text("Cancel");
        okay = AlertWindow::showOkCancelBox (AlertWindow::WarningIcon,
                                             title,message,button1Text,button2Text,
                                             this,nullptr);
    }
    
    if(okay)
    {
        ISSEPluginEvent event(ISSEPluginEvent::Normal);
        event.name = id;
        event.value = slider->getValue();;
        SeparationEngine::getInstance()->postEvent(event);
    }
    else
        slider->setValue(plugin->getParameter(id)+1, dontSendNotification);
}

void PluginComponent::constructGUI()
{
    int leftAdjust(200);
    int oldHeight = height;
    int oldWidth = maxWidth;
    
    height = 100;
    maxWidth = 100;
    
    Colour textColour(ProgramSettings::textColour);
    Colour backgroundColour(ProgramSettings::mainBackgroundColor);
    int labelWidth(90);
    
    
    addAndMakeVisible (nameLabel = new Label (L"name label",
                                              L"Name:"));
    nameLabel->setFont (Font (15.0000f));
    nameLabel->setJustificationType (Justification::centredLeft);
    nameLabel->setEditable (false, false, false);
    nameLabel->setColour (Label::textColourId, textColour);
    nameLabel->setColour (Label::backgroundColourId, backgroundColour);
    nameLabel->setBounds (8, 8, labelWidth, 24);
    
    addAndMakeVisible (descLabel = new Label (L"desc label",
                                              L"Description:"));
    descLabel->setFont (Font (15.0000f));
    descLabel->setJustificationType (Justification::centredLeft);
    descLabel->setEditable (false, false, false);
    descLabel->setColour (Label::textColourId, textColour);
    descLabel->setColour (Label::backgroundColourId, backgroundColour);
    descLabel->setBounds (8, 33, labelWidth, 24);
    
    addAndMakeVisible (makerLabel = new Label (L"maker label",
                                               L"Company:"));
    makerLabel->setFont (Font (15.0000f));
    makerLabel->setJustificationType (Justification::centredLeft);
    makerLabel->setEditable (false, false, false);
    makerLabel->setColour (Label::textColourId, textColour);
    makerLabel->setColour (Label::backgroundColourId, backgroundColour);
    makerLabel->setBounds (8, 60, labelWidth, 24);
    
    addAndMakeVisible (copyrightLabel = new Label (L"copyright label",
                                                   L"Copyright:"));
    copyrightLabel->setFont (Font (15.0000f));
    copyrightLabel->setJustificationType (Justification::centredLeft);
    copyrightLabel->setEditable (false, false, false);
    copyrightLabel->setColour (Label::backgroundColourId, backgroundColour);
    copyrightLabel->setColour (Label::textColourId, textColour);
    copyrightLabel->setBounds (8, 88, labelWidth, 24);
    
    
    addAndMakeVisible (versionLabel = new Label (L"version label",
                                                   L"Version:"));
    
    versionLabel->setFont (Font (15.0000f));
    versionLabel->setJustificationType (Justification::centredLeft);
    versionLabel->setEditable (false, false, false);
    versionLabel->setColour (Label::backgroundColourId, backgroundColour);
    versionLabel->setColour (Label::textColourId, textColour);
    versionLabel->setBounds (8, copyrightLabel->getBottom(), labelWidth, 24);
    
    addAndMakeVisible (versionLabel2 = new Label (L"version label2",
                                                 L"1"));
    versionLabel2->setFont (Font (15.0000f));
    versionLabel2->setJustificationType (Justification::centredLeft);
    versionLabel2->setEditable (false, false, false);
    versionLabel2->setColour (Label::backgroundColourId, backgroundColour);
    versionLabel2->setColour (Label::textColourId, textColour);
    versionLabel2->setBounds (leftAdjust, copyrightLabel->getBottom(), labelWidth, 24);
    versionLabel2->setText(String(plugin->getVersion().c_str()),dontSendNotification);
    
 
    addAndMakeVisible (programsComboBox = new ComboBox (L"programs combo box"));
    programsComboBox->setEditableText (false);
    programsComboBox->setJustificationType (Justification::centredLeft);
    programsComboBox->setTextWhenNothingSelected (String::empty);
    programsComboBox->setTextWhenNoChoicesAvailable (L"(no choices)");
    programsComboBox->addListener (this);

    addAndMakeVisible (nameLabel2 = new Label (L"name label 2", L""));
    nameLabel2->setFont (Font (15.0000f));
    nameLabel2->setJustificationType (Justification::centredLeft);
    nameLabel2->setEditable (false, false, false);
    nameLabel2->setColour (Label::textColourId, textColour);
    nameLabel2->setColour (Label::backgroundColourId, backgroundColour);
    nameLabel2->setText(String(plugin->getName().c_str()),dontSendNotification);

    addAndMakeVisible (makerLabel2 = new Label (L"maker label 2", L""));
    makerLabel2->setFont (Font (15.0000f));
    makerLabel2->setJustificationType (Justification::centredLeft);
    makerLabel2->setEditable (false, false, false);
    makerLabel2->setColour (Label::textColourId, textColour);
    makerLabel2->setColour (Label::backgroundColourId, backgroundColour);
    makerLabel2->setText(String(plugin->getMaker().c_str()),dontSendNotification);

    addAndMakeVisible (descLabel2 = new Label (L"desc label 2", L""));
    descLabel2->setFont (Font (15.0000f));
    descLabel2->setJustificationType (Justification::centredLeft);
    descLabel2->setEditable (false, false, false);
    descLabel2->setColour (Label::textColourId, textColour);
    descLabel2->setColour (Label::backgroundColourId, backgroundColour);
    descLabel2->setText(String(plugin->getDescription().c_str()),dontSendNotification);


    addAndMakeVisible (copyrightLabel2 = new Label (L"copyright label 2", L""));
    copyrightLabel2->setFont (Font (15.0000f));
    copyrightLabel2->setJustificationType (Justification::centredLeft);
    copyrightLabel2->setEditable (false, false, false);
    copyrightLabel2->setColour (Label::textColourId, textColour);
    copyrightLabel2->setColour (Label::backgroundColourId, backgroundColour);
    copyrightLabel2->setText(String(plugin->getCopyright().c_str()),dontSendNotification);

    addAndMakeVisible (programsLabel = new Label (L"programs label 2",
                                                  L"Programs:"));
    programsLabel->setFont (Font (15.0000f));
    programsLabel->setJustificationType (Justification::centredLeft);
    programsLabel->setEditable (false, false, false);
    programsLabel->setColour (Label::textColourId, textColour);
    programsLabel->setColour (Label::backgroundColourId, backgroundColour);

    
    Rectangle<int> b = nameLabel->getBounds();
    int w = jmax( nameLabel2->getFont().getStringWidth(nameLabel2->getText()), 100 );
    nameLabel2->setBounds (leftAdjust, b.getY(), w, b.getHeight());
    updateMaxWidth(w + nameLabel2->getX() );

    b = makerLabel->getBounds();
    w = jmax( makerLabel2->getFont().getStringWidth(makerLabel2->getText()), 100);
    makerLabel2->setBounds(leftAdjust, b.getY(), w, b.getHeight());
    updateMaxWidth(w + makerLabel2->getX() );

    b = descLabel->getBounds();
    w = jmax( descLabel2->getFont().getStringWidth(descLabel2->getText()), 100);
    descLabel2->setBounds(leftAdjust, b.getY(), w, b.getHeight());
    updateMaxWidth(w + descLabel2->getX() );

    b = copyrightLabel->getBounds();
    w = jmax( copyrightLabel2->getFont().getStringWidth(copyrightLabel2->getText()), 100 );
    copyrightLabel2->setBounds(leftAdjust, b.getY(), w, b.getHeight());
    updateMaxWidth(w + copyrightLabel2->getX() );

    labels.clear();
    sliders.clear();
    comboBoxes.clear();
    programsComboBox->clear();


    Point<int> p(8, versionLabel->getBottom()+5); // IO label hieght

    programsLabel->setBounds(p.getX(), p.getY(), 100, 25);
    updateMaxWidth(programsLabel->getX() + programsLabel->getWidth());

    programsComboBox->setBounds(p.getX()+leftAdjust, p.getY()+5, 250, 25);
    updateMaxWidth(programsComboBox->getX() + programsComboBox->getWidth());
    p.setY(p.getY()+30);// Update Y
    

    
    p.setY(programsComboBox->getBottom()+10);
    
    
    
    parameterList = plugin->getParameterDescriptors();

    if(parameterList.size()==0)
    {
        optionalGroup->setVisible(true);

    }
    
    int optParamStartY = p.getY();
    p.setY(p.getY()+20);
    parameterMap.clear();
    for(ParameterList::iterator it = parameterList.begin();
        it!=parameterList.end();
        it++)
    {
        ParameterDescriptor d = *it;
        parameterMap[d.identifier] = d.causesReInit;
        Label * name = new Label(d.name.c_str(), d.name.c_str());
        name->setBounds(p.getX(), p.getY(), 250, 25);
        name->setColour (Label::textColourId, textColour);
        name->setColour (Label::backgroundColourId, backgroundColour);
        labels.add( name );
        addAndMakeVisible(name);
        updateMaxWidth(name->getWidth() + name->getX());

        // Slider
        if( !d.isQuantized || (d.isQuantized && (d.valueNames.size()==0)) )
        {
            Slider * slider = new Slider(d.identifier.c_str());
            slider->addListener(this);
            slider->setBounds(p.getX()+leftAdjust, p.getY(), 250, 25);
            slider->setSliderStyle(Slider::Rotary);
            slider->setColour( Slider::rotarySliderOutlineColourId, Colours::white);
            slider->setColour( Slider::rotarySliderFillColourId, Colours::white);

            slider->setTextValueSuffix(" " + String(d.unit.c_str()) );
            slider->setTextBoxStyle (Slider::TextBoxLeft, false, 200, name->getHeight() );

            float MIN_QUANTIZE = .000001;
            if( d.quantizeStep < MIN_QUANTIZE)
                d.quantizeStep = 0;

            slider->setRange(d.minValue, d.maxValue, d.quantizeStep );

            slider->setValue(plugin->getParameter(d.identifier), dontSendNotification);
            sliders.add(slider);
            addAndMakeVisible(slider);
            updateMaxWidth(slider->getX() + slider->getWidth());
        }
        else // Combo box
        {
            ComboBox * box = new ComboBox(d.identifier.c_str());
            box->addListener(this);
            for(int itemID = 0; itemID < d.valueNames.size(); itemID++)
            {
                box->addItem( String(d.valueNames[itemID].c_str()) + String(" ") + d.unit.c_str() , itemID+1);
            }
            box->setBounds(p.getX()+leftAdjust, p.getY(), 200, 25);
            box->setSelectedId(plugin->getParameter(d.identifier)+1, dontSendNotification);
            comboBoxes.add(box);
            addAndMakeVisible(box);
            updateMaxWidth(box->getX() + box->getWidth());
            
            
                        
            
        }

        // update the y
        p.setY(p.getY()+30);

    }

    optionalGroup->setBounds(0, optParamStartY, p.getX()+leftAdjust+260, p.getY()+10 - optParamStartY);

    height = p.getY() + 40;
 
    programList = plugin->getPrograms();
    int i=1;
    for (ProgramList::iterator it = programList.begin();
         it!=programList.end();
         it++, i++)
    {
        std::string program = *it;
        programsComboBox->addItem( String(program.c_str())+ " ", i);
    }
    // disable program combo box if there's no programs
    if(programList.size()==0)
    {
        programsComboBox->setEnabled(false);
    }
    else
    {
        programsComboBox->setEnabled(true);
        std::string prog = plugin->getCurrentProgram();

        i=1;
        for (ProgramList::iterator it = programList.begin();
             it!=programList.end();
             it++, i++)
        {
            std::string program = *it;
            if(program.compare(prog)==0)
            {
                programsComboBox->setSelectedId(i, sendNotificationAsync);
                break;
            }
        }
    }

    if( (oldHeight!= height) || (oldWidth!=maxWidth) )
    {
 
        setSize (maxWidth, height);

    }
    resized();

}
 
