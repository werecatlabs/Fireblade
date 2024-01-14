#include <FBAudio/FBAudioManager.h>
#include <FBCore/FBCore.h>

#include <iostream>
#include <fstream>

#include "FBAudio/Sound2.h"
#include "FBAudio/Sound3.h"

#if defined FB_PLATFORM_WIN32
#    define FB_USE_WASAPI 0
#    define FB_USE_XAUDIO2 1

#    include <windows.h>
#    include <mmsystem.h>

#    if FB_USE_XAUDIO2
#        define _WIN32_WINNT 0x0A00  // 0x0A00 corresponds to Windows 10
#        include <xaudio2.h>
#        include <iostream>
#    elif FB_USE_WASAPI
#        include <Audioclient.h>
#        include <mmdeviceapi.h>
#    endif
#elif defined FB_PLATFORM_APPLE
#    define FB_USE_WASAPI 0
#    define FB_USE_XAUDIO2 0

#    include <CoreAudio/CoreAudio.h>
#    include <AudioToolbox/AudioQueue.h>
#    include <AudioToolbox/AudioFile.h>
#    include <CoreFoundation/CoreFoundation.h>
#endif

#if FB_USE_XAUDIO2
IXAudio2 *pXAudio2 = nullptr;
IXAudio2MasteringVoice *pMasterVoice = nullptr;
IXAudio2SourceVoice *pSourceVoice = nullptr;
#endif

namespace fb
{
    const f32 DISTANCEFACTOR =
        1.0f;  // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.

    FBAudioManager::FBAudioManager()
    {
    }

    FBAudioManager::~FBAudioManager()
    {
    }

#if FB_USE_XAUDIO2
    bool LoadWavFile2( const char *filename, WAVEFORMATEX *wfx, XAUDIO2_BUFFER *buffer )
    {
        HMMIO hmmio;
        MMIOINFO mmioInfo;
        memset( &mmioInfo, 0, sizeof( MMIOINFO ) );

        // Open the file for reading
        hmmio = mmioOpenA( const_cast<LPSTR>( filename ), &mmioInfo, MMIO_READ );
        if( hmmio == NULL )
        {
            // Handle error
            return false;
        }

        MMCKINFO ckRIFF, ckData;
        HANDLE hFile = CreateFileA( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                    FILE_FLAG_SEQUENTIAL_SCAN, NULL );
        if( hFile == nullptr )
        {
            mmioClose( hmmio, 0 );
            return false;
        }

        // Read the RIFF chunk
        ckRIFF.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );
        if( mmioDescend( hmmio, &ckRIFF, NULL, MMIO_FINDRIFF ) != 0 )
        {
            CloseHandle( hFile );
            return false;
        }

        // Read the format chunk
        WAVEFORMATEX format;
        MMCKINFO ckFormat;
        ckFormat.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
        if( mmioDescend( hmmio, &ckFormat, &ckRIFF, MMIO_FINDCHUNK ) != 0 )
        {
            CloseHandle( hFile );
            return false;
        }
        if( mmioRead( hmmio, reinterpret_cast<HPSTR>( &format ), sizeof( WAVEFORMATEX ) ) !=
            sizeof( WAVEFORMATEX ) )
        {
            CloseHandle( hFile );
            return false;
        }
        mmioAscend( hmmio, &ckFormat, 0 );

        // Read the data chunk
        ckData.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
        if( mmioDescend( hmmio, &ckData, &ckRIFF, MMIO_FINDCHUNK ) != 0 )
        {
            CloseHandle( hFile );
            return false;
        }

        // Allocate memory for the wave data
        BYTE *waveData = new BYTE[ckData.cksize];
        if( waveData == nullptr )
        {
            CloseHandle( hFile );
            return false;
        }

        // Read the wave data
        if( mmioRead( hmmio, reinterpret_cast<HPSTR>( waveData ), ckData.cksize ) !=
            static_cast<LONG>( ckData.cksize ) )
        {
            delete[] waveData;
            CloseHandle( hFile );
            return false;
        }

        // Close the file
        CloseHandle( hFile );

        // Fill the XAUDIO2_BUFFER structure
        buffer->AudioBytes = ckData.cksize;
        buffer->pAudioData = waveData;
        buffer->Flags = XAUDIO2_END_OF_STREAM;

        // Set the output parameters
        if( wfx != nullptr )
        {
            *wfx = format;
        }

        // Clean up resources
        mmioClose( hmmio, 0 );
        CloseHandle( hFile );

        return true;
    }

#    include <windows.h>
#    include <xaudio2.h>

#    pragma comment( lib, "xaudio2.lib" )

    IXAudio2 *pXAudio2 = nullptr;
    IXAudio2MasteringVoice *pMasterVoice = nullptr;
    IXAudio2SourceVoice *pSourceVoice = nullptr;

    void Cleanup()
    {
        if( pSourceVoice )
        {
            pSourceVoice->Stop();
            pSourceVoice->DestroyVoice();
        }

        if( pMasterVoice )
        {
            pMasterVoice->DestroyVoice();
        }

        if( pXAudio2 )
        {
            pXAudio2->Release();
        }
    }

    bool InitializeXAudio2()
    {
        HRESULT hr = XAudio2Create( &pXAudio2 );
        if( FAILED( hr ) )
        {
            return false;
        }

        hr = pXAudio2->CreateMasteringVoice( &pMasterVoice );
        if( FAILED( hr ) )
        {
            Cleanup();
            return false;
        }

        return true;
    }

    bool LoadWavFile( const char *filename )
    {
        // Open the WAV file
        HMMIO hmmio = mmioOpen( const_cast<LPSTR>( filename ), nullptr, MMIO_READ );
        if( hmmio == nullptr )
        {
            return false;
        }

        // Read the WAV file format
        MMCKINFO ckRIFF, ckFormat;
        ckRIFF.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );
        if( mmioDescend( hmmio, &ckRIFF, nullptr, MMIO_FINDRIFF ) != 0 )
        {
            mmioClose( hmmio, 0 );
            return false;
        }

        ckFormat.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
        if( mmioDescend( hmmio, &ckFormat, &ckRIFF, MMIO_FINDCHUNK ) != 0 )
        {
            mmioClose( hmmio, 0 );
            return false;
        }

        WAVEFORMATEX wfx;
        if( mmioRead( hmmio, reinterpret_cast<HPSTR>( &wfx ), sizeof( WAVEFORMATEX ) ) !=
            sizeof( WAVEFORMATEX ) )
        {
            mmioClose( hmmio, 0 );
            return false;
        }

        mmioAscend( hmmio, &ckFormat, 0 );

        // Read the WAV file data
        MMCKINFO ckData;
        ckData.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
        if( mmioDescend( hmmio, &ckData, &ckRIFF, MMIO_FINDCHUNK ) != 0 )
        {
            mmioClose( hmmio, 0 );
            return false;
        }

        DWORD dataSize = ckData.cksize;
        BYTE *pData = new BYTE[dataSize];
        if( mmioRead( hmmio, reinterpret_cast<HPSTR>( pData ), dataSize ) !=
            static_cast<LONG>( dataSize ) )
        {
            delete[] pData;
            mmioClose( hmmio, 0 );
            return false;
        }

        mmioClose( hmmio, 0 );

        // Create the source voice
        HRESULT hr = pXAudio2->CreateSourceVoice( &pSourceVoice, &wfx );
        if( FAILED( hr ) )
        {
            delete[] pData;
            return false;
        }

        // Submit the audio data
        XAUDIO2_BUFFER buffer = {};
        buffer.AudioBytes = dataSize;
        buffer.pAudioData = pData;
        buffer.Flags = XAUDIO2_END_OF_STREAM;

        hr = pSourceVoice->SubmitSourceBuffer( &buffer );
        if( FAILED( hr ) )
        {
            delete[] pData;
            return false;
        }

        return true;
    }

    void PlayWavFile()
    {
        pSourceVoice->Start( 0 );
        // Optionally, you can wait for the sound to finish playing
        // before cleaning up by calling Sleep or using a loop with
        // IXAudio2SourceVoice::GetState to check the state.
    }

    //int main()
    //{
    //    if (!InitializeXAudio2())
    //    {
    //        // Handle initialization failure
    //        return -1;
    //    }
    //
    //    const wchar_t* wavFilename = L"your_wav_file.wav";
    //    if (LoadWavFile(wavFilename))
    //    {
    //        PlayWavFile();
    //        // Optionally, wait for the sound to finish playing before exiting
    //        // Sleep(5000); // Wait for 5 seconds
    //    }
    //    else
    //    {
    //        // Handle WAV file loading failure
    //    }
    //
    //    Cleanup();
    //
    //    return 0;
    //}

#endif

    void FBAudioManager::load( SmartPtr<ISharedObject> data )
    {
#if defined FB_PLATFORM_WIN32
#    if FB_USE_WASAPI
        CoInitialize( nullptr );

        // Initialize COM for audio operations
        HRESULT hr;
        IMMDeviceEnumerator *pEnumerator = nullptr;
        hr = CoCreateInstance( __uuidof( MMDeviceEnumerator ), nullptr, CLSCTX_ALL,
                               __uuidof( IMMDeviceEnumerator ), (void **)&pEnumerator );
        if( FAILED( hr ) )
        {
            CoUninitialize();
            //return hr;
        }

        // Get the default audio rendering device
        IMMDevice *pDevice = nullptr;
        hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eConsole, &pDevice );
        pEnumerator->Release();
        if( FAILED( hr ) )
        {
            CoUninitialize();
            //return hr;
        }

        // Initialize the audio client
        IAudioClient *pAudioClient = nullptr;
        hr = pDevice->Activate( __uuidof( IAudioClient ), CLSCTX_ALL, nullptr, (void **)&pAudioClient );
        pDevice->Release();
        if( FAILED( hr ) )
        {
            CoUninitialize();
            //return hr;
        }

        // Configure the audio format
        WAVEFORMATEX *pwfx = nullptr;
        hr = pAudioClient->GetMixFormat( &pwfx );
        if( FAILED( hr ) )
        {
            pAudioClient->Release();
            CoUninitialize();
            //return hr;
        }

        // Initialize the audio render client
        hr = pAudioClient->Initialize( AUDCLNT_SHAREMODE_SHARED, 0, 0, 0, pwfx, nullptr );
        CoTaskMemFree( pwfx );
        if( FAILED( hr ) )
        {
            pAudioClient->Release();
            CoUninitialize();
            //return hr;
        }

        // Get the audio buffer size
        UINT32 bufferFrameCount;
        hr = pAudioClient->GetBufferSize( &bufferFrameCount );
        if( FAILED( hr ) )
        {
            pAudioClient->Release();
            CoUninitialize();
            //return hr;
        }

        // Create an audio render client
        IAudioRenderClient *pRenderClient = nullptr;
        hr = pAudioClient->GetService( __uuidof( IAudioRenderClient ), (void **)&pRenderClient );
        if( FAILED( hr ) )
        {
            pAudioClient->Release();
            CoUninitialize();
            //return hr;
        }
#    elif FB_USE_XAUDIO2
        CoInitialize( nullptr );

        if( !InitializeXAudio2() )
        {
            // Handle initialization failure
            return;
        }

        auto wavFilename = "car_drive.wav";
        if( LoadWavFile( wavFilename ) )
        {
            //PlayWavFile();
            // Optionally, wait for the sound to finish playing before exiting
            // Sleep(5000); // Wait for 5 seconds
        }
        else
        {
            // Handle WAV file loading failure
        }

        /*


        // Initialize XAudio2
        if( FAILED( XAudio2Create( &pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR ) ) )
        {
            // Handle initialization error
            return;
        }

        // Create a mastering voice
        if( FAILED( pXAudio2->CreateMasteringVoice( &pMasterVoice ) ) )
        {
            // Handle voice creation error
            pXAudio2->Release();
            return;
        }

        // Load the .wav file and configure the source voice
        WAVEFORMATEX wfx;
        XAUDIO2_BUFFER buffer;
        if( !LoadWavFile( "C:/dev/fireblade/Bin/Media/game_dubstep.wav", &wfx, &buffer ) )
        {
            // Handle file loading error
            pMasterVoice->DestroyVoice();
            pXAudio2->Release();
            return;
        }

        // Create a source voice for audio playback
        if( FAILED( pXAudio2->CreateSourceVoice( &pSourceVoice, &wfx ) ) )
        {
            // Handle source voice creation error
            pMasterVoice->DestroyVoice();
            pXAudio2->Release();
            return;
        }

        // Submit the audio buffer for playback
        pSourceVoice->SubmitSourceBuffer( &buffer );

        // Start playing the audio
        pSourceVoice->Start( 0 );

        // Optionally wait for the audio to finish playing
        //pSourceVoice->WaitForSourceVoiceStop( INFINITE );

        Thread::sleep( 10.0 );

        // Clean up and release resources
        //pSourceVoice->DestroyVoice();
        //pMasterVoice->DestroyVoice();
        //pXAudio2->Release();
        */
#    endif
#endif
    }

#if defined FB_PLATFORM_WIN32
    void playAudio( IAudioRenderClient *pRenderClient, std::ifstream &wavFile, UINT32 bufferFrameCount )
    {
        BYTE *buffer = nullptr;  // Declare the buffer locally within the function
        UINT32 numFramesAvailable;
        pRenderClient->GetBuffer( bufferFrameCount, &buffer );

        if( numFramesAvailable > 0 )
        {
            wavFile.read( reinterpret_cast<char *>( buffer ), numFramesAvailable * sizeof( float ) );

            if( wavFile.eof() )
            {
                // If the end of the file is reached, stop playing
                return;
            }

            pRenderClient->ReleaseBuffer( numFramesAvailable, 0 );
        }
    }
#endif

    void FBAudioManager::unload( SmartPtr<ISharedObject> data )
    {
#if defined FB_PLATFORM_WIN32
        // Clean up and release resources
        if( pAudioClient )
        {
            pAudioClient->Stop();
            pAudioClient->Release();
        }

        Cleanup();
        CoUninitialize();
#endif
    }

    void FBAudioManager::loadSoundMap( const Properties &soundMap )
    {
        //Array<Property> properties = soundMap.getProperties();
        //for( u32 i = 0; i < properties.size(); ++i )
        //{
        //    Property &property = properties[i];
        //    m_soundMap[property.getName()] = property.getValue();
        //}
    }

    bool FBAudioManager::LoadSoundEvents( const String &filePath )
    {
        return false;
    }

    SmartPtr<ISound2> FBAudioManager::addSound2( const String &name, bool bLoop )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        String filePath;

        auto it = m_soundMap.find( name );
        if( it != m_soundMap.end() )
        {
            filePath = it->second;
        }
        else
        {
            filePath = name;
            FB_LOG_MESSAGE( "Sound", String( "Sound not found. Sound name: " ) + name );
        }

        auto sound2D = fb::make_ptr<Sound2>();
        sound2D->Initialize( filePath, bLoop, false );
        m_sounds2d.push_back( sound2D );
        return sound2D;
    }

    SmartPtr<ISound3> FBAudioManager::addSound3( const String &name, const Vector3F &position,
                                                 bool bLoop )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        String filePath;

        auto it = m_soundMap.find( name );
        if( it != m_soundMap.end() )
        {
            filePath = it->second;
        }
        else
        {
            filePath = name;
        }

        auto sound3D = fb::make_ptr<Sound3>();
        sound3D->Initialize( position, filePath, bLoop, false );
        m_sounds3d.push_back( sound3D );
        return sound3D;
    }

    SmartPtr<ISoundListener3> FBAudioManager::addListener3( const String &name,
                                                            const Vector3F &position )
    {
        //RecursiveMutex::ScopedLock lock( SoundMutex );

        //CSoundListener3DPtr listener;  // (new FMODSoundListener3(), true);
        //listener->setPosition( position );
        //m_listeners[name] = listener;
        //return m_listeners[name];

        return nullptr;
    }

    bool FBAudioManager::isRealtime() const
    {
        return false;
    }

    bool FBAudioManager::isMute() const
    {
        return false;
    }

    void FBAudioManager::setMute( bool mute )
    {
    }

    SmartPtr<ISoundListener3> FBAudioManager::findListener3( const String &name )
    {
        //RecursiveMutex::ScopedLock lock(SoundMutex);

        //SoundListenerMap::iterator it = m_listeners.find(name);
        //if( it != m_listeners.end() )
        //{
        //	return (it->second).getReference<ISoundListener3>();
        //}

        return nullptr;
    }

    void FBAudioManager::update( const s32 &task, const time_interval &t, const time_interval &dt )
    {
    }

    void *FBAudioManager::GetEventSystem() const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return nullptr;
    }

    void FBAudioManager::setBGMusic( const String &file_path )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        SmartPtr<ISound2> pBackGroundMusic = m_pBackGroundMusic;
        if( pBackGroundMusic != nullptr )
        {
            //compare paths
            //c8 FileName[260];
            //extractFileName((c8*)file_path.c_str(), (c8*)FileName, 260);
            //String fileName = FileName;
            ////fileName = fileName.subString( 0, fileName.size() - 4 );

            //if( !pBackGroundMusic->GetName()==(fileName) )
            {
                Remove2DSound( pBackGroundMusic );
                pBackGroundMusic = addSound2( file_path, true );
                pBackGroundMusic->play();
            }
        }
        else
        {
            pBackGroundMusic = addSound2( file_path, true );
            pBackGroundMusic->play();
        }

        m_pBackGroundMusic = pBackGroundMusic;
    }

    void FBAudioManager::setBGMusic( SmartPtr<ISound2> &bgMusic )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( !m_pBackGroundMusic )
        {
            m_pBackGroundMusic = bgMusic;
        }
        else
        {
            m_pBackGroundMusic->stop();
            //m_pBackGroundMusic->removeReference();
        }
    }

    const SmartPtr<ISound2> &FBAudioManager::getBGMusic() const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        return m_pBackGroundMusic;
    }

    SmartPtr<ISound2> &FBAudioManager::getBGMusic()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        return m_pBackGroundMusic;
    }

    void FBAudioManager::stopBGMusic()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( m_pBackGroundMusic != nullptr )
        {
            m_pBackGroundMusic->stop();
        }
    }

    void FBAudioManager::setVolume( f32 fVolume )
    {
    }

    f32 FBAudioManager::getVolume() const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        //m_pSystem->getMasterChannelGroup(&pChannelGroup);

        f32 fVolume;
        //result = pChannelGroup->getVolume(&fVolume);
        //ERRCHECK(result);

        return fVolume;
    }

    void FBAudioManager::RemoveAll()
    {
        m_sounds2d.clear();
        m_sounds3d.clear();
        m_listeners.clear();
    }

    bool FBAudioManager::Remove2DSound( SmartPtr<ISound2> &pSoundEffect2D )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        for( u32 i = 0; i < m_sounds2d.size(); ++i )
        {
            //FBFMODStudioSound2Ptr &pListedSoundEffect2D = m_sounds2d[i];
            //if( pSoundEffect2D == pListedSoundEffect2D )
            //{
            //    pSoundEffect2D->stop();
            //    //m_sounds2d.erase_element_index(i);
            //    return true;
            //}
        }

        return false;
    }

    bool FBAudioManager::Remove3DSound( SmartPtr<ISound3> &pSoundEffect3D )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        for( u32 i = 0; i < m_sounds3d.size(); i++ )
        {
            //FBFMODStudioSound3Ptr &pListedSoundEffect3D = m_sounds3d[i];
            //if( pSoundEffect3D == pListedSoundEffect3D )
            //{
            //    pSoundEffect3D->stop();
            //    //m_sounds3d.erase_element_index(i);
            //    return true;
            //}
        }

        return false;
    }

    const String &FBAudioManager::getType() const
    {
        static String type( "FMODSoundManager" );
        return type;
    }

    void FBAudioManager::startRecording()
    {
    }

    void FBAudioManager::stopRecording()
    {
    }

    u32 FBAudioManager::getBufferSize() const
    {
        return 0;
    }

    void FBAudioManager::copyContentsToMemory( void *buffer, u32 size )
    {
    }

    SmartPtr<IResource> FBAudioManager::create( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> FBAudioManager::create( const String &uuid, const String &name )
    {
        return nullptr;
    }

    Pair<SmartPtr<IResource>, bool> FBAudioManager::createOrRetrieve( const String &uuid,
                                                                      const String &path,
                                                                      const String &type )
    {
        return Pair<SmartPtr<IResource>, bool>();
    }

    Pair<SmartPtr<IResource>, bool> FBAudioManager::createOrRetrieve( const String &path )
    {
        return Pair<SmartPtr<IResource>, bool>();
    }

    void FBAudioManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
    {
    }

    SmartPtr<IResource> FBAudioManager::loadFromFile( const String &filePath )
    {
        return nullptr;
    }

    SmartPtr<IResource> FBAudioManager::loadResource( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> FBAudioManager::getByName( const String &name )
    {
        return nullptr;
    }

    SmartPtr<IResource> FBAudioManager::getById( const String &uuid )
    {
        return nullptr;
    }

    void FBAudioManager::_getObject( void **ppObject ) const
    {
    }
}  // end namespace fb
