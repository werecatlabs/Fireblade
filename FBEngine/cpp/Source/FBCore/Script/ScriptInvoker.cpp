#include <FBCore/FBCorePCH.h>
#include <FBCore/Script/ScriptInvoker.h>
#include <FBCore/Interface/Script/IScriptManager.h>
#include <FBCore/Interface/Script/IScriptEvent.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{

    ScriptInvoker::ScriptInvoker() : m_object( nullptr )
    {
    }

    ScriptInvoker::ScriptInvoker( SmartPtr<ISharedObject> scriptObject ) :
        m_object( scriptObject )
    {
    }

    ScriptInvoker::~ScriptInvoker()
    {
        unload( nullptr );
    }

    void ScriptInvoker::callObjectMember( const String &functionName )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto scriptManager = applicationManager->getScriptManager();

        if( auto object = getOwner() )
        {
            if( auto scriptData = object->getScriptData() )
            {
                scriptManager->callObjectMember( object, functionName );
            }
        }
    }

    void ScriptInvoker::callObjectMember( const String &functionName, const Parameters &params )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto scriptManager = applicationManager->getScriptManager();

        if( auto object = getOwner() )
        {
            if( auto scriptData = object->getScriptData() )
            {
                scriptManager->callObjectMember( object, functionName, params );
            }
        }
    }

    void ScriptInvoker::callObjectMember( const String &functionName, const Parameters &params,
                                                  Parameters &results )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto scriptManager = applicationManager->getScriptManager();
        if( auto object = getOwner() )
        {
            if( auto scriptData = object->getScriptData() )
            {
                scriptManager->callObjectMember( object, functionName, params, results );
            }
        }
    }

    void ScriptInvoker::event( hash_type hash )
    {
        THREAD_WRITE( m_threadDebug );

        auto it = m_events.find( hash );
        if( it != m_events.end() )
        {
            const auto &event = it->second;

            auto applicationManager = core::IApplicationManager::instance();
            auto scriptManager = applicationManager->getScriptManager();
            if( !scriptManager )
            {
                auto message = String( "No script manager found. When on event class: " ) +
                               event->getClassName() + String( " function: " ) + event->getFunction();
                FB_LOG_ERROR( message );
            }

            if( m_object && m_object->getScriptData() )
            {
                scriptManager->callObjectMember( m_object, event->getFunction() );
            }
            else
            {
                scriptManager->callMember( event->getClassName(), event->getFunction() );
            }
        }
    }

    void ScriptInvoker::event( hash_type hash, const Parameters &params )
    {
        THREAD_WRITE( m_threadDebug );

        auto it = m_events.find( hash );
        if( it != m_events.end() )
        {
            const auto &event = it->second;

            auto applicationManager = core::IApplicationManager::instance();
            auto scriptManager = applicationManager->getScriptManager();
            if( !scriptManager )
            {
                auto message = String( "No script manager found. When on event class: " ) +
                               event->getClassName() + String( " function: " ) + event->getFunction();

                FB_LOG_ERROR( message );
            }

            if( m_object && m_object->getScriptData() )
            {
                scriptManager->callObjectMember( m_object, event->getFunction(), params );
            }
            else
            {
                scriptManager->callMember( event->getClassName(), event->getFunction(), params );
            }
        }
    }

    void ScriptInvoker::event( hash_type hash, const Parameters &params, Parameters &results )
    {
        THREAD_WRITE( m_threadDebug );

        auto it = m_events.find( hash );
        if( it != m_events.end() )
        {
            const auto &event = it->second;

            auto applicationManager = core::IApplicationManager::instance();
            auto scriptManager = applicationManager->getScriptManager();
            if( !scriptManager )
            {
                auto message = String( "No script manager found. When on event class: " ) +
                               event->getClassName() + String( " function: " ) + event->getFunction();
                FB_LOG_ERROR( message );
            }

            if( m_object && m_object->getScriptData() )
            {
                scriptManager->callObjectMember( m_object, event->getFunction(), params, results );
            }
            else
            {
                scriptManager->callMember( event->getClassName(), event->getFunction(), params,
                                           results );
            }
        }
    }

    bool ScriptInvoker::hasEvent( hash_type hash ) const
    {
        THREAD_READ( m_threadDebug );

        auto it = m_events.find( hash );
        if( it != m_events.end() )
        {
            return true;
        }

        return false;
    }

    void ScriptInvoker::setEventFunction( hash_type hash, SmartPtr<IEvent> event )
    {
        THREAD_WRITE( m_threadDebug );
        m_events[hash] = event;
    }

    SmartPtr<IEvent> ScriptInvoker::getEventFunction( hash_type hash ) const
    {
        THREAD_READ( m_threadDebug );
        auto it = m_events.find( hash );
        if( it != m_events.end() )
        {
            return it->second;
        }

        return nullptr;
    }

    void ScriptInvoker::set( hash_type hash, const Parameter &param )
    {
        auto it = m_events.find( hash );
        if( it != m_events.end() )
        {
            const auto &event = it->second;

            auto engine = core::IApplicationManager::instance();
            auto scriptMgr = engine->getScriptManager();
            if( !scriptMgr )
            {
                FB_LOG( "No script manager found." );
            }

            Parameters params( 3 );
            params.reserve( 3 );
            params[0].setPtr( m_object.get() );
            params[1] = param;

            scriptMgr->callMember( event->getClassName(), event->getFunction(), params );
        }
        else
        {
            FB_LOG_ERROR( "Script event not found." );
        }
    }

    void ScriptInvoker::set( const String &id, const Parameter &param )
    {
        auto hash = StringUtil::getHash( id );
        set( hash, param );
    }

    Parameter ScriptInvoker::get( hash_type hash )
    {
        auto it = m_events.find( hash );
        if( it != m_events.end() )
        {
            const auto &event = it->second;

            auto engine = core::IApplicationManager::instance();
            auto scriptMgr = engine->getScriptManager();
            if( !scriptMgr )
            {
                FB_LOG( "No script manager found." );
                return Parameter::VOID_PARAM;
            }

            Parameters params;
            params[0].setPtr( m_object.get() );

            Parameters results;
            scriptMgr->callMember( event->getClassName(), event->getFunction(), params, results );

            return !results.empty() ? results[0] : Parameter::VOID_PARAM;
        }

        FB_LOG_MESSAGE( "Script", "Script event not found." );

        return Parameter::VOID_PARAM;
    }

    Parameter ScriptInvoker::get( const String &id )
    {
        THREAD_READ( m_threadDebug );
        auto hash = StringUtil::getHash( id );
        return get( hash );
    }

    u32 ScriptInvoker::getNumEvents() const
    {
        THREAD_READ( m_threadDebug );
        return static_cast<u32>( m_events.size() );
    }

    void ScriptInvoker::setOwner( SmartPtr<ISharedObject> owner )
    {
        THREAD_WRITE( m_threadDebug );
        m_object = owner;
    }

    SmartPtr<ISharedObject> ScriptInvoker::getOwner() const
    {
        THREAD_READ( m_threadDebug );
        return m_object;
    }

}  // end namespace fb
