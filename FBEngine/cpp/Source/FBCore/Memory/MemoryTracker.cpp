/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include <FBCore/FBCorePCH.h>
#include <FBCore/FBCoreConfig.h>
#include <FBCore/Memory/MemoryTracker.h>
#include <FBCore/Base/StringTypes.h>
#include <iostream>
#include <assert.h>
#include <sstream>
#include <fstream>

#if FB_ENABLE_TRACE
#    include <boost/stacktrace.hpp>
#endif

#ifdef FB_PLATFORM_WIN32
#    include <windows.h>
#    define FBOutputCString( str ) ::OutputDebugStringA( str )
#    define FBOutputWString( str ) ::OutputDebugStringW( str )
#else
#    define FBOutputCString( str ) std::cerr << str
#    define FBOutputWString( str ) std::cerr << str
#endif

namespace fb
{
#if FB_ENABLE_MEMORY_TRACKER

    //-----------------------------------------------
    MemoryTracker::MemoryTracker() :
        m_fileName( "MemoryLeaks.log" ),
        m_dumpToStdOut( true ),
        m_totalAllocations( 0 ),
        m_recordEnable( true )
    {
    }

    //-----------------------------------------------
    MemoryTracker::~MemoryTracker()
    {
        reportLeaks();
    }

    //-----------------------------------------------
    bool MemoryTracker::getRecordEnable() const
    {
        return m_recordEnable;
    }

    //-----------------------------------------------
    MemoryTracker &MemoryTracker::get()
    {
        static MemoryTracker tracker;
        return tracker;
    }

    //-----------------------------------------------
    void MemoryTracker::_recordAlloc( void *ptr, size_t sz, unsigned int pool, const char *file,
                                      size_t ln, const char *func )
    {
        if( m_recordEnable )
        {
            FB_LOCK_MUTEX( Mutex );

            // FB_ASSERT(mAllocations.find(ptr) == mAllocations.end() && "Double allocation with same
            // address - " 	"this probably means you have a mismatched allocation / deallocation style, "
            //	"check if you're are using OGRE_ALLOC_T / OGRE_FREE and OGRE_NEW_T / OGRE_DELETE_T
            // consistently");

            m_allocations[ptr] = Alloc( sz, pool, file, ln, func );
            if( pool >= m_allocationsByPool.size() )
                m_allocationsByPool.resize( pool + 1, 0 );
            m_allocationsByPool[pool] += sz;
            m_totalAllocations += sz;
        }
    }

    //-----------------------------------------------
    void MemoryTracker::_recordDealloc( void *ptr )
    {
        if( m_recordEnable )
        {
            // deal cleanly with null pointers
            if( !ptr )
            {
                return;
            }

            FB_LOCK_MUTEX( Mutex );

            AllocationMap::iterator i = m_allocations.find( ptr );
            FB_ASSERT( i != m_allocations.end() &&
                       "Unable to locate allocation unit - "
                       "this probably means you have a mismatched allocation / deallocation style, "
                       "check if you're are using OGRE_ALLOC_T / OGRE_FREE and OGRE_NEW_T / "
                       "OGRE_DELETE_T consistently" );
            // update category stats
            m_allocationsByPool[i->second.pool] -= i->second.bytes;
            // global stats
            m_totalAllocations -= i->second.bytes;
            m_allocations.erase( i );
        }
    }

    //-----------------------------------------------
    void MemoryTracker::setRecordEnable( bool recordEnable )
    {
        m_recordEnable = recordEnable;
    }

    //-----------------------------------------------
    const String &MemoryTracker::getReportFileName() const
    {
        return m_fileName;
    }

    //-----------------------------------------------
    void MemoryTracker::setReportToStdOut( bool rep )
    {
        m_dumpToStdOut = rep;
    }

    //-----------------------------------------------
    bool MemoryTracker::getReportToStdOut() const
    {
        return m_dumpToStdOut;
    }

    //-----------------------------------------------
    size_t MemoryTracker::getTotalMemoryAllocated() const
    {
        return m_totalAllocations;
    }

    //-----------------------------------------------
    size_t MemoryTracker::getMemoryAllocatedForPool( u32 pool ) const
    {
        return m_allocationsByPool[pool];
    }

    //-----------------------------------------------
    void MemoryTracker::reportLeaks()
    {
        if( m_recordEnable )
        {
            std::stringstream os;

            if( m_allocations.empty() )
            {
                os << "Memory: No memory leaks" << std::endl;
            }
            else
            {
                os << "Memory: Detected memory leaks !!! " << std::endl;
                os << "Memory: (" << m_allocations.size() << ") Allocation(s) with total "
                   << m_totalAllocations << " bytes." << std::endl;
                os << "Memory: Dumping allocations -> " << std::endl;

                for( AllocationMap::const_iterator i = m_allocations.begin(); i != m_allocations.end();
                     ++i )
                {
                    os << std::endl;

                    const Alloc &alloc = i->second;
                    if( !alloc.filename.empty() )
                        os << alloc.filename;
                    else
                        os << "(unknown source):";

                    os << "(" << alloc.line << ") : {" << alloc.bytes << " bytes}"
                       << " function: " << alloc.function << std::endl;
                    os << std::endl;
                }
                os << std::endl;
            }

            if( m_dumpToStdOut )
            {
                std::cout << os.str();
            }

            std::ofstream of;
            of.open( m_fileName.c_str() );
            of << os.str();
            of.close();

            FBOutputCString( os.str().c_str() );
        }
    }

    //-----------------------------------------------
    void MemoryTracker::setReportFileName( const String &name )
    {
        m_fileName = name;
    }

    //-----------------------------------------------
    MemoryTracker::Alloc::Alloc( size_t sz, u32 p, const char *file, size_t ln, const char *func ) :
        bytes( sz ),
        pool( p ),
        line( ln )
    {
        if( file )
        {
            filename = file;
        }

        if( func )
        {
            function = func;
        }
    }

    //-----------------------------------------------
    MemoryTracker::Alloc::Alloc() : bytes( 0 ), line( 0 )
    {
    }

#endif
}  // end namespace fb
