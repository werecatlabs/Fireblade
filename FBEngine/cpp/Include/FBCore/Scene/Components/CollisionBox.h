#ifndef CollisionBox_h__
#define CollisionBox_h__

#include <FBCore/Scene/Components/Collision.h>

namespace fb
{
    namespace scene
    {

        /** Collision box component. */
        class CollisionBox : public Collision
        {
        public:
            /** Constructor. */
            CollisionBox();

            /** Destructor. */
            ~CollisionBox() override;

            /** @copydoc Collision::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Collision::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Collision::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Collision::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Collision::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc Collision::setExtents */
            void setExtents( const Vector3<real_Num> &extents ) override;

            /** @copydoc Collision::isValid */
            bool isValid() const override;

            /** @copydoc Component::updateTransform */
            void updateTransform() override;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CollisionBox_h__
