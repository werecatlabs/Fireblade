#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Core/CompositorManager.h>
#include "FBGraphicsOgre/Ogre/HDRListener.h"
#include "FBGraphicsOgre/Ogre/DepthOfFieldEffect.h"
#include <FBCore/FBCore.h>
#include <OgreViewport.h>

namespace fb
{
    namespace render
    {

        CompositorManager::CompositorManager() :
            hdrListener( nullptr )
        {
            // createEffects();
            // createDOFCompositor();
        }


        CompositorManager::~CompositorManager()
        {
            FB_SAFE_DELETE( hdrListener );

            auto it = m_dofEffects.begin();
            for(; it != m_dofEffects.end(); ++it)
            {
                FB_SAFE_DELETE( it->second );
            }
        }


        void CompositorManager::registerCompositors( SmartPtr<IViewport> viewport )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::Viewport *vp;
            viewport->_getObject( (void **)&vp );
            FB_ASSERT_TRUE( !vp );
            if(!vp)
            {
                FB_EXCEPTION( "Viewport not found." );
            }

            // hvListener = new HeatVisionListener();
            // hdrListener = new HDRListener();
            ////gaussianListener = new GaussianListener();

            // mCompositorSelectorViewManager = new ItemSelectorViewManager("CompositorSelectorWin");
            //  tell view manager to notify us when an item changes selection state
            // mCompositorSelectorViewManager->setItemSelectorController(this);
            // iterate through Compositor Managers resources and add name keys ast Item selectors to
            // Compositor selector view manager
            Ogre::CompositorManager::ResourceMapIterator resourceIterator =
                Ogre::CompositorManager::getSingleton().getResourceIterator();

            // add all compositor resources to the view container
            while(resourceIterator.hasMoreElements())
            {
                Ogre::ResourcePtr resource = resourceIterator.getNext();
                const Ogre::String &compositorName = resource->getName();
                // Don't add base Ogre/Scene compositor to view
                if(compositorName == "Ogre/Scene")
                    continue;

                // mCompositorSelectorViewManager->addItemSelector(compositorName);
                int addPosition = -1;
                if(compositorName.find( "HDR" ) != -1)
                {
                    // HDR must be first in the chain
                    addPosition = 0;

                    if(!hdrListener)
                        hdrListener = new HDRListener();
                }

                Ogre::CompositorInstance *instance =
                    Ogre::CompositorManager::getSingleton().addCompositor( vp, compositorName,
                        addPosition );
                Ogre::CompositorManager::getSingleton().setCompositorEnabled( vp, compositorName,
                    false );

                // special handling for Heat Vision which uses a listener
                if(instance && ( compositorName == "Heat Vision" ))
                {
                    // instance->addListener(hvListener);
                }
                else if(instance && ( compositorName == "HDR" ))
                {
                    instance->addListener( hdrListener );
                    hdrListener->notifyViewportSize( vp->getActualWidth(), vp->getActualHeight() );
                    hdrListener->notifyCompositor( instance );
                }
                else if(instance && ( compositorName == "Gaussian Blur" ))
                {
                    // instance->addListener(gaussianListener);
                    // gaussianListener->notifyViewportSize(vp->getActualWidth(), vp->getActualHeight());
                }
            }
        }


        void CompositorManager::registerCompositor( SmartPtr<IViewport> viewport,
                                                    const String &compositorName )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::Viewport *vp;
            viewport->_getObject( (void **)&vp );
            FB_ASSERT_TRUE( !vp );
            if(!vp)
            {
                FB_EXCEPTION( "Viewport not found." );
            }

            Ogre::CompositorManager *compositorManager = Ogre::CompositorManager::getSingletonPtr();

            int addPosition = -1;
            if(compositorName.find( "HDR" ) != -1)
            {
                // HDR must be first in the chain
                addPosition = 0;

                if(!hdrListener)
                    hdrListener = new HDRListener();
            }

            Ogre::CompositorInstance *instance =
                compositorManager->addCompositor( vp, compositorName.c_str(), addPosition );

            if(!instance)
                return;

            // special handling for Heat Vision which uses a listener
            if(compositorName == "Heat Vision")
            {
                // instance->addListener(hvListener);
            }
            else if(compositorName.find( "HDR" ) != -1)
            {
                instance->addListener( hdrListener );
                hdrListener->notifyViewportSize( vp->getActualWidth(), vp->getActualHeight() );
                hdrListener->notifyCompositor( instance );
            }
            else if(compositorName == "DoF_Compositor")
            {
                auto dofEffect = new DepthOfFieldEffect( vp );
                instance->addListener( dofEffect );
                m_dofEffects[vp] = dofEffect;
            }
            else if(compositorName == "Gaussian Blur")
            {
                // instance->addListener(gaussianListener);
                // gaussianListener->notifyViewportSize(vp->getActualWidth(), vp->getActualHeight());
            }
        }


        void CompositorManager::removeCompositor( SmartPtr<IViewport> viewport,
                                                  const String &compositorName )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::Viewport *vp;
            viewport->_getObject( (void **)&vp );
            FB_ASSERT_TRUE( !vp );
            if(!vp)
            {
                FB_EXCEPTION( "Viewport not found." );
            }

            Ogre::CompositorManager *compositorMgr = Ogre::CompositorManager::getSingletonPtr();
            compositorMgr->removeCompositor( vp, compositorName.c_str() );
        }


        void CompositorManager::setCompositorEnabled( const String &name, SmartPtr<IViewport> viewport,
                                                      bool enabled )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::Viewport *vp;
            viewport->_getObject( (void **)&vp );
            FB_ASSERT_TRUE( !vp );

            Ogre::CompositorManager *compositorManager = Ogre::CompositorManager::getSingletonPtr();
            compositorManager->setCompositorEnabled( vp, name.c_str(), enabled );
        }


        bool CompositorManager::isCompositorEnabled( const String &name,
                                                     SmartPtr<IViewport> viewport ) const
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::Viewport *vp;
            viewport->_getObject( (void **)&vp );
            FB_ASSERT_TRUE( !vp );

            Ogre::CompositorManager *compositorManager = Ogre::CompositorManager::getSingletonPtr();
            Ogre::CompositorChain *compositorChain = compositorManager->getCompositorChain( vp );
            Ogre::CompositorInstance *compositorInstance =
                compositorChain->getCompositor( name.c_str() );
            return compositorInstance->getEnabled();
        }


        void CompositorManager::setCompositorProperties( const String &name,
                                                         SmartPtr<IViewport> viewport,
                                                         const Properties &properties )
        {
            Ogre::Viewport *vp;
            viewport->_getObject( (void **)&vp );
            FB_ASSERT_TRUE( !vp );

            if(name == ( "DoF_Compositor" ))
            {
                auto it = m_dofEffects.find( vp );
                if(it != m_dofEffects.end())
                {
                    DepthOfFieldEffect *dofEffect = it->second;

                    f32 nearDepth = 0;
                    f32 focalDepth = 0;
                    f32 farDepth = 0;

                    if(!properties.getPropertyValue( "nearDepth", nearDepth ))
                        FB_LOG_MESSAGE( "CompositorManager",
                                    "CCompositorManager::setCompositorProperties - nearDepth "
                                    "property not found" );

                    if(!properties.getPropertyValue( "focalDepth", focalDepth ))
                        FB_LOG_MESSAGE( "CompositorManager",
                                    "CCompositorManager::setCompositorProperties - focalDepth "
                                    "property not found" );

                    if(!properties.getPropertyValue( "farDepth", farDepth ))
                        FB_LOG_MESSAGE( "CompositorManager",
                                    "CCompositorManager::setCompositorProperties - farDepth "
                                    "property not found" );

                    dofEffect->setFocalDepths( nearDepth, focalDepth, farDepth );
                }
            }
        }


        void CompositorManager::getCompositorProperties( const String &name,
                                                         SmartPtr<IViewport> viewport,
                                                         Properties &properties ) const
        {
        }

        SmartPtr<IGraphicsScene> CompositorManager::getSceneManager() const
        {
            return nullptr;
        }

        void CompositorManager::setSceneManager( SmartPtr<IGraphicsScene> val )
        {
        }

        SmartPtr<IWindow> CompositorManager::getWindow() const
        {
            return nullptr;
        }

        void CompositorManager::setWindow( SmartPtr<IWindow> val )
        {
        }

        SmartPtr<ICamera> CompositorManager::getCamera() const
        {
            return nullptr;
        }

        void CompositorManager::setCamera( SmartPtr<ICamera> val )
        {
        }

        String CompositorManager::getWorkspaceName() const
        {
            return "";
        }

        void CompositorManager::setWorkspaceName( const String &val )
        {
        }

        bool CompositorManager::isEnabled() const
        {
            return false;
        }

        void CompositorManager::setEnabled( bool val )
        {
        }


        void CompositorManager::createEffects()
        {
            /// Motion blur effect
            Ogre::CompositorPtr comp3 = Ogre::CompositorManager::getSingleton().create(
                "Motion Blur", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
            {
                Ogre::CompositionTechnique *t = comp3->createTechnique();
                {
                    Ogre::CompositionTechnique::TextureDefinition *def =
                        t->createTextureDefinition( "scene" );
                    def->width = 0;
                    def->height = 0;
                    def->formatList.push_back( Ogre::PF_R8G8B8 );
                }
                {
                    Ogre::CompositionTechnique::TextureDefinition *def =
                        t->createTextureDefinition( "sum" );
                    def->width = 0;
                    def->height = 0;
                    def->formatList.push_back( Ogre::PF_R8G8B8 );
                }
                {
                    Ogre::CompositionTechnique::TextureDefinition *def =
                        t->createTextureDefinition( "temp" );
                    def->width = 0;
                    def->height = 0;
                    def->formatList.push_back( Ogre::PF_R8G8B8 );
                }
                /// Render scene
                {
                    Ogre::CompositionTargetPass *tp = t->createTargetPass();
                    tp->setInputMode( Ogre::CompositionTargetPass::IM_PREVIOUS );
                    tp->setOutputName( "scene" );
                }
                /// Initialisation pass for sum texture
                {
                    Ogre::CompositionTargetPass *tp = t->createTargetPass();
                    tp->setInputMode( Ogre::CompositionTargetPass::IM_PREVIOUS );
                    tp->setOutputName( "sum" );
                    tp->setOnlyInitial( true );
                }
                /// Do the motion blur
                {
                    Ogre::CompositionTargetPass *tp = t->createTargetPass();
                    tp->setInputMode( Ogre::CompositionTargetPass::IM_NONE );
                    tp->setOutputName( "temp" );
                    {
                        Ogre::CompositionPass *pass = tp->createPass();
                        pass->setType( Ogre::CompositionPass::PT_RENDERQUAD );
                        pass->setMaterialName( "Ogre/Compositor/Combine" );
                        pass->setInput( 0, "scene" );
                        pass->setInput( 1, "sum" );
                        pass->setIdentifier( 666 );
                    }
                }
                /// Copy back sum texture
                {
                    Ogre::CompositionTargetPass *tp = t->createTargetPass();
                    tp->setInputMode( Ogre::CompositionTargetPass::IM_NONE );
                    tp->setOutputName( "sum" );
                    {
                        Ogre::CompositionPass *pass = tp->createPass();
                        pass->setType( Ogre::CompositionPass::PT_RENDERQUAD );
                        pass->setMaterialName( "Ogre/Compositor/Copyback" );
                        pass->setInput( 0, "temp" );
                    }
                }
                /// Display result
                {
                    Ogre::CompositionTargetPass *tp = t->getOutputTargetPass();
                    tp->setInputMode( Ogre::CompositionTargetPass::IM_NONE );
                    {
                        Ogre::CompositionPass *pass = tp->createPass();
                        pass->setType( Ogre::CompositionPass::PT_RENDERQUAD );
                        pass->setMaterialName( "Ogre/Compositor/MotionBlur" );
                        pass->setInput( 0, "sum" );
                    }
                }
            }
        }
    } // end namespace render
}     // end namespace fb
