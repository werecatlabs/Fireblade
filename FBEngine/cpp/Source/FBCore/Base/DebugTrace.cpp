#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/DebugTrace.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Base/StringUtil.h>

#include "FBCore/System/DebugUtil.h"

#if defined FB_PLATFORM_WIN32
#    include "windows.h"
#    include "psapi.h"
#endif

namespace fb
{
    RecursiveMutex DebugTrace::m_debugObjectsMutex;

    Map<String, List<RawPtr<DebugTrace>>> DebugTrace::m_debugObjects;

    DebugTrace::DebugTrace()
    {
    }

    DebugTrace::DebugTrace( bool bCheckHeap, bool bCheckGrowth ) :
        m_bCheckHeap( bCheckHeap ),
        m_bCheckGrowth( bCheckGrowth ),
        m_bEnableConsoleOutput( false ),
        m_virtualMemUsed( 0 ),
        m_physMemUsed( 0 )
    {
        m_currentTask = -1;

#if FB_ENABLE_HEAP_DEBUG
        bCheckHeap = true;

        if( getCheckGrowth() )
        {
            PROCESS_MEMORY_COUNTERS_EX pmc = {};
            pmc.cb = sizeof( pmc );
            GetProcessMemoryInfo( GetCurrentProcess(),
                                  reinterpret_cast<PROCESS_MEMORY_COUNTERS *>( &pmc ), pmc.cb );
            SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
            SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

            setVirtualMemUsed( virtualMemUsedByMe );
            setPhysMemUsed( physMemUsedByMe );
        }

        if( getCheckHeap() )
        {
            int heapstatus = _heapchk();
            switch( heapstatus )
            {
            case _HEAPOK:
            {
                if( getEnableConsoleOutput() )
                {
                    printf( " OK - heap is fine\n" );
                }
            }
            break;
            case _HEAPEMPTY:
            {
                FB_ASSERT( false );

                if( getEnableConsoleOutput() )
                {
                    printf( " OK - heap is empty\n" );
                }
            }
            break;
            case _HEAPBADBEGIN:
            {
                FB_ASSERT( false );

                if( getEnableConsoleOutput() )
                {
                    printf( "ERROR - bad start of heap\n" );
                }
            }
            break;
            case _HEAPBADNODE:
            {
                FB_ASSERT( false );

                if( getEnableConsoleOutput() )
                {
                    printf( "ERROR - bad node in heap\n" );
                }
            }
            break;
            }
        }
#endif

        // if (timer)
        //{
        //	m_startTime = timer->now();
        // }
    }

    DebugTrace::~DebugTrace()
    {
        // auto applicationManager = core::IApplicationManager::instance();
        // auto timer = applicationManager->getTimer();
        //
        // if (timer)
        //{
        //	m_endTime = timer->now();
        // }

        // double timeTaken = m_endTime - m_startTime;

        // FB_LOG(m_sourceFile + " " + StringUtil::toString(m_lineNumber) + " " + m_functionName +
        // " time: " + StringUtil::toString((f32)timeTaken));

        /*
        u32 profileId = StringUtil::getHash(m_sourceFile + m_functionName +
        StringUtil::toString(m_lineNumber));

        if (Thread::getCurrentTaskId() == Tasks::ET_PRIMARY)
        {
            auto applicationManager = core::IApplicationManager::instance();
            if (applicationManager)
            {
                SmartPtr<Profiler> profiler = applicationManager->getProfiler();
                if (profiler)
                {
                    SmartPtr<Profile> profile = profiler->getProfile(profileId);
                    if (profile)
                    {
                        double averageTimeTaken = profile->getAverageTimeTaken();
                        if (timeTaken > averageTimeTaken * 1.5)
                        {
                            LogManager::getInstance().logMessage("profile", "spike detected: " +
        m_functionName + StringUtil::toString(m_lineNumber));

                            profiler->setDirty(true);
                        }

                        profile->setTimeTaken((float)timeTaken);
                    }
                    else
                    {
                        profile = new Profile;
                        profile->setLabel(m_functionName + ": " + StringUtil::toString(m_lineNumber));
                        profile->setId(profileId);
                        profiler->addProfile(profile);
                    }
                }
            }
        }
        */

#if FB_ENABLE_HEAP_DEBUG
        if( getCheckGrowth() )
        {
            PROCESS_MEMORY_COUNTERS_EX pmc = {};
            pmc.cb = sizeof( pmc );
            GetProcessMemoryInfo( GetCurrentProcess(),
                                  reinterpret_cast<PROCESS_MEMORY_COUNTERS *>( &pmc ), pmc.cb );
            SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
            SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

            FB_ASSERT( getVirtualMemUsed() == virtualMemUsedByMe );
            FB_ASSERT( ( physMemUsedByMe - getPhysMemUsed() ) > 8192 * 1024 * 1024 );
        }

        if( getCheckHeap() )
        {
            int heapstatus = _heapchk();
            switch( heapstatus )
            {
            case _HEAPOK:
            {
                if( getEnableConsoleOutput() )
                {
                    printf( " OK - heap is fine\n" );
                }
            }
            break;
            case _HEAPEMPTY:
            {
                FB_ASSERT( false );

                if( getEnableConsoleOutput() )
                {
                    printf( " OK - heap is empty\n" );
                }
            }
            break;
            case _HEAPBADBEGIN:
            {
                FB_ASSERT( false );

                if( getEnableConsoleOutput() )
                {
                    printf( "ERROR - bad start of heap\n" );
                }
            }
            break;
            case _HEAPBADNODE:
            {
                FB_ASSERT( false );

                if( getEnableConsoleOutput() )
                {
                    printf( "ERROR - bad node in heap\n" );
                }
            }
            break;
            }
        }
#endif
    }

    void DebugTrace::setEnableConsoleOutput( bool val )
    {
        m_bEnableConsoleOutput = val;
    }

    bool DebugTrace::getEnableConsoleOutput() const
    {
        return m_bEnableConsoleOutput;
    }

    void DebugTrace::setPhysMemUsed( size_t val )
    {
        m_physMemUsed = val;
    }

    size_t DebugTrace::getPhysMemUsed() const
    {
        return m_physMemUsed;
    }

    void DebugTrace::setVirtualMemUsed( size_t val )
    {
        m_virtualMemUsed = val;
    }

    size_t DebugTrace::getVirtualMemUsed() const
    {
        return m_virtualMemUsed;
    }

    void DebugTrace::setCheckGrowth( bool val )
    {
        m_bCheckGrowth = val;
    }

    bool DebugTrace::getCheckGrowth() const
    {
        return m_bCheckGrowth;
    }

    bool DebugTrace::getCheckHeap() const
    {
        return m_bCheckHeap;
    }

    void DebugTrace::setCheckHeap( bool val )
    {
        m_bCheckHeap = val;
    }

    void DebugTrace::handleFunctionEnd()
    {
    }

    void DebugTrace::handleFunctionStart()
    {
    }

    void DebugTrace::setLineNumber( int val )
    {
        m_lineNumber = val;
    }

    void DebugTrace::writeTrace()
    {
        auto str = m_sourceFile + " " + StringUtil::toString( m_lineNumber ) + " " + m_functionName;
        FB_LOG( str );
    }

    void DebugTrace::addDebugObject( RawPtr<DebugTrace> ptr )
    {
        RecursiveMutex::ScopedLock lock( m_debugObjectsMutex );

        auto str = StringUtil::toString( ptr->m_lineNumber ) + ptr->m_sourceFile + ptr->m_functionName;
        auto debugCheckList = m_debugObjects[str];
        debugCheckList.push_back( ptr );
    }

    void DebugTrace::removeDebugObject( RawPtr<DebugTrace> ptr )
    {
        RecursiveMutex::ScopedLock lock( m_debugObjectsMutex );

        auto str = StringUtil::toString( ptr->m_lineNumber ) + ptr->m_sourceFile + ptr->m_functionName;
        auto debugCheckList = m_debugObjects[str];
        auto it = std::find( debugCheckList.begin(), debugCheckList.end(), ptr );
        if( it != debugCheckList.end() )
        {
            debugCheckList.erase( it );
        }
    }

    s32 DebugTrace::getLineNumber() const
    {
        return m_lineNumber;
    }

    void DebugTrace::setFunctionName( const String &val )
    {
        m_functionName = val;
    }

    String DebugTrace::getFunctionName() const
    {
        return m_functionName;
    }

    void DebugTrace::setSourceFile( const String &val )
    {
        m_sourceFile = val;
    }

    String DebugTrace::getSourceFile() const
    {
        return m_sourceFile;
    }

    DebugTrace::DebugCheckFunction::~DebugCheckFunction()
    {
        if( m_debugCheck )
        {
            m_debugCheck->handleFunctionEnd();
        }

        m_debugCheck = nullptr;
    }

    DebugTrace::DebugCheckFunction::DebugCheckFunction( DebugTrace *debugCheck ) :
        m_debugCheck( debugCheck )
    {
        if( m_debugCheck )
        {
            m_debugCheck->handleFunctionStart();
        }
    }

    DebugTrace::DebugCheckFunction::DebugCheckFunction()
    {
    }
}  // end namespace fb
