#ifndef FBPhysxBoxShape_h__
#define FBPhysxBoxShape_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IBoxShape3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBPhysx/FBPhysxShape.h>
#include <geometry/PxBoxGeometry.h>

namespace fb
{
    namespace physics
    {

        /** Implementation of a box shape. */
        class PhysxBoxShape : public PhysxShape<IBoxShape3>
        {
        public:
            PhysxBoxShape();
            ~PhysxBoxShape() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            Vector3<real_Num> getExtents() const override;
            void setExtents( const Vector3<real_Num> &extents ) override;

            void setAABB( const AABB3<real_Num> &box ) override;
            AABB3<real_Num> getAABB() const override;

            bool isValid() const;

        protected:
            class BoxShapeStateListener : public SharedObject<IStateListener>
            {
            public:
                BoxShapeStateListener() = default;
                ~BoxShapeStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                PhysxBoxShape *getOwner() const;
                void setOwner( PhysxBoxShape *owner );

            protected:
                PhysxBoxShape *m_owner = nullptr;
            };

            void createShape();

            physx::PxBoxGeometry createGeometry();

            Vector3<real_Num> m_extents = Vector3<real_Num>::unit();
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // FBPhysxBoxShape_h__
