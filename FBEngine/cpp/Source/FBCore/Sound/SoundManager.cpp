#include <FBCore/FBCorePCH.h>
#include <FBCore/Sound/SoundManager.h>
#include <FBCore/Interface/Sound/ISound.h>
#include <FBCore/Interface/Sound/ISound2.h>
#include <FBCore/Interface/Sound/ISound3.h>
#include <FBCore/Interface/Sound/ISoundListener3.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    SoundManager::SoundManager()
    {
    }

    SoundManager::~SoundManager()
    {
    }

    SmartPtr<ISound> SoundManager::addSound( const String &name, bool loop /*= true */ )
    {
        return nullptr;
    }

    SmartPtr<ISound2> SoundManager::addSound2( const String &name, bool loop /*= true */ )
    {
        return nullptr;
    }

    SmartPtr<ISound3> SoundManager::addSound3( const String &name,
                                                const Vector3F &position /*= Vector3F::zero()*/,
                                                bool loop /*= true */ )
    {
        return nullptr;
    }

    SmartPtr<ISoundListener3> SoundManager::addListener3(
        const String &name, const Vector3F &position /*= Vector3F::zero() */ )
    {
        return nullptr;
    }

    SmartPtr<ISoundListener3> SoundManager::findListener3( const String &name )
    {
        return nullptr;
    }

    void SoundManager::setVolume( f32 volume )
    {
    }

    f32 SoundManager::getVolume() const
    {
        return 0.0f;
    }

    void SoundManager::startRecording()
    {
    }

    void SoundManager::stopRecording()
    {
    }

    u32 SoundManager::getBufferSize() const
    {
        return 0;
    }

    void SoundManager::copyContentsToMemory( void *buffer, u32 size )
    {
    }

    bool SoundManager::isRealtime() const
    {
        return false;
    }

    bool SoundManager::isMute() const
    {
        return false;
    }

    void SoundManager::setMute( bool mute )
    {
    }

    SmartPtr<IResource> SoundManager::create( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> SoundManager::create( const String &uuid, const String &name )
    {
        return nullptr;
    }

    Pair<SmartPtr<IResource>, bool> SoundManager::createOrRetrieve( const String &uuid,
                                                                     const String &path,
                                                                     const String &type )
    {
        return Pair<SmartPtr<IResource>, bool>();
    }

    Pair<SmartPtr<IResource>, bool> SoundManager::createOrRetrieve( const String &path )
    {
        return Pair<SmartPtr<IResource>, bool>();
    }

    void SoundManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
    {
    }

    SmartPtr<IResource> SoundManager::loadFromFile( const String &filePath )
    {
        return nullptr;
    }

    SmartPtr<IResource> SoundManager::load( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> SoundManager::getByName( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> SoundManager::getById( const String &uuid )
    {
        return nullptr;
    }

    void SoundManager::_getObject( void **ppObject ) const
    {
    }
}  // namespace fb
