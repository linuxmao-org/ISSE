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
#ifndef __THREADEDFILELOGGER_H_753936B8__
#define __THREADEDFILELOGGER_H_753936B8__


#include "JuceHeader.h"


#define ISSTIME ( ISSLogger::getTimeDifference() )

class ISSLogger : public FileLogger
{
public:
    
    ISSLogger (const File &fileToWriteTo, const String &welcomeMessage);
    
    ~ISSLogger();
    
    // Logs with a time stamp
    void log(const String & message );
    
    static String getTimeDifference();

private:
    
    CriticalSection logLock;
    static int64 pastTime;
    
};




#endif  // __THREADEDFILELOGGER_H_753936B8__
