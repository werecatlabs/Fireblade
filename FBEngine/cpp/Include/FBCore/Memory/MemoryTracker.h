#ifndef __FB_MemoryTracker_h__
#define __FB_MemoryTracker_h__

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

#include <FBCore/FBCoreConfig.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/Core/StringTypes.h>
#include <unordered_map>

namespace fb
{
#if FB_ENABLE_MEMORY_TRACKER

    /** This class tracks the allocations and deallocations made, and
        is able to report memory statistics and leaks.
    @note
        This class is only available in debug builds.
    */
    class MemoryTracker
    {
    public:
        // ctor
        MemoryTracker();
        ~MemoryTracker();

        void reportLeaks();

        /** Set the name of the report file that will be produced on exit. */
        void setReportFileName( const String &name );

        /// Return the name of the file which will contain the report at exit
        const String &getReportFileName() const;

        /// Sets whether the memory report should be sent to stdout
        void setReportToStdOut( bool rep );

        /// Gets whether the memory report should be sent to stdout
        bool getReportToStdOut() const;

        /// Get the total amount of memory allocated currently.
        size_t getTotalMemoryAllocated() const;

        /// Get the amount of memory allocated in a given pool
        size_t getMemoryAllocatedForPool( u32 pool ) const;

        /** Record an allocation that has been made. Only to be called by
            the memory management subsystem.
            @param ptr The pointer to the memory
            @param sz The size of the memory in bytes
            @param pool The memory pool this allocation is occurring from
            @param file The file in which the allocation is being made
            @param ln The line on which the allocation is being made
            @param func The function in which the allocation is being made
        */
        void _recordAlloc( void *ptr, size_t sz, u32 pool = 0, const char *file = 0, size_t ln = 0,
                           const char *func = 0 );

        /** Record the deallocation of memory. */
        void _recordDealloc( void *ptr );

        /// Sets whether the record alloc/dealloc enabled.
        void setRecordEnable( bool recordEnable );

        /// Gets whether the record alloc/dealloc enabled.
        bool getRecordEnable() const;

        /// Static utility method to get the memory tracker instance
        static MemoryTracker &get();

    protected:
        RecursiveMutex m_mutex;  

        // Allocation record
        struct Alloc
        {
            Alloc();
            Alloc( size_t sz, unsigned int p, const char *file, size_t ln, const char *func );

            u32 pool;
            size_t bytes;
            size_t line;
            String filename;
            String function;
        };

        size_t m_totalAllocations = 0;

        bool m_recordEnable = true;
        bool m_dumpToStdOut = false;

        String m_fileName;

        typedef std::unordered_map<void *, Alloc> AllocationMap;
        AllocationMap m_allocations;

        typedef Array<size_t> AllocationsByPool;
        AllocationsByPool m_allocationsByPool;
    };

#endif
}  // end namespace fb

#endif  // MemoryTracker_h__
