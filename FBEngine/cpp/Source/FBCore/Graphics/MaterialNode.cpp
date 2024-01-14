#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/MaterialNode.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IMaterialPass.h>
#include <FBCore/Interface/Graphics/IMaterialTexture.h>
#include <FBCore/Interface/Graphics/IMaterialTechnique.h>
#include <FBCore/Core/Properties.h>

namespace fb::render
{
    template <class T>
    MaterialNode<T>::MaterialNode() : m_parent( nullptr )
    {
    }

    template <class T>
    MaterialNode<T>::MaterialNode( u32 numChildren )
    {
        m_children.resize( numChildren );
    }

    template <class T>
    MaterialNode<T>::~MaterialNode()
    {
        unload( nullptr );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( auto stateManager = applicationManager->getStateManager() )
        {
            if( auto stateContext = MaterialNode<T>::getStateContext() )
            {
                if( auto stateListener = MaterialNode<T>::getStateListener() )
                {
                    stateContext->removeStateListener( stateListener );
                }

                stateManager->removeStateObject( stateContext );

                stateContext->unload( nullptr );
                MaterialNode<T>::setStateContext( nullptr );
            }

            if( auto stateListener = MaterialNode<T>::getStateListener() )
            {
                stateListener->unload( nullptr );
                MaterialNode<T>::setStateListener( nullptr );
            }
        }
    }

    template <class T>
    void MaterialNode<T>::load( SmartPtr<ISharedObject> data )
    {
    }

    template <class T>
    void MaterialNode<T>::reload( SmartPtr<ISharedObject> data )
    {
    }

    template <class T>
    void MaterialNode<T>::unload( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( auto stateManager = applicationManager->getStateManager() )
        {
            if( auto stateContext = MaterialNode<T>::getStateContext() )
            {
                if( auto stateListener = MaterialNode<T>::getStateListener() )
                {
                    stateContext->removeStateListener( stateListener );
                }
            }
        }

        if( auto stateListener = MaterialNode<T>::getStateListener() )
        {
            stateListener->unload( nullptr );
            MaterialNode<T>::setStateListener( nullptr );
        }

        m_children.clear();
        m_parent = nullptr;
    }

    template <class T>
    void MaterialNode<T>::addChild( SmartPtr<IMaterialNode> child )
    {
        if( child )
        {
            m_children.push_back( child );
            child->setParent( this );
        }
    }

    template <class T>
    void MaterialNode<T>::addChild( SmartPtr<IMaterialNode> child, int index )
    {
        if( child )
        {
            m_children[index] = child;
            child->setParent( this );
        }
    }

    template <class T>
    void MaterialNode<T>::removeChild( SmartPtr<IMaterialNode> child )
    {
        auto it = std::find( m_children.begin(), m_children.end(), child );
        if( it != m_children.end() )
        {
            m_children.erase( it );
            child->setParent( nullptr );
        }
    }

    template <class T>
    void MaterialNode<T>::remove()
    {
        auto parent = getParent();
        if( parent )
        {
            parent->removeChild( this );
        }
    }

    template <class T>
    auto MaterialNode<T>::getNumChildren() const -> u32
    {
        return (u32)m_children.size();
    }

    template <class T>
    auto MaterialNode<T>::getChildByIndex( u32 index ) const -> SmartPtr<IMaterialNode>
    {
        return m_children[index];
    }

    template <class T>
    auto MaterialNode<T>::getChildById( hash32 id ) const -> SmartPtr<IMaterialNode>
    {
        return nullptr;
    }

    template <class T>
    auto MaterialNode<T>::getChildren() const -> Array<SmartPtr<IMaterialNode>>
    {
        return m_children;
    }

    template <class T>
    auto MaterialNode<T>::getParent() const -> SmartPtr<IMaterialNode>
    {
        return m_parent;
    }

    template <class T>
    void MaterialNode<T>::setParent( SmartPtr<IMaterialNode> parent )
    {
        m_parent = parent;
    }

    template <class T>
    auto MaterialNode<T>::getMaterial() const -> SmartPtr<IMaterial>
    {
        return m_material;
    }

    template <class T>
    void MaterialNode<T>::setMaterial( SmartPtr<IMaterial> material )
    {
        m_material = material;
    }

    template <class T>
    auto MaterialNode<T>::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = fb::make_ptr<Properties>();
        properties->setProperty( "loaded", this->isLoaded() );
        properties->setProperty( "references", this->getReferences() );
        properties->setProperty( "weakReferences", this->getWeakReferences() );
        return properties;
    }

    template <class T>
    void MaterialNode<T>::setProperties( SmartPtr<Properties> properties )
    {
    }

    // explicit instantiation
    template class MaterialNode<IMaterialNode>;
    template class MaterialNode<IMaterialPass>;
    template class MaterialNode<IMaterialTexture>;
    template class MaterialNode<IMaterialTechnique>;
    // template class CMaterialNode<IMaterialAnimatedTexture>;
}  // namespace fb::render
