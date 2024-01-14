#ifndef IDebugCircle_h__
#define IDebugCircle_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace render
    {
        /** Interface for a debug line. */
        class IDebugCircle : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IDebugCircle() override = default;

            virtual hash_type getId() const = 0;

            virtual void setId( hash_type id ) = 0;

            virtual Vector3<real_Num> getPosition() const = 0;
            virtual void setPosition( const Vector3<real_Num> &position ) = 0;

            virtual Quaternion<real_Num> getOrientation() const = 0;

            virtual void setOrientation( const Quaternion<real_Num> &orientation ) = 0;

            /** Gets the line life time. */
            virtual time_interval getLifeTime() const = 0;

            /** Sets the line life time. */
            virtual void setLifeTime( time_interval lifeTime ) = 0;

            /** Gets the line max life time. */
            virtual time_interval getMaxLifeTime() const = 0;

            /** Sets the line max life time. */
            virtual void setMaxLifeTime( time_interval maxLifeTime ) = 0;

            /** Gets if the debug line is visible.
            @return A boolean indicating the visibility.
            */
            virtual bool isVisible() const = 0;

            /** Sets if the debug line is visible.
            @param visible A boolean to set the visibility.
             */
            virtual void setVisible( bool visible ) = 0;

            virtual real_Num getRadius() const = 0;
            virtual void setRadius( real_Num radius ) = 0;

            virtual u32 getColor() const = 0;
            virtual void setColor( u32 color ) = 0;

            virtual bool isDirty() const = 0;
            virtual void setDirty( bool dirty ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace render
}  // namespace fb

#endif  // IDebugCircle_h__
