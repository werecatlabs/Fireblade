#ifndef __DebugCheck_h__
#define __DebugCheck_h__

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Base/List.h>
#include <FBCore/Base/Map.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Thread/RecursiveMutex.h>

namespace fb
{
    /** Used to check race conditions and heap corruption. */
    class DebugTrace : public CSharedObject<ISharedObject>
    {
    public:
        class DebugCheckFunction
        {
        public:
            DebugCheckFunction();
            DebugCheckFunction( DebugTrace *debugCheck );
            ~DebugCheckFunction();

            RawPtr<DebugTrace> m_debugCheck;
        };

        DebugTrace();
        DebugTrace( bool bCheckHeap = false, bool bCheckGrowth = false );
        ~DebugTrace() override;

        bool getEnableConsoleOutput() const;
        void setEnableConsoleOutput( bool val );

        size_t getVirtualMemUsed() const;
        void setVirtualMemUsed( size_t val );

        size_t getPhysMemUsed() const;
        void setPhysMemUsed( size_t val );

        bool getCheckHeap() const;
        void setCheckHeap( bool val );

        bool getCheckGrowth() const;
        void setCheckGrowth( bool val );

        void handleFunctionStart();
        void handleFunctionEnd();

        String getSourceFile() const;
        void setSourceFile( const String &val );

        String getFunctionName() const;
        void setFunctionName( const String &val );

        s32 getLineNumber() const;
        void setLineNumber( s32 val );

        void writeTrace();

    protected:
        static void addDebugObject( RawPtr<DebugTrace> pDebugCheck );
        static void removeDebugObject( RawPtr<DebugTrace> pDebugCheck );

        bool m_bCheckHeap = false;
        bool m_bCheckGrowth = false;
        bool m_bEnableConsoleOutput = false;
        size_t m_virtualMemUsed = 0;
        size_t m_physMemUsed = 0;
        atomic_s32 m_currentTask = 0;
        String m_sourceFile;
        String m_functionName;
        s32 m_lineNumber = 0;
        f64 m_startTime = 0.0;
        f64 m_endTime = 0.0;

        static Map<String, List<RawPtr<DebugTrace>>> m_debugObjects;
        static RecursiveMutex m_debugObjectsMutex;
    };
}  // end namespace fb

#if FB_ENABLE_DEBUG_TRACE
#    if FB_ENABLE_HEAP_DEBUG
#        define FB_DEBUG_TRACE \
            DebugTrace debugCheck( true, false ); \
            debugCheck.setSourceFile( __FILE__ ); \
            debugCheck.setFunctionName( __FUNCTION__ ); \
            debugCheck.setLineNumber( __LINE__ ); \
            debugCheck.writeTrace();
#    else
#        define FB_DEBUG_TRACE \
            DebugTrace debugCheck( false, false ); \
            debugCheck.setSourceFile( __FILE__ ); \
            debugCheck.setFunctionName( __FUNCTION__ ); \
            debugCheck.setLineNumber( __LINE__ ); \
            debugCheck.writeTrace();
#    endif
#else
#    define FB_DEBUG_TRACE
#    define FB_DEBUG_TRACE_FUNCTION
#endif

#endif  // DebugCheck_h__
