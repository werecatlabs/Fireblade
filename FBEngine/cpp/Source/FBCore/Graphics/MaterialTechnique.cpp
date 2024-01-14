#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/MaterialTechnique.h>
#include <FBCore/Graphics/MaterialPass.h>
#include <FBCore/FBCore.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, MaterialTechnique, IMaterialTechnique );

    MaterialTechnique::MaterialTechnique() = default;

    MaterialTechnique::~MaterialTechnique()
    {
        unload( nullptr );
    }

    void MaterialTechnique::load( SmartPtr<ISharedObject> data )
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

    void MaterialTechnique::reload( SmartPtr<ISharedObject> data )
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

                setPassesPtr( nullptr );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MaterialTechnique::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            if( auto p = getPassesPtr() )
            {
                auto &passes = *p;
                for( auto pass : passes )
                {
                    if( pass )
                    {
                        pass->unload( nullptr );
                    }
                }

                setPassesPtr( nullptr );
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto MaterialTechnique::getScheme() const -> hash32
    {
        return m_scheme;
    }

    void MaterialTechnique::setScheme( hash32 scheme )
    {
        m_scheme = scheme;
    }

    auto MaterialTechnique::getNumPasses() const -> u32
    {
        if( auto p = getPassesPtr() )
        {
            auto &passes = *p;
            return static_cast<u32>( passes.size() );
        }

        return 0;
    }

    auto MaterialTechnique::createPass() -> SmartPtr<IMaterialPass>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
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

    void MaterialTechnique::addPass( SmartPtr<IMaterialPass> pass )
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

    void MaterialTechnique::removePass( SmartPtr<IMaterialPass> pass )
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

    void MaterialTechnique::removePasses()
    {
        if( auto p = getPassesPtr() )
        {
            auto &passes = *p;
            passes.clear();
        }
    }

    auto MaterialTechnique::getPasses() const -> Array<SmartPtr<IMaterialPass>>
    {
        if( auto p = getPassesPtr() )
        {
            return *p;
        }

        return {};
    }

    void MaterialTechnique::setPasses( Array<SmartPtr<IMaterialPass>> passes )
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

    auto MaterialTechnique::getPassesPtr() const -> SharedPtr<Array<SmartPtr<IMaterialPass>>>
    {
        return m_passes;
    }

    void MaterialTechnique::setPassesPtr( SharedPtr<Array<SmartPtr<IMaterialPass>>> ptr )
    {
        m_passes = ptr;
    }

    auto MaterialTechnique::toData() const -> SmartPtr<ISharedObject>
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

    void MaterialTechnique::fromData( SmartPtr<ISharedObject> data )
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

    auto MaterialTechnique::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = MaterialNode<IMaterialTechnique>::getProperties();

        auto handle = getHandle();
        properties->setProperty( "name", handle->getName() );

        return properties;
    }

    void MaterialTechnique::setProperties( SmartPtr<Properties> properties )
    {
    }

    auto MaterialTechnique::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto passes = getPasses();

        auto objects = Array<SmartPtr<ISharedObject>>();
        objects.reserve( passes.size() );

        for( auto pass : passes )
        {
            objects.emplace_back( pass );
        }

        return objects;
    }

    void MaterialTechnique::MaterialTechniqueStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
    }

    void MaterialTechnique::MaterialTechniqueStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
    }

    void MaterialTechnique::MaterialTechniqueStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }
}  // namespace fb::render
