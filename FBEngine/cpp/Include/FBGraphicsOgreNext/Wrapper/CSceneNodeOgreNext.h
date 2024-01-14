#ifndef __CSceneNodeOgreNext_H
#define __CSceneNodeOgreNext_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/SceneNode.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Atomics/Atomics.h>
#include <OgreNode.h>

namespace fb
{
    namespace render
    {
        class CSceneNodeOgreNext : public SceneNode
        {
        public:
            CSceneNodeOgreNext();
            CSceneNodeOgreNext( SmartPtr<IGraphicsScene> creator );
            ~CSceneNodeOgreNext() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::SceneNode *sceneNode );
            void destroy();

            void setMaterialName( const String &materialName, bool cascade = true );

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

            Array<SmartPtr<ISceneNode>> getChildren() const override;

            u32 getNumChildren() const override;

            void needUpdate( bool forceParentUpdate = false ) override;

            SmartPtr<ISceneNode> clone( SmartPtr<ISceneNode> parent = nullptr,
                                        const String &name = StringUtil::EmptyString ) const override;

            void showBoundingBox( bool show ) override;
            bool getShowBoundingBox() const override;

            void _getObject( void **ppObject ) const override;

            Ogre::SceneNode *getSceneNode() const;

            void updateBounds() override;

            void setVisibilityFlags( u32 flags ) override;
            u32 getVisibilityFlags() const;

            void setFlag( u32 flag, bool value ) override;
            bool getFlag( u32 flag ) const override;
            bool _getFlag( u32 flag ) const;

            /** @copydoc ISceneNode::getProperties */
            SmartPtr<Properties> getProperties() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            void setTransformationDirty( u32 msgType );
            void _updateBoundingBox();

            void *_getRenderSystemTransform() const override;

            u32 getObjectsBuffer( SmartPtr<IGraphicsObject> *buffer, u32 bufferSize ) const override;

            void removeChildren() override;

            u32 getChildrenBuffer( SmartPtr<ISceneNode> *children, u32 bufferSize ) const override;

            class SceneNodeStateListener : public IStateListener
            {
            public:
                SceneNodeStateListener() = default;
                ~SceneNodeStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<CSceneNodeOgreNext>& getOwner() ;
                const SmartPtr<CSceneNodeOgreNext>& getOwner() const;
                void setOwner( SmartPtr<CSceneNodeOgreNext> owner );

            protected:
                SmartPtr<CSceneNodeOgreNext> m_owner;
            };

            class NodeListener : public Ogre::Node::Listener
            {
            public:
                NodeListener( CSceneNodeOgreNext *owner );
                ~NodeListener() override;

                void nodeUpdated( const Ogre::Node *node ) override;
                void nodeDestroyed( const Ogre::Node *node ) override;
                void nodeAttached( const Ogre::Node *node ) override;
                void nodeDetached( const Ogre::Node *node ) override;

            protected:
                CSceneNodeOgreNext *m_owner = nullptr;
            };

            NodeListener *m_nodeListener = nullptr;
            Ogre::SceneNode *m_sceneNode = nullptr;

            atomic_u32 m_flags;

            atomic_u32 m_lastUpdate;
            atomic_u32 m_transformUpdate;
            u32 m_visibilityFlags = 0;

            atomic_bool m_isCulled;

            SpinRWMutex m_sceneNodeMutex;
            SpinRWMutex m_statesMutex;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
