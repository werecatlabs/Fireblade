#ifndef CResourceGraphics_h__
#define CResourceGraphics_h__

#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Resource/Resource.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace render
    {

        template <class T>
        class ResourceGraphics : public Resource<T>
        {
        public:
            ResourceGraphics()
            {
            }

            ~ResourceGraphics() override
            {
            }

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data )
            {
                Resource<T>::unload( data );
            }

            void saveToFile( const String &filePath )
            {
            }

            void loadFromFile( const String &filePath )
            {
            }

            /** @copydoc IResource::getProperties */
            virtual SmartPtr<Properties> getProperties() const
            {
                auto properties = Resource<T>::getProperties();

                const auto handle = this->getHandle();
                properties->setProperty( "name", handle->getName() );
                return properties;
            }

            /** @copydoc IResource::setProperties */
            virtual void setProperties( SmartPtr<Properties> properties )
            {
                auto name = String();
                properties->getProperty( "name", name );

                auto handle = this->getHandle();
                handle->setName( name );
            }

            void _getObject( void **ppObject ) const
            {
                *ppObject = nullptr;
            }

            virtual void createStateObject()
            {
            }

            virtual void destroyStateObject()
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( auto stateManager = applicationManager->getStateManager() )
                {
                    if( auto stateContext = Resource<T>::getStateContext() )
                    {
                        if( auto stateListener = Resource<T>::getStateListener() )
                        {
                            stateContext->removeStateListener( stateListener );
                        }

                        if( stateManager )
                        {
                            stateManager->removeStateObject( stateContext );
                        }

                        stateContext->unload( nullptr );
                    }

                    if( auto stateListener = Resource<T>::getStateListener() )
                    {
                        stateListener->unload( nullptr );
                    }
                }

                Resource<T>::setStateListener( nullptr );
                Resource<T>::setStateContext( nullptr );
            }

            bool isThreadSafe() const
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto renderTask = graphicsSystem->getRenderTask();

                auto task = Thread::getCurrentTask();

                const auto &loadingState = T::getLoadingState();

                return loadingState == LoadingState::Loaded && task == renderTask;
            }

            void addMessage( SmartPtr<IStateMessage> message )
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                if( auto stateContext = Resource<T>::getStateContext() )
                {
                    const auto stateTask = graphicsSystem->getStateTask();
                    stateContext->addMessage( stateTask, message );
                }
            }

            FB_CLASS_REGISTER_TEMPLATE_DECL( ResourceGraphics, T );
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, ResourceGraphics, T, Resource<T> );

    }  // end namespace render
}  // end namespace fb

#endif  // CResourceOgre_h__
