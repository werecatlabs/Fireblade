#include <FBCore/FBCorePCH.h>
#include <FBCore/Sound/SoundManager.h>
#include <FBCore/Interface/Sound/ISound.h>
#include <FBCore/Interface/Sound/ISoundListener3.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    SoundManager::SoundManager() = default;

    SoundManager::~SoundManager() = default;

    auto SoundManager::addSound( const String &name, bool loop /*= true */ ) -> SmartPtr<ISound>
    {
        return nullptr;
    }

    auto SoundManager::addListener3( const String &name,
                                     const Vector3F &position /*= Vector3F::zero() */ )
        -> SmartPtr<ISoundListener3>
    {
        return nullptr;
    }

    auto SoundManager::findListener3( const String &name ) -> SmartPtr<ISoundListener3>
    {
        return nullptr;
    }

    void SoundManager::setVolume( f32 volume )
    {
    }

    auto SoundManager::getVolume() const -> f32
    {
        return 0.0f;
    }

    void SoundManager::startRecording()
    {
    }

    void SoundManager::stopRecording()
    {
    }

    auto SoundManager::getBufferSize() const -> u32
    {
        return 0;
    }

    void SoundManager::copyContentsToMemory( void *buffer, u32 size )
    {
    }

    auto SoundManager::isRealtime() const -> bool
    {
        return false;
    }

    auto SoundManager::isMute() const -> bool
    {
        return false;
    }

    void SoundManager::setMute( bool mute )
    {
    }

    auto SoundManager::create( const String &name ) -> SmartPtr<IResource>
    {
        return nullptr;
    }

    auto SoundManager::create( const String &uuid, const String &name ) -> SmartPtr<IResource>
    {
        return nullptr;
    }

    auto SoundManager::createOrRetrieve( const String &uuid, const String &path, const String &type )
        -> Pair<SmartPtr<IResource>, bool>
    {
        return {};
    }

    auto SoundManager::createOrRetrieve( const String &path ) -> Pair<SmartPtr<IResource>, bool>
    {
        return {};
    }

    void SoundManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
    {
    }

    auto SoundManager::loadFromFile( const String &filePath ) -> SmartPtr<IResource>
    {
        return nullptr;
    }

    auto SoundManager::loadResource( const String &name ) -> SmartPtr<IResource>
    {
        return nullptr;
    }

    auto SoundManager::getByName( const String &name ) -> SmartPtr<IResource>
    {
        return nullptr;
    }

    auto SoundManager::getById( const String &uuid ) -> SmartPtr<IResource>
    {
        return nullptr;
    }

    void SoundManager::_getObject( void **ppObject ) const
    {
    }
}  // namespace fb
