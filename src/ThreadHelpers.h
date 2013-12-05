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
#ifndef __THREADHELPERS_H_CF8C715__
#define __THREADHELPERS_H_CF8C715__

#include "JuceHeader.h"



class XMLSerializable;
class MainContentComp;

class SaveThread  : public ThreadWithProgressWindow
{
public:
    SaveThread( XMLSerializable * comp_, const File & file_, const File & logFile_ );
    void run();
    
private:
    XMLSerializable * comp;
    const File & file;
    const File & logFile;
    
};

class LoadThread  : public ThreadWithProgressWindow
{
public:
    LoadThread( XMLSerializable * comp_, const File & file_);
    
    void run();
    
private:
    XMLSerializable * comp;
    const File & file;
    
};



class CreateNewProjectThread  : public ThreadWithProgressWindow
{
public:
    
    CreateNewProjectThread( MainContentComp * comp_, const File & file_);
    
    void run();
    
private:
    MainContentComp * comp;
    const File & file;
};





#endif  // __THREADHELPERS_H_CF8C715__
