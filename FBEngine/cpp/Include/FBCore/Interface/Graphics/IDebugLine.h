#ifndef __IDebugLine_h__
#define __IDebugLine_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    namespace render
    {

        /** Interface for a debug line. */
        class IDebugLine : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IDebugLine() override = default;

            /** Gets the id. */
            virtual hash32 getId() const = 0;

            /** Sets the id. */
            virtual void setId( hash32 id ) = 0;

            /** Gets the line vector. */
            virtual Vector3<real_Num> getVector() const = 0;

            /** Sets the line vector. */
            virtual void setVector( const Vector3<real_Num> &vector ) = 0;

            /** Gets the line position. */
            virtual Vector3<real_Num> getPosition() const = 0;

            /** Sets the line position. */
            virtual void setPosition( const Vector3<real_Num> &position ) = 0;

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

            virtual u32 getColour() const = 0;

            virtual void setColour( u32 colour ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IDebug_h__
