#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CMaterialTechniqueOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialPassOgre.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>

#include "Wrapper/CMaterialOgre.h"

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialTechniqueOgre, IMaterialTechnique );

        CMaterialTechniqueOgre::CMaterialTechniqueOgre()
        {
        }

        CMaterialTechniqueOgre::~CMaterialTechniqueOgre()
        {
            unload( nullptr );
        }

        void CMaterialTechniqueOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto pMaterial = getMaterial();
                FB_ASSERT( pMaterial );
                //FB_ASSERT( pMaterial->isValid() );

                auto material = fb::static_pointer_cast<CMaterialOgre>( pMaterial );

                if( !m_technique )
                {
                    auto ogreMaterial = material->getMaterial();
                    auto technique = ogreMaterial->createTechnique();
                    setTechnique( technique );
                }

                FB_ASSERT( m_technique );

                if( getNumPasses() == 0 )
                {
                    if( auto p = fb::static_pointer_cast<CMaterialPassOgre>( createPass() ) )
                    {
                        p->setMaterial( pMaterial );
                        p->setParent( this );
                    }
                }

                auto ogrePasses = m_technique->getPasses();

                if( auto p = getPassesPtr() )
                {
                    auto count = 0;

                    auto &passes = *p;
                    for( auto pPass : passes )
                    {
                        auto pass = fb::static_pointer_cast<CMaterialPassOgre>( pPass );

                        if( count < ogrePasses.size() )
                        {
                            pass->setPass( ogrePasses[count] );
                        }

                        pass->setMaterial( pMaterial );
                        pass->setParent( this );
                        pass->load( nullptr );

                        count++;
                    }
                }

                FB_ASSERT( pMaterial->isValid() );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialTechniqueOgre::reload( SmartPtr<ISharedObject> data )
        {
            try
            {
                if( auto p = getPassesPtr() )
                {
                    auto &passes = *p;
                    for( auto pass : passes )
                    {
                        pass->reload( data );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialTechniqueOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                if( auto p = getPassesPtr() )
                {
                    auto &passes = *p;
                    for( auto pass : passes )
                    {
                        pass->unload( nullptr );
                    }
                }

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialTechniqueOgre::initialise( Ogre::Technique *technique )
        {
            setTechnique( technique );
        }

        SmartPtr<Properties> CMaterialTechniqueOgre::getProperties() const
        {
            auto properties = CMaterialNode<IMaterialTechnique>::getProperties();

            auto handle = getHandle();
            properties->setProperty( "name", handle->getName() );

            return properties;
        }

        void CMaterialTechniqueOgre::setProperties( SmartPtr<Properties> properties )
        {
        }

        Array<SmartPtr<ISharedObject>> CMaterialTechniqueOgre::getChildObjects() const
        {
            auto passes = getPasses();

            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( passes.size() );

            for( auto pass : passes )
            {
                objects.push_back( pass );
            }

            return objects;
        }

        Ogre::Technique *CMaterialTechniqueOgre::getTechnique() const
        {
            return m_technique;
        }

        void CMaterialTechniqueOgre::setTechnique( Ogre::Technique *technique )
        {
            if( m_technique != technique )
            {
                m_technique = technique;

                auto ogrePasses = technique->getPasses();

                auto numPasses = 0;
                if( auto p = getPassesPtr() )
                {
                    auto &_passes = *p;
                    numPasses = (s32)_passes.size();
                }

                if( ogrePasses.size() != numPasses )
                {
                    removePasses();

                    for( auto pass : ogrePasses )
                    {
                        auto pPass = fb::make_ptr<CMaterialPassOgre>();
                        pPass->setParent( this );
                        pPass->initialise( pass );
                        addPass( pPass );
                    }
                }
            }
        }

        void CMaterialTechniqueOgre::addPass( SmartPtr<IMaterialPass> pass )
        {
            //if( auto p = getPassesPtr() )
            //{
            //    FB_ASSERT( p->empty() );
            //}

            CMaterialTechnique::addPass( pass );

            //auto pPass = fb::static_pointer_cast<CMaterialPassOgre>( pass );
            //if (pPass)
            //{
            //    FB_ASSERT( pPass->getPass() );
            //}
        }

        void CMaterialTechniqueOgre::MaterialTextureOgreStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        void CMaterialTechniqueOgre::MaterialTextureOgreStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
        }

        void CMaterialTechniqueOgre::MaterialTextureOgreStateListener::handleQuery(
            SmartPtr<IStateQuery> &query )
        {
        }

    }  // end namespace render
}  // end namespace fb
