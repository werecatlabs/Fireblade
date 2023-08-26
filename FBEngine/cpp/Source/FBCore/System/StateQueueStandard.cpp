#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateQueueStandard.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateMessage.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateQueueStandard, IStateQueue );

    StateQueueStandard::StateQueueStandard() : IStateQueue()
    {
    }

    StateQueueStandard::StateQueueStandard( const StateQueueStandard &other )
    {
    }

    StateQueueStandard::~StateQueueStandard()
    {
        unload( nullptr );
    }

    void StateQueueStandard::unload( SmartPtr<ISharedObject> data )
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

    void StateQueueStandard::update()
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

    void StateQueueStandard::queueMessage( const SmartPtr<IStateMessage> &message )
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

    void StateQueueStandard::clear()
    {
        auto messageQueue = fb::make_shared<ConcurrentArray<SmartPtr<IStateMessage>>>();
        setMessageQueue( messageQueue );
    }

    u32 StateQueueStandard::getTaskId() const
    {
        return m_taskId;
    }

    void StateQueueStandard::setTaskId( u32 taskId )
    {
        m_taskId = taskId;
    }

    bool StateQueueStandard::isEmpty() const
    {
        auto messages = getMessageQueue();
        if( messages )
        {
            return messages->empty();
        }

        return true;
    }

    void StateQueueStandard::addStateUpdate( SmartPtr<IState> state )
    {
    }

    SmartPtr<IStateContext> StateQueueStandard::getOwner() const
    {
        return m_owner;
    }

    void StateQueueStandard::setOwner( SmartPtr<IStateContext> owner )
    {
        m_owner = owner;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> StateQueueStandard::getMessages() const
    {
        return m_messageQueue;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> StateQueueStandard::getMessagesAndClear()
    {
        auto messageQueue = boost::make_shared<ConcurrentArray<SmartPtr<IStateMessage>>>();
        return m_messageQueue.exchange( messageQueue );
    }

    void StateQueueStandard::setMessageQueue(
        SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> messageQueue )
    {
        m_messageQueue = messageQueue;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> StateQueueStandard::getMessageQueue() const
    {
        return m_messageQueue;
    }
}  // end namespace fb
