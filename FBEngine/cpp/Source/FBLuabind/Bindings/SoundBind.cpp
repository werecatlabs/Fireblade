#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/SoundBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    SmartPtr<ISound> _addSound2( ISoundManager *soundMgr, const char *name, bool loop )
    {
        return soundMgr->addSound( name, loop );
    }

    SmartPtr<ISound> _addSound3( ISoundManager *soundMgr, const char *name )
    {
        return soundMgr->addSound( name );
    }

    //
    //ISound2 *_getSound2Hash( SoundContainer *container, lua_Integer id )
    //{
    //    Sound2Ptr sound;  // = container->getSound(id);
    //    return sound.get();
    //}

    //
    //ISound2 *_getSound2( SoundContainer *container, const char *id )
    //{
    //    auto hash = StringUtil::getHash( id );
    //    return _getSound2Hash( container, hash );
    //}

    //
    //void _setSoundHash( SoundContainer *container, lua_Integer id, SoundPtr sound )
    //{
    //    container->setSound( id, sound );
    //}

    //
    //void _setSound( SoundContainer *container, const char *id, SoundPtr sound )
    //{
    //    auto hash = StringUtil::getHash( id );
    //    return _setSoundHash( container, hash, sound );
    //}

    //
    //void _setSound2Hash( SoundContainer *container, lua_Integer id, Sound2Ptr sound )
    //{
    //    container->setSound( id, sound );
    //}

    //
    //void _setSound2( SoundContainer *container, const char *id, Sound2Ptr sound )
    //{
    //    auto hash = StringUtil::getHash( id );
    //    return _setSound2Hash( container, hash, sound );
    //}

    //
    //void _setSound3Hash( SoundContainer *container, lua_Integer id, Sound3Ptr sound )
    //{
    //    container->setSound( id, sound );
    //}

    //
    //void _setSound3( SoundContainer *container, const char *id, Sound3Ptr sound )
    //{
    //    auto hash = StringUtil::getHash( id );
    //    return _setSound3Hash( container, hash, sound );
    //}

    //
    //ISound3 *_getSound3Hash( SoundContainer *container, lua_Integer id )
    //{
    //    Sound3Ptr sound;  // = container->getSound(id);
    //    return sound.get();
    //}

    //ISound3 *_getSound3( SoundContainer *container, const char *id )
    //{
    //    auto hash = StringUtil::getHash( id );
    //    return _getSound3Hash( container, hash );
    //}

    void bindSound( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<ISound, ISharedObject, SmartPtr<ISound>>( "Sound" )
                        .def( "play", &ISound::play )
                        .def( "stop", &ISound::stop )
                        .def( "isPlaying", &ISound::isPlaying )
                        .def( "setVolume", &ISound::setVolume )
                        .def( "getVolume", &ISound::getVolume )
                        .def( "setLoop", &ISound::setLoop )
                        .def( "getLoop", &ISound::getLoop )
                        .def( "getName", &ISound::getName )];

        module( L )[class_<ISoundManager, ISharedObject, SmartPtr<ISoundManager>>( "SoundManager" )
                        .def( "addSound2", _addSound2 )
                        .def( "addSound3", _addSound3 )];
    }

}  // end namespace fb
