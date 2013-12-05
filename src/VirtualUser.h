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
#ifndef __VIRTUALUSER_H_3B075CB0__
#define __VIRTUALUSER_H_3B075CB0__

#include "JuceHeader.h"
#ifdef __UNIX__
  #include <unistd.h>
#endif


class MainContentComp;

class VirtualUserHost
{
public:
    virtual void routeGUIMessage( const XmlElement & xml )=0;
    virtual ~VirtualUserHost(){}

};
class VirtualUser : public Thread
{
public:
    VirtualUser() : Thread(""){}
    
    virtual ~VirtualUser(){};
    virtual void run()=0;
    virtual void stop()=0;
    virtual bool isOn()=0;
    
private:
    
    VirtualUserHost * host;
    File file;
    bool shouldStop;
    
};





class VirtualRealTimeUser : public VirtualUser
{
public:
    
    VirtualRealTimeUser(VirtualUserHost * host, File file);
    virtual ~VirtualRealTimeUser();
    virtual void run();
    virtual void stop();
    virtual bool isOn();
 
private:
    
    VirtualUserHost * host;
    File file;
    bool shouldStop;
    bool on;
    
};



#endif  // __VIRTUALUSER_H_3B075CB0__
