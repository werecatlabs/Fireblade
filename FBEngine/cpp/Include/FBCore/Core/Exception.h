#ifndef __FBException_H_
#define __FBException_H_

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/StringTypes.h>
#include <exception>

namespace fb
{
    /** Default constructor. */
    class FBCore_API Exception : public std::exception
    {
    public:
        /** Default constructor. */
        Exception();

        /** Constructor. */
        Exception( const String &description, const char *file = __FILE__, s32 line = __LINE__,
                   const char *func = nullptr );

        /** Destructor. */
        ~Exception() override;

    protected:
        s32 m_line = 0;
        String m_typeName;
        String m_description;
        String m_source;
        String m_file;
        String m_fullDesc;
    };

    class RuntimeException : public Exception
    {
    public:
        RuntimeException( const String &description, const char *file = __FILE__, long line = __LINE__,
                          const char *func = nullptr );
    };

    class FatalException : public Exception
    {
    public:
        FatalException( const String &description, const char *file = __FILE__, long line = __LINE__,
                        const char *func = nullptr );
    };

    class ScriptException : public Exception
    {
    public:
        ScriptException( const String &description, const char *file = __FILE__, long line = __LINE__,
                         const char *func = nullptr );
    };
}  // end namespace fb

#if FB_EXCEPTIONS
#    define FB_EXCEPTION( desc ) \
        throw fb::RuntimeException( String( desc ), __FILE__, __LINE__, __FUNCTION__ )
#    define FB_FATAL_EXCEPTION( desc ) \
        throw fb::FatalException( String( desc ), __FILE__, __LINE__, __FUNCTION__ )
#    define FB_SCRIPT_EXCEPTION( desc ) \
        throw fb::ScriptException( String( desc ), __FILE__, __LINE__, __FUNCTION__ )
#else
#    define FB_EXCEPTION( desc )
#    define FB_FATAL_EXCEPTION( desc )
#    define FB_SCRIPT_EXCEPTION( desc )
#endif

#endif
