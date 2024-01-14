#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateQueue.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateQueue, IStateQueue );

    StateQueue::StateQueue() : IStateQueue()
    {
    }

    StateQueue::StateQueue( const StateQueue &other )
    {
    }

    StateQueue::~StateQueue()
    {
        unload( nullptr );
    }

    void StateQueue::unload( SmartPtr<ISharedObject> data )
    {
        if( auto pMessageQueue = getMessagesAndClear() )
        {
            auto &messageQueue = *pMessageQueue;
            for( auto &message : messageQueue )
            {
                message->unload( nullptr );
            }
        }

        m_messageQueue = nullptr;
    }

    void StateQueue::update()
    {
        if( auto pMessageQueue = getMessagesAndClear() )
        {
            auto &messageQueue = *pMessageQueue;
            for( auto &message : messageQueue )
            {
                m_owner->sendMessage( message );
            }
        }
    }

    void StateQueue::queueMessage( const SmartPtr<IStateMessage> &message )
    {
        auto pMessageQueue = getMessageQueue();
        if( !pMessageQueue )
        {
            pMessageQueue = fb::make_shared<ConcurrentArray<SmartPtr<IStateMessage>>>();
            setMessageQueue( pMessageQueue );
        }

        if( pMessageQueue )
        {
            auto &messageQueue = *pMessageQueue;
            messageQueue.push_back( message );
        }
    }

    void StateQueue::clear()
    {
        auto messageQueue = fb::make_shared<ConcurrentArray<SmartPtr<IStateMessage>>>();
        setMessageQueue( messageQueue );
    }

    auto StateQueue::getTaskId() const -> u32
    {
        return m_taskId;
    }

    void StateQueue::setTaskId( u32 taskId )
    {
        m_taskId = taskId;
    }

    auto StateQueue::isEmpty() const -> bool
    {
        auto messages = getMessageQueue();
        if( messages )
        {
            return messages->empty();
        }

        return true;
    }

    void StateQueue::addStateUpdate( SmartPtr<IState> state )
    {
    }

    auto StateQueue::getOwner() const -> SmartPtr<IStateContext>
    {
        return m_owner;
    }

    void StateQueue::setOwner( SmartPtr<IStateContext> val )
    {
        m_owner = val;
    }

    auto StateQueue::getMessages() const -> SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>>
    {
        return m_messageQueue;
    }

    auto StateQueue::getMessagesAndClear() -> SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>>
    {
        auto messageQueue = boost::make_shared<ConcurrentArray<SmartPtr<IStateMessage>>>();
        return m_messageQueue.exchange( messageQueue );
    }

    void StateQueue::setMessageQueue( SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> messageQueue )
    {
        m_messageQueue = messageQueue;
    }

    auto StateQueue::getMessageQueue() const -> SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>>
    {
        return m_messageQueue;
    }
}  // end namespace fb
