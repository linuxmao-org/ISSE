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


#ifndef __PLUGINTHREAD_H_F5A774F8__
#define __PLUGINTHREAD_H_F5A774F8__


#include "JuceHeader.h"
#include "api/ISSEngine.h"
#include "api/ISSPluginBase.h"

#include <queue>


class ISSPluginBase;
class ISSMask;
class ISSEPluginWindow;
class FileAudioSink;


class PluginThread : public Thread
{
public:
    
    PluginThread(File inputFile,
                 Array<ISSMask*> m,
                 ISSPluginBase * plugin);
    
    ~PluginThread();
    
    void start();
    
    void stop();
    
    bool middleOfProcessing();
    
    bool save( String filepath );
    
    void postEvent( ISSEPluginEvent event );
    
    bool loadFromXML( const XmlElement & xml, const File & assetsFolder );
    
    virtual void run ();
 
    
private:
    bool checkDirty();
    void generateNewTempFiles();
    void newSeparationOutputs();
    void emptyQueue( );
    void initPlugin( );
    void reInit();
    void processPlugin();
    
    
    CriticalSection mutex;
    
    std::queue<ISSEPluginEvent> eventQueue;

    ISSPluginBase * plugin;
    bool needsTraining;
    bool shouldProcess;
    bool middleProcessing;
    double progress;
    int numSources;
    
    Array<ISSMask*> masks;
    std::vector<Matrix*> matrices;
    
    ScopedPointer<ISSAudioSource> issAudioSource;
    OwnedArray<FileAudioSink> issAudioSinks;
    std::vector<ISSAudioSink*> sinks;
    std::vector<Matrix*> mMap;
    Array<File> tempFiles;
};




#endif  // __PLUGINTHREAD_H_F5A774F8__
