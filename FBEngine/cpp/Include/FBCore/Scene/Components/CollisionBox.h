#ifndef CollisionBox_h__
#define CollisionBox_h__

#include <FBCore/Scene/Components/Collision.h>

namespace fb
{
    namespace scene
    {
        class CollisionBox : public Collision
        {
        public:
            CollisionBox();
            ~CollisionBox() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc Collision::setExtents */
            void setExtents( const Vector3<real_Num> &extents ) override;

            bool isValid() const;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CollisionBox_h__
