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

#include "PluginThread.h"

#include "SeparationEngine.h"

#include "AudioSink.h"
#include "AudioSource.h"

#include "api/ISSMask.h"
#include "Track.h"
#include "Main.h"

#include "ProgramSettings.h"

#include "api/ISSMask.h"
#include "AudioSink.h"

#ifdef __WINDOWS__
  #include <windows.h>
  #include <process.h>
#elif __UNIX__
  #include <unistd.h>
#endif
 


PluginThread::PluginThread( File inputFile,
                            Array<ISSMask*> m,
                            ISSPluginBase * plugin_)
    :   Thread( "" ),
        plugin(plugin_),
        needsTraining(false),
        shouldProcess(false),
        middleProcessing(false),
        progress(0),
        numSources(m.size())
{
    
    issAudioSinks.clear();
    tempFiles.clear();
    masks.clear();
    matrices.clear();
    for(int s=0; s < numSources; s++)
    {
        matrices.push_back( new Matrix(0, 0) );
        mMap.push_back(matrices[s]);
        masks.add(m[s]);
        tempFiles.add(File());
        issAudioSinks.add(NULL);
        masks[s]->dirty();
    }
    
    issAudioSource = new FileAudioSource( inputFile );
    
    // Generate outputs
    generateNewTempFiles();
    
    initPlugin();
 
}

bool PluginThread::loadFromXML( const XmlElement & xml, const File & assetsFolder )
{
    
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
    
    forEachXmlChildElement (*engineXML, e)
    {
        if (e->hasTagName ("mask"))
        {
            int source = e->getIntAttribute ("source");
            String filename  = e->getStringAttribute ("filepath");
            File maskfile = assetsFolder.getFullPathName() + "/" + filename ;
            
            if(source < 0)
                return false;
            if(source >= numSources)
                return false;
            masks[source]->loadFromBinary(maskfile);
            
            
        }
    }
    
    // check if it needs training
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
                
           
                needsTraining = true;
                break;
            }
        }
    }
    
    
    File pluginFile(assetsFolder.getFullPathName().toUTF8() + "/plugin.settings");
    
//    std::cout << pluginFile.getFullPathName() << std::endl;
    
    // Load xml and masks if needed
    if(pluginFile.existsAsFile())
    {
        // Load masks
        plugin->load(std::string(pluginFile.getFullPathName().toUTF8()));
        
        // Make dirty
        if( masks.size() > 0 )
            masks[0]->dirty();
        
   
    }
    
    
    SeparationEngine::getInstance()->notifyEngineListeners(SeparationEngine::Listener::PluginLoadedXML);
    
    return true;
    
}

void PluginThread::start()
{
    const ScopedLock myScopedLock (mutex);
    shouldProcess = true;
}

void PluginThread::stop()
{
    const ScopedLock myScopedLock (mutex);
    shouldProcess = false;
}

void PluginThread::generateNewTempFiles()
{
    static int count = 0;
    File tempDir = File::getSpecialLocation( File::tempDirectory );        
    for(int i=0; i < numSources; ++i)
        tempFiles.set(i, File(tempDir.getFullPathName() + "/" +
                        String(i+1) + "-temp" + String((count % 2)) + ".wav" ));
    
    count++;
    
    double fs(issAudioSource->getSampleRate());
    size_t lenInSamps(issAudioSource->getLengthInSamples());
    size_t numChan(issAudioSource->getNumChannels());
    
    sinks.clear();
    issAudioSinks.clear();
    for(int s=0; s < numSources; s++)
    {
        FileAudioSink * sink(new FileAudioSink  (tempFiles[s], fs, numChan, lenInSamps));
        issAudioSinks.set(s, sink);
        sinks.push_back( issAudioSinks[s] );
    }
}


void PluginThread::postEvent( ISSEPluginEvent event )
{
    const ScopedLock myScopedLock (mutex);
    eventQueue.push( event );
}

void PluginThread::newSeparationOutputs()
{
    // Close the file to finish the write
    for(int i=0; i < numSources; ++i)
        issAudioSinks[i]->close();
    
    // Notify listeners 
    if(!threadShouldExit())
        SeparationEngine::getInstance()->updateWithNewFiles(tempFiles);
    
    // Create new sinks
    generateNewTempFiles();

}

bool PluginThread::checkDirty()
{
    bool mDirty(false);
    for(int i=0; i < masks.size(); ++i)
    {
        bool isDirty = masks[i]->isDirty();
        if(isDirty)
            mDirty = true;
    
        // Copy
        (*matrices[i]) = masks[i]->getMask();
        (*matrices[i]) = matrices[i]->array() + 1;
        masks[i]->clearDirty();
        
    }
    
    if(needsTraining)
    {
        mDirty = true;
        needsTraining = false;
    }

    return mDirty;
}

PluginThread::~PluginThread()
{
    for(int i=0; i < masks.size(); ++i)
        delete matrices[i];
    matrices.clear();
}

bool PluginThread::save( String filepath )
{
    std::string f(filepath.toUTF8());
    return plugin->save(f);
}

void PluginThread::emptyQueue( )
{
    // Update all plugin parameter settings
    {
        const ScopedLock myScopedLock (mutex);
        
        ISSStftParams paramsOld = plugin->getSTFTParams();
        std::pair<size_t,size_t> maskSizeOld = plugin->getMaskSize();
        
        while(eventQueue.size()>0)
        {
            ISSEPluginEvent event = eventQueue.front();
            eventQueue.pop();
            if(event.type==ISSEPluginEvent::Training)
                needsTraining = true;
            else if(event.type==ISSEPluginEvent::ProgramChange)
                plugin->selectProgram(event.name);
            else
                plugin->setParameter(event.name, event.value);
            masks[0]->dirty();
        }
        
        ISSStftParams newOld = plugin->getSTFTParams();
        std::pair<size_t,size_t> maskSizeNew = plugin->getMaskSize();
     
        if( (newOld!=paramsOld) ||  maskSizeNew!=maskSizeOld )
        {
            std::pair<long,long>  maskSize(plugin->getMaskSize());
            for(int i=0; i < masks.size(); ++i)
                masks[i]->resize(maskSize.second, maskSize.first);
            
            SeparationEngine::getInstance()->notifyEngineListeners( SeparationEngine::Listener::NewMaskSTFTParams );
        }
        
    }
}

void PluginThread::initPlugin( )
{
    // Check to see if it needs reiniting
 
    plugin->init(issAudioSource->getSampleRate(),
                 issAudioSource->getLengthInSamples(),
                 issAudioSource->getNumChannels());
        
    {
        const MessageManagerLock mml (Thread::getCurrentThread());
        if (! mml.lockWasGained()) return;
        
        std::pair<long,long>  maskSize(plugin->getMaskSize());
        for(int i=0; i < masks.size(); ++i)
            masks[i]->resize(maskSize.second, maskSize.first);
            
        SeparationEngine::getInstance()->notifyEngineListeners(SeparationEngine::Listener::NewPluginLoaded);
    }
    

}

void PluginThread::processPlugin()
{
    {
        const ScopedLock myScopedLock (mutex);
        middleProcessing = true;
        
    }
    
    std::map<size_t,size_t> training = SeparationEngine::getInstance()->getTraining();
    double & progress = SeparationEngine::getInstance()->getProgress();
    bool didProcess(false);
    ISSPluginBase::ProcessType type(ISSPluginBase::NORMAL);
    if(needsTraining) type = ISSPluginBase::TRAIN;
    
    if( checkDirty() )
        didProcess = plugin->process( issAudioSource, mMap, sinks,
                                         training, progress, type );
        
    if(didProcess)
        newSeparationOutputs();
    
    {
        const ScopedLock myScopedLock (mutex);
        middleProcessing = false;
    }
}

void PluginThread::reInit()
{
  
//    if(shouldProcess)
    {
        // Check to see if it needs reiniting
        if(plugin->needsToInit())
        {
            plugin->init(issAudioSource->getSampleRate(),
                         issAudioSource->getLengthInSamples(),
                         issAudioSource->getNumChannels());
            
            // lock messenger thread
            {
                const MessageManagerLock mml (Thread::getCurrentThread());
                if (! mml.lockWasGained()) return;
                SeparationEngine::getInstance()->notifyEngineListeners(SeparationEngine::Listener::UpdateGUI);
            }
        }
    }
    
}

 


bool PluginThread::middleOfProcessing()
{
    const ScopedLock myScopedLock (mutex);
    
    bool proc(middleProcessing);
    
    
    return proc;
}

void PluginThread::run ()
{

    // loop forever
    while ( true )
    {
        if(threadShouldExit ())
        {
            shouldProcess = false;
            return;
        }
        
        // Write messages from gui to the internal plugin
        emptyQueue();
        
        if(shouldProcess)
        {
            // Check to see if it needs reiniting
            reInit();

            // process
            processPlugin();
        }
        
        #if defined __WINDOWS__
            Sleep(100); // milliseconds
        #else
            usleep(100*1000);
        #endif
        
        
    }
}











