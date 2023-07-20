#ifndef __FB_MemoryTracker_h__
#define __FB_MemoryTracker_h__

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
        /** Default constructor. */
        MemoryTracker();

        /** Destructor. */
        ~MemoryTracker();

        /** Report any memory leaks to the default output (usually stderr)
         */
        void reportLeaks();

        /** Set the name of the report file that will be produced on exit. */
        void setReportFileName( const String &name );

        /** Return the name of the file which will contain the report at exit */
        String getReportFileName() const;

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
        void _recordAlloc( void *ptr, size_t sz, u32 pool = 0, const c8 *file = nullptr, size_t ln = 0,
                           const c8 *func = nullptr );

        /** Record the deallocation of memory. */
        void _recordDealloc( void *ptr );

        /// Sets whether the record alloc/dealloc enabled.
        void setRecordEnable( bool recordEnable );

        /// Gets whether the record alloc/dealloc enabled.
        bool getRecordEnable() const;

        /// Static utility method to get the memory tracker instance
        static MemoryTracker &get();

    protected:
        mutable RecursiveMutex m_mutex;

        // Allocation record
        struct Alloc
        {
            Alloc();
            Alloc( size_t sz, u32 p, const c8 *file, size_t ln, const c8 *func );

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

        std::unordered_map<void *, Alloc> m_allocations;
        Array<size_t> m_allocationsByPool;
    };

#endif
}  // end namespace fb

#endif  // MemoryTracker_h__
