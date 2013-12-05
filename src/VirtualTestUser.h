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
#ifndef __VIRTUALTESTUSER_H_C5168A62__
#define __VIRTUALTESTUSER_H_C5168A62__

#include "VirtualUser.h"

class VirtualTestUser : public VirtualUser
{
public:
    
    VirtualTestUser(VirtualUserHost * host, File file);
    virtual ~VirtualTestUser();
    virtual void run();
    virtual void stop();
    
    bool shouldSkipMessage(XmlElement & xml);
    
private:
    
    VirtualUserHost * host;
    File file;
    bool shouldStop;
    
};



#endif  // __VIRTUALTESTUSER_H_C5168A62__
