/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"

#include "OgreRenderable.h"
#include "OgreHlmsLowLevelDatablock.h"
#include "OgreHlms.h"
#include "OgreHlmsManager.h"
#include "OgreMaterialManager.h"
#include "OgreLogManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreRoot.h"

namespace Ogre
{
    uint8 Renderable::msDefaultRenderQueueSubGroup = 0;
    //-----------------------------------------------------------------------------------
    Renderable::Renderable() :
        mHlmsHash( 0 ),
        mHlmsCasterHash( 0 ),
        mHlmsDatablock( nullptr ),
        mCustomParameter( 0 ),
        mRenderQueueSubGroup( msDefaultRenderQueueSubGroup ),
        mHasSkeletonAnimation( false ),
        mCurrentMaterialLod( 0 ),
        mLodMaterial( &MovableObject::c_DefaultLodMesh ),
        mHlmsGlobalIndex( std::numeric_limits<uint32>::max() ),
        mRenderableVisible( true ),
        mPolygonModeOverrideable( true ),
        mUseIdentityProjection( false ),
        mUseIdentityView( false )
    {
    }

    //-----------------------------------------------------------------------------------
    Renderable::~Renderable()
    {
        if(mHlmsDatablock)
        {
            mHlmsDatablock->_unlinkRenderable( this );
            mHlmsDatablock = nullptr;
        }
    }


    bool Renderable::getUseIdentityViewProjMatrixIsDynamic( void ) const
    {
        return false;
    }

    void Renderable::setUseIdentityProjection( bool useIdentityProjection )
    {
        mUseIdentityProjection = useIdentityProjection;
    }

    bool Renderable::getUseIdentityProjection( void ) const
    {
        return mUseIdentityProjection;
    }

    void Renderable::setUseIdentityView( bool useIdentityView )
    {
        mUseIdentityView = useIdentityView;
    }

    bool Renderable::getUseIdentityView( void ) const
    {
        return mUseIdentityView;
    }

    bool Renderable::getCastsShadows( void ) const
    {
        return false;
    }

    void Renderable::setCustomParameter( size_t index, const Vector4 &value )
    {
        mCustomParameters[index] = value;
    }

    void Renderable::removeCustomParameter( size_t index )
    {
        mCustomParameters.erase( index );
    }

    bool Renderable::hasCustomParameter( size_t index ) const
    {
        return mCustomParameters.find( index ) != mCustomParameters.end();
    }

    const Vector4 &Renderable::getCustomParameter( size_t index ) const
    {
        auto i = mCustomParameters.find( index );
        if(i != mCustomParameters.end())
        {
            return i->second;
        }
        OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Parameter at the given index was not found.",
                     "Renderable::getCustomParameter" );
    }

    const Renderable::CustomParameterMap &Renderable::getCustomParameters( void ) const
    {
        return mCustomParameters;
    }

    //-----------------------------------------------------------------------------------
    void Renderable::_updateCustomGpuParameter(
        const GpuProgramParameters_AutoConstantEntry &constantEntry, GpuProgramParameters *params ) const
    {
        auto i = mCustomParameters.find( constantEntry.data );
        if(i != mCustomParameters.end())
        {
            params->_writeRawConstant( constantEntry.physicalIndex, i->second,
                                       constantEntry.elementCount );
        }
    }


    void Renderable::setPolygonModeOverrideable( bool override )
    {
        mPolygonModeOverrideable = override;
    }

    bool Renderable::getPolygonModeOverrideable( void ) const
    {
        return mPolygonModeOverrideable;
    }

    OGRE_DEPRECATED void Renderable::setUserAny( const Any &anything )
    {
        getUserObjectBindings().setUserAny( anything );
    }

    OGRE_DEPRECATED const Any &Renderable::getUserAny( void ) const
    {
        return getUserObjectBindings().getUserAny();
    }

    const UserObjectBindings &Renderable::getUserObjectBindings() const
    {
        return mUserObjectBindings;
    }


    UserObjectBindings &Renderable::getUserObjectBindings()
    {
        return mUserObjectBindings;
    }

    const VertexArrayObjectArray &Renderable::getVaos( VertexPass vertexPass ) const
    {
        return mVaoPerLod[vertexPass];
    }

    uint32 Renderable::getHlmsHash( void ) const
    {
        return mHlmsHash;
    }

    uint32 Renderable::getHlmsCasterHash( void ) const
    {
        return mHlmsCasterHash;
    }

    HlmsDatablock *Renderable::getDatablock( void ) const
    {
        return mHlmsDatablock;
    }

    //-----------------------------------------------------------------------------------
    const String &Renderable::getDatablockOrMaterialName() const
    {
        HlmsDatablock *datablock = getDatablock();
        if(datablock && datablock->getCreator()->getType() != HLMS_LOW_LEVEL)
            if(const String *nameStr = datablock->getNameStr()) // could be null if leaked
                return *nameStr;

        if(MaterialPtr mat = getMaterial())
            return mat->getName();

        return BLANKSTRING;
    }

    //-----------------------------------------------------------------------------------
    void Renderable::setDatablockOrMaterialName( String materialName, String resourceGroup )
    {
        //Try first Hlms materials, then the low level ones.
        HlmsManager *hlmsManager = Root::getSingleton().getHlmsManager();
        HlmsDatablock *datablock = hlmsManager->getDatablockNoDefault( materialName );

        if(datablock)
            this->setDatablock( datablock );
        else
            this->setMaterialName( materialName, resourceGroup );
    }

    //-----------------------------------------------------------------------------------
    void Renderable::setDatablock( IdString datablockName )
    {
        HlmsManager *hlmsManager = Root::getSingleton().getHlmsManager();
        setDatablock( hlmsManager->getDatablock( datablockName ) );
    }

    //-----------------------------------------------------------------------------------
    void Renderable::setDatablock( HlmsDatablock *datablock )
    {
        if(mHlmsDatablock != datablock)
        {
            if(mHlmsDatablock)
                mHlmsDatablock->_unlinkRenderable( this );

            if(!datablock || datablock->getCreator()->getType() != HLMS_LOW_LEVEL)
                mMaterial.setNull();

            mHlmsDatablock = datablock;
            try
            {
                uint32 hash, casterHash;
                mHlmsDatablock->getCreator()->calculateHashFor( this, hash, casterHash );
                this->_setHlmsHashes( hash, casterHash );
            }
            catch(Exception &e)
            {
                LogManager::getSingleton().logMessage( e.getFullDescription() );
                LogManager::getSingleton().logMessage( "Couldn't apply datablock '" +
                                                       datablock->getName().getFriendlyText() + "' to "
                                                       "this renderable. Using default one. Check "
                                                       "previous log messages to see if there's more "
                                                       "information.", LML_CRITICAL );

                if(mHlmsDatablock->mType == HLMS_LOW_LEVEL)
                {
                    HlmsManager *hlmsManager = Root::getSingleton().getHlmsManager();
                    mHlmsDatablock = hlmsManager->getDefaultDatablock();
                }
                else
                {
                    //Try to use the default datablock from the same
                    //HLMS as the one the user wanted us to apply
                    mHlmsDatablock = mHlmsDatablock->getCreator()->getDefaultDatablock();
                }

                uint32 hash, casterHash;
                mHlmsDatablock->getCreator()->calculateHashFor( this, hash, casterHash );
                this->_setHlmsHashes( hash, casterHash );
            }

            mHlmsDatablock->_linkRenderable( this );
        }
    }

    //-----------------------------------------------------------------------------------
    void Renderable::_setNullDatablock( void )
    {
        if(mHlmsDatablock)
            mHlmsDatablock->_unlinkRenderable( this );

        mMaterial.setNull();

        mHlmsDatablock = nullptr;
        mHlmsHash = 0;
        mHlmsCasterHash = 0;
    }

    //-----------------------------------------------------------------------------------
    void Renderable::_setHlmsHashes( uint32 hash, uint32 casterHash )
    {
        mHlmsHash = hash;
        mHlmsCasterHash = casterHash;

        assert( (mHlmsDatablock == 0 || mHlmsDatablock->getAlphaTest() == CMPF_ALWAYS_PASS ||
                mVaoPerLod[0].empty() || mVaoPerLod[0][0] == mVaoPerLod[1][0])
            && "v2 objects must overload _setHlmsHashes to disable special "
            "shadow mapping buffers on objects with alpha testing materials" );
    }


    uint8 Renderable::getCurrentMaterialLod( void ) const
    {
        return mCurrentMaterialLod;
    }

    void Renderable::setRenderQueueSubGroup( uint8 subGroup )
    {
        mRenderQueueSubGroup = subGroup;
    }

    uint8 Renderable::getRenderQueueSubGroup( void ) const
    {
        return mRenderQueueSubGroup;
    }

    void Renderable::setDefaultRenderQueueSubGroup( uint8 subGroup )
    {
        msDefaultRenderQueueSubGroup = subGroup;
    }

    uint8 Renderable::getDefaultRenderQueueSubGroup()
    {
        return msDefaultRenderQueueSubGroup;
    }

    //-----------------------------------------------------------------------------------
    void Renderable::setMaterialName( const String &name, const String &groupName )
    {
        MaterialPtr material;

        if(!name.empty())
            material = MaterialManager::getSingleton().getByName( name, groupName );

        if(material.isNull())
        {
            if(!name.empty())
            {
                LogManager::getSingleton().logMessage( "Can't assign material " + name +
                                                       " because this Material does not exist. "
                                                       "Have you forgotten to define it in a "
                                                       ".material script?", LML_CRITICAL );
            }

            HlmsManager *hlmsManager = Root::getSingleton().getHlmsManager();
            setDatablock( hlmsManager->getDefaultDatablock() );
        }
        else
        {
            setMaterial( material );
        }
    }

    //-----------------------------------------------------------------------------------
    void Renderable::setMaterial( const MaterialPtr &material )
    {
        // Ensure new material loaded (will not load again if already loaded)
        material->load();
        mMaterial = material;
        setDatablock( material->getTechnique( 0 )->getPass( 0 )->_getDatablock() );
        mLodMaterial = material->_getLodValues();
    }

    //-----------------------------------------------------------------------------------
    MaterialPtr Renderable::getMaterial( void ) const
    {
        return mMaterial;
    }


    bool Renderable::preRender( SceneManager *sm, RenderSystem *rsys )
    {
        (void)sm;
        (void)rsys;
        return true;
    }

    void Renderable::postRender( SceneManager *sm, RenderSystem *rsys )
    {
        (void)sm;
        (void)rsys;
    }

    unsigned short Renderable::getNumWorldTransforms( void ) const
    {
        return 1;
    }

    bool Renderable::hasSkeletonAnimation( void ) const
    {
        return mHasSkeletonAnimation;
    }

    //-----------------------------------------------------------------------------------
    unsigned short Renderable::getNumPoses( void ) const
    {
        return mPoseData ? mPoseData->numPoses : 0;
    }

    //-----------------------------------------------------------------------------------
    bool Renderable::getPoseHalfPrecision() const
    {
        return mPoseData ? mPoseData->halfPrecision : false;
    }

    //-----------------------------------------------------------------------------------
    bool Renderable::getPoseNormals() const
    {
        return mPoseData ? mPoseData->hasNormals : false;
    }

    //-----------------------------------------------------------------------------------
    float *Renderable::getPoseWeights( void ) const
    {
        return mPoseData ? mPoseData->weights : nullptr;
    }

    //-----------------------------------------------------------------------------------
    float Renderable::getPoseWeight( size_t index ) const
    {
        assert( (index < OGRE_MAX_POSES) && "Pose weight index out of bounds" );
        return mPoseData ? mPoseData->weights[index] : 0;
    }

    //-----------------------------------------------------------------------------------
    void Renderable::setPoseWeight( size_t index, float w )
    {
        if(!mPoseData)
            return;

        assert( (index < OGRE_MAX_POSES && index < mPoseData->numPoses) &&
            "Pose weight index out of bounds" );
        mPoseData->weights[index] = w;
    }

    //-----------------------------------------------------------------------------------
    void Renderable::addPoseWeight( size_t index, float w )
    {
        if(!mPoseData)
            return;

        assert( (index < OGRE_MAX_POSES && index < mPoseData->numPoses) &&
            "Pose weight index out of bounds" );
        mPoseData->weights[index] += w;
    }

    //-----------------------------------------------------------------------------------
    TexBufferPacked *Renderable::getPoseTexBuffer( void ) const
    {
        return mPoseData ? mPoseData->buffer : nullptr;
    }

    bool Renderable::getUseIdentityWorldMatrix( void ) const
    {
        return false;
    }

    //-----------------------------------------------------------------------------------
    RenderableAnimated::RenderableAnimated() :
        Renderable(),
        mBlendIndexToBoneIndexMap( nullptr )
    {
    }

    //-----------------------------------------------------------------------------------
    Renderable::PoseData::PoseData():
        numPoses( 0 ),
        buffer( nullptr ),
        halfPrecision( false ),
        hasNormals( false )
    {
        memset( weights, 0, OGRE_MAX_POSES * sizeof( float ) );
    }
}
