#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Memory/IObject.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Memory/ObjectTracker.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Handle.h>

#if FB_ENABLE_TRACE
#    include <boost/stacktrace.hpp>
#endif

#if FB_ENABLE_MEMORY_TRACKER
#    include <FBCore/Memory/MemoryTracker.h>
#endif

namespace fb
{
    FB_CLASS_REGISTER( fb, IObject );

    void IObject::preUpdate()
    {
    }

    void IObject::update()
    {
    }

    void IObject::postUpdate()
    {
    }

    void IObject::setUserData( hash32 id, void *userData )
    {
    }

    void IObject::setUserData( void *data )
    {
    }

    bool IObject::isValid() const
    {
        return true;
    }

    void *IObject::getCreatorData() const
    {
        return nullptr;
    }

    void IObject::setCreatorData( void *data )
    {
    }

    hash32 IObject::getFactoryData() const
    {
        return 0;
    }

    void IObject::setFactoryData( hash32 data )
    {
    }

    String IObject::toString() const
    {
        return "";
    }

    void *IObject::getUserData( hash32 id ) const
    {
        return nullptr;
    }

    void *IObject::getUserData() const
    {
        return nullptr;
    }

    bool IObject::derived( u32 type ) const
    {
        auto typeManager = TypeManager::instance();

        auto typeInfo = getTypeInfo();
        return typeManager->isDerived( typeInfo, type );
    }

    bool IObject::exactly( u32 type ) const
    {
        auto typeManager = TypeManager::instance();

        auto typeInfo = getTypeInfo();
        return typeManager->isExactly( typeInfo, type );
    }

#if FB_USE_CUSTOM_NEW_DELETE

    void *IObject::operator new( size_t sz )
    {
        void *ptr = nullptr;

#    if FB_OBJECT_ALIGNED_ALLOC
        ptr = Memory::ScalableAlignedMalloc( sz, FB_ALIGNMENT );
#    else
        ptr = Memory::malloc( sz );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_64
#            if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        backtraceMsg << boost::stacktrace::stacktrace();
        auto msg = backtraceMsg.str();
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, msg.c_str() );
#            else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, "" );
#            endif
#        else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, "" );
#        endif
#    endif

        return ptr;
    }

    void *IObject::operator new( size_t sz, void *ptr )
    {
        (void)sz;
        return ptr;
    }

    void *IObject::operator new( size_t sz, [[maybe_unused]] const c8 *file, [[maybe_unused]] s32 line,
                                 [[maybe_unused]] const c8 *func )
    {
        void *ptr = nullptr;

#    if FB_OBJECT_ALIGNED_ALLOC
        ptr = Memory::ScalableAlignedMalloc( sz, FB_ALIGNMENT );
#    else
        ptr = Memory::malloc( sz );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_64
#            if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        backtraceMsg << boost::stacktrace::stacktrace();
        auto msg = backtraceMsg.str();
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, msg.c_str() );
#            else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, "" );
#            endif
#        else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, "" );
#        endif
#    endif

        return ptr;
    }

    void *IObject::operator new( size_t sz, void *ptr, [[maybe_unused]] const c8 *file,
                                 [[maybe_unused]] s32 line, [[maybe_unused]] const c8 *func )
    {
        (void)sz;

#    if FB_ENABLE_MEMORY_TRACKER
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_64
#            if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        backtraceMsg << boost::stacktrace::stacktrace();
        auto msg = backtraceMsg.str();
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, msg.c_str() );
#            else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, "" );
#            endif
#        else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, "" );
#        endif
#    endif

        return ptr;
    }

    void *IObject::operator new[]( size_t sz, [[maybe_unused]] const char *file,
                                   [[maybe_unused]] int line, [[maybe_unused]] const char *func )
    {
        void *ptr = nullptr;

#    if FB_OBJECT_ALIGNED_ALLOC
        ptr = Memory::ScalableAlignedMalloc( sz, FB_ALIGNMENT );
#    else
        ptr = Memory::malloc( sz );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line );
#    endif

        return ptr;
    }

    void *IObject::operator new[]( size_t sz )
    {
        void *ptr = nullptr;

#    if FB_OBJECT_ALIGNED_ALLOC
        ptr = Memory::ScalableAlignedMalloc( sz, FB_ALIGNMENT );
#    else
        ptr = Memory::malloc( sz );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_64
#            if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        backtraceMsg << boost::stacktrace::stacktrace();
        auto msg = backtraceMsg.str();
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, msg.c_str() );
#            else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, "" );
#            endif
#        endif
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, "" );
#    endif
        return ptr;
    }

    void IObject::operator delete( void *ptr )
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    void IObject::operator delete( void *ptr, void * )
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    void IObject::operator delete[]( void *ptr )
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    void IObject::operator delete( void *ptr, [[maybe_unused]] const char *file,
                                   [[maybe_unused]] int line, [[maybe_unused]] const char *func ) throw()
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    void IObject::operator delete( void *ptr, void *, const char *file, int line, const char *func )
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    void IObject::operator delete[]( void *ptr, const char *file, int line, const char *func ) throw()
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }
#endif
}  // end namespace fb
