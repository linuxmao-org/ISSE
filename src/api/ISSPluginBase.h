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

#ifndef __ISSPluginBase_H_8AA4F37B__
#define __ISSPluginBase_H_8AA4F37B__

#include <string>
#include <map>
#include <vector>

#include "ISSDefs.h"
#include "ISSStft.h"


class ISSAudioSource;
class ISSAudioSink;

// Templated help to acces value in const map
template <typename T>
typename T::mapped_type get(T const& map, typename T::key_type const& key)
{
    typename T::const_iterator iter(map.find(key));
    return iter != map.end() ? iter->second : typename T::mapped_type();
}

struct ISSEPluginEvent
{
    enum Type{ ProgramChange, Training, Normal };
    
    ISSEPluginEvent(Type type_) : type(type_), value(0)
    {}
    
    Type type;
    std::string name;
    float value;
    
};



struct ParameterDescriptor
{
    //! Default constructor
    ParameterDescriptor()
    {
        identifier = "";
        name = "";
        description = "";
        unit = "";
        minValue = 0;
        maxValue = 1;
        defaultValue = .5;
        isQuantized = false;
        quantizeStep = .1;
        isInteractive = false;
        causesReInit = false;
    }
    
    //! The name of the parameter, in computer-usable form.
    std::string identifier;
 	
    //! The human-readable name of the parameter.
    std::string name;
 	
    //! A human-readable short text describing the parameter.
    std::string description;
 	
    //! The unit of the parameter, in human-readable form.
    std::string unit;
 	
    //! The minimum value of the parameter.
    float minValue;
 	
    //! The maximum value of the parameter.
    float maxValue;
 	
    //! The default value of the parameter.
    float defaultValue;
 	
    //! True if the parameter values are quantized to a particular resolution.
    bool isQuantized;
 	
    //! Quantization resolution of the parameter values
    float quantizeStep;
    
    //!Names for the quantized values.
    std::vector< std::string > 	valueNames;
    
    //! flag that denotes whether to reprocess when the parameter is changed
    bool isInteractive;
    
    bool causesReInit;
    
    //! Overload comparison operators to only compare the identifier
    bool operator==(ParameterDescriptor const& x) const
    {
        if( identifier.compare(x.identifier)==0)
            return true;
        else
            return false;
    }
    
    //! Overload comparison operators to only compare the identifier
    bool operator!=(ParameterDescriptor const& x) const
    {
        if( identifier.compare(x.identifier)==0)
            return true;
        else
            return false;
    }
    
};
      
    
typedef std::vector<ParameterDescriptor> ParameterList;
typedef std::vector<std::string> ProgramList;
    
    
class ISSPluginBase
{
        
public:
 
    enum ProcessType{ TRAIN, NORMAL };

public:
    
    //! Default Contructor
    ISSPluginBase(){}
    
    //! Default Destructor
    virtual ~ISSPluginBase(){}
    
    virtual bool save( std::string filepath )=0;
    
    virtual bool load( std::string filepath )=0;
    
    //! Initialize the plugin
    virtual bool init(double samplerate,
                      size_t lengthInSamples,
                      size_t numInputChannels)=0;
    
    //! A flag to tell the host to reinitialize the plugin
    virtual bool needsToInit()=0;
        
    //! Get a human-readable name or title of the plugin (read-only)
    virtual std::string getName() const=0;
    
    //! Get a human-readable dscription for the plugin (read-only)
    virtual std::string getDescription() const = 0;
        
    //! Get the name of the author or vendeor of the plugin in human-readable form (read-only)
    virtual std::string getMaker() const = 0;
        
    //! Get the copyright statement or licensing summary for the plugin (read-only)
    virtual std::string getCopyright() const = 0;
        
    //! Get the version of the plugin
    virtual std::string getVersion () const =0;
        
    //! Get the controllable parameters of this plugin (read-only)
    virtual ParameterList getParameterDescriptors() = 0;
        
    //! Get the value of a named parameter (read-only)
    virtual float getParameter(std::string paramName) = 0;
        
    //! Set the value of a named parameters
    virtual void setParameter(std::string paramName, float value)=0;
        
    //! Get the program settings for the plugin 
    virtual ProgramList getPrograms() = 0;
        
    //! Selects the given program to run.  Internally the client must keep track of this.
    virtual void selectProgram(std::string programName) = 0;
        
    //! Get the currently selected program
    virtual std::string getCurrentProgram()=0;
    
    //! Get the current short-time Fourier Transform parameters
    virtual ISSStftParams getSTFTParams()=0;

    //! Get the mask size dimensions (width x height)
    virtual std::pair<size_t,size_t> getMaskSize()=0;
    
    
        
    // process can be called directly, but it will not pull or push data
    virtual bool process(ISSAudioSource * audioSource,
                         std::vector<Matrix*> & mMap,
                         std::vector<ISSAudioSink*> & audioSinks,
                         std::map<size_t, size_t> & training,
                         double & progress,
                         ProcessType type)=0;
    
};
    
    




#endif  // __ISSPluginBase_H_8AA4F37B__
