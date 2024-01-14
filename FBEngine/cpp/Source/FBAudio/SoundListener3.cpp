#include "FBAudio/SoundListener3.h"
#include <FBCore/FBCore.h>

namespace fb
{

    SoundListener3::SoundListener3() :
        m_vCurPosition( Vector3F( 0.f, 0.f, 0.f ) ),
        m_vVelocity( Vector3F( 0.f, 0.f, 0.f ) ),
        m_vForwardVector( Vector3F( 0.f, 0.f, 0.f ) )
    {
    }

    SoundListener3::~SoundListener3()
    {
    }

    void SoundListener3::setPosition( const Vector3F &position )
    {
        m_vCurPosition = position;
    }

    Vector3F SoundListener3::getPosition() const
    {
        return m_vCurPosition;
    }

    void SoundListener3::setForwardVector( const Vector3F &vForwardVector )
    {
        m_vForwardVector = vForwardVector;
    }

    Vector3F SoundListener3::getForwardVector() const
    {
        return m_vForwardVector;
    }

    void SoundListener3::setVelocity( const Vector3F &velocity )
    {
        m_vVelocity = velocity;
    }

    Vector3F SoundListener3::setVelocity() const
    {
        return m_vVelocity;
    }

}  // end namespace fb
