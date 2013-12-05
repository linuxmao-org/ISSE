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
#include "SeparationEngine.h"

#include "AudioSink.h"
#include "AudioSource.h"

#include "api/ISSMask.h"
#include "Track.h"
#include "Main.h"

 
#include "ProgramSettings.h"
#include "PluginComponent.h"


#include "api/ISSMask.h"
#include "PluginThread.h"
#include "api/ISSEngine.h"

#include "ClearMaskAction.h"




juce_ImplementSingleton (SeparationEngine)

SeparationEngine::SeparationEngine() :
                    numTracks(3),
                    numSources(2),
                    progress(0),
                    needsTraining(false),
                    shouldProcess(false),
                    pluginComponent(nullptr)
{
    // create and launch separation engine
    plugin = new ISSEngine();
    
    for(int i=0; i < numSources; ++i)
        masks.set(i,new ISSMask(10,10));
    


}

SeparationEngine::~SeparationEngine()
{    
    if(processThread)
    {
        processThread->signalThreadShouldExit ();
        processThread->stopThread(1000);
    }
    
    clearSingletonInstance();
}


double & SeparationEngine::getProgress()
{
    return progress;
}


void SeparationEngine::updateWithNewFiles( const Array<File> & files )
{
    
    const MessageManagerLock mml (Thread::getCurrentThread());
    if (! mml.lockWasGained()) return;
    for(int i=0; i < files.size(); ++i)
        listeners.call (&SeparationEngine::Listener::engineNewOutputs, files[i], i);
    
    if(!shouldProcess)
        ProgramSettings::processing = false;
    
    
}

void SeparationEngine::addListener( SeparationEngine::Listener * listener )
{
    const MessageManagerLock mml (Thread::getCurrentThread());
    if (! mml.lockWasGained()) return;

    listeners.add(listener);

}

void SeparationEngine::removeListener( SeparationEngine::Listener * listener)
{
    const MessageManagerLock mml (Thread::getCurrentThread());
    if (! mml.lockWasGained()) return;
    
    listeners.remove(listener);
}


std::map<size_t, size_t> SeparationEngine::getTraining()
{
    return trainingIndsToSource;
}

//void SeparationEngine::setTraining(std::map<unsigned long, unsigned long> & trainingIndsToSource_)
//{
//    trainingIndsToSource = trainingIndsToSource_;
//    
//    needsTraining = true;
//    
//    listeners.call (&SeparationEngine::Listener::engineTrainingChanged);
//    postEvent(ISSEPluginEvent::Training);
//}


void SeparationEngine::addTraining(unsigned int source, unsigned int xmin, unsigned int xmax)
{
   
    for(size_t i=xmin; i <= xmax; ++i)
        trainingIndsToSource[i] = source+1;
    
    needsTraining = true;
    
    listeners.call (&SeparationEngine::Listener::engineTrainingChanged);
    postEvent(ISSEPluginEvent::Training);
}

void SeparationEngine::subtractTraining(unsigned int source, unsigned int xmin, unsigned int xmax)
{
    std::map<size_t, size_t>::iterator iter;
    for(size_t i=xmin; i <= xmax; ++i)
    {
        iter = trainingIndsToSource.find(i);
        if(iter!=trainingIndsToSource.end())
            trainingIndsToSource.erase(iter);
    } 
    needsTraining = true;
    
    listeners.call (&SeparationEngine::Listener::engineTrainingChanged);
    postEvent(ISSEPluginEvent::Training);
}

ISSMask * SeparationEngine::getMask( unsigned int source )
{
    if(source >= masks.size())
        return NULL;
    else
        return masks[source];
}
  

void SeparationEngine::shutdown()
{
    if(processThread)
    {
        processThread->stop();
        processThread->threadShouldExit();
    }
}

bool SeparationEngine::saveToXML( XmlElement & xml, const File & folderForAssets )
{
    
    XmlElement * engineXML = new XmlElement ("engine");
      
    // Save internal mask layer
    for(int i=0; i < masks.size(); ++i)
    {
        XmlElement * maskXML = new XmlElement ("mask");
        
        String maskFileName =  String(i) + ".Mask";
        maskXML->setAttribute("filepath", maskFileName);
        maskXML->setAttribute("source", i);
        
        File f(folderForAssets.getFullPathName() + "/" + maskFileName);
        masks[i]->saveAsBinary(f);
        engineXML->addChildElement(maskXML);
    }
    
    // Add training
    XmlElement * trainingXML = new XmlElement("training");
    
    std::stringstream ss;
    std::map<size_t,size_t>::iterator iter = trainingIndsToSource.begin();
    for( ; iter!=trainingIndsToSource.end(); iter++)
    {
        if(iter->second > 0)
            ss << iter->first  << "," << iter->second << ",";
    }
    
    String txt(ss.str().c_str());
    trainingXML->addTextElement(txt);
    
 
    
    engineXML->addChildElement(trainingXML);
    
    xml.addChildElement(engineXML);
    
    
    processThread->save( folderForAssets.getFullPathName() + "/plugin.settings" );
    

    return true;
}

bool SeparationEngine::loadFromXML( const XmlElement & xml, const File & assetsFolder  )
{
    reset();
    
//    for(int i=0; i < masks.size(); ++i)
//        masks[i]->resize(0, 0);
    
    XmlElement * engineXML(nullptr);
    forEachXmlChildElement (xml, e)
    {
        if (e->hasTagName ("engine"))
        {
            engineXML = e;
            break;
        }
    }
    if(!engineXML) return false;
    
//    forEachXmlChildElement (*engineXML, e)
//    {
//        if (e->hasTagName ("mask"))
//        {
//            int source = e->getIntAttribute ("source");
//            String filename  = e->getStringAttribute ("filepath");
//            File maskfile = assetsFolder.getFullPathName() + "/" + filename ;
//            
//            if(source < 0)
//                return false;
//            if(source >= numSources)
//                return false;
//            masks[source]->loadFromBinary(maskfile);
//            
//            
//        }
//    }
    
    needsTraining = false;
    forEachXmlChildElement(*engineXML, e)
    {
        if(e->hasTagName("training"))
        {
            String txt(e->getAllSubText());
            std::string str(txt.toUTF8());
            std::stringstream ss(str);
            
           
            int first(0);
            int second(0);
            while (ss >> first)
            {
                if (ss.peek() == ',')
                    ss.ignore();
                ss >> second;
                    

                if (ss.peek() == ',')
                    ss.ignore();

                trainingIndsToSource[first] = second;
                needsTraining = true;
            }
        }
    }
    
    
    int trackNum = 0;
    File loadFile;
    forEachXmlChildElement (xml, e)
    {
        if (e->hasTagName ("TRACK"))
        {
            // Setup and launch the separation engine
            if(trackNum==0)
            {
                loadFile = File(assetsFolder.getFullPathName() + "/" + e->getStringAttribute ("filename"));
                
                break;
            }
        }
    }
    
    return setupAndLaunchEngine(loadFile, xml, assetsFolder, true, StandardMode);
}


void SeparationEngine::reset()
{
    if(processThread)
	{
        processThread->stop();
		processThread->signalThreadShouldExit();
		processThread->stopThread(1000);
		 
	}
	processThread = nullptr;

    clearTraining();
    
    needsTraining = false;
    
    progress = 0;
}

void SeparationEngine::clearMasks()
{
    Array<ISSMask*> m;
    
    for(int i=0; i < masks.size(); ++i)
        m.add(masks[i]);
    
    ClearMaskAction * action = new ClearMaskAction(m);
    
    MAIN_APP->undoManager.perform(action, "Clear Masks");
    MAIN_APP->undoManager.sendChangeMessage();
}



void SeparationEngine::clearTraining()
{
    const MessageManagerLock mml (Thread::getCurrentThread());
    if (! mml.lockWasGained()) return;
    
    trainingIndsToSource.clear();
    needsTraining = true;
    listeners.call (&SeparationEngine::Listener::engineTrainingChanged);
    postEvent(ISSEPluginEvent::Training);
}

void SeparationEngine::start()
{
    shouldProcess = true;
    
    if(processThread)
        processThread->start();
    
    
    ProgramSettings::processing = true;
     
}

void SeparationEngine::postEvent(ISSEPluginEvent event)
{
    if(processThread)
        processThread->postEvent(event);
}

void SeparationEngine::stop()
{
    
    
    if(processThread)
        processThread->stop();
    
    shouldProcess = false;
    
    if(processThread && !processThread->middleOfProcessing())
        ProgramSettings::processing = false;
    
    
}

Component * SeparationEngine::createPluginGUI()
{
    if(pluginComponent)
        removeListener(pluginComponent);
    
    
    pluginComponent = new PluginComponent(plugin);
    addListener(pluginComponent);
    return pluginComponent;
}

void SeparationEngine::setupAndLaunchEngine(File inputFile_, PluginType pluginName )
{
    const XmlElement xml("trash");
    setupAndLaunchEngine(inputFile_, xml, File(), false, pluginName);
}

 
void SeparationEngine::notifyEngineListeners( SeparationEngine::Listener::Msg msg )
{
    // Lock
    const MessageManagerLock mml (Thread::getCurrentThread());
    if (! mml.lockWasGained()) return;
    
    
    
    listeners.call (&SeparationEngine::Listener::engineNewPlugin, plugin, msg );
}

 

bool SeparationEngine::setupAndLaunchEngine( File inputFile,
                                             const XmlElement & xml,
                                             const File & assetsFolder,
                                             bool fromxml,
                                             PluginType pluginName )
{
    if(!fromxml)
    reset();
    
    try
    {
        if(!inputFile.exists())
            return false;
        
        if(processThread)
        {
            processThread->stop();
            processThread->signalThreadShouldExit();
            processThread->stopThread(1000);
            processThread->waitForThreadToExit(1000);
        }
        
        plugin = new ISSEngine();
        
        Array<ISSMask*> m;
        for(int i=0; i < masks.size(); ++i)
            m.add(masks[i]);
        
        
        processThread = new PluginThread( inputFile, m, plugin );
        if(fromxml)
        {
            bool okay = processThread->loadFromXML(xml, assetsFolder);
            if(!okay)
                return false;
        }
        
        processThread->startThread();
        
    }
    catch(ISSException & e)
    {
        return false;
    }
    
    return true;
}



