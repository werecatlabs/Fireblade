#ifndef _SoundManager_H
#define _SoundManager_H

#include "FBCore/Sound/SoundManager.h"
#include <FBCore/Interface/Sound/ISoundManager.h>
#include <FBCore/Core/Singleton.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <map>

#if defined FB_PLATFORM_WIN32
#    include <Audioclient.h>
#    include <mmdeviceapi.h>
#endif

namespace fb
{

    class FBAudioManager : public SoundManager
    {
    public:
        FBAudioManager();
        ~FBAudioManager();

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void loadSoundMap( const Properties &soundMap );

        bool LoadSoundEvents( const String &filePath );

        SmartPtr<ISound2> addSound2( const String &filePath, bool loop = true );
        SmartPtr<ISound3> addSound3( const String &filePath, const Vector3F &position = Vector3F::zero(),
                                     bool loop = true );
        SmartPtr<ISoundListener3> addListener3( const String &name, const Vector3F &position );

        bool Remove2DSound( SmartPtr<ISound2> &soundEffect2D );
        bool Remove3DSound( SmartPtr<ISound3> &soundEffect3D );

        void RemoveAll();

        SmartPtr<ISoundListener3> findListener3( const String &name );

        void update( const s32 &task, const time_interval &t, const time_interval &dt );
        void *GetEventSystem() const;

        void setBGMusic( const String &filePath );
        void setBGMusic( SmartPtr<ISound2> &bgMusic );
        const SmartPtr<ISound2> &getBGMusic() const;
        SmartPtr<ISound2> &getBGMusic();
        void stopBGMusic();

        void setVolume( f32 fVolume );
        f32 getVolume() const;

        const String &getType() const;

        void startRecording();
        void stopRecording();
        u32 getBufferSize() const;
        void copyContentsToMemory( void *buffer, u32 size );

        SmartPtr<IResource> create( const String &name ) override;

        SmartPtr<IResource> create( const String &uuid, const String &name ) override;

        Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &uuid, const String &path,
                                                          const String &type ) override;

        Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path ) override;

        void saveToFile( const String &filePath, SmartPtr<IResource> resource ) override;

        SmartPtr<IResource> loadFromFile( const String &filePath ) override;

        SmartPtr<IResource> loadResource( const String &name ) override;

        SmartPtr<IResource> getByName( const String &name ) override;

        SmartPtr<IResource> getById( const String &uuid ) override;

        void _getObject( void **ppObject ) const override;

        bool isRealtime() const override;

        bool isMute() const override;

        void setMute( bool mute ) override;

    private:
#if defined FB_PLATFORM_WIN32
        IAudioClient *pAudioClient = nullptr;
        IMMDevice *pDevice = nullptr;
#endif

        Array<SmartPtr<ISound2>> m_sounds2d;
        Array<SmartPtr<ISound3>> m_sounds3d;

        typedef std::map<String, SmartPtr<ISoundListener3>> SoundListenerMap;
        SoundListenerMap m_listeners;

        SmartPtr<ISound2> m_pBackGroundMusic;

        typedef std::map<String, String> SoundMap;
        SoundMap m_soundMap;

        mutable RecursiveMutex m_mutex;
    };

}  // end namespace fb

#endif
