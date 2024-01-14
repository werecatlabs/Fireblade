#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTechniqueOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialPassOgreNext.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <OgreTechnique.h>

namespace fb::render
{

    FB_CLASS_REGISTER_DERIVED( fb, CMaterialTechniqueOgreNext, IMaterialTechnique );

    CMaterialTechniqueOgreNext::CMaterialTechniqueOgreNext() = default;

    CMaterialTechniqueOgreNext::~CMaterialTechniqueOgreNext()
    {
        unload( nullptr );
    }

    void CMaterialTechniqueOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto passes = getPasses();

            if( passes.empty() )
            {
                auto pass = createPass();
                FB_ASSERT( pass );
            }

            passes = getPasses();
            for( auto pass : passes )
            {
                pass->load( nullptr );
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialTechniqueOgreNext::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto passes = getPasses();
            for( auto pass : passes )
            {
                pass->reload( data );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialTechniqueOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto passes = getPasses();
            for( auto pass : passes )
            {
                pass->unload( nullptr );
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CMaterialTechniqueOgreNext::initialise( Ogre::Technique *technique )
    {
        m_technique = technique;

        // auto passes = m_technique->getPasses();
        // for( auto pass : passes )
        //{
        //     auto pPass = fb::make_ptr<CMaterialPass>();
        //     pPass->initialise( pass );
        //     m_passes.push_back( pPass );
        // }
    }

    auto CMaterialTechniqueOgreNext::getScheme() const -> hash32
    {
        return 0;
    }

    void CMaterialTechniqueOgreNext::setScheme( hash32 val )
    {
    }

    auto CMaterialTechniqueOgreNext::createPass() -> SmartPtr<IMaterialPass>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto pass = factoryManager->make_ptr<CMaterialPassOgreNext>();
            FB_ASSERT( pass );

            auto material = getMaterial();
            pass->setMaterial( material );

            pass->setParent( this );
            addPass( pass );

            FB_ASSERT( getPasses().size() == 1 );
            return pass;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CMaterialTechniqueOgreNext::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
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

    auto CMaterialTechniqueOgreNext::getTechnique() const -> Ogre::Technique *
    {
        return m_technique;
    }

    void CMaterialTechniqueOgreNext::setTechnique( Ogre::Technique *technique )
    {
        if( m_technique != technique )
        {
            m_technique = technique;

            //auto passes = technique->getPasses();
            //if( passes.size() != m_passes.size() )
            //{
            //    m_passes.clear();

            //    for( auto pass : passes )
            //    {
            //        auto pPass = fb::make_ptr<CMaterialPass>();
            //        pPass->setParent( this );
            //        pPass->initialise( pass );
            //        m_passes.push_back( pPass );
            //    }
            //}
        }
    }

}  // namespace fb::render
