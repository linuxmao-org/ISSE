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

#ifndef __SEPARATIONENGINE_H_57117662__
#define __SEPARATIONENGINE_H_57117662__


#include "JuceHeader.h"
#include "api/ISSPluginBase.h"

#include <map>


class ISSMask;
class PluginThread;
class ISSPluginBase;
class PluginComponent;

class SeparationEngine  
{
public:
    
    enum PluginType{ StandardMode, PitchMode };
    
    class Listener
    {
    public:

        enum Msg{ TrainingChanged,
                  NewPluginLoaded,
                  NewMaskSTFTParams, PluginLoadedXML,
                  UpdateGUI };
        
        virtual ~Listener(){}

        //! New audio outputs
        virtual void engineNewOutputs( File file, int source )=0;
        
        //! Render the spectrogram
        virtual void engineTrainingChanged()=0;
        
        //! New plugin loaded, reset everything
        virtual void engineNewPlugin( ISSPluginBase * plugin, Msg msg )=0;
    };
    
public:
    
    SeparationEngine();
    ~SeparationEngine();
    
    void setupAndLaunchEngine(File inputFile_, PluginType pluginName );
    
    void addListener( SeparationEngine::Listener * listener );
    void removeListener( SeparationEngine::Listener * listener);

    std::map<size_t, size_t> getTraining();
    void addTraining(unsigned int source, unsigned int xmin, unsigned int xmax);
    void subtractTraining(unsigned int source, unsigned int xmin, unsigned int xmax);
    void clearTraining();
    void clearMasks();
    
    double & getProgress();
    
    void start();
    void stop();
    void shutdown();
    void reset();
    
    bool saveToXML( XmlElement & xml, const File & folderForAssets );
    bool loadFromXML( const XmlElement & xml, const File & assetsFolder  );
    
    
    Component * createPluginGUI(); // Caller must delete
    ISSMask * getMask(unsigned int source);
    
    void postEvent(ISSEPluginEvent event);
    
    
    void notifyEngineListeners( SeparationEngine::Listener::Msg msg );
    
    
    void updateWithNewFiles( const Array<File> & files );
    
    juce_DeclareSingleton (SeparationEngine, false)
    
private:
    
   
    bool setupAndLaunchEngine(File inputFile_, const XmlElement & xml, const File & assetsFolder, bool fromxml, PluginType pluginName );
    
private:
    
    ListenerList <SeparationEngine::Listener> listeners;

    int numTracks;
    int numSources;
    double progress;
    bool needsTraining;
    bool shouldProcess;
    
   
    ScopedPointer<ISSPluginBase> plugin;
    ScopedPointer<PluginThread> processThread;
    PluginComponent * pluginComponent;
    
    std::map<size_t, size_t> trainingIndsToSource;

    OwnedArray<ISSMask> masks;

};



#endif  // __SEPARATIONENGINE_H_57117662__




