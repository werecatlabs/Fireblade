#ifndef CMaterialNode_h__
#define CMaterialNode_h__

#include <FBCore/Interface/Graphics/IMaterialNode.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBGraphics/Wrapper/CSharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        template <class T>
        class CMaterialNode : public CSharedGraphicsObject<T>
        {
        public:
            CMaterialNode();
            CMaterialNode( u32 numChildren );
            ~CMaterialNode() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void addChild( SmartPtr<IMaterialNode> child ) override;
            void addChild( SmartPtr<IMaterialNode> child, int index ) override;
            void removeChild( SmartPtr<IMaterialNode> child ) override;

            void remove() override;

            u32 getNumChildren() const override;

            SmartPtr<IMaterialNode> getChildByIndex( u32 index ) const override;

            SmartPtr<IMaterialNode> getChildById( hash32 id ) const override;

            Array<SmartPtr<IMaterialNode>> getChildren() const override;

            SmartPtr<IMaterialNode> getParent() const override;

            void setParent( SmartPtr<IMaterialNode> parent ) override;

            SmartPtr<IMaterial> getMaterial() const override;
            void setMaterial( SmartPtr<IMaterial> material ) override;

            /** @copydoc IObject::toJson */
            String toJson() const override;

            /** @copydoc IObject::toData */
            SmartPtr<IData> toData() const override;

            /** @copydoc IObject::fromData */
            void fromData( SmartPtr<IData> data ) override;

            /** @copydoc IResource::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IResource::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

	        bool isEnabled() const
            {
                return m_enabled;
            }

            void setEnabled( bool enabled )
            {
                m_enabled = enabled;
            }

        protected:
            class MaterialNodeStateListener : public CSharedObject<IStateListener>
            {
            public:
                MaterialNodeStateListener() = default;
                ~MaterialNodeStateListener() override = default;

                void unload(SmartPtr<ISharedObject> data) override
                {
                    m_owner = nullptr;
                }

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override
                {
                    
                }

                void handleStateChanged( SmartPtr<IState> &state ) override
                {
                    
                }

                void handleQuery( SmartPtr<IStateQuery> &query ) override
                {
                    
                }

                SmartPtr<CMaterialNode> getOwner() const
                {
                    return m_owner;
                }

                void setOwner( SmartPtr<CMaterialNode> owner )
                {
                    m_owner = owner;
                }

            protected:
                AtomicSmartPtr<CMaterialNode> m_owner;
            };

            atomic_bool m_enabled = true;

            SmartPtr<IMaterial> m_material;
            SmartPtr<IMaterialNode> m_parent;
            Array<SmartPtr<IMaterialNode>> m_children;
        };
    }  // namespace render
}  // namespace fb

#endif  // CMaterialComponent_h__
