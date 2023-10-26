#ifndef Quad_h__
#define Quad_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace render
    {
        //--------------------------------------------
        class Quad : public ISharedObject
        {
        public:
            Quad();
            ~Quad();

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

	        Ogre::Rectangle2D *getRectangle() const;
            void setRectangle( Ogre::Rectangle2D *rectangle );

	        Ogre::SceneNode *getSceneNode() const;

            void setSceneNode( Ogre::SceneNode *sceneNode );

	        SmartPtr<IGraphicsScene> getSceneManager() const;

            void setSceneManager( SmartPtr<IGraphicsScene> sceneManager );

        protected:
            SmartPtr<IGraphicsScene> m_sceneManager;
            Ogre::SceneNode *m_sceneNode = nullptr;
            Ogre::Rectangle2D *m_rectangle = nullptr;
        };

    }  // namespace render
}  // namespace fb

#endif  // Quad_h__
