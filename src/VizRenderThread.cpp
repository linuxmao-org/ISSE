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
#include "VizRenderThread.h"

#include "api/ISSStft.h"
#include "AudioSource.h"
#include "ImageLayer.h"
#include "ProgramSettings.h"



Image * vizRenderer( const Matrix & magnitude,
                     ISSFloat clipLimit,
                     ColorMap::ColorMapType ct,
                     std::map<size_t, size_t> & training,
                     Colour c1, Colour c2, float maxValue)
{
    
    
    //    return renderSpec( magnitude, clipLimit, ct, training, c1, c2, maxValue);
    if( magnitude.rows()==0 || magnitude.cols()==0)
        return NULL;
    
    ColorMap colorMap;
    colorMap.setColorMap( ct );

    colorMap.setMaxAndMin( 1, 0 );
    
    int numRows = magnitude.rows();
    
    Image * image = new Image(Image::RGB, magnitude.cols(), numRows, false);
    
    Thread * thread = Thread::getCurrentThread ();
    
    
    for(int j=0; j < magnitude.cols(); j++)// X
    {
        int source = training[j];
        if(source==0)
        {
            for(int i=0; i < numRows; i++)//Y
            {
                ISSFloat v = magnitude(i,j) ;
                
                
                image->setPixelAt(j, numRows-i-1, colorMap.getColor( v ));
            }
        }
        else if( source == 1 )
        {
            for(int i=0; i < numRows; i++)//Y
            {
                
                ISSFloat v = magnitude(i,j);
                image->setPixelAt(j, numRows-i-1, colorMap.getColor(v, c1));
            }
        }
        else if( source == 2 )
        {
            for(int i=0; i < numRows; i++)//Y
            {
                
                ISSFloat v =  magnitude(i,j);
                image->setPixelAt(j, numRows-i-1, colorMap.getColor(v, c2));
            }
        }
        
        if(thread->threadShouldExit())
            break;
    }
    
    return image;
    
    
}


template<typename Scalar>
struct CwiseDB
{
    CwiseDB()
    {}
    
    const Scalar operator()(const Scalar& x) const
    {
        return 20.0 * log10( x + ISSEPS);
    }
};



template<typename Scalar>
struct CwiseClipDB
{
    CwiseClipDB(const Scalar& inf) : m_inf(inf), m_infpos(abs(inf))
    {}
    
    const Scalar operator()(const Scalar& x) const
    {
        if (x < m_inf)
            return 0;
        else
            return (m_infpos + x)/m_infpos;
            
    }
    
 
    Scalar m_inf;
    Scalar m_infpos;
};




VizRenderThread::VizRenderThread (VizRenderThread::Listener * listener_,
                                    File * file_,
                                    ISSStft * stft_,
                                    std::map<size_t, size_t> training_,
                                    Colour c1_,
                                    Colour c2_,
                                    float clipLimit_,
                                    ColorMap::ColorMapType type, int trackNum_)
        :   Thread("Spectrogram Render Thread"),
            listener(listener_),
            file(file_),
            stft(stft_),
            training(training_),
            c1(c1_),
            c2(c2_),
            clipLimit(clipLimit_),
            ct(type),
            trackNum(trackNum_)
{}


void VizRenderThread::run ()
{
    
    if(file && file->exists())
    {
        
        ScopedPointer<ISSAudioSource> audioSource(new FileAudioSource( *file ) );
        audioSource->setChannel(2);
        
        if(threadShouldExit ())
            return;
        
        Matrix magnitude;
        stft->magnitude( *(audioSource.get()), magnitude );
        if(threadShouldExit ())
            return;
        
        if(magnitude.rows()==0 || magnitude.cols()==0) return;
 
        // Take the log
        magnitude = magnitude.unaryExpr(CwiseDB<ISSFloat>());
 
        if(trackNum==0)
            ProgramSettings::maxMag  = magnitude.maxCoeff();
        
    
        float m(ProgramSettings::maxMag);
        
        if(m==0)
            m = magnitude.maxCoeff();

        // Subtract off the maximum
        magnitude = magnitude.array() - m;
        magnitude = magnitude.unaryExpr(CwiseClipDB<ISSFloat>(clipLimit));
  
        ScopedPointer<Image> spectrogram( vizRenderer( magnitude, clipLimit, ct, training, c1, c2, m ) );

        if(threadShouldExit ())
            return;
        
        listener->vizRenderThreadDone( spectrogram  );

    }
}




