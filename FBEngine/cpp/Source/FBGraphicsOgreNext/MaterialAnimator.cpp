#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/MaterialAnimator.h>
#include <FBCore/Interface/Graphics/IMaterial.h>

namespace fb::render
{

    MaterialAnimator::MaterialAnimator() = default;

    MaterialAnimator::~MaterialAnimator() = default;

    void MaterialAnimator::setId( u32 id )
    {
    }

    auto MaterialAnimator::getId() const -> u32
    {
        return 0;
    }

    void MaterialAnimator::start()
    {
    }

    void MaterialAnimator::stop()
    {
    }

    void MaterialAnimator::setLoop( bool loop )
    {
    }

    auto MaterialAnimator::isLoop() const -> bool
    {
        return false;
    }

    void MaterialAnimator::setReverse( bool reverse )
    {
    }

    auto MaterialAnimator::isReverse() const -> bool
    {
        return false;
    }

    auto MaterialAnimator::isFinished() const -> bool
    {
        return false;
    }

    void MaterialAnimator::setAnimationLength( f32 animationLength )
    {
    }

    auto MaterialAnimator::getOwner() const -> IMaterial *
    {
        return m_owner;
    }

    void MaterialAnimator::setOwner( IMaterial *val )
    {
        m_owner = val;
    }

}  // namespace fb::render
