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
#include "MaterialGenerator.h"

#include "OgreStringConverter.h"
#include "OgreException.h"

#include "OgrePass.h"
#include "OgreTechnique.h"

#include "OgreHighLevelGpuProgram.h"
#include "OgreHighLevelGpuProgramManager.h"

#include "FBGraphicsOgre/Deferred/DeferredShading.h"
#include <FBCore/FBCoreTypes.h>

using namespace Ogre;

MaterialGenerator::MaterialGenerator( DeferredShadingSystem *system ) :
    mDeferredShadingSystem( system ),
    vsMask( 0 ),
    fsMask( 0 ),
    matMask( 0 ),
    mImpl( 0 )
{
}
MaterialGenerator::~MaterialGenerator()
{
    delete mImpl;
}

const Ogre::MaterialPtr &MaterialGenerator::getMaterial( Perm permutation )
{
    /// Check if material/shader permutation already was generated
    MaterialMap::iterator i = mMaterials.find( permutation );
    if( i != mMaterials.end() )
    {
        return i->second;
    }
    else
    {
        /// Create it
        Ogre::MaterialPtr templ = getTemplateMaterial( permutation & matMask );
        GpuProgramPtr vs = getVertexShader( permutation & vsMask );
        GpuProgramPtr fs = getFragmentShader( permutation & fsMask );

        /// Create material name
        String name = materialBaseName + StringConverter::toString( permutation );

        // std::cerr << name << " " << vs->getName() << " " << fs->getName() << std::endl;
        /// Create material from template, and set shaders
        Ogre::MaterialPtr mat = templ->clone( name );
        Technique *tech = mat->getTechnique( 0 );
        Pass *pass = tech->getPass( 0 );
        pass->setFragmentProgram( fs->getName() );
        pass->setVertexProgram( vs->getName() );

        /// And store it
        mMaterials[permutation] = mat;

        if( tech )
        {
            Ogre::Technique::PassIterator passIt = tech->getPassIterator();
            while( passIt.hasMoreElements() )
            {
                Ogre::Pass *pass = passIt.getNext();
                FB_ASSERT( pass );

                Ogre::Pass::TextureUnitStateIterator textureUnitStateIt =
                    pass->getTextureUnitStateIterator();
                while( textureUnitStateIt.hasMoreElements() )
                {
                    Ogre::TextureUnitState *textureStateUnit = textureUnitStateIt.getNext();
                    textureStateUnit->setCompositorReference(
                        mDeferredShadingSystem->getGBufferCompositorName(),
                        mDeferredShadingSystem->getRenderTargetName(),
                        textureStateUnit->getReferencedMRTIndex() );
                }
            }
        }

        return mMaterials[permutation];
    }
}

const GpuProgramPtr &MaterialGenerator::getVertexShader( Perm permutation )
{
    ProgramMap::iterator i = mVs.find( permutation );
    if( i != mVs.end() )
    {
        return i->second;
    }
    else
    {
        /// Create it
        mVs[permutation] = mImpl->generateVertexShader( permutation );
        return mVs[permutation];
    }
}

const GpuProgramPtr &MaterialGenerator::getFragmentShader( Perm permutation )
{
    ProgramMap::iterator i = mFs.find( permutation );
    if( i != mFs.end() )
    {
        return i->second;
    }
    else
    {
        /// Create it
        mFs[permutation] = mImpl->generateFragmentShader( permutation );
        return mFs[permutation];
    }
}

const Ogre::MaterialPtr &MaterialGenerator::getTemplateMaterial( Perm permutation )
{
    MaterialMap::iterator i = mTemplateMat.find( permutation );
    if( i != mTemplateMat.end() )
    {
        return i->second;
    }
    else
    {
        /// Create it
        mTemplateMat[permutation] = mImpl->generateTemplateMaterial( permutation );
        return mTemplateMat[permutation];
    }
}

MaterialGenerator::Impl::~Impl()
{
}
