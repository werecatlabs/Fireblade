#ifndef _ISceneNode_H
#define _ISceneNode_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    namespace render
    {

        /** Interface for a class representing a node in the scene graph.

            This class provides an interface for the creation, management and manipulation of nodes in the scene graph.
            A scene graph is a hierarchical structure that represents a 3D scene as a collection of nodes, where each node can have
            child nodes and/or objects attached to it, and a parent node which it is connected to.

            Some of the common operations that can be performed on a scene node include positioning, rotation, scaling,
            visibility, attaching and detaching objects, setting and retrieving properties, and more.

            In addition, there are various flags and state messages that can be used to control the behavior of scene nodes
            and the objects attached to them.

            The ISceneNode interface is implemented by various scene graph systems, and provides a consistent and unified
            way to work with scene nodes in a graphics engine. Users can use this interface to create and manipulate scene nodes,
            and to perform operations such as rendering and ray casting on the scene graph.

            The ISceneNode interface is a subclass of ISharedObject, which means that scene nodes are reference-counted objects.
            Users can obtain smart pointers to scene nodes, which automatically manage the lifetime of the nodes and their objects.

            @note This interface does not provide any implementation details, and is meant to be used as an abstract base class only.
            Actual implementation details are provided by concrete subclasses of ISceneNode, which are specific to the graphics engine
            being used.

            @see ISharedObject
        */
        class ISceneNode : public ISharedObject
        {
        public:
            /** Bitmask value that includes all the properties of a SceneNode. */
            static const u32 ALL_PROPERTIES = ( 1 << 0 );

            /** Hash code used to query the local axis-aligned bounding box of a node. */
            static const hash_type STATE_QUERY_TYPE_LOCAL_AABB;

            /** Hash code used to query the world axis-aligned bounding box of a node. */
            static const hash_type STATE_QUERY_TYPE_WORLD_AABB;

            /** Flag indicating that the node should look at a target point. */
            static const u32 STATE_FLAG_LOOK_AT = ( 1 << 1 );

            /** Flag indicating that the node position has changed. */
            static const u32 STATE_FLAG_POSITION = ( 1 << 2 );

            /** Flag indicating that the node orientation has changed. */
            static const u32 STATE_FLAG_ORIENTATION = ( 1 << 3 );

            /** Flag indicating that the node should be updated. */
            static const u32 STATE_FLAG_UPDATE = ( 1 << 4 );

            /** Hash code used to send a position state message to a node. */
            static const hash_type STATE_MESSAGE_POSITION;

            /** Hash code used to send a scale state message to a node. */
            static const hash_type STATE_MESSAGE_SCALE;

            /** Hash code used to send an orientation state message to a node. */
            static const hash_type STATE_MESSAGE_ORIENTATION;

            /** Hash code used to send a look-at state message to a node. */
            static const hash_type STATE_MESSAGE_LOOK_AT;

            /** Hash code used to send an add state message to a node. */
            static const hash_type STATE_MESSAGE_ADD;

            /** Hash code used to send a remove state message to a node. */
            static const hash_type STATE_MESSAGE_REMOVE;

            /** Hash code used to send an add-child state message to a node. */
            static const hash_type STATE_MESSAGE_ADD_CHILD;

            /** Hash code used to send a remove-child state message to a node. */
            static const hash_type STATE_MESSAGE_REMOVE_CHILD;

            /** Hash code used to send an attach-object state message to a node. */
            static const hash_type STATE_MESSAGE_ATTACH_OBJECT;
            static const hash_type STATE_MESSAGE_DETACH_OBJECT;
            static const hash_type STATE_MESSAGE_DETACH_ALL_OBJECTS;

            /** Virtual destructor. */
            ~ISceneNode() override = default;

            /** Sets the position of the node relative to it's parent. */
            virtual void setPosition( const Vector3<real_Num> &position ) = 0;

            /** Gets the position of the node relative to it's parent. */
            virtual Vector3<real_Num> getPosition() const = 0;

            /** Gets the position of the node as derived from all parents. */
            virtual Vector3<real_Num> getWorldPosition() const = 0;

            /** Sets the rotation of the scene node. */
            virtual void setRotationFromDegrees( const Vector3<real_Num> &degrees ) = 0;

            /** Sets the orientation of this node via Quaternion parameters. */
            virtual void setOrientation( const Quaternion<real_Num> &orientation ) = 0;

            /** Returns a Quaternion representing the nodes orientation. */
            virtual Quaternion<real_Num> getOrientation() const = 0;

            /** Gets the orientation of the node as derived from all parents. */
            virtual Quaternion<real_Num> getWorldOrientation() const = 0;

            /** Sets the scaling factor applied to this node. */
            virtual void setScale( const Vector3<real_Num> &scale ) = 0;

            /** Gets the scaling factor of this node. */
            virtual Vector3<real_Num> getScale() const = 0;

            /** Gets the scaling factor of the node as derived from all parents. */
            virtual Vector3<real_Num> getWorldScale() const = 0;

            /** Gets the transformation cached for the render system. */
            virtual void *_getRenderSystemTransform() const = 0;

            /** Points the node at a location in world space. */
            virtual void lookAt( const Vector3<real_Num> &targetPoint ) = 0;

            /** Tells the node whether to yaw around it's own local Y axis or a fixed axis of choice. */
            virtual void setFixedYawAxis(
                bool useFixed, const Vector3<real_Num> &fixedAxis = Vector3<real_Num>::UNIT_Y ) = 0;

            /** Gets the local axis-aligned bounding box of this node.
             */
            virtual AABB3<real_Num> getLocalAABB() const = 0;

            /** Gets the world axis-aligned bounding box of this node.
             */
            virtual AABB3<real_Num> getWorldAABB() const = 0;

            /** Makes all objects attached to this node become visible / invisible.
             */
            virtual void setVisible( bool isVisible, bool cascade = true ) = 0;

            /** Sets a flag indicating whether or not this node is culled. */
            virtual void setCulled( bool culled ) = 0;

            /** Returns a boolean indicting whether or not this node is culled. */
            virtual bool isCulled() const = 0;

            /** Adds an instance of a scene object to this node.
             */
            virtual void attachObject( SmartPtr<IGraphicsObject> object ) = 0;

            /** Detaches an instance of a scene object to this node.
             */
            virtual void detachObject( SmartPtr<IGraphicsObject> object ) = 0;

            /** Detaches all objects. */
            virtual void detachAllObjects() = 0;

            /** Gets the attached objects. */
            virtual Array<SmartPtr<IGraphicsObject>> getObjects() const = 0;

            /** Gets the attached objects.
            @return The number of objects.
            */
            virtual u32 getObjectsBuffer( SmartPtr<IGraphicsObject> *buffer, u32 bufferSize ) const = 0;

            /** Gets the number of objects. */
            virtual u32 getNumObjects() const = 0;

            /** Creates an unnamed new SceneNode as a child of this node. */
            virtual SmartPtr<ISceneNode> addChildSceneNode(
                const String &name = StringUtil::EmptyString ) = 0;

            /** Creates an unnamed new SceneNode as a child of this node. */
            virtual SmartPtr<ISceneNode> addChildSceneNode( const Vector3<real_Num> &position ) = 0;

            /** Used to set the scene node's parent. For internal use only. */
            virtual void setParent( SmartPtr<ISceneNode> parent ) = 0;

            /** Returns the parent of this scene node. */
            virtual SmartPtr<ISceneNode> getParent() const = 0;

            /** Adds a child to this entity. */
            virtual void addChild( SmartPtr<ISceneNode> child ) = 0;

            /** Removes a child of this entity. */
            virtual bool removeChild( SmartPtr<ISceneNode> child ) = 0;

            /** Adds the scene node to the scene. */
            virtual void add() = 0;

            /** Remove the child from its parent. */
            virtual void remove() = 0;

            /** Removes all children. */
            virtual void removeChildren() = 0;

            /** Return the child with the name provided. Return null if a child is not found. */
            virtual SmartPtr<ISceneNode> findChild( const String &name ) = 0;

            /** Gets children. */
            virtual Array<SmartPtr<ISceneNode>> getChildren() const = 0;

            /** Fills an Array with the children of this entity. */
            virtual u32 getChildrenBuffer( SmartPtr<ISceneNode> *children, u32 bufferSize ) const = 0;

            /** Gets the number of children. */
            virtual u32 getNumChildren() const = 0;

            /** To be called in the event of transform changes to this node that require it's
             * recalculation. */
            virtual void needUpdate( bool forceParentUpdate = false ) = 0;

            /** Clones this node an the objects attached to it.  */
            virtual SmartPtr<ISceneNode> clone( SmartPtr<ISceneNode> parent = nullptr,
                                                const String &name = StringUtil::EmptyString ) const = 0;

            /** Allows the showing of the node's bounding box.
            @remarks
            Use this to show or hide the bounding box of the node.
            */
            virtual void showBoundingBox( bool show ) = 0;

            /** This allows scene managers to determine if the node's bounding box
            should be added to the rendering queue.
            @remarks
            Scene Managers that implement their own _findVisibleObjects will have to
            check this flag and then use _addBoundingBoxToQueue to add the bounding box
            wireframe.
            */
            virtual bool getShowBoundingBox() const = 0;

            /** Updates the bounds of the scene node. */
            virtual void _updateBounds() = 0;

            /** Sets the visibility flags for the attached graphic objects. */
            virtual void setVisibilityFlags( u32 flags ) = 0;

            /** Gets a pointer to the underlying scene node object. This is dependent on the graphics
             * library used. */
            virtual void _getObject( void **ppObject ) const = 0;

            /** Sets a flag of the scene node. */
            virtual void setFlag( u32 flag, bool value ) = 0;

            /** Gets a value of a flag. */
            virtual bool getFlag( u32 flag ) const = 0;

            /** Gets the scene manager that created the scene node. */
            virtual SmartPtr<IGraphicsScene> getCreator() const = 0;

            /** Sets the scene manager that created the scene node. */
            virtual void setCreator( SmartPtr<IGraphicsScene> creator ) = 0;

            /** Gets the data as a properties object.
            @return The data as a properties object.
            */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /** Sets the data as a properties object.
            @param properties The properties object.
            */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif
