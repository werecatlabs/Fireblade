#ifndef CParticleNode_h__
#define CParticleNode_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Graphics/IParticleNode.h>

namespace fb
{
    namespace render
    {

        template <class T>
        class CParticleNode : public T
        {
        public:
            CParticleNode();

            ~CParticleNode();

            virtual void addChild( SmartPtr<IParticleNode> child );

            virtual void addChild( SmartPtr<IParticleNode> child, int index );

            virtual void removeChild( SmartPtr<IParticleNode> child );

            virtual void remove();

            virtual u32 getNumChildren() const;

            virtual SmartPtr<IParticleNode> &getChildByIndex( u32 index );

            virtual const SmartPtr<IParticleNode> &getChildByIndex( u32 index ) const;

            virtual SmartPtr<IParticleNode> &getChildById( hash32 id );

            virtual const SmartPtr<IParticleNode> &getChildById( hash32 id ) const;

            virtual Array<SmartPtr<IParticleNode>> getChildren() const;

            virtual SmartPtr<IParticleNode> getParent() const;

            virtual void setParent( SmartPtr<IParticleNode> parent );

            virtual void setPosition( const Vector3F &position );

            virtual Vector3F getPosition() const;

            virtual Vector3F getAbsolutePosition() const;

            virtual SmartPtr<IParticleSystem> getParticleSystem() const;

            virtual void setParticleSystem( SmartPtr<IParticleSystem> particleSystem );

            FB_CLASS_REGISTER_TEMPLATE_DECL( CParticleNode, T );

        protected:
            Array<SmartPtr<IParticleNode>> m_children;
            Vector3F m_position;
            Vector3F m_absolutePosition;
            SmartPtr<IParticleNode> m_parent;
            SmartPtr<IParticleSystem> m_particleSystem;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, CParticleNode, T, T );

        template <class T>
        CParticleNode<T>::CParticleNode()
        {
        }

        template <class T>
        CParticleNode<T>::~CParticleNode()
        {
        }

        template <class T>
        void CParticleNode<T>::addChild( SmartPtr<IParticleNode> child )
        {
            m_children.push_back( child );
        }

        template <class T>
        void CParticleNode<T>::addChild( SmartPtr<IParticleNode> child, int index )
        {
            m_children[index] = child;
        }

        template <class T>
        void CParticleNode<T>::removeChild( SmartPtr<IParticleNode> child )
        {
            // m_children.erase_element(child);
            child->setParent( nullptr );
        }

        template <class T>
        void CParticleNode<T>::remove()
        {
            // if (m_parent)
            //	m_parent->removeChild(this);
        }

        template <class T>
        u32 CParticleNode<T>::getNumChildren() const
        {
            return m_children.size();
        }

        template <class T>
        SmartPtr<IParticleNode> &CParticleNode<T>::getChildByIndex( u32 index )
        {
            return m_children[index];
        }

        template <class T>
        const SmartPtr<IParticleNode> &CParticleNode<T>::getChildByIndex( u32 index ) const
        {
            return m_children[index];
        }

        template <class T>
        SmartPtr<IParticleNode> &CParticleNode<T>::getChildById( hash32 id )
        {
            static SmartPtr<IParticleNode> ptr;
            return ptr;
        }

        template <class T>
        const SmartPtr<IParticleNode> &CParticleNode<T>::getChildById( hash32 id ) const
        {
            return nullptr;
        }

        template <class T>
        Array<SmartPtr<IParticleNode>> CParticleNode<T>::getChildren() const
        {
            return m_children;
        }

        template <class T>
        SmartPtr<IParticleNode> CParticleNode<T>::getParent() const
        {
            return m_parent;
        }

        template <class T>
        void CParticleNode<T>::setParent( SmartPtr<IParticleNode> parent )
        {
            m_parent = parent;
        }

        template <class T>
        void CParticleNode<T>::setPosition( const Vector3F &position )
        {
            m_position = position;
        }

        template <class T>
        Vector3F CParticleNode<T>::getPosition() const
        {
            return m_position;
        }

        template <class T>
        Vector3F CParticleNode<T>::getAbsolutePosition() const
        {
            return m_absolutePosition;
        }

        template <class T>
        SmartPtr<IParticleSystem> CParticleNode<T>::getParticleSystem() const
        {
            return m_particleSystem;
        }

        template <class T>
        void CParticleNode<T>::setParticleSystem( SmartPtr<IParticleSystem> particleSystem )
        {
            m_particleSystem = particleSystem;
        }

    }  // end namespace render
}  // end namespace fb

#endif  // CParticleNode_h__
