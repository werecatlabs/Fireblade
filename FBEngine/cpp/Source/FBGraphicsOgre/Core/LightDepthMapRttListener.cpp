#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Core/LightDepthMapRttListener.h>
#include <Ogre.h>

namespace fb
{
    namespace render
    {

 LightDepthMapRttListener::LightDepthMapRttListener( Ogre::SceneManager *sm ) : mSceneMgr( sm )
        {
        }

        void LightDepthMapRttListener::preRenderTargetUpdate( const Ogre::RenderTargetEvent &evt )
        {
            Ogre::SceneManager::MovableObjectIterator EntityIterator =
                mSceneMgr->getMovableObjectIterator( "Entity" );
            Ogre::Entity *CurrentEntity = NULL;
            unsigned int k = 0;

            mMaterials.empty();

            mBillboardSet->setVisible( false );

            while( EntityIterator.hasMoreElements() )
            {
                CurrentEntity = static_cast<Ogre::Entity *>( EntityIterator.peekNextValue() );
                for( k = 0; k < CurrentEntity->getNumSubEntities(); k++ )
                {
                    mMaterials.push( CurrentEntity->getSubEntity( k )->getMaterialName() );
                    CurrentEntity->getSubEntity( k )->setMaterialName( "LightShaftsDepth" );
                    break;
                }

                EntityIterator.moveNext();
            }
        }

        void LightDepthMapRttListener::postRenderTargetUpdate( const Ogre::RenderTargetEvent &evt )
        {
            Ogre::SceneManager::MovableObjectIterator EntityIterator =
                mSceneMgr->getMovableObjectIterator( "Entity" );
            Ogre::Entity *CurrentEntity = NULL;
            unsigned int k = 0;

            mBillboardSet->setVisible( true );

            while( EntityIterator.hasMoreElements() )
            {
                CurrentEntity = static_cast<Ogre::Entity *>( EntityIterator.peekNextValue() );
                for( k = 0; k < CurrentEntity->getNumSubEntities(); k++ )
                {
                    CurrentEntity->getSubEntity( k )->setMaterialName( mMaterials.front() );
                    mMaterials.pop();
                    break;
                }

                EntityIterator.moveNext();
            }
        }

    }  // namespace render
}

