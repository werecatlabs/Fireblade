#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CMaterialNode.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        template <class T>
        CMaterialNode<T>::CMaterialNode() : m_parent( nullptr )
        {
        }

        template <class T>
        CMaterialNode<T>::CMaterialNode( u32 numChildren )
        {
            m_children.resize( numChildren );
        }

        template <class T>
        CMaterialNode<T>::~CMaterialNode()
        {
            unload( nullptr );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto stateManager = applicationManager->getStateManager() )
            {
                if( auto stateContext = CMaterialNode<T>::getStateObject() )
                {
                    if( auto stateListener = CMaterialNode<T>::getStateListener() )
                    {
                        stateContext->removeStateListener( stateListener );
                    }

                    stateManager->removeStateObject( stateContext );

                    stateContext->unload( nullptr );
                    CMaterialNode<T>::setStateObject( nullptr );
                }

                if( auto stateListener = CMaterialNode<T>::getStateListener() )
                {
                    stateListener->unload( nullptr );
                    CMaterialNode<T>::setStateListener( nullptr );
                }
            }
        }

        template <class T>
        void CMaterialNode<T>::load( SmartPtr<ISharedObject> data )
        {
        }

        template <class T>
        void CMaterialNode<T>::reload( SmartPtr<ISharedObject> data )
        {
        }

        template <class T>
        void CMaterialNode<T>::unload( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto stateManager = applicationManager->getStateManager() )
            {
                if( auto stateContext = CMaterialNode<T>::getStateObject() )
                {
                    if( auto stateListener = CMaterialNode<T>::getStateListener() )
                    {
                        stateContext->removeStateListener( stateListener );
                    }
                }
            }

            if( auto stateListener = CMaterialNode<T>::getStateListener() )
            {
                stateListener->unload( nullptr );
                CMaterialNode<T>::setStateListener( nullptr );
            }

            m_children.clear();
            m_parent = nullptr;
        }

        template <class T>
        void CMaterialNode<T>::addChild( SmartPtr<IMaterialNode> child )
        {
            if( child )
            {
                m_children.push_back( child );
                child->setParent( this );
            }
        }

        template <class T>
        void CMaterialNode<T>::addChild( SmartPtr<IMaterialNode> child, int index )
        {
            if( child )
            {
                m_children[index] = child;
                child->setParent( this );
            }
        }

        template <class T>
        void CMaterialNode<T>::removeChild( SmartPtr<IMaterialNode> child )
        {
            auto it = std::find( m_children.begin(), m_children.end(), child );
            if( it != m_children.end() )
            {
                m_children.erase( it );
                child->setParent( nullptr );
            }
        }

        template <class T>
        void CMaterialNode<T>::remove()
        {
            // IMaterialNode* parent = getParent();
            // if(parent)
            //	parent->removeChild(this);
        }

        template <class T>
        u32 CMaterialNode<T>::getNumChildren() const
        {
            return (u32)m_children.size();
        }

        template <class T>
        SmartPtr<IMaterialNode> CMaterialNode<T>::getChildByIndex( u32 index ) const
        {
            return m_children[index];
        }

        template <class T>
        SmartPtr<IMaterialNode> CMaterialNode<T>::getChildById( hash32 id ) const
        {
            return nullptr;
        }

        template <class T>
        Array<SmartPtr<IMaterialNode>> CMaterialNode<T>::getChildren() const
        {
            return m_children;
        }

        template <class T>
        SmartPtr<IMaterialNode> CMaterialNode<T>::getParent() const
        {
            return m_parent;
        }

        template <class T>
        void CMaterialNode<T>::setParent( SmartPtr<IMaterialNode> parent )
        {
            m_parent = parent;
        }

        template <class T>
        SmartPtr<IMaterial> CMaterialNode<T>::getMaterial() const
        {
            return m_material;
        }

        template <class T>
        void CMaterialNode<T>::setMaterial( SmartPtr<IMaterial> material )
        {
            m_material = material;
        }

        template <class T>
        SmartPtr<Properties> CMaterialNode<T>::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();
            properties->setProperty( "loaded", this->isLoaded() );
            properties->setProperty( "references", this->getReferences() );
            properties->setProperty( "weakReferences", this->getWeakReferences() );
            return properties;
        }

        template <class T>
        void CMaterialNode<T>::setProperties( SmartPtr<Properties> properties )
        {
        }

        // explicit instantiation
        template class CMaterialNode<IMaterialNode>;
        template class CMaterialNode<IMaterialPass>;
        template class CMaterialNode<IMaterialTexture>;
        template class CMaterialNode<IMaterialTechnique>;
        // template class CMaterialNode<IMaterialAnimatedTexture>;
    }  // end namespace render
}  // end namespace fb
