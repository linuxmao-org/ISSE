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

#include "ISSEngine.h"


 
#include "ISSFft.h"

#include "ISSStft.h"
#include "ISSRandom.h"
#include "ISSSource.h"
#include "ISSSink.h"

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
 
#if defined __WINDOWS__
#include <windows.h>
#include <process.h>

#endif

//#if defined __UNIX__
#include <float.h>
//#endif
 

using namespace std;

//double _tic_start_time;
//double tic()
//{
//	struct timeval time;
//	gettimeofday( &time, NULL);
//	return time.tv_sec + double( time.tv_usec)/1e6;
//}

// Get elapsed time
//double toc( double t)
//{
//	struct timeval time;
//	gettimeofday( &time, NULL);
//	double end_time = time.tv_sec + double( time.tv_usec)/1e6;
//	return end_time - t;
//}




ISSEngine::ISSEngine()  :   needToRender(false),
                            fs(0),
                            lengthInSamples(0),
                            numInputChannels(1),
                            backBasis(50),
                            foreBasis(50),
                            needsInit(true),
                            firstTimeAfterInit(true), firstTimeInit(true),
#ifdef DEBUG
maxCumulativeIters(1)
#else
maxCumulativeIters(50)
#endif
{    
    backBasisDesc.identifier = "backbasis";
    backBasisDesc.name = "Source 1 Basis Vectors";
    backBasisDesc.description = "Number of basis to explain source 1";
    backBasisDesc.unit = "basis";
    backBasisDesc.minValue = 1;
    backBasisDesc.maxValue = 300;
    backBasisDesc.defaultValue = backBasis;
    backBasisDesc.isQuantized = true;
    backBasisDesc.quantizeStep = 1;
    
    foreBasisDesc.identifier = "forebasis";
    foreBasisDesc.name = "Source 2 Basis Vectors";
    foreBasisDesc.description = "Number of basis to explain source 2";
    foreBasisDesc.unit = "basis";
    foreBasisDesc.minValue = 1;
    foreBasisDesc.maxValue = 300;
    foreBasisDesc.defaultValue = foreBasis;
    foreBasisDesc.isQuantized = true;
    foreBasisDesc.quantizeStep = 1;
    
    iterCountDesc.identifier = "iterations";
    iterCountDesc.name = "Iterations";
    iterCountDesc.description = "Number of iterations/process";
    iterCountDesc.unit = "iters";
    iterCountDesc.minValue = 0;
    iterCountDesc.maxValue = 100;
    iterCountDesc.defaultValue = maxCumulativeIters;
    iterCountDesc.isQuantized = true;
    iterCountDesc.quantizeStep = 1;
    
    fftSizes.push_back(128);
    fftSizes.push_back(256);
    fftSizes.push_back(512);
    fftSizes.push_back(1024);
    fftSizes.push_back(2048);
    fftSizes.push_back(4096);
    fftSizes.push_back(8192);
    fftSizes.push_back(16384);


    fftSizeDesc.identifier = "fftsize";
    fftSizeDesc.name = "FFT Size";
    fftSizeDesc.description = "Controls the frequency resolution";
    fftSizeDesc.unit = "Samples";
    fftSizeDesc.minValue = 128;
    fftSizeDesc.maxValue = 16384;
    fftSizeDesc.defaultValue = 4;
    fftSizeDesc.isQuantized = true;
    fftSizeDesc.quantizeStep = 0;
    fftSizeDesc.causesReInit = true;
    for(int i=0; i < fftSizes.size(); ++i)
    {
        std::stringstream ss;
        ss << fftSizes[i];
        fftSizeDesc.valueNames.push_back( ss.str() );
    }
    
    hopSizeDesc.identifier = "hopsize";
    hopSizeDesc.name = "Hop Size";
    hopSizeDesc.description = "Controls the hop resolution";
    hopSizeDesc.unit = "Samples";
    hopSizeDesc.minValue = 128;
    hopSizeDesc.maxValue = 16384;
    hopSizeDesc.defaultValue = 3;
    hopSizeDesc.isQuantized = true;
    hopSizeDesc.quantizeStep = 0;
    hopSizeDesc.causesReInit = true;
    for(int i=0; i < fftSizes.size(); ++i)
    {
        std::stringstream ss;
        ss << fftSizes[i];
        hopSizeDesc.valueNames.push_back( ss.str() );
    }
    
#ifdef DEBUG
    std::cout << "Launching engine..." << std::endl;
#endif
    
 
}

bool ISSEngine::save( std::string filepath )
{
 
    File f(filepath.c_str());
    f.create();

    XmlElement xml("PLUGIN");

    xml.setAttribute("FFTSIZE", (int)stftparams.fftsize);
    xml.setAttribute("HOPSIZE", (int)stftparams.hopsize);
    
    
    return  xml.writeToFile (f, String::empty);

}

bool ISSEngine::load( std::string filepath )
{
    File f(filepath.c_str());
    ScopedPointer<XmlElement>  xml =  XmlDocument::parse (f);
    
    stftparams.fftsize = xml->getIntAttribute("FFTSIZE");
    stftparams.windowsize = stftparams.fftsize;
    stftparams.hopsize = xml->getIntAttribute("HOPSIZE");
    
    init(fs, lengthInSamples, numInputChannels);
    
    return true;

}

bool ISSEngine::init(double samplerate,
                     size_t lengthInSamples_,
                     size_t numInputChannels_ )
{
    fs = samplerate;
    lengthInSamples = lengthInSamples_;
    numInputChannels = numInputChannels_;
    
    
    if(firstTimeInit && fs > 16000)
    {
        stftparams.fftsize = 4096;
        stftparams.windowsize = 4096;
        stftparams.hopsize = 512;
    }
    firstTimeInit = false;
    
   
    
    needsInit = false;
     stft = new ISSStft(stftparams);
    firstTimeAfterInit = true;
    
    return true;
}


ISSStftParams ISSEngine::getSTFTParams()
{
    return stftparams;
}
int ISSEngine::getNumCompsOfSource(unsigned int source)
{
    if(source==0)
        return backBasis;
    else
        return foreBasis;
}

int ISSEngine::getTotalNumComps()
{
    return backBasis+foreBasis;
}

std::pair<size_t,size_t> ISSEngine::getMaskSize()
{
    return ISSStft::getDimensions(stftparams, lengthInSamples );
}


bool ISSEngine::process( ISSAudioSource * audioSource,
                         std::vector<Matrix*> & mMap,
                         std::vector<ISSAudioSink*> & audioSinks_,
                         std::map<size_t, size_t> & training,
                         double & progress,
                         ProcessType type)
{
    
    audioSinks = audioSinks_;
    
    if(firstTimeAfterInit)
    {
        for(int i=0; i < 2; i++)
            updateW[i] = true;

                // compute stft
        for(int c=0; c < numInputChannels; c++)
        {
            audioSource->setChannel(c);
            stft->forward(*audioSource, Vmag[c], Vphase[c]);
            Vmag[c] = Vmag[c].array() + ISSEPS;
        }

        
        if(numInputChannels==2)
            Vn = Vmag[0] + Vmag[1];
        else
            Vn = Vmag[0];

        
        Vn = Vn / Vn.sum();
        
        initMatrices();
        
        firstTimeAfterInit = false;
    }

    if(type==TRAIN)
        train( training );
    
    reInitMatrices( training );
    updateMixtureSeparation(mMap[1], mMap[0], progress);
    
    return true;
}


ISSEngine::~ISSEngine()
{}


std::string ISSEngine::getName() const
{
    return "ISSE Default";
}

 

std::string ISSEngine::getDescription() const
{
    return "Core separation algorithm";
    
}

std::string ISSEngine::getMaker() const
{
    return "Stanford University & Adobe Research";
}

std::string ISSEngine::getCopyright() const
{
    return "GNU General Public License, v.3";
}

std::string ISSEngine::getVersion () const
{
    return std::string(ProjectInfo::versionString);
}

bool ISSEngine::needsToInit()
{
    return needsInit;
}

ParameterList ISSEngine::getParameterDescriptors()
{
    ParameterList list;
    list.push_back(fftSizeDesc);
    list.push_back(hopSizeDesc);
    list.push_back(backBasisDesc);
    list.push_back(foreBasisDesc);
    list.push_back(iterCountDesc);
    return list;
}

float ISSEngine::getParameter(std::string paramName)
{

    if(paramName.compare("fftsize")==0)
    {
        for(int i=0; i < fftSizes.size(); ++i)
        {
            size_t v = fftSizes[i];
            if( v==stftparams.fftsize )
                return i;
        }
        return 0;
    }
    else if(paramName.compare("hopsize")==0)
    {
        for(int i=0; i < fftSizes.size(); ++i)
        {
            if( fftSizes[i]==stftparams.hopsize )
                return i;
        }
        return 0;
    }
    else if(paramName.compare("backbasis")==0)
        return backBasis;
    else if(paramName.compare("forebasis")==0)
        return foreBasis;
    else if(paramName.compare("iterations")==0)
        return maxCumulativeIters;
    
    return 0.0;
}

void ISSEngine::setParameter(std::string paramName, float value)
{
    if(paramName.compare("fftsize")==0)
    {
        int v = value;
        
        if( (v >= 0) && (v < fftSizes.size()) )
        {
            
            if(fftSizes[v]!=stftparams.fftsize)
            {
                stftparams.fftsize = fftSizes[v];
                stftparams.windowsize = stftparams.fftsize;
            
                if(stftparams.hopsize >= stftparams.fftsize/2)
                    stftparams.hopsize = stftparams.fftsize/2;
            
            needsInit = true;
            }
        }
        
    }
    else if(paramName.compare("hopsize")==0)
    {
        int v = value;
        if( (v >= 0) && (v < fftSizes.size()) )
        {
            if(stftparams.hopsize != fftSizes[v])
            {
                stftparams.hopsize = fftSizes[v];
            
                if(stftparams.hopsize >= stftparams.fftsize/2)
                {
                    stftparams.fftsize = stftparams.hopsize*2;
                    stftparams.windowsize = stftparams.fftsize;
                }
            
                needsInit = true;
            }
        }
        
    }
    else if(paramName.compare("backbasis")==0)
    {
        backBasis = int(value);
        initMatrices();
        
    }
    else if(paramName.compare("forebasis")==0)
    {
        foreBasis = int(value);
        initMatrices();
    }
    else if(paramName.compare("iterations")==0)
    {
        maxCumulativeIters = int(value);
    }
}

ProgramList ISSEngine::getPrograms()
{
    return ProgramList();
}

void ISSEngine::selectProgram(std::string programName)
{
    
}

std::string ISSEngine::getCurrentProgram()
{
    return "";
}


void ISSEngine::render()
{
    
    int rows = Vmag[0].rows();
    int cols = Vmag[0].cols();
    Matrix WHinv( rows, cols );
    Matrix weinerMask( rows, cols );
 
    WHinv = ((W*H).array()+ISSEPS).cwiseInverse();
    
    int left = 0;
    int right = getNumCompsOfSource(0);
    for(size_t i=0; i < audioSinks.size(); ++i)
    {
        weinerMask = (W.block(0, left, rows, right) * H.block(left, 0, right, cols )).cwiseProduct(WHinv);
        
        if(numInputChannels==1)
        {
            
            Matrix output;
            stft->reverse( Vmag[0].cwiseProduct(weinerMask), Vphase[0], output );
            const ISSFloat * channelData[2];
            channelData[0] = output.data();
            channelData[1] = nullptr;
            audioSinks[i]->write(channelData, 1, output.rows());
        }
        else if(numInputChannels==2)
        {
            const ISSFloat * channelData[3];
            Matrix output[2];
            for(int c=0; c < numInputChannels; ++c)
            {
                stft->reverse( Vmag[c].cwiseProduct(weinerMask), Vphase[c], output[c] );
                channelData[c] = output[c].data();
            }
            channelData[numInputChannels] = nullptr;
            audioSinks[i]->write(channelData, numInputChannels, output[0].rows());
        }

        
        // Update indices
        left = right;
        right = getNumCompsOfSource(i+1);
    }
}


void ISSEngine::train( std::map<size_t, size_t> & trainingIndsToSource )
{
    if(trainingIndsToSource.size()==0)
        return;
    
    // clear all matrices
    initMatrices();
    
    int Nf = Vn.rows();
    int left = 0;
    int right = getNumCompsOfSource(0);
    
    for(int s=0; s < 2; s++)
    {
        updateW[s] = true;
        
        // get indices for source
        std::set<size_t> strain;
        for(std::map<size_t, size_t>::iterator iter = trainingIndsToSource.begin();
            iter != trainingIndsToSource.end();
            iter++)
        {
            if(iter->second==s+1)
                strain.insert(iter->first);
        }
        
        if(strain.size()==0)
        {
            left = right;
            right = getNumCompsOfSource(s+1);
            continue;
        }
        updateW[s] = false;
        
        int Nt = strain.size();
        Matrix Vs(Nf, Nt);
        Matrix WH(Nf, Nt);
        int index = 0;
        for(std::set<size_t>::iterator iter = strain.begin(); iter!=strain.end(); iter++, index++)
            Vs.block(0, index, Nf, 1) = Vn.block(0, *iter, Nf, 1);
        
        
        // process for a while
        for(int iter=0; iter < maxCumulativeIters; ++iter)
        {
            WH =  W.block(0,  left, Nf, right) * H.block(left,  0, right, Nt);
            
            // Add float point precision to avoid nan
            WH = WH.array() + ISSEPS;
            
            // Update activations
            H.block(left,  0, right, Nt)
            = H.block(left,  0, right, Nt).cwiseProduct( W.block(0,  left, Nf, right).transpose() * (Vs.cwiseQuotient(WH)) );
            
            
            // Update dictionaries
            W.block(0,  left, Nf, right)
            = W.block(0,  left, Nf, right).cwiseProduct( (Vs.cwiseQuotient(WH) ) * H.block(left,  0, right, Nt).transpose() );
            
            // normalized W
            for(int c = left; c < (left+right); ++c )
                W.col(c) = W.col(c) / W.col(c).sum();
        }
        left = right;
        right = getNumCompsOfSource(s+1);
    }
}

void ISSEngine::initMatrices()
{
    unsigned int numBasis(getTotalNumComps());
    W.resize(Vn.rows(), numBasis);
    H.resize(numBasis, Vn.cols());
    
    ISSRandom rgenrator(.001, 1, 1);
    
    // init W
    for (unsigned int j = 0; j < W.cols(); ++j) // over the cols
    {
        // over the rows
        for (unsigned int i = 0; i < W.rows(); ++ i)
            W(i, j) = rgenrator.getRandom<ISSFloat>();
        
        W.col(j) = W.col(j) / W.col(j).sum();
    }
    
    for (unsigned int j = 0; j < H.cols(); ++j)
        for (unsigned int i = 0; i < H.rows(); ++i)
            H(i, j) = rgenrator.getRandom<ISSFloat>();
}


void ISSEngine::reInitMatrices( std::map<size_t, size_t> & trainingIndsToSource )
{
    
    ISSRandom rgenrator(.001, 1, 1);
    
    int left = 0;
    int right = getNumCompsOfSource(0);
    
    for(int s=0; s < 2; ++s)
    {
        // random init W
        if(updateW[s])
        {
            // init W
            for (unsigned j = left; j < (left+right); ++j) // over the cols
            {
                for (unsigned i = 0; i < W.rows(); ++ i)
                    W (i, j) = rgenrator.getRandom<ISSFloat>();
                
                W.col(j) = W.col(j) / W.col(j).sum();
            }
        }
        
        // Reinitialize H for the given source
        for (unsigned j = 0; j < H.cols(); ++j)
            for (unsigned i = left; i < (left+right); ++i)
                H(i, j) = rgenrator.getRandom<ISSFloat>();

        // get all training indices not for the source
        for(std::map<size_t, size_t>::iterator iter = trainingIndsToSource.begin();
            iter != trainingIndsToSource.end();
            iter++)
        {
            if((iter->second!=0) && iter->second!=(s+1)) // current source
                H.block(left, iter->first, right, 1) = Matrix::Zero(right, 1);
        }
        
        left = right;
        right = getNumCompsOfSource(s+1);
    }
}



 
bool isNanCheck(Matrix & m)
{
	using namespace std;
    
    for(int i=0; i < m.rows(); ++i)
    {
        for(int j=0; j < m.cols(); ++j)
        {
            if( isnan( m(i,j) ) )
                return true;
        }
    }
    
    return false;
}





void ISSEngine::updateMixtureSeparation( Matrix *M1, Matrix *M2, double & progress)
{
    //double totalTime(0);
    //totalTime = tic();
    
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

    
    int Nf = Vn.rows();
    int K1 = getNumCompsOfSource(0);
    int K2 = getNumCompsOfSource(1);
    int Nt = Vn.cols();
 
    Matrix Z;
    Matrix V2 = Vn.cwiseProduct(*M1);
    Matrix V1 = Vn.cwiseProduct(*M2);
    
    //double NMFTime(0);
    //NMFTime = tic();
    
    // process for a while
    for(int iter = 0; iter < maxCumulativeIters; ++iter)
    {
        
        progress = ( iter*1.0/maxCumulativeIters );
        
        Z =  M1->cwiseProduct( W.block(0,  0, Nf, K1) * H.block(0,  0, K1, Nt) ) +
        M2->cwiseProduct( W.block(0, K1, Nf, K2) * H.block(K1, 0, K2, Nt) );
        Z =  Z.array()  + ISSEPS;
        
        int left = 0;
        int right = getNumCompsOfSource(0);
        for(int s=0; s < 2 ; s++)
        {
            Matrix * V = &V1;
            if(s>0) V = &V2;
            
            if(updateW[s])
            {
                W.block(0, left, Nf, right)  =
                    W.block(0, left, Nf, right).cwiseProduct( V->cwiseQuotient(Z)*H.block(left, 0, right, Nt).transpose());
                for(int i=left; i < (left+right); ++i )
                    W.col(i) = W.col(i) / W.col(i).sum();
            }
            
            H.block(left, 0, right, Nt) =
                H.block(left, 0, right, Nt).cwiseProduct(  W.block(0,  left, Nf, right).transpose()*V->cwiseQuotient(Z)  );
            
            left = right;
            right = getNumCompsOfSource(s+1);
            
        }
    }

    // normalized W
    for(int i=0; i < W.cols(); ++i )
    {
        float gain = W.col(i).sum();
        W.col(i) = W.col(i) / gain;
        H.row(i) = H.row(i) * gain;
    }
    
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    
    render();
    progress = ( 1.0 );
}


 

//void ISSEngine::basicNMF()
//{
//    Matrix WH;
//    // process for a while
//    for(int iter=0; iter < consecutiveIters; ++iter)
//    {
//        WH = W*H;
//
//        // Add float point precision to avoid nan
//        WH = WH.array() + EPS;
//
//        // Update activations
//        H = H.cwiseProduct( W.transpose() * (V.cwiseQuotient(WH)) );
//
//
//        // Update dictionaries
//        W = W.cwiseProduct( (V.cwiseQuotient(WH) ) * H.transpose() );
//
//
//        // normalized W
//        for(int i=0; i < W.cols(); ++i )
//            W.col(i) = W.col(i) / W.col(i).sum();
//    }
//}

