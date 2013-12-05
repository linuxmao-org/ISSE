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
#include "ThreadHelpers.h"
#include "XMLSerializable.h"
#include "Main.h"
#include "MainContentComp.h"
#include "ISSLogger.h"

 
SaveThread::SaveThread( XMLSerializable * comp_, const File & file_, const File & logFile_ )
    : ThreadWithProgressWindow ("Saving...", false, false), comp(comp_), file(file_), logFile(logFile_)
    {
        
    }

void SaveThread::run()
    {
        
        const MessageManagerLock mml (Thread::getCurrentThread());
        if (! mml.lockWasGained()) return;
        
        XmlElement xmldoc("PROJECT");
        
        File assetFolder( file.getParentDirectory().getFullPathName ()
                         + "/" +  file.getFileNameWithoutExtension() + "-assets");
        
//        assetFolder.deleteRecursively();
        
        Result 	res = assetFolder.createDirectory ();
        xmldoc.setAttribute( "assetfolder", file.getFileNameWithoutExtension() + "-assets" );
        
        // serialize the document state
        comp->saveToXML(xmldoc, assetFolder);
        
        // Write to a file
        bool okay =	xmldoc.writeToFile (file, "");
        
        xmldoc.deleteAllChildElements ();
        
        if(okay)
            MAIN_APP->undoManager.clearUndoHistory ();
}

LoadThread::LoadThread( XMLSerializable * comp_, const File & file_)
:   ThreadWithProgressWindow ("Loading...", false, false),
comp(comp_),
file(file_)
{}

void LoadThread::run()
{
    
    
    // Stop the current engine
    XmlDocument doc(file);
    
    XmlElement * elem = doc.getDocumentElement();
    
    String assetFolder = elem->getStringAttribute("assetfolder");
    
    assetFolder = file.getParentDirectory().getFullPathName() + "/" + assetFolder;
    
    comp->loadFromXML(*elem, assetFolder);
    
    // Delete the xml object after loading
    deleteAndZero(elem);
    
//    MAIN_APP->logger = new ISSLogger(File::getSpecialLocation( File::tempDirectory ).getFullPathName() + "/log.xml", "LOG");
}


 
    
    CreateNewProjectThread::CreateNewProjectThread( MainContentComp * comp_, const File & file_)
    : ThreadWithProgressWindow ("loading...", false, false), comp(comp_), file(file_)
    {}
    
void CreateNewProjectThread::run()
    {
        const MessageManagerLock mml (Thread::getCurrentThread());
        if (! mml.lockWasGained()) return;
        comp->createNewProject(file, false);
    }
    
 
