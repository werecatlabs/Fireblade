#ifndef IPhysicsDebug_h__
#define IPhysicsDebug_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/ColourF.h>

namespace fb
{
    namespace physics
    {
        //-------------------------------------------------
        class IPhysicsDebug : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IPhysicsDebug() override = default;

            /** */
            virtual void drawLine( const Vector3F &start, const Vector3F &end,
                                   const ColourF &color ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsDebug_h__
