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

#ifndef iss_ISSEngine_h
#define iss_ISSEngine_h


#include "ISSDefs.h"

#include "ISSStft.h"
#include "ISSPluginBase.h"
#include "../JuceHeader.h"



#include <vector>
#include <map>
#include <set>


class ISSAudioSink;
class ISSAudioSource;

class ISSEngine : public ISSPluginBase
{
    
public:
    
    //! Default constructor
    ISSEngine();
    
    virtual ~ISSEngine();
    
    virtual bool save( std::string filepath );
    
    virtual bool load( std::string filepath );
    
    //! Initialize the plugin
    virtual bool init(double samplerate, size_t lengthInSamples, size_t numInputChannels);
    
    //! A flag to tell the host to reinitialize the plugin
    virtual bool needsToInit();
    
    //! Get a human-readable name or title of the plugin (read-only)
    virtual std::string getName() const;
    
    //! Get a human-readable dscription for the plugin (read-only)
    virtual std::string getDescription() const;
    
    //! Get the name of the author or vendeor of the plugin in human-readable form (read-only)
    virtual std::string getMaker() const;
    
    //! Get the copyright statement or licensing summary for the plugin (read-only)
    virtual std::string getCopyright() const;
    
    //! Get the version of the plugin
    virtual std::string getVersion () const;
    
    //! Get the controllable parameters of this plugin (read-only)
    virtual ParameterList getParameterDescriptors();
    
    //! Get the value of a named parameter (read-only)
    virtual float getParameter(std::string paramName);
    
    //! Set the value of a named parameters
    virtual void setParameter(std::string paramName, float value);
    
    //! Get the program settings for the plugin
    virtual ProgramList getPrograms();
    
    //! Selects the given program to run.  Internally the client must keep track of this.
    virtual void selectProgram(std::string programName);
    
    //! Get the currently selected program
    virtual std::string getCurrentProgram();
    
    //! Get the mask size dimensions (width x height)
    virtual std::pair<size_t,size_t> getMaskSize();
    
    virtual bool process(ISSAudioSource * audioSource,
                         std::vector<Matrix*> & Mmap,
                         std::vector<ISSAudioSink*> & audioSinks,
                         std::map<size_t, size_t> & training,
                         double & progress,
                         ProcessType type);

    virtual ISSStftParams getSTFTParams();  

private:
    
    int getNumCompsOfSource(unsigned int source);
    
    int getTotalNumComps();
    
    // Initialize all the internal matrices with random numbers
    void initMatrices();
    
    void reInitMatrices( std::map<size_t, size_t> & trainingIndsToSource );
    
    //! method that computes nmf for the main mixture separation process
    void updateMixtureSeparation(Matrix *M1, Matrix *M2, double & progress);
    
    void train(  std::map<size_t, size_t> & trainingIndsToSource );
 
    void render();
    
    // Helper thread classes
    friend class ISSRenderThread;
    
    ISSEngine(const ISSEngine&); // no implementation
    ISSEngine& operator=(const ISSEngine&); // no implementation
    
    
    // engine parameters
    ISSStftParams stftparams;
    bool needToRender;
    std::vector<ISSAudioSink*> audioSinks;
    double fs;
    size_t lengthInSamples;
    size_t numInputChannels;
//    std::pair<size_t,size_t> maskSize;
    
    // The current estimates of the W and H matrix
    Matrix Vphase[2];
    Matrix Vmag[2];
    Matrix W;
    Matrix H;
    Matrix Vn;
    
    ISSStft * stft;
    
    // FFT are only used by the render thread, but must the memory must be declared on the main thread
    int backBasis;
    int foreBasis;
    ParameterDescriptor backBasisDesc;
    ParameterDescriptor foreBasisDesc;
    ParameterDescriptor iterCountDesc;
    ParameterDescriptor fftSizeDesc;
    ParameterDescriptor hopSizeDesc;
    std::vector<int> fftSizes;

    bool updateW[2];
    bool needsInit;
    bool firstTimeAfterInit;
    bool firstTimeInit;

    size_t maxCumulativeIters;    
};


#endif





