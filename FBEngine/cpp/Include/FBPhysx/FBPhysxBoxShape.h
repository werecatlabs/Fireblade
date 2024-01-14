#ifndef FBPhysxBoxShape_h__
#define FBPhysxBoxShape_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IBoxShape3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
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
            /** Constructor */
            PhysxBoxShape();

            /** Destructor */
            ~PhysxBoxShape() override;

            /** @copydoc PhysxShape<IBoxShape3>::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc PhysxShape<IBoxShape3>::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc PhysxShape<IBoxShape3>::getExtents */
            Vector3<real_Num> getExtents() const override;

            /** @copydoc PhysxShape<IBoxShape3>::setExtents */
            void setExtents( const Vector3<real_Num> &extents ) override;

            /** @copydoc PhysxShape<IBoxShape3>::setAABB */
            void setAABB( const AABB3<real_Num> &box ) override;

            /** @copydoc PhysxShape<IBoxShape3>::getAABB */
            AABB3<real_Num> getAABB() const override;

            /** @copydoc PhysxShape<IBoxShape3>::isValid */
            bool isValid() const;

        protected:
            class BoxShapeStateListener : public IStateListener
            {
            public:
                BoxShapeStateListener() = default;
                ~BoxShapeStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<PhysxBoxShape> getOwner() const;
                void setOwner( SmartPtr<PhysxBoxShape> owner );

            protected:
                AtomicSmartPtr<PhysxBoxShape> m_owner;
            };

            void createShape();

            physx::PxBoxGeometry createGeometry();

            Vector3<real_Num> m_extents = Vector3<real_Num>::unit();
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // FBPhysxBoxShape_h__
