#ifndef __FB_ObjectTracker_H_
#define __FB_ObjectTracker_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/List.h>
#include <FBCore/Base/Map.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Thread/RecursiveMutex.h>
#include <thread>
#include <vector>
#include <map>
#include <mutex>

namespace fb
{
    //-------------------------------------------------
    // this class lets the user know what has a reference to a reference counted object
    class FBCore_API ObjectTracker
    {
    public:
        ObjectTracker();
        ~ObjectTracker();

        void dumpReport( ISharedObject *object, const String &filePath );

        void dumpReport( ISharedObject *object );
        void dumpReport( SmartPtr<ISharedObject> object );
        void dumpReport();

        void addRef( ISharedObject *object, void *address, const c8 *file, u32 line, const c8 *func );
        void removeRef( ISharedObject *object, void *address, const c8 *file, u32 line, const c8 *func );
        void addObjectRecord( ISharedObject *object );
        void removeObjectRecord( ISharedObject *object );

        void addObject( void *address, const c8 *file, u32 line, const c8 *func );
        void removeObject( void *address );
        void dumpObjectReport();

        static ObjectTracker &instance();

    private:
        struct ReferenceRecord
        {
            ReferenceRecord();

            ReferenceRecord( void *address, const char *file, u32 line, const char *func,
                             const String &s );

            void *address = nullptr;
            String sourceFile;
            String sourceFunc;
            String stack;
            u32 sourceLine = 0;
        };

        struct ObjectRecord
        {
            ObjectRecord();
            ObjectRecord( void *address, const char *file, u32 line, const char *func, const String &s );

            ISharedObject *object = nullptr;
            void *address = nullptr;
            String sourceFile;
            String sourceFunc;
            String stack;
            u32 sourceLine = 0;
            Map<void *, ReferenceRecord> referenceRecords;
        };

        static void threadFunc( void );

        ///
        bool m_logReferences = true;

        ///
        atomic_bool m_isRunning = true;

        ///
        std::thread *m_thread = nullptr;

        ///
        Map<void *, ObjectRecord> m_objectRecords;

        mutable RecursiveMutex m_mutex;
    };
}  // end namespace fb

#endif
