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
#include "VirtualTestUser.h"
#include "AppCommandIDs.h"

#ifdef __WINDOWS__
  #include <windows.h>
  #include <process.h>
#elif __UNIX__
  #include <unistd.h>
#endif
 

VirtualTestUser::VirtualTestUser( VirtualUserHost * host_, File file_ )
                                :  host(host_), file(file_)
{
    
}

VirtualTestUser::~VirtualTestUser()
{}

void VirtualTestUser::stop()
{
    shouldStop = true;
    stopThread	(	1000 );
}

bool VirtualTestUser::shouldSkipMessage(XmlElement & xml)
{
//    // TODO: Skip certain messages
//    // Play, Pause, Rewind, FastForward, Next, Previous, Process, ChangeVolume
//    int commandID = AppCommandIDs::commandToID[ xml.getStringAttribute ("type") ];
//    
//    bool skip(false);
//    if(commandID==AppCommandIDs::Play ||
//       commandID==AppCommandIDs::Pause ||
//       commandID==AppCommandIDs::Rewind ||
//       commandID==AppCommandIDs::FastForward ||
//       commandID==AppCommandIDs::Next ||
//       commandID==AppCommandIDs::Previous ||
//       commandID==AppCommandIDs::Process ||
//       commandID==AppCommandIDs::ChangeVolume
//       )
//        skip = true;
//    
//    return skip;
    return false;
}

void VirtualTestUser::run()
{
    if(file.exists())
    {
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
                    return;

                if(shouldSkipMessage(*e))
                    continue;
                #if defined __WINDOWS__
					Sleep( 50 );
				#else
					usleep(50*1000);
				#endif
               
                 
                int64 timediffInMs = 1;//e->getStringAttribute("time").getLargeIntValue ();
                if(numEvents<1)
                {
                    const MessageManagerLock mml (Thread::getCurrentThread());
                    if (! mml.lockWasGained()) break;
                    host->routeGUIMessage(*e);
                }
                else
                {
                    int64 numIntervals = floor(timediffInMs / 10.0);
                    int64 remaining = timediffInMs / 10.0 - numIntervals;
                    for(int i=0; i < numIntervals; ++i)
                    {
                    #if defined __WINDOWS__
					Sleep( 50 );
					#else
					usleep(50*1000);
					#endif



                        if(shouldStop)
                            return;
                    }
					#if defined __WINDOWS__
					Sleep( remaining );
					#else
					usleep(remaining*1000);
					#endif


                    if(shouldStop)
                        return;
                    {
                        const MessageManagerLock mml (Thread::getCurrentThread());
                        if (! mml.lockWasGained()) break;
                        host->routeGUIMessage(*e);
                    }
                }
                
                // TODO: evaluate separation results
                // 1) Turn processing on
                // 2) Wait until convergence
                // 3) Compute SDR, SIR, and SAR
                
                
                numEvents++;
            }
            
                      
            
        }
        
        XmlElement x("Event");
        x.setAttribute("time", 0);
        x.setAttribute("type", "Process");
        {
            const MessageManagerLock mml (Thread::getCurrentThread());
            if (! mml.lockWasGained()) return;
            host->routeGUIMessage(x);
        }

        
    }
}


