#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/ObjectTracker.h>

#include <FBCore/Thread/Threading.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/StringUtil.h>
#include <sstream>
#include <fstream>

#if FB_ENABLE_TRACE
#    include <boost/stacktrace.hpp>
#endif

namespace fb
{

    ObjectTracker::ObjectTracker() : m_isRunning( true )
    {
        m_thread = new std::thread( threadFunc );
    }

    ObjectTracker::~ObjectTracker()
    {
        m_isRunning = false;

        if( m_thread )
        {
            m_thread->join();
            delete m_thread;
            m_thread = nullptr;
        }

        dumpReport();
        dumpObjectReport();
    }

    void ObjectTracker::dumpReport( ISharedObject *pObject, const String &filePath )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        std::stringstream stream;
        stream << "ObjectTracker report for: " << StringUtil::getCurrentDateTime() << std::endl;
        stream << std::endl;
        stream << "Num objects " << StringUtil::toString( static_cast<s32>( m_objectRecords.size() ) )
               << std::endl;
        stream << std::endl;

        for( auto &it : m_objectRecords )
        {
            auto &r = it.second;
            auto object = r.object;
            if( pObject == object )
            {
                auto &referenceRecords = r.referenceRecords;

                const char *debugName = typeid( *object ).name();

                stream << std::endl;

                auto address = (size_t)object;
                auto references = object->getReferences();
                stream << "DebugName: " << debugName << " ref count: " << references
                       << " address: " << address << std::endl;
                stream << std::endl;

                for( auto referenceRecordIt : referenceRecords )
                {
                    auto &reference = referenceRecordIt.second;
                    auto referenceStack = reference.stack.c_str();
                    auto sourceFile = reference.sourceFile.c_str();
                    auto sourceFunc = reference.sourceFunc.c_str();
                    auto sourceLine = reference.sourceLine;

                    stream << "DebugName: " << debugName << " stack: " << referenceStack
                           << " Address: " << reference.address << sourceFile << " " << sourceFunc << " "
                           << sourceLine << std::endl;
                }
            }
        }

        auto path = StringUtil::replaceAll( filePath, "<", "" );
        path = StringUtil::replaceAll( path, ">", "" );

        std::fstream fs;
        fs.open( path, std::fstream::out | std::fstream::trunc );
        fs << stream.str();
        fs.flush();
        fs.close();
    }

    void ObjectTracker::dumpReport( ISharedObject *pObject )
    {
        auto path = String( "ObjectTracker.log" );
        dumpReport( pObject, path );
    }

    void ObjectTracker::dumpReport( SmartPtr<ISharedObject> pObject )
    {
        auto p = pObject.get();
        dumpReport( p );
    }

    void ObjectTracker::dumpReport()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        std::stringstream stream;
        stream << "ObjectTracker report for: " << StringUtil::getCurrentDateTime() << std::endl;
        stream << std::endl;
        stream << "Num objects " << StringUtil::toString( static_cast<s32>( m_objectRecords.size() ) )
               << std::endl;
        stream << std::endl;

        for( auto &it : m_objectRecords )
        {
            auto &r = it.second;
            auto object = r.object;
            auto &referenceRecords = r.referenceRecords;

            const char *debugName = typeid( *object ).name();

            stream << std::endl;

            auto address = (size_t)object;
            auto references = object->getReferences();
            stream << "DebugName: " << debugName << " ref count: " << references
                   << " address: " << address << std::endl;
            stream << std::endl;

            for( auto &referenceRecordIt : referenceRecords )
            {
                auto &reference = referenceRecordIt.second;
                auto referenceStack = reference.stack.c_str();
                auto sourceFile = reference.sourceFile.c_str();
                auto sourceFunc = reference.sourceFunc.c_str();
                auto sourceLine = reference.sourceLine;

                stream << "DebugName: " << debugName << " stack: " << referenceStack
                       << " Address: " << reference.address << sourceFile << " " << sourceFunc << " "
                       << sourceLine << std::endl;
            }
        }

        auto path = String( "ObjectTracker.log" );
        std::fstream fs;
        fs.open( path, std::fstream::out );
        fs << stream.str();
        fs.close();
    }

    void ObjectTracker::dumpObjectReport()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        std::stringstream stream;
        stream << "ObjectTracker report for: " << StringUtil::getCurrentDateTime() << std::endl;
        stream << std::endl;
        stream << "Num objects " << StringUtil::toString( static_cast<s32>( m_objectRecords.size() ) )
               << std::endl;
        stream << std::endl;

        for( auto it : m_objectRecords )
        {
            auto &r = it.second;
            auto object = r.object;

            const char *debugName = typeid( *object ).name();

            stream << std::endl;

            auto address = (size_t)object;
            auto references = object->getReferences();
            stream << "DebugName: " << debugName << " ref count: " << references
                   << " address: " << address << std::endl;
            stream << std::endl;
        }

        auto path = String( "ObjectTracker.log" );

        std::fstream fs;
        fs.open( path, std::fstream::out );
        fs << stream.str();
        fs.close();
    }

    void ObjectTracker::addRef( [[maybe_unused]] ISharedObject *object, [[maybe_unused]] void *address,
                                [[maybe_unused]] const char *file, [[maybe_unused]] const u32 line,
                                [[maybe_unused]] const char *func )
    {
#if FB_TRACK_REFERENCES
        if( object->getEnableReferenceTracking() )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( !m_logReferences )
            {
                return;
            }

            std::stringstream strStream;

#    if FB_ENABLE_TRACE
            strStream << boost::stacktrace::stacktrace();
#    endif

            auto it = m_objectRecords.find( object );
            if( it != m_objectRecords.end() )
            {
                auto &r = it->second;
                ReferenceRecord referenceRecord( address, file, line, func, strStream.str() );
                r.referenceRecords[address] = referenceRecord;
            }
        }
#endif
    }

    void ObjectTracker::removeRef( [[maybe_unused]] ISharedObject *object,
                                   [[maybe_unused]] void *address, [[maybe_unused]] const char *file,
                                   [[maybe_unused]] const u32 line, [[maybe_unused]] const char *func )
    {
#if FB_TRACK_REFERENCES
        if( object->getEnableReferenceTracking() )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( !m_logReferences )
            {
                return;
            }

            auto it = m_objectRecords.find( object );
            if( it != m_objectRecords.end() )
            {
                auto &r = it->second;

                auto count = 0;
                for( auto it : r.referenceRecords )
                {
                    auto &reference = it.second;

                    if( reference.address == address )
                    {
                        auto recordsIt = r.referenceRecords.begin();
                        std::advance( recordsIt, count );

                        r.referenceRecords.erase( recordsIt );
                        return;
                    }

                    count++;
                }
            }
        }
#endif
    }

    void ObjectTracker::addObjectRecord( ISharedObject *object )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        std::stringstream strStream;

#if FB_ENABLE_TRACE
        strStream << boost::stacktrace::stacktrace();
#endif

        ObjectRecord r( nullptr, "", 0, "", strStream.str() );
        r.object = object;
        m_objectRecords[object] = r;
    }

    void ObjectTracker::removeObjectRecord( ISharedObject *object )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto it = m_objectRecords.find( object );
        if( it != m_objectRecords.end() )
        {
            m_objectRecords.erase( it );
        }
    }

    void ObjectTracker::addObject( void *address, const c8 *file, const u32 line, const c8 *func )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( !m_logReferences )
        {
            return;
        }

        auto it = m_objectRecords.find( address );
        if( it != m_objectRecords.end() )
        {
            std::stringstream strStream;

#if FB_ENABLE_TRACE
            strStream << boost::stacktrace::stacktrace();
#endif

            ObjectRecord r( address, file, line, func, strStream.str() );
            m_objectRecords[address] = r;
        }
    }

    void ObjectTracker::removeObject( void *address )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto it = m_objectRecords.find( address );
        if( it != m_objectRecords.end() )
        {
            m_objectRecords.erase( it );
        }
    }

    void ObjectTracker::threadFunc()
    {
        while( instance().m_isRunning )
        {
            bool dumpReport = false;
            if( dumpReport )
            {
                // long stackSize = 0;
                // get_StackCommitSize(&stackSize);

                instance().dumpReport();
                dumpReport = false;
            }

            Thread::sleep( 2.0 );
        }
    }

    auto ObjectTracker::instance() -> ObjectTracker &
    {
        static ObjectTracker objectTracker;
        return objectTracker;
    }

    ObjectTracker::ObjectRecord::ObjectRecord( void *address, const char *file, const u32 line,
                                               const char *func, const String &s ) :
        address( address ),
        sourceFile( file ),
        sourceFunc( func ),
        stack( s ),
        sourceLine( line )
    {
    }

    ObjectTracker::ObjectRecord::ObjectRecord() = default;

    ObjectTracker::ReferenceRecord::ReferenceRecord( void *address, const char *file, const u32 line,
                                                     const char *func, const String &s ) :
        address( address ),
        sourceFile( file ),
        sourceFunc( func ),
        stack( s ),
        sourceLine( line )
    {
    }

    ObjectTracker::ReferenceRecord::ReferenceRecord() = default;
}  // end namespace fb
