#include <FBCore/Core/StringTypes.h>
#include <FBCore/FBCoreConfig.h>
#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/MemoryTracker.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

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

    MemoryTracker::MemoryTracker() :
        m_fileName( "MemoryLeaks.log" ),
        m_dumpToStdOut( true ),
        m_totalAllocations( 0 ),
        m_recordEnable( true )
    {
    }

    MemoryTracker::~MemoryTracker()
    {
        reportLeaks();
    }

    bool MemoryTracker::getRecordEnable() const
    {
        return m_recordEnable;
    }

    MemoryTracker &MemoryTracker::get()
    {
        static MemoryTracker tracker;
        return tracker;
    }

    void MemoryTracker::_recordAlloc( void *ptr, size_t sz, unsigned int pool, const char *file,
                                      size_t ln, const char *func )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( m_recordEnable )
        {
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

    void MemoryTracker::_recordDealloc( void *ptr )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( m_recordEnable )
        {
            // deal cleanly with null pointers
            if( !ptr )
            {
                return;
            }

            auto i = m_allocations.find( ptr );
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

    void MemoryTracker::setRecordEnable( bool recordEnable )
    {
        m_recordEnable = recordEnable;
    }

    String MemoryTracker::getReportFileName() const
    {
        return m_fileName;
    }

    void MemoryTracker::setReportToStdOut( bool rep )
    {
        m_dumpToStdOut = rep;
    }

    bool MemoryTracker::getReportToStdOut() const
    {
        return m_dumpToStdOut;
    }

    size_t MemoryTracker::getTotalMemoryAllocated() const
    {
        return m_totalAllocations;
    }

    size_t MemoryTracker::getMemoryAllocatedForPool( u32 pool ) const
    {
        return m_allocationsByPool[pool];
    }

    void MemoryTracker::reportLeaks()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

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

                for( auto i = m_allocations.begin(); i != m_allocations.end(); ++i )
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

    void MemoryTracker::setReportFileName( const String &name )
    {
        m_fileName = name;
    }

    MemoryTracker::Alloc::Alloc( size_t sz, u32 p, const c8 *file, size_t ln, const c8 *func ) :
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

    MemoryTracker::Alloc::Alloc() : bytes( 0 ), line( 0 )
    {
    }

#endif
}  // end namespace fb
