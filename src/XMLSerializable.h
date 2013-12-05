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
#ifndef __XMLSERIALIZABLE_H_D9FBB9F0__
#define __XMLSERIALIZABLE_H_D9FBB9F0__


#include "JuceHeader.h"


class XMLSerializable
{
public:
    virtual bool saveToXML( XmlElement & xml, const File & folderForAssets )=0;
    virtual bool loadFromXML( const XmlElement & xml, const File & assetsFolder  )=0;
    virtual ~XMLSerializable(){}
};


class Action
{
public:
    
private:
};



#endif  // __XMLSERIALIZABLE_H_D9FBB9F0__
