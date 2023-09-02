#ifndef _CSceneNode_H
#define _CSceneNode_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Script/IScriptReceiver.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/Parameter.h>
#include <FBCore/Graphics/CSceneNode.h>
#include <OgreNode.h>

namespace fb
{
    namespace render
    {

        /** Implements ISceneNode interface for Ogre. */
        class CSceneNodeOgre : public CSceneNode
        {
        public:
            CSceneNodeOgre();
            CSceneNodeOgre( SmartPtr<IGraphicsScene> creator );
            ~CSceneNodeOgre() override;

            virtual void load( SmartPtr<ISharedObject> data ) override;
            virtual void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::SceneNode *sceneNode );
            void destroy();

            void setMaterialName( const String &materialName, bool cascade = true );

            void setPosition( const Vector3F &position ) override;
            Vector3F getPosition() const override;
            Vector3F getWorldPosition() const override;

            void setRotationFromDegrees( const Vector3F &degrees ) override;

            void setOrientation( const QuaternionF &orientation ) override;
            QuaternionF getOrientation() const override;
            QuaternionF getWorldOrientation() const override;

            void setScale( const Vector3F &scale ) override;
            Vector3F getScale() const override;
            Vector3F getWorldScale() const override;

            void lookAt( const Vector3F &targetPoint ) override;

            void setFixedYawAxis( bool useFixed, const Vector3F &fixedAxis = Vector3F::UNIT_Y ) override;

            AABB3F getLocalAABB() const override;

            AABB3F calculateAABB() const;

            AABB3F getWorldAABB() const override;

            void setVisible( bool visible, bool cascade = true ) override;

            bool isVisible() const;

            void setCulled( bool culled ) override;
            bool isCulled() const override;

            void attachObject( SmartPtr<IGraphicsObject> object ) override;
            void detachObject( SmartPtr<IGraphicsObject> object ) override;
            void detachAllObjects() override;

            Array<SmartPtr<IGraphicsObject>> getObjects() const override;
            SmartPtr<ISceneNode> addChildSceneNode(
                const String &name = StringUtil::EmptyString ) override;
            SmartPtr<ISceneNode> addChildSceneNode( const Vector3F &position ) override;

            u32 getNumObjects() const override;

            void addChild( SmartPtr<ISceneNode> child ) override;
            bool removeChild( SmartPtr<ISceneNode> child ) override;
            void add() override;
            void remove() override;
            void removeAllChildren();
            SmartPtr<ISceneNode> findChild( const String &name ) override;
            void getChildren( Array<SmartPtr<ISceneNode>> &children ) const;

            void removeChildren() override;

            Array<SmartPtr<ISceneNode>> getChildren() const override;

            u32 getNumChildren() const override;

            void needUpdate( bool forceParentUpdate = false ) override;

            SmartPtr<ISceneNode> clone( SmartPtr<ISceneNode> parent = nullptr,
                                        const String &name = StringUtil::EmptyString ) const override;

            void showBoundingBox( bool show ) override;
            bool getShowBoundingBox() const override;

            void _getObject( void **ppObject ) const override;

            Ogre::SceneNode *getSceneNode() const;

            void _updateBounds() override;

            void setVisibilityFlags( u32 flags ) override;
            u32 getVisibilityFlags() const;

            void setFlag( u32 flag, bool value ) override;
            bool getFlag( u32 flag ) const override;
            bool _getFlag( u32 flag ) const;

            /** @copydoc ISceneNode::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc ISceneNode::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc ISceneNode::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            void setTransformationDirty( u32 msgType );
            void _updateBoundingBox();

            void *_getRenderSystemTransform() const override;

            u32 getObjectsBuffer( SmartPtr<IGraphicsObject> *buffer, u32 bufferSize ) const override;

            u32 getChildrenBuffer( SmartPtr<ISceneNode> *children, u32 bufferSize ) const override;

            void destroyStateContext();

            class ScriptReceiver : public IScriptReceiver
            {
            public:
                ScriptReceiver( CSceneNodeOgre *node );

                s32 setProperty( hash_type id, const Parameter &param ) override;
                s32 setProperty( hash_type id, const Parameters &params ) override;
                s32 setProperty( hash_type hash, void *param ) override;
                s32 getProperty( hash_type id, Parameter &param ) const override;
                s32 getProperty( hash_type id, Parameters &params ) const override;
                s32 getProperty( hash_type hash, void *param ) const override;
                s32 callFunction( hash_type hashId, const Parameters &params,
                                  Parameters &results ) override;

            private:
                CSceneNodeOgre *m_node = nullptr;
            };

            class SceneNodeStateListener : public IStateListener
            {
            public:
                SceneNodeStateListener() = default;
                ~SceneNodeStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CSceneNodeOgre *getOwner() const;
                void setOwner( CSceneNodeOgre *owner );

            protected:
                CSceneNodeOgre *m_owner = nullptr;
            };

            class NodeListener : public Ogre::Node::Listener
            {
            public:
                NodeListener( CSceneNodeOgre *owner );
                ~NodeListener() override;

                void nodeUpdated( const Ogre::Node *node ) override;
                void nodeDestroyed( const Ogre::Node *node ) override;
                void nodeAttached( const Ogre::Node *node ) override;
                void nodeDetached( const Ogre::Node *node ) override;

            protected:
                CSceneNodeOgre *m_owner = nullptr;
            };

            SmartPtr<SceneNodeState> m_state;

            NodeListener *m_nodeListener = nullptr;
            Ogre::SceneNode *m_sceneNode = nullptr;

            atomic_u32 m_flags;

            atomic_u32 m_lastUpdate;
            atomic_u32 m_transformUpdate;
            u32 m_visibilityFlags = 0;

            atomic_bool m_isCulled;

            /// an array of this scene node's children
            Array<SmartPtr<ISceneNode>> m_children;

            /// The objects attached to this scene node.
            Array<SmartPtr<IGraphicsObject>> m_graphicsObjects;

            SpinRWMutex( SceneNodeMutex );
            SpinRWMutex( StatesMutex );

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
