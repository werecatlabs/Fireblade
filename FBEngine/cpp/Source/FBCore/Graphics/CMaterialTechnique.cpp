#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CMaterialTechnique.h>
#include <FBCore/Graphics/CMaterialPass.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialTechnique, IMaterialTechnique );

        CMaterialTechnique::CMaterialTechnique()
        {
        }

        CMaterialTechnique::~CMaterialTechnique()
        {
            unload( nullptr );
        }

        void CMaterialTechnique::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                //FB_ASSERT( m_technique );

                if( auto p = getPassesPtr() )
                {
                    auto &passes = *p;
                    for( auto pass : passes )
                    {
                        pass->setParent( this );
                        pass->load( nullptr );
                    }
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CMaterialTechnique::reload( SmartPtr<ISharedObject> data )
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

        void CMaterialTechnique::unload( SmartPtr<ISharedObject> data )
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

        hash32 CMaterialTechnique::getScheme() const
        {
            return m_scheme;
        }

        void CMaterialTechnique::setScheme( hash32 scheme )
        {
            m_scheme = scheme;
        }

        u32 CMaterialTechnique::getNumPasses() const
        {
            if( auto p = getPassesPtr() )
            {
                auto &passes = *p;
                return (u32)passes.size();
            }

            return 0;
        }

        SmartPtr<IMaterialPass> CMaterialTechnique::createPass()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto pass = factoryManager->make_object<IMaterialPass>();
                FB_ASSERT( pass );

                auto material = getMaterial();
                pass->setMaterial( material );

                pass->setParent( this );
                addPass( pass );
                return pass;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void CMaterialTechnique::addPass( SmartPtr<IMaterialPass> pass )
        {
            auto p = getPassesPtr();
            if( !p )
            {
                p = fb::make_shared<Array<SmartPtr<IMaterialPass>>>();
                setPassesPtr( p );
            }

            if( p )
            {
                auto &passes = *p;

                passes.push_back( pass );
            }
        }

        void CMaterialTechnique::removePass( SmartPtr<IMaterialPass> pass )
        {
            if( auto p = getPassesPtr() )
            {
                auto &passes = *p;

                auto it = std::find( passes.begin(), passes.end(), pass );
                if( it != passes.end() )
                {
                    passes.erase( it );
                }
            }
        }

        void CMaterialTechnique::removePasses()
        {
            if( auto p = getPassesPtr() )
            {
                auto &passes = *p;
                passes.clear();
            }
        }

        Array<SmartPtr<IMaterialPass>> CMaterialTechnique::getPasses() const
        {
            if( auto p = getPassesPtr() )
            {
                return *p;
            }

            return Array<SmartPtr<IMaterialPass>>();
        }

        void CMaterialTechnique::setPasses( Array<SmartPtr<IMaterialPass>> passes )
        {
            auto p = getPassesPtr();
            if( !p )
            {
                p = fb::make_shared<Array<SmartPtr<IMaterialPass>>>();
                setPassesPtr( p );
            }

            if( p )
            {
                *p = Array<SmartPtr<IMaterialPass>>( passes.begin(), passes.end() );
            }
        }

        SharedPtr<Array<SmartPtr<IMaterialPass>>> CMaterialTechnique::getPassesPtr() const
        {
            return m_passes;
        }

        void CMaterialTechnique::setPassesPtr( SharedPtr<Array<SmartPtr<IMaterialPass>>> ptr )
        {
            m_passes = ptr;
        }

        
        SmartPtr<ISharedObject> CMaterialTechnique::toData() const
        {
            auto data = fb::make_ptr<Properties>();

            auto passes = getPasses();
            for( auto pass : passes )
            {
                auto passData = fb::static_pointer_cast<Properties>( pass->toData() );
                data->addChild( passData );
            }

            return data;
        }

        void CMaterialTechnique::fromData( SmartPtr<ISharedObject> data )
        {
            auto properties = fb::static_pointer_cast<Properties>( data );

            auto count = 0;

            auto currentPasses = getPasses();

            auto passes = properties->getChildrenByName( "passes" );
            for( auto &pass : passes )
            {
                auto pPass = SmartPtr<IMaterialPass>();

                
                if( count < currentPasses.size() )
                {
                    pPass = currentPasses[count];
                }
                else
                {
                    pPass = createPass();
                }

                pPass->fromData( pass );

                count++;
            }
        }

        SmartPtr<Properties> CMaterialTechnique::getProperties() const
        {
            auto properties = CMaterialNode<IMaterialTechnique>::getProperties();

            auto handle = getHandle();
            properties->setProperty( "name", handle->getName() );

            return properties;
        }

        void CMaterialTechnique::setProperties( SmartPtr<Properties> properties )
        {
        }

        Array<SmartPtr<ISharedObject>> CMaterialTechnique::getChildObjects() const
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

        void CMaterialTechnique::MaterialTechniqueStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        void CMaterialTechnique::MaterialTechniqueStateListener::handleStateChanged(
            SmartPtr<IState> &state )
        {
        }

        void CMaterialTechnique::MaterialTechniqueStateListener::handleQuery(
            SmartPtr<IStateQuery> &query )
        {
        }
    }  // end namespace render
}  // end namespace fb
