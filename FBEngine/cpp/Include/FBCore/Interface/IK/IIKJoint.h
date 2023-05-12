#ifndef _IIKJOINT_H
#define _IIKJOINT_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    
    class IIKJoint : public ISharedObject
    {
    public:
        ~IIKJoint() override = default;

        // joint update
        virtual Vector3F GetAxis( int i ) = 0;
        virtual void UpdateWorldSRT() = 0;
        virtual void UpdateWorldRT() = 0;
        virtual bool UpdateLocalT( int i ) = 0;
        virtual bool UpdateLocalR( int i ) = 0;

        virtual void SetAllowTranslation( int i, bool value ) = 0;
        virtual bool GetAllowTranslation( int i ) const = 0;

        virtual void SetMinTranslation( int i, f32 value ) = 0;
        virtual f32 GetMinTranslation( int i ) const = 0;

        virtual void SetMaxTranslation( int i, f32 value ) = 0;
        virtual f32 GetMaxTranslation( int i ) const = 0;

        virtual void SetAllowRotation( int i, bool value ) = 0;
        virtual bool GetAllowRotation( int i ) const = 0;

        virtual void SetMinRotation( int i, f32 value ) = 0;
        virtual f32 GetMinRotation( int i ) const = 0;

        virtual void SetMaxRotation( int i, f32 value ) = 0;
        virtual f32 GetMaxRotation( int i ) const = 0;
    };

}  // end namespace fb

#endif
