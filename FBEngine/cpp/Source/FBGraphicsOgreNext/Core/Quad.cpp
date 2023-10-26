#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Core/Quad.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <OgreRectangle2D2.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {

        Quad::Quad()
        {
        }

        Quad::~Quad()
        {
        }

        void Quad::load( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Loading );

            using namespace Ogre;

            auto smgr = getSceneManager();

            Ogre::SceneManager *sceneManager = nullptr;
            smgr->_getObject( (void **)&sceneManager );

            auto sky = OGRE_NEW Rectangle2D( Id::generateNewId<MovableObject>(),
                                             &sceneManager->_getEntityMemoryManager( SCENE_STATIC ),
                                             sceneManager );
            // We can't use BT_DYNAMIC_* because the scene may be rendered from multiple cameras
            // in the same frame, and dynamic supports only one set of values per frame
            sky->initialize( BT_DEFAULT,
                             Rectangle2D::GeometryFlagQuad | Rectangle2D::GeometryFlagNormals );
            sky->setGeometry( -Ogre::Vector2::UNIT_SCALE, Ogre::Vector2( 2.0f ) );
            sky->setRenderQueueGroup( 212u );  // Render after most stuff
            sceneManager->getRootSceneNode( SCENE_STATIC )->attachObject( sky );

            setLoadingState( LoadingState::Loaded );
        }

        void Quad::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Ogre::Rectangle2D *Quad::getRectangle() const
        {
            return m_rectangle;
        }

        void Quad::setRectangle( Ogre::Rectangle2D *rectangle )
        {
            m_rectangle = rectangle;
        }

        Ogre::SceneNode *Quad::getSceneNode() const
        {
            return m_sceneNode;
        }

        void Quad::setSceneNode( Ogre::SceneNode *sceneNode )
        {
            m_sceneNode = sceneNode;
        }

        SmartPtr<fb::render::IGraphicsScene> Quad::getSceneManager() const
        {
            return m_sceneManager;
        }

        void Quad::setSceneManager( SmartPtr<IGraphicsScene> sceneManager )
        {
            m_sceneManager = sceneManager;
        }

    }  // namespace render
}  // namespace fb
