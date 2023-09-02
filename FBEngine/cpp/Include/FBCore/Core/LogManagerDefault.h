#ifndef __FB_Application_LogManager_h__
#define __FB_Application_LogManager_h__

#include <FBCore/Interface/System/ILogManager.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/SmartPtr.h>
#include <fstream>
#include <sstream>
#include <map>

namespace fb
{
    class LogManagerDefault : public ILogManager
    {
    public:
        LogManagerDefault() = default;
        ~LogManagerDefault() override = default;

        void logMessage( const String &message, Type type ) override;
        void logMessage( const StringW &message, Type type ) override;

        bool getEnableQueue() const override;
        void setEnableQueue( bool queue ) override;

        void open( const String &filePath ) override;
        void open( const StringW &filePath ) override;

        void close() override;
        void flush() override;

        FB_CLASS_REGISTER_DECL;

    protected:
        std::ofstream m_log;

        bool m_queue = false;
        bool m_clearLog = true;

        ConcurrentQueue<String> m_messages;

        static SmartPtr<LogManagerDefault> m_instance;
    };
}  // end namespace fb

#endif  // __FB_LogManager_h__
