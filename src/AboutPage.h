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
#ifndef __ABOUTPAGE_H_66508A38__
#define __ABOUTPAGE_H_66508A38__

#include "JuceHeader.h"


class AboutWebBrowserComponent : public WebBrowserComponent
{
public:
    
    class Listener
    {
    public:
        virtual void 	pageAboutToLoad (const String &newURL)=0;
        virtual void 	pageFinishedLoading (const String &url)=0;
        virtual ~Listener(){}
    };
    
    
public:
    
    AboutWebBrowserComponent()
    {
        
    }
    
    void addListener(AboutWebBrowserComponent::Listener * listener)
    {
        listeners.add(listener);
    }
    void removeListener(AboutWebBrowserComponent::Listener * listener)
    {
        listeners.remove(listener);
    }
    
    virtual void 	pageFinishedLoading (const String &url)
    {
        listeners.call(&AboutWebBrowserComponent::Listener::pageFinishedLoading, url);
    }
    
    virtual bool 	pageAboutToLoad (const String &newURL)
    {
        
        listeners.call(&AboutWebBrowserComponent::Listener::pageAboutToLoad, newURL);

        return true;
    }
private:
    
    ListenerList<AboutWebBrowserComponent::Listener> listeners;
    
};

//#define __UNIX1__


class AboutPage : public Component 
//#ifndef __UNIX1__
//		  ,public Button::Listener,
//		  public AboutWebBrowserComponent::Listener
//#endif
{
public:
  
    AboutPage();
    
//    #ifndef __UNIX1__
//    virtual void 	buttonClicked (Button *button);
//    virtual void	pageAboutToLoad (const String &newURL);
//    virtual void 	pageFinishedLoading (const String &url);
//    #endif
    void reload();
    
    virtual void resized();
    
  
private:
// #ifndef __UNIX1__
//    AboutWebBrowserComponent webbrowser;
//    ArrowButton backButton;
//    ArrowButton forwardButton;
//    String intialURL;
//#else 
    TextEditor textEditor;
//#endif
    
    
};



#endif  // __ABOUTPAGE_H_66508A38__
