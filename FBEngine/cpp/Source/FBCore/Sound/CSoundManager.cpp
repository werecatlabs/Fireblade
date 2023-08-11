#include <FBCore/FBCorePCH.h>
#include <FBCore/Sound/CSoundManager.h>
#include <FBCore/Interface/Sound/ISound.h>
#include <FBCore/Interface/Sound/ISound2.h>
#include <FBCore/Interface/Sound/ISound3.h>
#include <FBCore/Interface/Sound/ISoundListener3.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    CSoundManager::CSoundManager()
    {
    }

    CSoundManager::~CSoundManager()
    {
    }

    SmartPtr<ISound> CSoundManager::addSound( const String &name, bool loop /*= true */ )
    {
        return nullptr;
    }

    SmartPtr<ISound2> CSoundManager::addSound2( const String &name, bool loop /*= true */ )
    {
        return nullptr;
    }

    SmartPtr<ISound3> CSoundManager::addSound3( const String &name,
                                                const Vector3F &position /*= Vector3F::zero()*/,
                                                bool loop /*= true */ )
    {
        return nullptr;
    }

    SmartPtr<ISoundListener3> CSoundManager::addListener3(
        const String &name, const Vector3F &position /*= Vector3F::zero() */ )
    {
        return nullptr;
    }

    SmartPtr<ISoundListener3> CSoundManager::findListener3( const String &name )
    {
        return nullptr;
    }

    void CSoundManager::setVolume( f32 volume )
    {
    }

    f32 CSoundManager::getVolume() const
    {
        return 0.0f;
    }

    void CSoundManager::startRecording()
    {
    }

    void CSoundManager::stopRecording()
    {
    }

    u32 CSoundManager::getBufferSize() const
    {
        return 0;
    }

    void CSoundManager::copyContentsToMemory( void *buffer, u32 size )
    {
    }

    bool CSoundManager::isRealtime() const
    {
        return false;
    }

    bool CSoundManager::isMute() const
    {
        return false;
    }

    void CSoundManager::setMute( bool mute )
    {
    }

    SmartPtr<IResource> CSoundManager::create( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> CSoundManager::create( const String &uuid, const String &name )
    {
        return nullptr;
    }

    Pair<SmartPtr<IResource>, bool> CSoundManager::createOrRetrieve( const String &uuid,
                                                                     const String &path,
                                                                     const String &type )
    {
        return Pair<SmartPtr<IResource>, bool>();
    }

    Pair<SmartPtr<IResource>, bool> CSoundManager::createOrRetrieve( const String &path )
    {
        return Pair<SmartPtr<IResource>, bool>();
    }

    void CSoundManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
    {
    }

    SmartPtr<IResource> CSoundManager::loadFromFile( const String &filePath )
    {
        return nullptr;
    }

    SmartPtr<IResource> CSoundManager::load( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> CSoundManager::getByName( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> CSoundManager::getById( const String &uuid )
    {
        return nullptr;
    }

    void CSoundManager::_getObject( void **ppObject ) const
    {
    }
} // namespace fb
