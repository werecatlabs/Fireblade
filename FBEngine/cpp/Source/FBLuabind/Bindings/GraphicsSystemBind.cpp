#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/GraphicsSystemBind.h"

#include <luabind/luabind.hpp>
#include "FBLuabind/ParamConverter.h"
#include "FBLuabind/SmartPtrConverter.h"
#include <FBCore/FBCore.h>
#include "FBLuabind/ArrayFunctions.h"

#include "FBLuabind/Helpers/AnimationControllerHelper.h"
#include "FBLuabind/Helpers/SceneNodeHelper.h"
#include "FBLuabind/Helpers/ViewportHelper.h"
#include "FBLuabind/Helpers/SceneManagerHelper.h"
#include "FBLuabind/Helpers/GraphicsSystemHelper.h"
#include "FBLuabind/Helpers/GraphicsObjectHelper.h"
#include "FBLuabind/Helpers/GraphicsContainerHelper.h"
#include "FBLuabind/Helpers/ResourceManagerHelper.h"

namespace fb
{

    void bindGraphicsSystem( lua_State *L )
    {
        using namespace luabind;

        module(
            L )[class_<IGraphicsSystem, IScriptObject, boost::shared_ptr<IObject>>( "GraphicsSystem" )
                    //.def( "configure", GraphicsSystemHelper::_configureDefault )
                    //.def( "configure", GraphicsSystemHelper::_configure )

                    //.def( "addSceneManager", GraphicsSystemHelper::addSceneManager )
                    //.def( "getSceneManager", GraphicsSystemHelper::getSceneManager )
                    .def( "getSceneManagerById", &IGraphicsSystem::getSceneManagerById )
                    .def( "getOverlayManager", &IGraphicsSystem::getOverlayManager )
                    //.def( "getCompositorManager", &IGraphicsSystem::getCompositorManager )

                    .def( "getResourceGroupManager", &IGraphicsSystem::getResourceGroupManager )
                    .def( "getMaterialManager", &IGraphicsSystem::getMaterialManager )
                    .def( "getTextureManager", &IGraphicsSystem::getTextureManager )

                    //.def( "getRenderWindow", GraphicsSystemHelper::_getRenderWindow )
                    //.def( "getRenderWindow", GraphicsSystemHelper::_getRenderWindowNamed )

                    .def( "getDefaultWindow", &IGraphicsSystem::getDefaultWindow )
                    .def( "setDefaultWindow", &IGraphicsSystem::setDefaultWindow )

                    //.def( "loadResources", &IGraphicsSystem::loadResources )
                    //.def( "reloadResources", &IGraphicsSystem::reloadResources )

                    .def( "removeDeferredShadingSystem", &IGraphicsSystem::removeDeferredShadingSystem )
                    .def( "addDeferredShadingSystem", &IGraphicsSystem::addDeferredShadingSystem )];

        module( L )[class_<IDeferredShadingSystem, IScriptObject, boost::shared_ptr<IObject>>(
                        "IDeferredShadingSystem" )
                        .def( "getSSAO", &IDeferredShadingSystem::getSSAO )
                        .def( "setSSAO", &IDeferredShadingSystem::setSSAO )
                        .def( "getActive", &IDeferredShadingSystem::getActive )
                        .def( "setActive", &IDeferredShadingSystem::setActive )
                        .def( "getShadowsEnabled", &IDeferredShadingSystem::getShadowsEnabled )
                        .def( "setShadowsEnabled", &IDeferredShadingSystem::setShadowsEnabled )];

        module( L )[class_<IViewport, IScriptObject, boost::shared_ptr<IObject>>( "Viewport" )
                        .def( "getCamera", &IViewport::getCamera )
                        .def( "setCamera", &IViewport::setCamera )
                        .def( "getActualSize", ViewportHelper::_getActualSize )
                        .def( "setBackgroundColour", &IViewport::setBackgroundColour )
                        .def( "getBackgroundColour", &IViewport::getBackgroundColour )
                        .def( "setClearEveryFrame", ViewportHelper::_setClearEveryFrame )
                        .def( "setClearEveryFrame", ViewportHelper::_setClearEveryFrameFlags )
                        .def( "getClearEveryFrame", &IViewport::getClearEveryFrame )
                        .def( "getActualWidth", &IViewport::getActualWidth )
                        .def( "getActualHeight", &IViewport::getActualHeight )
                        .def( "getOverlaysEnabled", &IViewport::getOverlaysEnabled )
                        .def( "setOverlaysEnabled", &IViewport::setOverlaysEnabled )
                        .def( "getSkiesEnabled", &IViewport::getSkiesEnabled )
                        .def( "setSkiesEnabled", &IViewport::setSkiesEnabled )
                        .def( "getShadowsEnabled", &IViewport::getShadowsEnabled )
                        .def( "setShadowsEnabled", &IViewport::setShadowsEnabled )
                        .def( "getVisibilityMask", ViewportHelper::getVisibilityMask )
                        .def( "setVisibilityMask", ViewportHelper::setVisibilityMask )

                        .def( "setAutoUpdated", &IViewport::setAutoUpdated )
                        .def( "isAutoUpdated", &IViewport::isAutoUpdated )

                        .def( "getBackgroundTextureName", &IViewport::getBackgroundTextureName )
                        .def( "setBackgroundTextureName", &IViewport::setBackgroundTextureName )];

        module( L )[class_<IRenderTarget, IScriptObject, boost::shared_ptr<IObject>>( "RenderTarget" )
                        .def( "addViewport", ViewportHelper::_addViewportMinArgs )
                        .def( "addViewport", ViewportHelper::_addViewportAllArgs )
                        .def( "addViewport", ViewportHelper::_addViewportZorderArgs )
                        .def( "getNumViewports", &IRenderTarget::getNumViewports )
                        .def( "getViewport", &IRenderTarget::getViewport )
                        .def( "getViewportById", &IRenderTarget::getViewportById )
                        .def( "removeViewport", &IRenderTarget::removeViewport )
                        .def( "removeAllViewports", &IRenderTarget::removeAllViewports )
                        .def( "getSize", &IRenderTarget::getSize )
                        .def( "getColourDepth", &IRenderTarget::getColourDepth )

                        .def( "setAutoUpdated", &IRenderTarget::setAutoUpdated )
                        .def( "isAutoUpdated", &IRenderTarget::isAutoUpdated )];

        module( L )[class_<IWindow, IRenderTarget, boost::shared_ptr<IObject>>( "Window" )
                        //.def( "setFullscreen", &IWindow::setFullscreen )
                        .def( "destroy", &IWindow::destroy )
                        .def( "resize", &IWindow::resize )
                        .def( "windowMovedOrResized", &IWindow::windowMovedOrResized )
                        .def( "reposition", &IWindow::reposition )
                        .def( "isVisible", &IWindow::isVisible )
                        .def( "setVisible", &IWindow::setVisible )
                        .def( "isActive", &IWindow::isActive )
                        .def( "isClosed", &IWindow::isClosed )
                        .def( "isPrimary", &IWindow::isPrimary )
                        .def( "isFullScreen", &IWindow::isFullScreen )
                        .def( "suggestPixelFormat", &IWindow::suggestPixelFormat )
                        .def( "isDeactivatedOnFocusChange", &IWindow::isDeactivatedOnFocusChange )
                        .def( "setDeactivateOnFocusChange", &IWindow::setDeactivateOnFocusChange )];

        module(
            L )[class_<IGraphicsScene, IScriptObject, SmartPtr<IScriptObject>>( "GraphicsSceneManager" )
                    //.def( "update", &IGraphicsSceneManager::_update )
                    .def( "addGraphicsObject", SceneManagerHelper::_addGraphicsObject )
                    .def( "addGraphicsObject", SceneManagerHelper::_addGraphicsObjectNamed )

                    .def( "addSceneNode", SceneManagerHelper::_addSceneNode )
                    .def( "getSceneNode", &IGraphicsScene::getSceneNode )
                    .def( "getSceneNodeById", &IGraphicsScene::getSceneNodeById )
                    .def( "getRootSceneNode", SceneManagerHelper::_getRootSceneNode )

                    .def( "addLight", &IGraphicsScene::addLight )
                    .def( "addMesh", SceneManagerHelper::_addMesh )
                    .def( "addMesh", SceneManagerHelper::_addMeshNamed )
                    .def( "addParticleSystem", SceneManagerHelper::_addParticleSystem )
                    .def( "getParticleSystem", SceneManagerHelper::_getParticleSystem )
                    .def( "addCamera", &IGraphicsScene::addCamera )
                    //.def( "getCamera", &IGraphicsSceneManager::getCamera )
                    .def( "hasCamera", &IGraphicsScene::hasCamera )

                    .def( "clearScene", &IGraphicsScene::clearScene )

                    .def( "hasAnimation", &IGraphicsScene::hasAnimation )
                    .def( "destroyAnimation", &IGraphicsScene::destroyAnimation )

                    .def( "createAnimationStateController",
                          &IGraphicsScene::createAnimationStateController )

                    .def( "loadSceneFile", SceneManagerHelper::_loadSceneFile )
                    //.def( "loadScene", &IGraphicsSceneManager::loadScene )
                    .def( "setSkyBox", &IGraphicsScene::setSkyBox )
                    .def( "createTerrain", &IGraphicsScene::createTerrain )

                    .def( "getEnableShadows", &IGraphicsScene::getEnableShadows )
                    .def( "setEnableShadows", &IGraphicsScene::setEnableShadows )

                    .def( "createInstanceManager", &IGraphicsScene::createInstanceManager )
                    .def( "createInstancedObject", &IGraphicsScene::createInstancedObject )
                    .def( "destroyInstancedObject", &IGraphicsScene::destroyInstancedObject )];

        module( L )[class_<ISceneNode, IScriptObject, boost::shared_ptr<IObject>>( "ISceneNode" )
                        .def( "setMaterialName", SceneNodeHelper::setMaterialName )
                        .def( "setMaterialName", SceneNodeHelper::setMaterialNameCascade )

                        .def( "getParent", &ISceneNode::getParent )

                        .def( "add", &ISceneNode::add )
                        .def( "remove", &ISceneNode::remove )

                        .def( "addChild", &ISceneNode::addChild )
                        .def( "addChildSceneNode", SceneNodeHelper::_addChildSceneNode )
                        .def( "addChildSceneNode", SceneNodeHelper::_addChildSceneNodeNamed )
                        .def( "attachObject", SceneNodeHelper::_attachObject )
                        .def( "detachObject", &ISceneNode::detachObject )
                        .def( "detachAllObjects", &ISceneNode::detachAllObjects )

                        .def( "setPosition", &ISceneNode::setPosition )
                        .def( "getPosition", &ISceneNode::getPosition )

                        .def( "lookAt", &ISceneNode::lookAt )

                        .def( "setScale", &ISceneNode::setScale )
                        .def( "getScale", &ISceneNode::getScale )

                        .def( "setRotationFromDegrees", &ISceneNode::setRotationFromDegrees )
                        .def( "setOrientation", &ISceneNode::setOrientation )
                        .def( "getOrientation", &ISceneNode::getOrientation )

                        .def( "setVisible", SceneNodeHelper::_setVisible )

                        .def( "setVisibilityFlags", SceneNodeHelper::_setVisibilityFlags )
                        .def( "getVisibilityFlags", SceneNodeHelper::_getVisibilityFlags )];

        //graphics objects
        module( L )[class_<IGraphicsObject, IScriptObject, SharedPtr<ISharedObject>>( "IGraphicsObject" )
                        .def( "setMaterialName", GraphicsObjectHelper::_setMaterialName )
                        //.def("setMaterialName", GraphicsObjectHelper::_setMaterialNameStr )
                        .def( "setMaterialName", GraphicsObjectHelper::_setMaterialNameIndx )
                        // .def( "getMaterialName", &IGraphicsObject::getMaterialName )

                        .def( "getCastShadows", &IGraphicsObject::getCastShadows )
                        .def( "setCastShadows", &IGraphicsObject::setCastShadows )
                        //.def( "getRecieveShadows", &IGraphicsObject::getRecieveShadows )
                        //.def( "setRecieveShadows", &IGraphicsObject::setRecieveShadows )
                        .def( "setVisible", &IGraphicsObject::setVisible )
                        .def( "isVisible", &IGraphicsObject::isVisible )
                        .def( "setZOrder", &IGraphicsObject::setZOrder )
                        .def( "getZOrder", &IGraphicsObject::getZOrder )

                        .def( "setVisibilityFlags", GraphicsObjectHelper::_setVisibilityFlags )
                        .def( "getVisibilityFlags", GraphicsObjectHelper::_getVisibilityFlags )

        ];

        module(
            L )[class_<IInstancedObject, IGraphicsObject, SharedPtr<ISharedObject>>( "IInstancedObject" )
                    .def( "setPosition", &IInstancedObject::setPosition )
                    .def( "getPosition", &IInstancedObject::getPosition )
                    .def( "setScale", &IInstancedObject::setScale )
                    .def( "getScale", &IInstancedObject::getScale )
                    .def( "setOrientation", &IInstancedObject::setOrientation )
                    .def( "getOrientation", &IInstancedObject::getOrientation )

                    .def( "setCustomParam", &IInstancedObject::setCustomParam )
                    .def( "getCustomParam", &IInstancedObject::getCustomParam )];

        module( L )[class_<IFrustum, IGraphicsObject, SharedPtr<ISharedObject>>( "IFrustum" )
                        .def( "getNearClipDistance", &IFrustum::getNearClipDistance )
                        .def( "setNearClipDistance", &IFrustum::setNearClipDistance )
                        .def( "getFarClipDistance", &IFrustum::getFarClipDistance )
                        .def( "setFarClipDistance", &IFrustum::setFarClipDistance )
                        .def( "getAspectRatio", &IFrustum::getAspectRatio )
                        .def( "setAspectRatio", &IFrustum::setAspectRatio )];

        module( L )[class_<ICamera, IFrustum, SharedPtr<ISharedObject>>( "ICamera" )
                        .def( "setNearClipDistance", &ICamera::setNearClipDistance )
                        .def( "setFarClipDistance", &ICamera::setFarClipDistance )];

        module( L )[class_<IGraphicsMesh, IGraphicsObject, SharedPtr<ISharedObject>>( "IGraphicsMesh" )
                        .def( "getAnimationController", &IGraphicsMesh::getAnimationController )];

        module( L )[class_<ILight, IGraphicsObject, SharedPtr<ISharedObject>>( "ILight" )
                        //.def( "setType", &ILight::setType )
                        //.def( "getType", &ILight::getType )

                        .def( "setDiffuseColour", &ILight::setDiffuseColour )
                        .def( "getDiffuseColour", &ILight::getDiffuseColour )

                        .def( "setSpecularColour", &ILight::setSpecularColour )
                        .def( "getSpecularColour", &ILight::getSpecularColour )

                        .def( "setAttenuation", &ILight::setAttenuation )

                        .def( "getAttenuationRange", &ILight::getAttenuationRange )
                        .def( "getAttenuationConstant", &ILight::getAttenuationConstant )
                        .def( "getAttenuationLinear", &ILight::getAttenuationLinear )
                        .def( "getAttenuationQuadric", &ILight::getAttenuationQuadric )];

        //module(L)
        //	[
        //		class_<CMeshObject, IGraphicsMesh, SmartPtr<IGraphicsObject>>("CMeshObject")
        //	];

        //
        // Animation
        //
        module( L )[class_<IAnimationController, IScriptObject, SharedPtr<ISharedObject>>(
                        "AnimationController" )
                        //.def( "setAnimationEnabled", AnimationControllerHelper::_setAnimationEnabled )
                        //.def( "setAnimationEnabled",
                        //     AnimationControllerHelper::_setAnimationEnabledPosition )
                        .def( "isAnimationEnabled", &IAnimationController::isAnimationEnabled )
                        .def( "stopAllAnimations", &IAnimationController::stopAllAnimations )
                        .def( "hasAnimationEnded", &IAnimationController::hasAnimationEnded )
                        .def( "hasAnimation", &IAnimationController::hasAnimation )
                        .def( "setAnimationLoop", &IAnimationController::setAnimationLoop )
                        .def( "isAnimationLooping", &IAnimationController::isAnimationLooping )
                        .def( "setAnimationReversed", &IAnimationController::setAnimationReversed )
                        .def( "isAnimationReversed", &IAnimationController::isAnimationReversed )
                        .def( "setTimePosition", &IAnimationController::setTimePosition )
                        .def( "getTimePosition", &IAnimationController::getTimePosition )
                        .def( "getAnimationLength", &IAnimationController::getAnimationLength )];

        module(
            L )[class_<IAnimationStateController, IScriptObject, SharedPtr<ISharedObject>>(
                    "AnimationStateController" )
                    //.def( "setAnimationEnabled", AnimationControllerHelper::_setAnimationStateEnabled )
                    //.def( "setAnimationEnabled",
                    //      AnimationControllerHelper::_setAnimationStateEnabledPosition )
                    .def( "isAnimationEnabled", &IAnimationStateController::isAnimationEnabled )
                    .def( "stopAllAnimations", &IAnimationStateController::stopAllAnimations )
                    .def( "hasAnimationEnded", &IAnimationStateController::hasAnimationEnded )
                    .def( "hasAnimation", &IAnimationStateController::hasAnimation )
                    .def( "setAnimationLoop", &IAnimationStateController::setAnimationLoop )
                    .def( "isAnimationLooping", &IAnimationStateController::isAnimationLooping )
                    .def( "setAnimationReversed", &IAnimationStateController::setAnimationReversed )
                    .def( "isAnimationReversed", &IAnimationStateController::isAnimationReversed )
                    .def( "setTimePosition", &IAnimationStateController::setTimePosition )
                    .def( "getTimePosition", &IAnimationStateController::getTimePosition )
                //.def("getAnimationLength", &IAnimationStateController::getAnimationLength )
        ];

        module(
            L )[class_<IParticleSystem, IGraphicsObject, SharedPtr<ISharedObject>>( "IParticleSystem" )
                    .def( "reload", &IParticleSystem::reload )
                    .def( "prepare", &IParticleSystem::prepare )
                    .def( "start", &IParticleSystem::start )
                    .def( "stop", &IParticleSystem::stop )
                    .def( "stopFade", &IParticleSystem::stopFade )

                    .def( "pause", ( void( IParticleSystem::* )( void ) ) & IParticleSystem::pause )
                    .def( "pause", ( void( IParticleSystem::* )( float ) ) & IParticleSystem::pause )
                    .def( "resume", &IParticleSystem::resume )

                    .def( "setFastForward", &IParticleSystem::setFastForward )
                    .def( "getFastForwardTime", &IParticleSystem::getFastForwardTime )
                    .def( "getFastForwardInterval", &IParticleSystem::getFastForwardInterval )

                    .def( "setTemplateName", &IParticleSystem::setTemplateName )
                    .def( "getTemplateName", &IParticleSystem::getTemplateName )

                    .def( "setScale", &IParticleSystem::setScale )
                    .def( "getScale", &IParticleSystem::getScale )

                    .def( "getState", &IParticleSystem::getState )

                    .def( "getTechnique", &IParticleSystem::getTechnique )];

        module( L )[class_<IParticleTechnique, IScriptObject, SharedPtr<ISharedObject>>(
            "ParticleTechnique" )];

        //module(
        //    L )[class_<GraphicsContainer, IComponent, boost::shared_ptr<IObject>>( "GraphicsContainer" )
        //            .def( "setObject", &GraphicsContainer::setObject )
        //            .def( "setObject", GraphicsContainerHelper::_setObjectByName )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectSceneNode )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectSceneNodeHash )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectGfxObj )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectGfxObjHash )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectGfxMesh )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectGfxMeshHash )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectParticleSystem)
        //            //.def("setObject", GraphicsContainerHelper::_setObjectParticleSystemHash )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectAnimationCtrl )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectAnimationCtrlHash )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectAnimationStateCtrl )
        //            //.def("setObject", GraphicsContainerHelper::_setObjectAnimationStateCtrlHash )
        //            .def( "getSceneNode", GraphicsContainerHelper::_getSceneNode )
        //            .def( "getSceneNode", &GraphicsContainer::getSceneNode )
        //            .def( "getGraphicsObject", &GraphicsContainer::getGraphicsObject )
        //            .def( "getAnimationController", &GraphicsContainer::getAnimationController )
        //            .def( "getAnimationStateControl",
        //                  GraphicsContainerHelper::_getAnimationStateController )
        //            .def( "getAnimationStateControl", &GraphicsContainer::getAnimationStateController )
        //            .def( "getAnimationController", GraphicsContainerHelper::_getAnimationCtrl )
        //            .def( "getMesh", GraphicsContainerHelper::_getMesh )
        //            .def( "getMesh", GraphicsContainerHelper::_getMeshHash )];

        module(
            L )[class_<IResourceManager, IScriptObject, SharedPtr<ISharedObject>>( "IResourceManager" )
                    .def( "getById", &IResourceManager::getById )
                    .def( "getByName", &IResourceManager::getByName )];

        //
        // Material system
        //
        //module( L )[class_<IMaterialManager, IResourceManager, boost::shared_ptr<IObject>>(
        //                "MaterialManager" )
        //                .def( "cloneMaterial", &IMaterialManager::cloneMaterial )];

        module(
            L )[class_<IMaterial, IResource, SmartPtr<ISharedObject>>( "IMaterial" )
                    .def( "getRoot", &IMaterial::getRoot )
                    .def( "setRoot", &IMaterial::setRoot )
                    .def( "createTechnique", &IMaterial::createTechnique )
                    .def( "removeTechnique", &IMaterial::removeTechnique )
                    .def( "removeAllTechniques", &IMaterial::removeAllTechniques )
                    .def( "getTechniques", &IMaterial::getTechniques )
                    .def( "setTechniques", &IMaterial::setTechniques )
                    .def( "getTechniqueByScheme", &IMaterial::getTechniqueByScheme )
                    .def( "getNumTechniques", &IMaterial::getNumTechniques )
                    .def( "setTexture", static_cast<void ( IMaterial::* )( SmartPtr<ITexture>, u32 )>(
                                            &IMaterial::setTexture ) )
                    .def( "setTexture", static_cast<void ( IMaterial::* )( const std::string &, u32 )>(
                                            &IMaterial::setTexture ) )
                    .def( "getTextureName", &IMaterial::getTextureName )
                    .def( "getTexture", &IMaterial::getTexture )
                    //.def( "setCubicTexture", static_cast<void ( IMaterial::* )(
                    //                             Array<SmartPtr<render::ITexture>> &,
                    //                             u32 )>( &IMaterial::setCubicTexture ) )
                    .def( "setCubicTexture",
                          static_cast<void ( IMaterial::* )( const std::string &, bool, u32 )>(
                              &IMaterial::setCubicTexture ) )
                    .def( "setScale", &IMaterial::setScale )
                    //.def( "getMaterialType", &IMaterial::getMaterialType )
                    //.def( "setMaterialType", &IMaterial::setMaterialType )
                    .scope[def( "typeInfo", IMaterial::typeInfo )]];

        //module( L )[class_<IMaterialNodePass, IScriptObject, boost::shared_ptr<IObject>>( "Pass" )
        //                .def( "getName", &IMaterialNodePass::getName )
        //                .def( "setName", &IMaterialNodePass::setName )
        //                .def( "setSceneBlending", &IMaterialNodePass::setSceneBlending )
        //                .def( "isDepthCheckEnabled", &IMaterialNodePass::isDepthCheckEnabled )
        //                .def( "setDepthCheckEnabled", &IMaterialNodePass::setDepthCheckEnabled )

        //                .def( "isDepthWriteEnabled", &IMaterialNodePass::isDepthWriteEnabled )
        //                .def( "setDepthWriteEnabled", &IMaterialNodePass::setDepthWriteEnabled )];

        //module( L )[class_<IMaterialNodeTexture, IScriptObject, boost::shared_ptr<IObject>>(
        //                "TextureUnit" )
        //                .def( "getTextureName", &IMaterialNodeTexture::getTextureName )
        //                .def( "setTextureName", &IMaterialNodeTexture::setTextureName )];

        //module( L )[class_<ICompositorManager, IScriptObject, boost::shared_ptr<IObject>>(
        //                "ICompositorManager" )
        //                .def( "registerCompositors", &ICompositorManager::registerCompositors )
        //                .def( "registerCompositor", &ICompositorManager::registerCompositor )
        //                .def( "removeCompositor", &ICompositorManager::removeCompositor )
        //                .def( "setCompositorEnabled", &ICompositorManager::setCompositorEnabled )
        //                .def( "isCompositorEnabled", &ICompositorManager::isCompositorEnabled )
        //                .def( "setCompositorProperties", &ICompositorManager::setCompositorProperties )
        //                .def( "getCompositorProperties", &ICompositorManager::getCompositorProperties )];

        typedef Array<u8> ArrayU8;
        typedef ArrayFunctions<ArrayU8> ArrayU8Functions;

        module( L )[class_<ArrayU8>( "ArrayU8" )
                        .def( constructor<>() )

                        .def( "reserve", &ArrayU8::reserve )
                    //.def("set_used", &ArrayU8::set_used )

                    //.def("push_back", &ArrayU8::push_back )

                    //.def("erase", (void (ArrayU8::*)(u32))&ArrayU8::erase_element_index )
                    //.def("erase_element", &ArrayU8::erase_element )

                    //.def("get", ArrayU8Functions::get )
        ];

        module( L )[class_<ITerrain, IScriptObject, boost::shared_ptr<IObject>>( "Terrain" )
                        .def( "load", &ITerrain::load )
                        .def( "getHeightAtWorldPosition", &ITerrain::getHeightAtWorldPosition )
                        .def( "getSize", &ITerrain::getSize )
                        .def( "getTerrainSpacePosition", &ITerrain::getTerrainSpacePosition )
                        .def( "isVisible", &ITerrain::isVisible )
                        .def( "setVisible", &ITerrain::setVisible )
                        .def( "getMaterialName", &ITerrain::getMaterialName )
                        .def( "setMaterialName", &ITerrain::setMaterialName )
                        .def( "getHeightData", &ITerrain::getHeightData )];

        module( L )[class_<IWater, IScriptObject, boost::shared_ptr<IObject>>( "Water" )
                        .def( "getSceneManager", &IWater::getSceneManager )
                        .def( "setSceneManager", &IWater::setSceneManager )
                        .def( "getCamera", &IWater::getCamera )
                        .def( "setCamera", &IWater::setCamera )
                        .def( "getViewport", &IWater::getViewport )
                        .def( "setViewport", &IWater::setViewport )

                        .def( "getPosition", &IWater::getPosition )
                        .def( "setPosition", &IWater::setPosition )];

        //module(L)
        //	[
        //		class_<WaterStandard, IWater, WaterPtr>("WaterStandard")
        //	];

        //module(
        //    L )[class_<IGraphicsScene, IScriptObject, boost::shared_ptr<IObject>>( "GraphicsScene" )];

        module( L )[class_<ITextureManager, IResourceManager, boost::shared_ptr<IObject>>(
                        "ITextureManager" )
                        .def( "createManual", &ITextureManager::createManual )];

        module( L )[class_<ITexture, IResource, boost::shared_ptr<IObject>>( "ITexture" )
                        .def( "getRenderTarget", &ITexture::getRenderTarget )];

        //module( L )[class_<GraphicsTypes>( "GraphicsTypes" )];

        //LUA_CONST_START( GraphicsTypes )
        //LUA_CONST( GraphicsTypes, PF_R8G8B8A8 );

        //LUA_CONST( GraphicsTypes, TU_STATIC );
        //LUA_CONST( GraphicsTypes, TU_DYNAMIC );
        //LUA_CONST( GraphicsTypes, TU_WRITE_ONLY );
        //LUA_CONST( GraphicsTypes, TU_STATIC_WRITE_ONLY );
        //LUA_CONST( GraphicsTypes, TU_DYNAMIC_WRITE_ONLY );
        //LUA_CONST( GraphicsTypes, TU_DYNAMIC_WRITE_ONLY_DISCARDABLE );
        //LUA_CONST( GraphicsTypes, TU_AUTOMIPMAP );
        //LUA_CONST( GraphicsTypes, TU_RENDERTARGET );
        //LUA_CONST( GraphicsTypes, TU_DEFAULT );

        //LUA_CONST( GraphicsTypes, LT_POINT );
        //LUA_CONST_END;

        //LUA_CONST_START( ITexture )
        //LUA_CONST( ITexture, TU_STATIC );
        //LUA_CONST( ITexture, TU_DYNAMIC );
        //LUA_CONST( ITexture, TU_WRITE_ONLY );
        //LUA_CONST( ITexture, TU_STATIC_WRITE_ONLY );
        //LUA_CONST( ITexture, TU_DYNAMIC_WRITE_ONLY );
        //LUA_CONST( ITexture, TU_DYNAMIC_WRITE_ONLY_DISCARDABLE );
        //LUA_CONST( ITexture, TU_AUTOMIPMAP );
        //LUA_CONST( ITexture, TU_RENDERTARGET );
        //LUA_CONST( ITexture, TU_DEFAULT );

        //LUA_CONST( ITexture, TEX_TYPE_1D );
        //LUA_CONST( ITexture, TEX_TYPE_2D );
        //LUA_CONST( ITexture, TEX_TYPE_3D );
        //LUA_CONST( ITexture, TEX_TYPE_CUBE_MAP );
        //LUA_CONST( ITexture, TEX_TYPE_2D_ARRAY );

        //LUA_CONST( ITexture, MIP_UNLIMITED );
        //LUA_CONST( ITexture, MIP_DEFAULT );
        //LUA_CONST_END;
    }

}  // end namespace fb
