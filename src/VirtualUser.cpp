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
#include "VirtualUser.h"

#if defined __WINDOWS__
  #include <windows.h>
  #include <process.h>
#else
#include <chrono>
#include <thread>
#endif

 



VirtualRealTimeUser::VirtualRealTimeUser( VirtualUserHost * host_, File file_ ) :  host(host_), file(file_), shouldStop(false), on(false)
{}


VirtualRealTimeUser::~VirtualRealTimeUser()
{}

void VirtualRealTimeUser::stop()
{
    shouldStop = true;
    stopThread	(	1000 );
}

bool VirtualRealTimeUser::isOn()
{
    return on;
}

void VirtualRealTimeUser::run()
{    
    if(file.exists())
    {
        on = true;
        
        // Stop the current engine
        XmlDocument doc(file);
        
        ScopedPointer<XmlElement> xml(doc.getDocumentElement());
        if(!xml)
            return;
        
        int numEvents(0);      
        forEachXmlChildElement (*xml, e)
        {
            if (e->hasTagName ("Event"))
            {
                if(shouldStop)
                {
                    on = false;
                    return;
                }
                
                double timediffInMs = e->getStringAttribute("time").getDoubleValue ();
                if(numEvents < 1)
                {
                    const MessageManagerLock mml (Thread::getCurrentThread());
                    if (! mml.lockWasGained()) break;
                    
                    std::cout << "Routing Event Message" << std::endl;
                    host->routeGUIMessage(*e);
                }
                else
                {
                    double cumWait(0);
                    while( cumWait < timediffInMs-1 )
                    {
                   

				#if defined __WINDOWS__
					Sleep( 10 );
				#else
					usleep(1000);
				#endif


                        if(shouldStop)
                        {
                            on = false;
                            return;
                        }
                        cumWait += 1;
                    }
                    {
                        const MessageManagerLock mml (Thread::getCurrentThread());
                        if (! mml.lockWasGained()) break;
                            host->routeGUIMessage(*e);
                    }
                }
                numEvents++;
            }
        }
        
        on = false;
        
    }
}


