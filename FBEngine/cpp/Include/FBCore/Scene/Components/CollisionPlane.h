#ifndef CollisionPlane_h__
#define CollisionPlane_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Scene/Components/Collision.h>

namespace fb
{
    namespace scene
    {
        
        class CollisionPlane : public Collision
        {
        public:
            CollisionPlane();
            ~CollisionPlane() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CollisionPlane_h__
