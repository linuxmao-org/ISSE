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
#ifndef iss_ISSMask_h
#define iss_ISSMask_h


#include "ISSDefs.h"
#include "JuceHeader.h"


class ISSMask  
{
public:
    
    ISSMask( unsigned int width_, unsigned int height_)
    {
        M.resize(height_, width_);
        clearMask();
        dirtybit = true;
    }
    
    ISSMask( ISSMask & mask )
    {
        // copy
        M = mask.getMask();
        dirtybit = true;
    }

  
//    ISSMask& operator= ( ISSMask & rhs)
//    {
//        M = rhs.getMask();
//        return *this;
//    }
    
    bool saveAsBinary( File & file )
    {

        file.deleteFile();
        file.create();
        // Write Matrices
        FileOutputStream * ostream = file.createOutputStream();
        GZIPCompressorOutputStream compOutputStream(ostream, 0, true);
        int rows(M.rows());
        int cols(M.cols());
        compOutputStream.write (&rows, sizeof(int));
        compOutputStream.write (&cols, sizeof(int));
        compOutputStream.write (&M(0,0), rows*cols*sizeof(ISSFloat));
        compOutputStream.flush();
     
        return true;
    }
    
    bool loadFromBinary( File & file )
    {
        int rows(0), cols(0);
        
        FileInputStream * istream = file.createInputStream();
        GZIPDecompressorInputStream compInputStream(istream, true);
        compInputStream.read (&rows, sizeof(int));
        compInputStream.read (&cols, sizeof(int));
        M.resize(rows, cols);
        
        // read matrix
        compInputStream.read (&M(0,0), rows*cols*sizeof(ISSFloat));
        dirty();
        
        return true;
    }
    
    void resize(size_t rows, size_t cols)
    {
        M.resize(rows, cols);
        clearMask();
        dirty();
    }
    
    virtual Matrix & getMask()
    {
        return M;
    }
    
    virtual bool isDirty()
    {
        ScopedWriteLock scopedLock(lock);
        return dirtybit;
    }
    
    virtual void dirty()
    {
        ScopedWriteLock scopedLock(lock);
        dirtybit = true;
    }
    
    virtual void clearDirty()
    {
        ScopedWriteLock scopedLock(lock);
        dirtybit = false;
    }
    
    void clearMask()
    {
        M.setZero();
        dirty();
    }
    
    virtual ~ISSMask(){};
     
private:
    
    bool dirtybit;
    ReadWriteLock lock;
    Matrix M;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ISSMask)
 
};


#endif //iss_ISSMask_h


