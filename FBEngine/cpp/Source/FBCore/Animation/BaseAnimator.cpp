#include <FBCore/FBCorePCH.h>
#include "FBCore/Animation/BaseAnimator.h"
#include <FBCore/Core/StringUtil.h>

namespace fb
{

    u32 BaseAnimator::m_idExt = 0;

    const hash_type BaseAnimator::START_HASH = StringUtil::getHash( "start" );
    const hash_type BaseAnimator::STOP_HASH = StringUtil::getHash( "stop" );

    BaseAnimator::BaseAnimator() :
        m_id( 0 ),
        m_animationLength( 0.0f ),
        m_animationTime( 0.0f ),
        m_isPlaying( false ),
        m_loop( false ),
        m_reverse( false )
    {
        m_id = m_idExt++;
    }

    BaseAnimator::~BaseAnimator()
    {
    }

    void BaseAnimator::setId( u32 id )
    {
        m_id = id;
    }

    u32 BaseAnimator::getId() const
    {
        return m_id;
    }

    void BaseAnimator::setLoop( bool loop )
    {
        m_loop = loop;
    }

    bool BaseAnimator::isLoop() const
    {
        return m_loop;
    }

    void BaseAnimator::setReverse( bool reverse )
    {
        m_reverse = reverse;
    }

    bool BaseAnimator::isReverse() const
    {
        return m_reverse;
    }

    void BaseAnimator::start()
    {
        m_isPlaying = true;
    }

    void BaseAnimator::stop()
    {
        m_isPlaying = false;
    }

    bool BaseAnimator::isPlaying() const
    {
        return m_isPlaying;
    }

    bool BaseAnimator::isFinished() const
    {
        return false;
    }

    void BaseAnimator::setAnimationLength( f32 animationLength )
    {
        m_animationLength = animationLength;
    }

    fb::f32 BaseAnimator::getAnimationLength() const
    {
        return m_animationLength;
    }

    fb::f32 BaseAnimator::getAnimationTime() const
    {
        return m_animationTime;
    }

    void BaseAnimator::setAnimationTime( f32 animationTime )
    {
        m_animationTime = animationTime;
    }

    void BaseAnimator::update()
    {
    }

}  // end namespace fb
