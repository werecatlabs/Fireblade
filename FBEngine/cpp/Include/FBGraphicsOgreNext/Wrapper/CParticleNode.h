#ifndef CParticleNode_h__
#define CParticleNode_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Particle/IParticleNode.h>

namespace fb
{
    namespace render
    {

        template <class T>
        class CParticleNode : public T
        {
        public:
            CParticleNode()
            {
                m_handle = SmartPtr<Handle>( new Handle() );
            }

            ~CParticleNode()
            {
            }

            virtual void initialise( SmartPtr<scene::IDirector> objectTemplate )
            {
            }

            virtual void initialise( SmartPtr<scene::IDirector> objectTemplate,
                                     SmartPtr<Properties> instanceProperties )
            {
            }

            virtual void update( const s32 &task, const time_interval &t, const time_interval &dt )
            {
            }

            /** The handle. */
            virtual SmartPtr<Handle> &getHandle()
            {
                return m_handle;
            }

            /** The handle. */
            virtual const SmartPtr<Handle> &getHandle() const
            {
                return m_handle;
            }

            virtual void addChild( SmartPtr<IParticleNode> child )
            {
                m_children.push_back( child );
            }

            virtual void addChild( SmartPtr<IParticleNode> child, int index )
            {
                m_children[index] = child;
            }

            virtual void removeChild( SmartPtr<IParticleNode> child )
            {
                // m_children.erase_element(child);
                child->setParent( nullptr );
            }

            virtual void remove()
            {
                // if (m_parent)
                //	m_parent->removeChild(this);
            }

            virtual u32 getNumChildren() const
            {
                return m_children.size();
            }

            virtual SmartPtr<IParticleNode> &getChildByIndex( u32 index )
            {
                return m_children[index];
            }

            virtual const SmartPtr<IParticleNode> &getChildByIndex( u32 index ) const
            {
                return m_children[index];
            }

            virtual SmartPtr<IParticleNode> &getChildById( hash32 id )
            {
                static SmartPtr<IParticleNode> ptr;
                return ptr;
            }

            virtual const SmartPtr<IParticleNode> &getChildById( hash32 id ) const
            {
                return nullptr;
            }

            virtual Array<SmartPtr<IParticleNode>> getChildren() const
            {
                return m_children;
            }

            virtual SmartPtr<IParticleNode> getParent() const
            {
                return nullptr;  // m_parent;
            }

            virtual void setParent( SmartPtr<IParticleNode> parent )
            {
                m_parent = parent;
            }

            virtual void setPosition( const Vector3F &position )
            {
                m_position = position;
            }

            virtual Vector3F getPosition() const
            {
                return m_position;
            }

            virtual Vector3F getAbsolutePosition() const
            {
                return m_absolutePosition;
            }

            virtual SmartPtr<IParticleSystem> getParticleSystem() const
            {
                // return m_particleSystem;
                return nullptr;
            }

            virtual void setParticleSystem( SmartPtr<IParticleSystem> val )
            {
                m_particleSystem = val;
            }

        protected:
            SmartPtr<Handle> m_handle;
            Array<SmartPtr<IParticleNode>> m_children;
            Vector3F m_position;
            Vector3F m_absolutePosition;
            SmartPtr<IParticleNode> m_parent;
            SmartPtr<IParticleSystem> m_particleSystem;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // CParticleNode_h__
