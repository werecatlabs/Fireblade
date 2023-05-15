/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2013 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/
#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include "DeferredLightCP.h"

#include "Ogre.h"
using namespace Ogre;

#include "LightMaterialGenerator.h"

#include "FBGraphicsOgre/Deferred/DeferredShading.h"


DeferredLightRenderOperation::DeferredLightRenderOperation( DeferredShadingSystem *system,
                                                            CompositorInstance *instance,
                                                            const CompositionPass *pass ) :
    mDeferredShadingSystem( system ),
    m_instance( instance )
{
    mViewport = instance->getChain()->getViewport();

    // Get the names of the GBuffer textures
    const CompositionPass::InputTex &input0 = pass->getInput( 0 );
    mTexName0 = instance->getTextureInstanceName( input0.name, input0.mrtIndex );
    const CompositionPass::InputTex &input1 = pass->getInput( 1 );
    mTexName1 = instance->getTextureInstanceName( input1.name, input1.mrtIndex );

    // Create lights material generator
    mLightMaterialGenerator = new LightMaterialGenerator( mDeferredShadingSystem );

    // Create the ambient light
    mAmbientLight = new AmbientLight( mDeferredShadingSystem );
    auto &mat = mAmbientLight->getMaterial();
    mat->load();
}

DLight *DeferredLightRenderOperation::createDLight( Ogre::Light *light )
{
    DLight *rv = new DLight( mLightMaterialGenerator, light );
    mLights[light] = rv;
    return rv;
}

void injectTechnique( SceneManager *sm, Technique *tech, Renderable *rend,
                      const Ogre::LightList *lightList )
{
    for( unsigned short i = 0; i < tech->getNumPasses(); ++i )
    {
        Ogre::Pass *pass = tech->getPass( i );
        if( lightList != 0 )
        {
            sm->_injectRenderWithPass( pass, rend, false, false, lightList );
        }
        else
        {
            sm->_injectRenderWithPass( pass, rend, false );
        }
    }
}

void DeferredLightRenderOperation::execute( SceneManager *sm, RenderSystem *rs )
{
    Ogre::Camera *cam = mViewport->getCamera();
    sm->_setActiveCompositorChain( m_instance->getChain() );

    mAmbientLight->updateFromCamera( cam );
    Technique *tech = mAmbientLight->getMaterial()->getBestTechnique();
    injectTechnique( sm, tech, mAmbientLight, 0 );

    const LightList &lightList = sm->_getLightsAffectingFrustum();
    for( LightList::const_iterator it = lightList.begin(); it != lightList.end(); it++ )
    {
        Light *light = *it;
        Ogre::LightList ll;
        ll.push_back( light );

        // if (++i != 2) continue;
        // if (light->getType() != Light::LT_DIRECTIONAL) continue;
        // if (light->getDiffuseColour() != ColourValue::Red) continue;

        LightsMap::iterator dLightIt = mLights.find( light );
        DLight *dLight = 0;
        if( dLightIt == mLights.end() )
        {
            dLight = createDLight( light );
        }
        else
        {
            dLight = dLightIt->second;
            dLight->updateFromParent();
        }
        dLight->updateFromCamera( cam );
        tech = dLight->getMaterial()->getBestTechnique();

        // Update shadow texture
        if( dLight->getCastChadows() )
        {
            SceneManager::RenderContext *context = sm->_pauseRendering();

            sm->prepareShadowTextures( cam, mViewport, &ll );
            sm->_resumeRendering( context );

            Pass *pass = tech->getPass( 0 );
            TextureUnitState *tus = pass->getTextureUnitState( "ShadowMap" );
            assert( tus );
            const auto &shadowTex = sm->getShadowTexture( 0 );
            if( tus->_getTexturePtr() != shadowTex )
            {
                tus->_setTexturePtr( shadowTex );
            }
        }

        sm->_setActiveCompositorChain( m_instance->getChain() );
        injectTechnique( sm, tech, dLight, &ll );
    }
}

DeferredLightRenderOperation::~DeferredLightRenderOperation()
{
    for( LightsMap::iterator it = mLights.begin(); it != mLights.end(); ++it )
    {
        delete it->second;
    }
    mLights.clear();

    delete mAmbientLight;
    delete mLightMaterialGenerator;
}


DeferredLightCompositionPass::DeferredLightCompositionPass( DeferredShadingSystem *system,
                                                            Ogre::SceneManager *smgr ) :
    mDeferredShadingSystem( system ),
    smgr( smgr )
{
}

Ogre::CompositorInstance::RenderSystemOperation *DeferredLightCompositionPass::createOperation(
    Ogre::CompositorInstance *instance, const Ogre::CompositionPass *pass )
{
    Ogre::CompositorChain *chain = instance->getChain();
    smgr->_setActiveCompositorChain( chain );

    // FB_ASSERT(instance->getChain() == smgr->_getActiveCompositorChain());
    // FB_ASSERT(smgr->_getActiveCompositorChain()->getCompositor(mDeferredShadingSystem->getGBufferCompositorName()));

    return OGRE_NEW DeferredLightRenderOperation( mDeferredShadingSystem, instance, pass );
}
