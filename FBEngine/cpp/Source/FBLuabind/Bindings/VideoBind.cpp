#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/VideoBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    using namespace render;

    IVideo *_addVideo( IVideoManager *mgr, const char *fileName )
    {
        //VideoPtr video = mgr->addVideo( fileName );
        //return video.get();

        return nullptr;
    }

    IVideo *_addVideoWithName( IVideoManager *mgr, const char *id, const char *fileName )
    {
        //auto hash = StringUtil::getHash( id );
        //VideoPtr video = mgr->addVideo( hash, fileName );
        //return video.get();

        return nullptr;
    }

    IVideo *_addVideoAndTexture( IVideoManager *mgr, const char *id, const char *fileName,
                                 const char *textureName )
    {
        //try
        //{
        //    auto hash = StringUtil::getHash( id );
        //    VideoPtr video = mgr->addVideo( hash, fileName );
        //    if( video )
        //    {
        //        VideoTexturePtr videoTexture = mgr->createVideoTexture( textureName );
        //        videoTexture->initialise( textureName, video->getSize() );
        //        video->setVideoTexture( videoTexture );
        //        return video.get();
        //    }
        //}
        //catch( Exception &e )
        //{
        //    FB_LOG_MESSAGE( "Video", e.what() );
        //}
        //catch( std::exception &e )
        //{
        //    FB_LOG_MESSAGE( "Video", e.what() );
        //}
        //catch( ... )
        //{
        //    FB_LOG_MESSAGE( "Video", "Unknown error." );
        //}

        return nullptr;
    }

    SmartPtr<IVideo> _addVideoAndTextureSize( IVideoManager *mgr, const char *id, const char *fileName,
                                              const char *textureName, const Vector2I &size )
    {
        //try
        //{
        //    auto hash = StringUtil::getHash( id );
        //    VideoPtr video = mgr->addVideo( hash, fileName );
        //    if( video )
        //    {
        //        VideoTexturePtr videoTexture = mgr->createVideoTexture( textureName );
        //        if( videoTexture )
        //        {
        //            videoTexture->initialise( textureName, size );
        //            video->setVideoTexture( videoTexture );
        //        }
        //    }

        //    return video;
        //}
        //catch( Exception &e )
        //{
        //    FB_LOG_MESSAGE( "Video", e.what() );
        //}
        //catch( std::exception &e )
        //{
        //    FB_LOG_MESSAGE( "Video", e.what() );
        //}
        //catch( ... )
        //{
        //    FB_LOG_MESSAGE( "Video", "Unknown error." );
        //}

        return nullptr;
    }

    IVideo *_addVideoWithId( IVideoManager *mgr, hash32 hash, const char *fileName )
    {
        //VideoPtr video = mgr->addVideo( hash, fileName );
        //return video.get();

        return nullptr;
    }

    IVideo *_getVideoById( IVideoManager *mgr, lua_Integer id )
    {
        return mgr->getVideoById( id ).get();
    }

    IVideo *_getVideoByName( IVideoManager *mgr, const char *name )
    {
        hash32 id = StringUtil::getHash( name );
        return _getVideoById( mgr, id );
    }

    void _createTexture( IVideo *video, const char *textureName )
    {
        //core::ApplicationManager *engine = core::ApplicationManager::instance();
        //VideoManagerPtr videoMgr = engine->getVideoManager();
        //VideoTexturePtr videoTexture = videoMgr->createVideoTexture( textureName );
        //videoTexture->initialise( textureName, video->getSize() );
        //video->setVideoTexture( videoTexture );
    }

    void bindVideo( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<IVideo, ISharedObject, SmartPtr<IVideo>>( "Video" )
                        .def( "getId", &IVideo::getId )
                        .def( "play", &IVideo::play )
                        .def( "stop", &IVideo::stop )
                        .def( "setLoop", &IVideo::setLoop )
                        .def( "getLoop", &IVideo::getLoop )
                        .def( "getSize", &IVideo::getSize )
                        .def( "setSize", &IVideo::setSize )];

        module( L )[class_<IVideoManager, ISharedObject, SmartPtr<IVideoManager>>( "VideoManager" )
                        .def( "addVideo", _addVideo )
                        .def( "addVideo", _addVideoWithName )
                        .def( "addVideo", _addVideoAndTexture )
                        .def( "addVideo", _addVideoAndTextureSize )
                        .def( "addVideo", _addVideoWithId )
                        .def( "getVideoById", _getVideoById )
                        .def( "getVideoByName", _getVideoByName )];
    }

}  // end namespace fb
