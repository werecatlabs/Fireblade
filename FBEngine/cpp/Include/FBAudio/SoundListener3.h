#ifndef _SoundListener3_H
#define _SoundListener3_H

#include <FBCore/Interface/Sound/ISoundListener3.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class SoundListener3 : public ISoundListener3
    {
    public:
        SoundListener3();
        ~SoundListener3();

        void setPosition( const Vector3F &position );
        Vector3F getPosition() const;

        void setForwardVector( const Vector3F &vForwardVector );
        Vector3F getForwardVector() const;

        void setVelocity( const Vector3F &velocity );
        Vector3F setVelocity() const;

    private:
        Vector3F m_vCurPosition;
        Vector3F m_vVelocity;
        Vector3F m_vForwardVector;
    };

}  // end namespace fb

#endif
