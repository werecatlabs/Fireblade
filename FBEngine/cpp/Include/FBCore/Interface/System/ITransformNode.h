#ifndef __IStateNode_h__
#define __IStateNode_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    
    class ITransformNode : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~ITransformNode() override = default;

        virtual void _update( bool updateChildren, bool hasParentChanged ) = 0;
        virtual void _updateTransform( bool updateChildren, bool hasParentChanged ) = 0;
        virtual void _updateGraph( bool updateChildren, bool hasParentChanged ) = 0;
        virtual void _updateDerivedTransform( bool bUpdateChildren ) = 0;
        virtual void _updateState( bool updateChildren, bool hasParentChanged ) = 0;
        virtual void _updateAABB( bool updateChildren, bool hasParentChanged ) = 0;

        virtual void _updateBounds() = 0;

        virtual void setupOriginalWorldTransform() = 0;

        virtual void setWorldTransform( const Vector3<real_Num> &pos, const Quaternion<real_Num> &rot,
                                        const Vector3<real_Num> &scale ) = 0;
        virtual void setTransform( const Vector3<real_Num> &pos, const Quaternion<real_Num> &rot,
                                   const Vector3<real_Num> &scale ) = 0;
        // void getTransform(Vector3<real_Num>& pos, Quaternion<real_Num>& rot, Vector3<real_Num>& scale)
        // = 0;

        virtual SmartPtr<scene::IComponent> &getRigidBody() = 0;
        virtual const SmartPtr<scene::IComponent> &getRigidBody() const = 0;
        virtual void setRigidBody( SmartPtr<scene::IComponent> &val ) = 0;

        virtual void _setParent( SmartPtr<ITransformNode> parent ) = 0;
        virtual const SmartPtr<ITransformNode> &getParent() const = 0;
        virtual SmartPtr<ITransformNode> &getParent() = 0;

        virtual String getName() const = 0;
        virtual void setName( const String &name ) = 0;

        virtual s32 getHashName() const = 0;
        virtual void setHashName( s32 val ) = 0;

        virtual SmartPtr<ITransformNode> createChildNode() = 0;
        virtual SmartPtr<ITransformNode> createChildNode( const String &name ) = 0;

        virtual void addChild( SmartPtr<ITransformNode> child ) = 0;
        virtual void removeChild( SmartPtr<ITransformNode> child ) = 0;
        virtual SmartPtr<ITransformNode> getChild( int i ) = 0;
        virtual void removeAllChildren() = 0;

        virtual Array<SmartPtr<ITransformNode>> getAllChildren() const = 0;
        virtual Array<SmartPtr<ITransformNode>> getChildren() const = 0;

        virtual void _setChildren( const Array<SmartPtr<ITransformNode>> &children ) = 0;
        virtual int numChildren() const = 0;

        virtual SmartPtr<ITransformNode> &getChildByReference( u32 idx ) = 0;

        virtual Vector3<real_Num> convertLocalToWorldPosition( const Vector3<real_Num> &val ) = 0;

        virtual Vector3<real_Num> getPosition() const = 0;
        virtual void setPosition( const Vector3<real_Num> &val ) = 0;

        virtual Vector3<real_Num> getScale() const = 0;
        virtual void setScale( const Vector3<real_Num> &val ) = 0;
        virtual Quaternion<real_Num> getOrientation() const = 0;
        virtual void setOrientation( const Quaternion<real_Num> &val ) = 0;

        virtual Vector3<real_Num> _getDerivedPosition( bool usecached = true ) = 0;
        virtual Vector3<real_Num> _getDerivedScale( bool usecached = true ) = 0;
        virtual Quaternion<real_Num> _getDerivedOrientation( bool usecached = true ) = 0;
        virtual const Vector3<real_Num> _getDerivedPosition( bool usecached = true ) const = 0;
        virtual const Vector3<real_Num> _getDerivedScale( bool usecached = true ) const = 0;
        virtual const Quaternion<real_Num> _getDerivedOrientation( bool usecached = true ) const = 0;

        virtual Transform3<real_Num> getWorldTransform() const = 0;
        virtual void setWorldTransform( Transform3<real_Num> val ) = 0;

        virtual Transform3<real_Num> getLocalTransform() const = 0;
        virtual void setLocalTransform( Transform3<real_Num> val ) = 0;

        virtual void restoreOriginalTransform() = 0;

        virtual Vector3<real_Num> getOriginalPosition() const = 0;
        virtual void setOriginalPosition( const Vector3<real_Num> &val ) = 0;

        virtual Quaternion<real_Num> getOriginalOrientation() const = 0;
        virtual void setOriginalOrientation( const Quaternion<real_Num> &val ) = 0;

        virtual Vector3<real_Num> getOriginalWorldPosition() const = 0;
        virtual Quaternion<real_Num> getOriginalWorldOrientation() const = 0;

        virtual bool isTransformDirty() const = 0;
        virtual void setTransformDirty( bool val ) = 0;

        virtual AABB3<real_Num> getLocalAABB() const = 0;
        virtual void setLocalAABB( const AABB3<real_Num> &val ) = 0;

        virtual AABB3<real_Num> _getLocalAABB() const = 0;
        virtual AABB3<real_Num> _getWorldAABB() const = 0;

        virtual AABB3<real_Num> getWorldAABB() const = 0;
        virtual void setWorldAABB( const AABB3<real_Num> &val ) = 0;

        virtual void removeAndDestroyChild( SmartPtr<ITransformNode> sceneNode ) = 0;

        virtual int getAttachedObjects() const = 0;

        virtual bool isVisible() const = 0;
        virtual void setVisible( bool val, bool cascade = true ) = 0;

        virtual SmartPtr<ITransformManager> &getSceneManager() = 0;
        virtual const SmartPtr<ITransformManager> &getSceneManager() const = 0;
        virtual void setSceneManager( SmartPtr<ITransformManager> val ) = 0;

        virtual bool isRenderNode() const = 0;
        virtual void setRenderNode( bool val ) = 0;

        virtual void optimise() = 0;
    };
}  // end namespace fb

#endif  // __IStateNode_h__
