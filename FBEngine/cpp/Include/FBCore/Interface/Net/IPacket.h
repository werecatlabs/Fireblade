#ifndef _IPacket_H
#define _IPacket_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{

    class IPacket : public ISharedObject
    {
    public:
        /// Virtual destructor
        ~IPacket() override = default;

        virtual void read( s8 &value ) = 0;
        virtual void read( u8 &value ) = 0;

        virtual void read( u16 &value ) = 0;
        virtual void read( s16 &value ) = 0;

        virtual void read( u32 &value ) = 0;
        virtual void read( s32 &value ) = 0;
        virtual void read( f32 &value ) = 0;

        virtual void read( Vector2I &value ) = 0;
        virtual void read( Vector2F &value ) = 0;
        virtual void read( Vector3I &value ) = 0;
        virtual void read( Vector3F &value ) = 0;

        virtual void read( String &value ) = 0;

        virtual void read( bool &value ) = 0;

        virtual void write( s8 value ) = 0;
        virtual void write( u8 value ) = 0;

        virtual void write( u16 value ) = 0;
        virtual void write( s16 value ) = 0;

        virtual void write( u32 value ) = 0;
        virtual void write( s32 value ) = 0;
        virtual void write( f32 value ) = 0;

        virtual void write( const Vector2I &value ) = 0;
        virtual void write( const Vector2F &value ) = 0;
        virtual void write( const Vector3I &value ) = 0;
        virtual void write( const Vector3F &value ) = 0;

        virtual void write( const String &value ) = 0;

        virtual void write( const bool &value ) = 0;

        virtual SmartPtr<ISystemAddress> getSystemAddress() const = 0;
    };

}  // end namespace fb

#endif
