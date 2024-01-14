#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CSkyboxOgreNext.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBGraphicsOgreNext/Core/Quad.h>
#include <FBGraphicsOgreNext/Core/SkyBoxRenderer.h>
#include <OgreRectangle2D2.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, CSkyboxOgreNext, ISkybox );

    CSkyboxOgreNext::CSkyboxOgreNext() = default;

    CSkyboxOgreNext::~CSkyboxOgreNext()
    {
        unload( nullptr );
    }

    void CSkyboxOgreNext::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loading );

        auto visible = isVisible();
        if( visible )
        {
            m_skyBoxRenderer = fb::make_ptr<SkyBoxRenderer>();
            m_skyBoxRenderer->load( data );
        }

        setLoadingState( LoadingState::Loaded );
    }

    void CSkyboxOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            if( m_skyBoxRenderer )
            {
                m_skyBoxRenderer->unload( data );
                m_skyBoxRenderer = nullptr;
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CSkyboxOgreNext::setMaterial( SmartPtr<IMaterial> material )
    {
        m_material = material;
    }

    auto CSkyboxOgreNext::getMaterial() const -> SmartPtr<IMaterial>
    {
        return m_material;
    }

    auto CSkyboxOgreNext::getSceneManager() const -> SmartPtr<IGraphicsScene>
    {
        return m_sceneManager;
    }

    void CSkyboxOgreNext::setSceneManager( SmartPtr<IGraphicsScene> sceneManager )
    {
        m_sceneManager = sceneManager;
    }

    auto CSkyboxOgreNext::isVisible() const -> bool
    {
        return m_isVisible;
    }

    void CSkyboxOgreNext::setVisible( bool visible )
    {
        m_isVisible = visible;
    }

    auto CSkyboxOgreNext::getDistance() const -> f32
    {
        return m_distance;
    }

    void CSkyboxOgreNext::setDistance( f32 distance )
    {
        m_distance = distance;
    }

}  // namespace fb::render
