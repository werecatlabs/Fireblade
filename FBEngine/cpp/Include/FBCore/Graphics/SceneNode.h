#ifndef CSceneNode_h__
#define CSceneNode_h__

#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>

namespace fb
{
    namespace render
    {
        /**
         * @brief Implementation of the ISceneNode interface.
         */
        class SceneNode : public SharedGraphicsObject<ISceneNode>
        {
        public:
            /**
             * @brief Constructor.
             */
            SceneNode();

            /**
             * @brief Virtual destructor.
             */
            ~SceneNode() override;

            /**
             * @copydoc IGraphicsSystem::load
             */
            void load( SmartPtr<ISharedObject> data ) override;

            /**
             * @copydoc IGraphicsSystem::unload
             */
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * @brief Detaches the given object pointer from this scene node.
             * @param object The object to detach.
             */
            void detachObjectPtr( IGraphicsObject *object );

            /**
             * @brief Gets the parent scene node of this node.
             * @return The parent scene node.
             */
            SmartPtr<ISceneNode> getParent() const override;

            /**
             * @brief Sets the parent scene node of this node.
             * @param parent The parent scene node.
             */
            void setParent( SmartPtr<ISceneNode> parent ) override;

            /**
             * @brief Gets the scene manager that created this node.
             * @return The scene manager.
             */
            SmartPtr<IGraphicsScene> getCreator() const override;

            /**
             * @brief Sets the scene manager that created this node.
             * @param creator The scene manager.
             */
            void setCreator( SmartPtr<IGraphicsScene> creator ) override;

            void setPosition( const Vector3<real_Num> &position ) override;

            Vector3<real_Num> getPosition() const override;

            Vector3<real_Num> getWorldPosition() const override;

            void setRotationFromDegrees( const Vector3<real_Num> &degrees ) override;

            void setOrientation( const Quaternion<real_Num> &orientation ) override;

            Quaternion<real_Num> getOrientation() const override;

            Quaternion<real_Num> getWorldOrientation() const override;

            void setScale( const Vector3<real_Num> &scale ) override;

            Vector3<real_Num> getScale() const override;

            Vector3<real_Num> getWorldScale() const override;

            void *_getRenderSystemTransform() const override;

            void lookAt( const Vector3<real_Num> &targetPoint ) override;

            void setFixedYawAxis(
                bool useFixed, const Vector3<real_Num> &fixedAxis = Vector3<real_Num>::UNIT_Y ) override;

            AABB3<real_Num> getLocalAABB() const override;

            AABB3<real_Num> getWorldAABB() const override;

            void attachObject( SmartPtr<IGraphicsObject> object ) override;

            void detachObject( SmartPtr<IGraphicsObject> object ) override;

            void detachAllObjects() override;

            Array<SmartPtr<IGraphicsObject>> getObjects() const override;

            u32 getNumObjects() const override;

            SmartPtr<ISceneNode> addChildSceneNode(
                const String &name = StringUtil::EmptyString ) override;

            SmartPtr<ISceneNode> addChildSceneNode( const Vector3<real_Num> &position ) override;

            void addChild( SmartPtr<ISceneNode> child ) override;

            bool removeChild( SmartPtr<ISceneNode> child ) override;

            void remove() override;

            void removeChildren() override;

            SmartPtr<ISceneNode> findChild( const String &name ) override;

            Array<SmartPtr<ISceneNode>> getChildren() const override;

            u32 getNumChildren() const override;

            void needUpdate( bool forceParentUpdate = false ) override;

            SmartPtr<ISceneNode> clone( SmartPtr<ISceneNode> parent = nullptr,
                                        const String &name = StringUtil::EmptyString ) const override;

            void updateBounds() override;

            void _getObject( void **ppObject ) const override;

            SmartPtr<Properties> getProperties() const override;

            void setProperties( SmartPtr<Properties> properties ) override;

            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            /**< The scene manager that created this node. */
            AtomicWeakPtr<IGraphicsScene> m_creator;

            /**< The parent scene node. */
            AtomicWeakPtr<ISceneNode> m_parent;

            /// an array of this scene node's children
            Array<SmartPtr<ISceneNode>> m_children;

            /// The objects attached to this scene node.
            Array<SmartPtr<IGraphicsObject>> m_graphicsObjects;
        };
    } // namespace render
}     // namespace fb

#endif  // CSceneNode_h__
