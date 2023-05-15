#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CWater.h>
#include <FBGraphicsOgre/Ogre/WaterMesh.h>
#include <FBCore/FBCore.h>

#define COMPLEXITY 32  // watch out - number of polys is 2*ACCURACY*ACCURACY !
#define PLANE_SIZE 1000.0f

namespace fb
{
    namespace render
    {
        class WaterFrameListener : public Ogre::FrameListener
        {
        public:
            WaterFrameListener( CWater *water ) : m_water( water ), m_time( 0.0f )
            {
            }

            ~WaterFrameListener() override
            {
            }

            bool frameStarted( const Ogre::FrameEvent &evt ) override
            {
                return true;
            }

            bool frameRenderingQueued( const Ogre::FrameEvent &evt ) override
            {
                // m_time += evt.timeSinceLastFrame;

                WaterMesh *waterMesh = m_water->getWaterMesh();
                waterMesh->updateMesh( evt.timeSinceLastFrame );

                // Ogre::Root* root = Ogre::Root::getSingletonPtr();
                // Ogre::SceneManager* smgr = root->getSceneManager("GameSceneManager");
                // Ogre::Camera* camera = smgr->getCamera("DefaultCamera");
                // Ogre::Vector3 position = camera->getParentNode()->getPosition();

                // Vector3F waterPos = m_water->getPosition();
                // if(waterPos.Y() > position.y - 5.0)
                //{
                //	f32 tx = ((position.x - waterPos.X()) / PLANE_SIZE) * ((float)(COMPLEXITY-2) + 1);
                //	f32 ty = ((position.z - waterPos.Z()) / PLANE_SIZE) * ((float)(COMPLEXITY-2) + 1);
                //	waterMesh->push(tx, ty, -0.1, false);
                // }

                return true;
            }

            bool frameEnded( const Ogre::FrameEvent &evt ) override
            {
                return true;
            }

        protected:
            CWater *m_water;
            f32 m_time;
        };

        u32 CWater::m_nameExt = 0;

        CWater::CWater()
        {
        }

        CWater::~CWater()
        {
            Ogre::Root *root = Ogre::Root::getSingletonPtr();
            root->removeFrameListener( m_frameListener );
            FB_SAFE_DELETE( m_frameListener );
        }

        void CWater::initialise( SmartPtr<scene::IDirector> objectTemplate )
        {
            std::string meshName =
                std::string( "WaterStandard" ) + Ogre::StringConverter::toString( m_nameExt++ );
            m_waterMesh = new WaterMesh( meshName, PLANE_SIZE, COMPLEXITY );

            m_mesh = m_sceneMgr->addMesh( meshName.c_str() );
            m_sceneNode = m_sceneMgr->getRootSceneNode()->addChildSceneNode();
            m_sceneNode->attachObject( m_mesh );

            Ogre::MaterialPtr material;  // =
            // Ogre::MaterialManager::getSingletonPtr()->load("Water",
            // Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).staticCast<Ogre::Material>();

            m_mesh->setMaterialName( "Water" );

            Ogre::Root *root = Ogre::Root::getSingletonPtr();
            m_frameListener = new WaterFrameListener( this );
            root->addFrameListener( m_frameListener );
        }

        void CWater::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();

            switch( task )
            {
            case Thread::Task::Render:
                // m_waterMesh->updateMesh(dt);
                break;
            default:
            {
            }
            }
        }

        SmartPtr<IGraphicsScene> CWater::getSceneManager() const
        {
            return m_sceneMgr;
        }

        void CWater::setSceneManager( SmartPtr<IGraphicsScene> sceneMgr )
        {
            m_sceneMgr = sceneMgr;
        }

        SmartPtr<ICamera> CWater::getCamera() const
        {
            return m_camera;
        }

        void CWater::setCamera( SmartPtr<ICamera> camera )
        {
            m_camera = camera;
        }

        SmartPtr<IViewport> CWater::getViewport() const
        {
            return m_viewport;
        }

        void CWater::setViewport( SmartPtr<IViewport> viewport )
        {
            m_viewport = viewport;
        }

        WaterMesh *CWater::getWaterMesh() const
        {
            return m_waterMesh;
        }

        void CWater::setWaterMesh( WaterMesh *val )
        {
            m_waterMesh = val;
        }

        Vector3F CWater::getPosition() const
        {
            return m_sceneNode->getPosition();
        }

        void CWater::setPosition( const Vector3F &position )
        {
            m_sceneNode->setPosition( position );
        }
    }  // end namespace render
}  // end namespace fb
