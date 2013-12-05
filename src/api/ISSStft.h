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
#ifndef isscore_ISSSTFT_h
#define isscore_ISSSTFT_h


#include "ISSSource.h"
#include "ISSSink.h"
#include "ISSDefs.h"
#include "JuceHeader.h"



//! Short-Time Fourier Transform parameters
struct ISSStftParams
{
    enum WindowType {RECT=1, HANN=2, HAMMING=3};
    
    ISSStftParams()
    : fftsize(2048), hopsize(256), windowsize(fftsize), windowType(HANN)
    {}
    
    ISSStftParams( size_t fftsize_, size_t hopsize_, size_t windowsize_) 
    : fftsize(fftsize_), hopsize(hopsize_), windowsize(windowsize_), windowType(HANN)
    {}
    
    ISSStftParams( size_t fftsize_, size_t hopsize_, size_t windowsize_, WindowType windowType_ ) 
    : fftsize(fftsize_), hopsize(hopsize_), windowsize(windowsize_), windowType(windowType_)
    {}
    
    void reset()
    {
        fftsize = (2*2048);
        hopsize = (2*256);
        windowsize = fftsize;
        windowType = (HANN);
    }
    
    bool operator== (const ISSStftParams &other) const
    {
        
        
        if( fftsize==other.fftsize &&
            hopsize==other.hopsize &&
            windowsize==other.windowsize &&
            windowType==other.windowType)
        {
            return true;
        }
        else
            return false;
    }
    
    bool operator!= (const ISSStftParams &other) const
    {
        
        
        if( fftsize==other.fftsize &&
           hopsize==other.hopsize &&
           windowsize==other.windowsize &&
           windowType==other.windowType)
        {
            return false;
        }
        else
            return true;
    }
    
//    bool saveToXML( XmlElement & xml, const File & )
//    {
//        XmlElement* stftXML = new XmlElement ("STFT");
//        
//        stftXML->setAttribute ("FFTSIZE", (int)fftsize);
//        stftXML->setAttribute ("HOPSIZE", (int)hopsize);
//        stftXML->setAttribute ("WINDOWSIZE", (int)windowsize);
//        stftXML->setAttribute ("WINDOW", (int)windowType);
//        
//        
//        xml.addChildElement(stftXML);
//        return true;
//    }
    
//    bool loadFromXML( const XmlElement & xml, const File &  )
//    {
//        forEachXmlChildElement (xml, e)
//        {
//            if (e->hasTagName ("STFT"))
//            {
//                fftsize = e->getIntAttribute("FFTSIZE");
//                hopsize = e->getIntAttribute("HOPSIZE");
//                windowsize = e->getIntAttribute("WINDOWSIZE");
//                windowType = (WindowType)e->getIntAttribute("WINDOW");
//                break;
//            }
//        }        
//        return true;
//    }
    
    size_t fftsize;
    size_t hopsize; 
    size_t windowsize;
    WindowType windowType;
};


class FFT;

class ISSStft
{
public:
    
    ISSStft(const ISSStftParams & stftParams_);
    
    ~ISSStft();
    
    //! Return the internal stft parameters (fft size, window size, hop size, and window type)
    ISSStftParams getParams();
    
    
    //! Streams in the source buffer by buffer and outputs a complete linear-phase stft data matrix
    void forward(ISSAudioSource& source, Matrix & stftdata );
    
    //! Streams in the source buffer by buffer and outputs a magnitude and linear-phase stft data matrix
    void forward( ISSAudioSource& source, Matrix & magnitude, Matrix & phase );
    
    //! Returns only the magnitude of STFT
    void magnitude(  ISSAudioSource& source, Matrix & magnitude );

    //! A complete inverse stft given the independent magnitude and linear-phase matrices
    void reverse(const Matrix & magnitude, const Matrix & phase, ISSAudioSink & sink );
    void reverse(const Matrix & magnitude, const Matrix & phase, Matrix & sink );
    
public:
    
    static std::pair<size_t,size_t> getDimensions(const ISSStftParams & stftParams, size_t numSamples);
    
private:
    
    ISSStftParams params;
    Matrix window;
    Matrix fftbuffer;
    FFT * fft;
    FFT * ifft;
    
};





#endif
