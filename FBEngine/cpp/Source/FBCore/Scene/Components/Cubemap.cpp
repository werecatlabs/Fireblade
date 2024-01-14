#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Cubemap.h>
#include <FBCore/Interface/Graphics/ICubemap.h>
#include <FBCore/Interface/System/ITimer.h>

namespace fb::scene
{

    FB_CLASS_REGISTER_DERIVED( fb::scene, Cubemap, Component );

    Cubemap::Cubemap()
    {
    }

    Cubemap::~Cubemap() = default;

    void Cubemap::load( SmartPtr<ISharedObject> data )
    {
    }

    void Cubemap::reload( SmartPtr<ISharedObject> data )
    {
    }

    void Cubemap::unload( SmartPtr<ISharedObject> data )
    {
    }

    void Cubemap::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto timer = applicationManager->getTimer();
        FB_ASSERT( timer );

        auto task = Thread::getCurrentTask();
        switch( task )
        {
        case Thread::Task::Application:
        {
            if( m_cameraDistance < m_distanceTheshold )
            {
                //m_cubemapGenerator->setPosition( m_position );

                if( !m_cubemapGenerator->getEnable() )
                {
                    m_cubemapGenerator->setEnable( true );
                }
            }
            else
            {
                if( m_cubemapGenerator->getEnable() )
                {
                    m_cubemapGenerator->setEnable( false );
                }
            }
        }
        break;
        default:
        {
        }
        }
    }

    auto Cubemap::getCubemapGenerator() const -> SmartPtr<render::ICubemap>
    {
        return m_cubemapGenerator;
    }

    void Cubemap::setCubemapGenerator( SmartPtr<render::ICubemap> cubemapGenerator )
    {
        m_cubemapGenerator = cubemapGenerator;
    }

    auto Cubemap::getCameraDistance() const -> f32
    {
        return m_cameraDistance;
    }

    void Cubemap::setCameraDistance( f32 cameraDistance )
    {
        m_cameraDistance = cameraDistance;
    }

    auto Cubemap::getEnableDistanceTheshold() const -> f32
    {
        return m_distanceTheshold;
    }

    void Cubemap::setEnableDistanceTheshold( f32 distanceTheshold )
    {
        m_distanceTheshold = distanceTheshold;
    }
}  // namespace fb::scene
