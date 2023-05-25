#ifndef CInstanceObject_h__
#define CInstanceObject_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IInstancedObject.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.h>

namespace fb
{
    namespace render
    {
        class CInstancedObject : public CGraphicsObjectOgre<IInstancedObject>
        {
        public:
            CInstancedObject( SmartPtr<IGraphicsScene> smgr, const String &materialName,
                              const String &instanceManagerName );
            ~CInstancedObject() override;

            void update() override;

            void setPosition( const Vector3F &position ) override;
            Vector3F getPosition() const override;

            void setOrientation( const QuaternionF &orientation ) override;
            QuaternionF getOrientation() const override;

            void setScale( const Vector3F &scale ) override;
            Vector3F getScale() const override;

            String getName() const;

            u32 getId() const;

            void detachFromParent();

            void _attachToParent( SmartPtr<ISceneNode> parent );

            void setMaterialName( const String &materialName, s32 index = -1 ) override;

            String getMaterialName( s32 index = -1 ) const override;

            void setCastShadows( bool castShadows ) override;

            bool getCastShadows() const override;

            void setRecieveShadows( bool recieveShadows );

            bool getRecieveShadows() const;

            void setVisible( bool isVisible ) override;

            bool isVisible() const override;

            void setRenderQueueGroup( u8 renderQueueGroup );

            void setVisibilityFlags( u32 flags ) override;

            u32 getVisibilityFlags() const override;

            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;

            void _getObject( void **ppObject ) const override;

            void setPropertyValue( const String &name, const String &value );

            bool getPropertyValue( const String &name, String &value );

            void setProperties( const Properties &propertyGroup );

            bool getProperties( Properties &propertyGroup, u32 flags = AllProperties ) const;

            void setFlag( u32 flag, bool value ) override;

            bool getFlag( u32 flag ) const override;

            AABB3F getLocalAABB() const override;

            void setLocalAABB( const AABB3F &val ) override;

            Ogre::InstancedEntity *getInstancedEntity() const;
            void setInstancedEntity( Ogre::InstancedEntity *val );

            SmartPtr<IStateContext> &getStateObject();
            const SmartPtr<IStateContext> &getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> stateObject );

            void setCustomParam( unsigned char idx, const Vector4F &newParam ) override;
            Vector4F getCustomParam( unsigned char idx ) override;

            bool isAttached() const override;
            bool isLoaded() const override;

        protected:
            class InstancedObjectStateListener : public CSharedObject<IStateListener>
            {
            public:
                InstancedObjectStateListener( CInstancedObject *instancedObject );
                ~InstancedObjectStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CInstancedObject *m_owner = nullptr;
            };

            class InstancedObjectState : public CSharedObject<IState>
            {
            public:
                InstancedObjectState();
                ~InstancedObjectState() override;

                u32 getId() const;
                void setId( u32 val );

                bool isModified() const;
                void setModified( bool modified );

                u32 getModifiedFlags() const;
                void setModifiedFlags( u32 val );

                bool isDirty() const override;
                void setDirty( bool dirty ) override;

                void update() override;

                bool isRegistered() const;

                void setRegistered( bool val );

                SpinRWMutex Mutex;
                u32 m_visibilityMask;
                u32 m_renderQueueGroup;
                //UpdateCounter m_updateCount;
                bool m_isVisible;
                atomic_bool m_isRegistered;
                Vector4F m_customParams[MAX_CUSTOM_PARAMS];
            };

            using InstancedObjectStatePtr = SmartPtr<InstancedObjectState>;

            void setState( SmartPtr<IState> state );
            SmartPtr<IState> getState() const;

            void updateState();

            AABB3F m_localAABB;
            SmartPtr<IGraphicsScene> m_smgr;
            Ogre::InstancedEntity *m_instancedEntity;
            ISceneNode *m_owner = nullptr;
            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;
            InstancedObjectStatePtr m_state;

            String m_name;

            String m_materialName;
            String m_instanceManagerName;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CInstanceObject_h__
