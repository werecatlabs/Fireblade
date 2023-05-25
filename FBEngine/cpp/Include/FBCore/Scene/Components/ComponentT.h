#ifndef Component_h__
#define Component_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/Scene/IComponentEvent.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/FSM/IFSMListener.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Resource/CResource.h>

namespace fb
{
    namespace scene
    {

        /** Base class for a component object. */
        template <class T>
        class ComponentT : public CResource<T>
        {
        public:
            SmartPtr<Properties> getProperties() const override;

            SmartPtr<IStateContext> getStateObject() const override;

            void updateDirty( u32 flags, u32 oldFlags ) override
            {
            }

            void makeDirty() override
            {
            }

            void makeDirty( Thread::Task task ) override
            {
            }

            SmartPtr<scene::IActor> getActor() const override;

            void setActor( SmartPtr<scene::IActor> actor ) override
            {
            }

            u32 getEntity() const
            {
                return 0;
            }

            void setEntity( u32 entity )
            {
            }

            void setEnabled( bool enabled )
            {
            }

            /** @copydoc IComponent::isEnabled */
            bool isEnabled() const
            {
                return true;
            }

            void setupUpdateRegistration() override
            {
            }

            void updateTransform() override
            {
            }

            void updateVisibility() override
            {
            }

            void setState( IComponent::State state ) override
            {
            }

            IComponent::State getState() const override;

            void actorReset() override
            {
            }

            void actorUnload() override
            {
            }

            void reset() override
            {
            }

            void awake() override
            {
            }

            void destroy() override
            {
            }

            void levelWasLoaded( s32 level ) override
            {
            }

            void parentChanged( SmartPtr<scene::IActor> newParent,
                                SmartPtr<scene::IActor> oldParent ) override
            {
            }

            void hierarchyChanged() override
            {
            }

            void childAdded( SmartPtr<scene::IActor> child ) override
            {
            }

            void childRemoved( SmartPtr<scene::IActor> child ) override
            {
            }

            /** @copydoc IComponent::childAddedInHierarchy */
            virtual void childAddedInHierarchy( SmartPtr<IActor> child )
            {
            }

            /** @copydoc IComponent::childRemovedInHierarchy */
            virtual void childRemovedInHierarchy( SmartPtr<IActor> child )
            {
            }

            /** @copydoc IComponent::getEvents */
            Array<SmartPtr<IComponentEvent>> getEvents() const
            {
                return Array<SmartPtr<IComponentEvent>>();
            }

            /** @copydoc IComponent::setEvents */
            void setEvents( Array<SmartPtr<IComponentEvent>> events )
            {
            }

            /** @copydoc IComponent::addEvent */
            void addEvent( SmartPtr<IComponentEvent> event )
            {
            }

            /** @copydoc IComponent::removeEvent */
            void removeEvent( SmartPtr<IComponentEvent> event )
            {
            }

            /** @copydoc BaseComponent::addChild */
            virtual void addSubComponent( SmartPtr<ISubComponent> child )
            {
                auto p = getSubComponentsPtr();
                if( !p )
                {
                    p = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>();
                    setSubComponentsPtr( p );
                }

                if( p )
                {
                    auto &children = *p;
                    children.push_back( child );
                }
            }

            /** @copydoc BaseComponent::removeChild */
            virtual void removeSubComponent( SmartPtr<ISubComponent> child )
            {
                if( auto p = getSubComponentsPtr() )
                {
                    auto children = Array<SmartPtr<ISubComponent>>( p->begin(), p->end() );
                    auto it = std::find( children.begin(), children.end(), child );
                    if( it != children.end() )
                    {
                        children.erase( it );

                        auto newPtr = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>(
                            children.begin(), children.end() );
                        setSubComponentsPtr( newPtr );
                    }
                }
            }

            void removeSubComponentByIndex( u32 index ) override
            {
                if( auto p = getSubComponentsPtr() )
                {
                    auto children = Array<SmartPtr<ISubComponent>>( p->begin(), p->end() );
                    auto it = children.begin();
                    std::advance( it, index );

                    if( it != children.end() )
                    {
                        children.erase( it );

                        auto newPtr = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>(
                            children.begin(), children.end() );
                        setSubComponentsPtr( newPtr );
                    }
                }
            }

            u32 getNumSubComponents() const override
            {
                if( auto p = getSubComponentsPtr() )
                {
                    return (u32)p->size();
                }

                return 0;
            }

            /** @copydoc BaseComponent::getChildren */
            virtual Array<SmartPtr<ISubComponent>> getSubComponents() const
            {
                if( auto p = getSubComponentsPtr() )
                {
                    return Array<SmartPtr<ISubComponent>>( p->begin(), p->end() );
                }

                return Array<SmartPtr<ISubComponent>>();
            }

            /** @copydoc BaseComponent::setChildren */
            virtual void setSubComponents( Array<SmartPtr<ISubComponent>> children )
            {
                auto p = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>( children.begin(),
                                                                                    children.end() );
                setSubComponentsPtr( p );
            }

            /** @copydoc BaseComponent::getChildrenPtr */
            virtual SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> getSubComponentsPtr() const
            {
                return m_children;
            }

            /** @copydoc BaseComponent::setChildrenPtr */
            virtual void setSubComponentsPtr(
                SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> children )
            {
                m_children = children;
            }

            SmartPtr<IDirector> getDirector() const
            {
                return nullptr;
            }

            void setDirector( SmartPtr<IDirector> director )
            {
                
            }

        protected:
            SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> m_children;
        };

        template <class T>
        SmartPtr<Properties> ComponentT<T>::getProperties() const
        {
            return nullptr;
        }

        template <class T>
        SmartPtr<IStateContext> ComponentT<T>::getStateObject() const
        {
            return nullptr;
        }

        template <class T>
        SmartPtr<scene::IActor> ComponentT<T>::getActor() const
        {
            return nullptr;
        }

        template <class T>
        IComponent::State ComponentT<T>::getState() const
        {
            return IComponent::State::Count;
        }
    }  // namespace scene
}  // namespace fb

#include <FBCore/Scene/Components/ComponentT.inl>

#endif  // Component_h__
