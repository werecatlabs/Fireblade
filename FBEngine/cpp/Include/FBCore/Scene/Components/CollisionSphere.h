#ifndef CollisionSphere_h__
#define CollisionSphere_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Scene/Components/Collision.h>

namespace fb
{
    namespace scene
    {
        
        class CollisionSphere : public Collision
        {
        public:
            CollisionSphere();
            ~CollisionSphere() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CollisionSphere_h__
