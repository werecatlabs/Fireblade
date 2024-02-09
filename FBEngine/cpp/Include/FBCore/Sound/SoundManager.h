#ifndef CSoundManager_h__
#define CSoundManager_h__

#include <FBCore/Interface/Sound/ISoundManager.h>

namespace fb
{

    /** Sound manager base implementation.
     */
    class SoundManager : public ISoundManager
    {
    public:
        SoundManager();
        ~SoundManager() override;

        SmartPtr<ISound> addSound( const String &name, bool loop = true ) override;

        SmartPtr<ISoundListener3> addListener3( const String &name,
                                                const Vector3<real_Num> &position = Vector3<real_Num>::zero() ) override;

        SmartPtr<ISoundListener3> findListener3( const String &name ) override;

        void setVolume( f32 volume ) override;

        f32 getVolume() const override;

        void startRecording() override;

        void stopRecording() override;

        u32 getBufferSize() const override;

        void copyContentsToMemory( void *buffer, u32 size ) override;

        bool isRealtime() const override;

        bool isMute() const override;

        void setMute( bool mute ) override;

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

        FB_CLASS_REGISTER_DECL;

    protected:
        Array<SmartPtr<ISound>> m_sounds;
    };
}  // namespace fb

#endif  // CSoundManager_h__
