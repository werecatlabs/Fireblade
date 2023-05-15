#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>

#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreManualObject.h>

#include "FBGraphicsOgre/SSEffectRenderer.h"

namespace fb
{
    namespace render
    {

        int SSEffectRenderer::m_nameExt = 0;

        Ogre::Viewport *SSEffectRenderer::viewport() const
        {
            return _viewport;
        }

        Ogre::Camera *SSEffectRenderer::camera() const
        {
            return _camera;
        }

        Ogre::RenderSystem *SSEffectRenderer::renderSys() const
        {
            return Ogre::Root::getSingleton().getRenderSystem();
        }

        Ogre::ManualObject *SSEffectRenderer::quad() const
        {
            return _quad;
        }

        SSEffectRenderer::SSEffectRenderer( Ogre::SceneManager *sceneMgr, Ogre::Viewport *viewport ) :
            _sceneMgr( sceneMgr ),
            _viewport( viewport ),
            _camera( viewport->getCamera() ),
            _quad( NULL )
        {
            ensureQuad();
        }

        SSEffectRenderer::~SSEffectRenderer()
        {
            destroyQuad();
        }

        void SSEffectRenderer::destroyQuad()
        {
            if( _quad )
                _sceneMgr->destroyManualObject( _quad );

            _quad = NULL;
        }

        void SSEffectRenderer::buildQuad( Ogre::Camera *cam )
        {
            // destroyQuad();
            _quad = _sceneMgr->createManualObject( "renderer::_quad" +
                                                   Ogre::StringConverter::toString( m_nameExt++ ) );

            //! (courtesy of Ogre API docs)
            //! The corners are ordered as follows:
            //! [0] = top-right near
            //! [1] = top-left near
            //! [2] = bottom-left near
            //! [3] = bottom-right near
            //! [4] = top-right far    <--- want
            //! [5] = top-left far     <--- want
            //! [6] = bottom-left far  <--- want
            //! [7] = bottom-right far <--- want
            const Ogre::Vector3 *CORNERS = cam->getWorldSpaceCorners();

            //! we want them in VIEW-SPACE!!! very important!
            Ogre::Matrix4 viewMat = cam->getViewMatrix( true );
            // Ogre::Matrix4 viewMat = Ogre::Matrix4::IDENTITY;

            _quad->clear();
            _quad->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST );

            float zPos = 0.f;

            {
                //! store frustum corner in normal attribute
                //! go anti-clockwise

                // top-left
                _quad->position( Ogre::Vector3( -1, 1, zPos ) );
                _quad->normal( viewMat * CORNERS[5] );
                _quad->textureCoord( Ogre::Vector2( 0, 0 ) );

                // bottom-left
                _quad->position( Ogre::Vector3( -1, -1, zPos ) );
                _quad->normal( viewMat * CORNERS[6] );
                _quad->textureCoord( Ogre::Vector2( 0, 1 ) );

                // bottom-right
                _quad->position( Ogre::Vector3( 1, -1, zPos ) );
                _quad->normal( viewMat * CORNERS[7] );
                _quad->textureCoord( Ogre::Vector2( 1, 1 ) );

                // top-right
                _quad->position( Ogre::Vector3( 1, 1, zPos ) );
                _quad->normal( viewMat * CORNERS[4] );
                _quad->textureCoord( Ogre::Vector2( 1, 0 ) );

                // we put the vertices in anti-clockwise,
                // so just start at 0 and go to 3
                _quad->quad( 0, 1, 2, 3 );
            }

            _quad->end();

            //_quad->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
        }

        void SSEffectRenderer::ensureQuad()
        {
            if( !_quad )
                buildQuad( _camera );
        }

        void SSEffectRenderer::fsquad( Ogre::Pass *p, Ogre::Viewport *vp ) const
        {
            if( !_quad )
                return;

            Ogre::RenderOperation rop;
            _quad->getSection( 0 )->getRenderOperation( rop );

#if 0
			Ogre::RenderSystem* renderSys = Ogre::Root::getSingleton().getRenderSystem();
			renderSys->_setViewport(vp); // render to said viewport

			_sceneMgr->_setPass(p, true, false); // parse the pass

			// bind/unbind GPU programs

			if (p->hasVertexProgram())
				renderSys->bindGpuProgramParameters(
					Ogre::GPT_VERTEX_PROGRAM, p->getVertexProgramParameters(), Ogre::GPV_ALL);
			else
				renderSys->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

			if (p->hasFragmentProgram())
				renderSys->bindGpuProgramParameters(
					Ogre::GPT_FRAGMENT_PROGRAM, p->getFragmentProgramParameters(), Ogre::GPV_ALL);
			else
				renderSys->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);

			// clear any matrices
			renderSys->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
			renderSys->_setViewMatrix(Ogre::Matrix4::IDENTITY);
			renderSys->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);

			renderSys->_beginFrame();
			{
				renderSys->_render(rop);
			}
			renderSys->_endFrame();
#else
            Ogre::Matrix4 worldMatrix = Ogre::Matrix4::IDENTITY;
            worldMatrix.setTrans( Ogre::Vector3::NEGATIVE_UNIT_Z * 0.0 );
            // vp->getCamera()->setFarClipDistance(1000.0f);
            //_sceneMgr->manualRender(&rop, p, vp, worldMatrix, vp->getCamera()->getViewMatrix(),
            //vp->getCamera()->getProjectionMatrix(), true ); _sceneMgr->manualRender(&rop, p, vp,
            //worldMatrix, Ogre::Matrix4::IDENTITY, Ogre::Matrix4::IDENTITY, true );

            //_sceneMgr->manualRender(&rop, p, vp, Ogre::Matrix4::IDENTITY, Ogre::Matrix4::IDENTITY,
            //Ogre::Matrix4::IDENTITY, true );
#endif
        }

        Ogre::SceneManager *SSEffectRenderer::sceneMgr() const
        {
            return _sceneMgr;
        }

    }  // namespace render
}  // namespace fb
