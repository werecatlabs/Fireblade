#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include "FBGraphicsOgre/Ogre/MaterialAnimator.h"

namespace fb
{
    namespace render
    {

        MaterialAnimator::MaterialAnimator()
        {
        }

        MaterialAnimator::~MaterialAnimator()
        {
        }

        void MaterialAnimator::setId( u32 id )
        {
        }

        u32 MaterialAnimator::getId() const
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

        bool MaterialAnimator::isLoop() const
        {
            return false;
        }

        void MaterialAnimator::setReverse( bool reverse )
        {
        }

        bool MaterialAnimator::isReverse() const
        {
            return false;
        }

        bool MaterialAnimator::isFinished() const
        {
            return false;
        }

        void MaterialAnimator::setAnimationLength( f32 animationLength )
        {
        }

        fb::render::IMaterial *MaterialAnimator::getOwner() const
        {
            return m_owner;
        }

        void MaterialAnimator::setOwner( IMaterial *val )
        {
            m_owner = val;
        }

    }  // namespace render
}  // namespace fb
