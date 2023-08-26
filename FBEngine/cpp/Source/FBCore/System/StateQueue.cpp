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

    u32 StateQueue::getTaskId() const
    {
        return m_taskId;
    }

    void StateQueue::setTaskId( u32 taskId )
    {
        m_taskId = taskId;
    }

    bool StateQueue::isEmpty() const
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

    SmartPtr<IStateContext> StateQueue::getOwner() const
    {
        return m_owner;
    }

    void StateQueue::setOwner( SmartPtr<IStateContext> val )
    {
        m_owner = val;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> StateQueue::getMessages() const
    {
        return m_messageQueue;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> StateQueue::getMessagesAndClear()
    {
        auto messageQueue = boost::make_shared<ConcurrentArray<SmartPtr<IStateMessage>>>();
        return m_messageQueue.exchange( messageQueue );
    }

    void StateQueue::setMessageQueue( SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> messageQueue )
    {
        m_messageQueue = messageQueue;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateMessage>>> StateQueue::getMessageQueue() const
    {
        return m_messageQueue;
    }
}  // end namespace fb
