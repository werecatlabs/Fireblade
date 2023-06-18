#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Cubemap.h>
#include <FBCore/Interface/Graphics/ICubemap.h>
#include <FBCore/Interface/System/ITimer.h>

namespace fb
{
    namespace scene
    {

        Cubemap::Cubemap() : m_cameraDistance( 0.0f ), m_distanceTheshold( 0.0f )
        {
        }

        Cubemap::~Cubemap()
        {
        }

        void Cubemap::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
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

        SmartPtr<render::ICubemap> Cubemap::getCubemapGenerator() const
        {
            return m_cubemapGenerator;
        }

        void Cubemap::setCubemapGenerator( SmartPtr<render::ICubemap> cubemapGenerator )
        {
            m_cubemapGenerator = cubemapGenerator;
        }

        f32 Cubemap::getCameraDistance() const
        {
            return m_cameraDistance;
        }

        void Cubemap::setCameraDistance( f32 cameraDistance )
        {
            m_cameraDistance = cameraDistance;
        }

        f32 Cubemap::getEnableDistanceTheshold() const
        {
            return m_distanceTheshold;
        }

        void Cubemap::setEnableDistanceTheshold( f32 distanceTheshold )
        {
            m_distanceTheshold = distanceTheshold;
        }
    }  // namespace scene
}  // end namespace fb
