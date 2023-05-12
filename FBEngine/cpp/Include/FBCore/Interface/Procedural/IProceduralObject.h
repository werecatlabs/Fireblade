#ifndef IProceduralObject_h__
#define IProceduralObject_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Math/Polygon2.h>
#include <FBCore/Math/Polygon3.h>
#include <FBCore/Math/Sphere3.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralObject : public ISharedObject
        {
        public:
            ~IProceduralObject() override = default;

            virtual void build() = 0;

            virtual void updateBounds() = 0;

            virtual String getName() const = 0;
            virtual void setName( const String &name ) = 0;

            virtual Transform3<real_Num> getWorldTransform() const = 0;
            virtual void setWorldTransform( Transform3<real_Num> transform ) = 0;

            virtual void setPosition( const Vector3<real_Num> &position ) = 0;
            virtual Vector3<real_Num> getPosition() const = 0;

            virtual Vector3<real_Num> getScale() const = 0;
            virtual void setScale( const Vector3<real_Num> &scale ) = 0;

            virtual Quaternion<real_Num> getOrientation() const = 0;
            virtual void setOrientation( const Quaternion<real_Num> &orientation ) = 0;

            virtual SmartPtr<Properties> getProperties() const = 0;
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            virtual SmartPtr<IProceduralScene> getScene() const = 0;
            virtual void setScene( SmartPtr<IProceduralScene> val ) = 0;

            virtual SmartPtr<ISharedObject> clone() = 0;

            virtual void addNode( SmartPtr<IProceduralNode> node ) = 0;
            virtual void removeNode( SmartPtr<IProceduralNode> node ) = 0;
            virtual Array<SmartPtr<IProceduralNode>> getNodes() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralNode_h__
