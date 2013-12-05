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
#include "AboutPage.h"
#include "ProgramSettings.h"
#include "JuceHeader.h"


AboutPage::AboutPage() 
//#ifndef __UNIX1__
//: backButton("Back",.5, ProgramSettings::textColour ),
//forwardButton("Forward",0.0, ProgramSettings::textColour )
//#endif
{
    setSize(800, 650);
    
    
//#ifdef __UNIX1__
    
    using namespace BinaryData;
    int aboutembeddedSize(0);
    const char* aboutembedded_html = getNamedResource ( "about_txt", aboutembeddedSize);
    textEditor.setReadOnly (true);
    textEditor.setBounds(getBounds());
    textEditor.setMultiLine(true);
    textEditor.setColour(TextEditor::textColourId, ProgramSettings::textColour);
    textEditor.setColour(TextEditor::backgroundColourId, ProgramSettings::mainBackgroundColor);
    
    
    
    textEditor.setText(String(aboutembedded_html) + String("\nVersion: ") + String(ProjectInfo::versionString));
    
    
    
    
    
    addAndMakeVisible(&textEditor);
    
    
//#else
//    reload();
//    
//    webbrowser.setBounds(0,20,getWidth(), getHeight());
//    
//    webbrowser.addListener(this);
//    addAndMakeVisible(&webbrowser);
//
//    backButton.setBounds(0, 0, 20, 20);
//    backButton.addListener(this);
//    forwardButton.setBounds(getWidth()-20, 0, 20, 20);
//    forwardButton.addListener(this);
//
//    addAndMakeVisible(&backButton);
//    addAndMakeVisible(&forwardButton);
//#endif
}

void AboutPage::reload()
{
//#ifndef __UNIX1__
//    File tempDir(File::getSpecialLocation(File::tempDirectory));
//    
//    using namespace BinaryData;
//    
//    int aboutembeddedSize(0);
//    const char* aboutembedded_html = getNamedResource ( "about_html", aboutembeddedSize);
//    File file1(tempDir.getFullPathName() + "/about.html");
//    file1.deleteFile();
//    file1.create();
//    FileOutputStream outputstream1(file1);
//    outputstream1.write(aboutembedded_html, aboutembeddedSize);
//    outputstream1.flush();
//    
//    int style_cssSize(0);
//    const char*   style_css = getNamedResource ( "style_css", style_cssSize);
//    File file2(tempDir.getFullPathName() + "/style.css");
//    file2.deleteFile();
//    file2.create();
//    FileOutputStream outputstream2(file2);
//    outputstream2.write(style_css, style_cssSize);
//    outputstream2.flush();
//    
//    
//    int COPYINGSize(0);
//    const char*   COPYING = getNamedResource ( "COPYING", COPYINGSize);
//    File file3(tempDir.getFullPathName() + "/LICENSE.txt");
//    file3.deleteFile();
//    file3.create();
//    FileOutputStream outputstream3(file3);
//    outputstream3.write(COPYING, COPYINGSize);
//    outputstream3.flush();
//    
//    webbrowser.goToURL(file1.getFullPathName());
//#endif
    
}


void AboutPage::resized()
{
//#ifndef __UNIX1__
//    int w(getWidth());
//    int h(getHeight());
//    webbrowser.setBounds(0, 20, w, h-20);
//    backButton.setBounds(0, 0, 20, 20);
//    forwardButton.setBounds(w-20, 0, 20, 20);
//#else
    Rectangle<int> b(getBounds());
    textEditor.setBounds(b.getX(), 0, getWidth(), getHeight());
//#endif

}



//#ifndef __UNIX1__
//void 	AboutPage::pageFinishedLoading (const String &url)
//{
//
//    webbrowser.repaint();
// 
//}
//
//void AboutPage::pageAboutToLoad (const String &newURL)
//{
////    std::cout << newURL << std::endl;
//}
//
//
//void AboutPage::buttonClicked (Button *button)
//{
//  
//    if(button==&backButton)
//    {
//        webbrowser.goBack();
//    }
//    else if(button==&forwardButton)
//    {
//        webbrowser.goForward();
//    }
//
//    
//}
//
//#endif





