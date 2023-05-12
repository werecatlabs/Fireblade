#ifndef ThreadDebug_h__
#define ThreadDebug_h__

#include <FBCore/FBCoreTypes.h>

namespace fb
{
    class ThreadDebug
    {
    public:
        class Call
        {
        public:
            Call( ThreadDebug *debug, bool bWrite, const char *file = __FILE__,
                  const char *function = nullptr, unsigned int line = __LINE__ );
            ~Call();

            u32 getReadThreadId() const;
            void setReadThreadId( u32 val );

            bool getWrite() const;
            void setWrite( bool val );

        protected:
            ThreadDebug *m_debug = nullptr;
            u32 m_readThreadId = 0;
            bool m_bWrite = false;
            const char *m_file;
            const char *m_function;
            unsigned int m_line = 0;
        };

        ThreadDebug();
        ~ThreadDebug();

        u32 getWriteThreadId() const;
        void setWriteThreadId( u32 val );

    protected:
        u32 m_writeThreadId = 0;
    };
}  // end namespace fb

#ifdef _DEBUG
#    define THREAD_DEBUG( name ) fb::ThreadDebug name;
#    define THREAD_DEBUG_MUTABLE( name ) mutable fb::ThreadDebug name;
#    define THREAD_WRITE( name ) \
        fb::ThreadDebug::Call threadFunctionCall( &name, true, __FILE__, __FUNCTION__, __LINE__ );
#    define THREAD_READ( name ) \
        fb::ThreadDebug::Call threadFunctionCall( &name, false, __FILE__, __FUNCTION__, __LINE__ );
#else
#    define THREAD_DEBUG( name )
#    define THREAD_DEBUG_MUTABLE( name )
#    define THREAD_WRITE( name )
#    define THREAD_READ( name )
#endif

#endif  // ThreadDebug_h__
