#ifndef __IProceduralCollision_h__
#define __IProceduralCollision_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Triangle3.h>
#include <FBCore/Math/Sphere3.h>
#include <FBCore/Math/Polygon2.h>
#include <FBCore/Math/Line2.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralCollision : public ISharedObject
        {
        public:
            ~IProceduralCollision() override = default;

            virtual bool rayTest( const Vector3<real_Num> &start, const Vector3<real_Num> &dir,
                                  Vector3<real_Num> &outHitPos ) = 0;
            virtual bool rayTest( const Vector3<real_Num> &start, const Vector3<real_Num> &dir,
                                  Vector3<real_Num> &outHitPos, Triangle3<real_Num> &outTriangle ) = 0;
            virtual bool rayTest( const Vector3<real_Num> &start, const Vector3<real_Num> &dir,
                                  Vector3<real_Num> &outHitPos, Triangle3<real_Num> &outTriangle,
                                  const Array<String> &filter ) = 0;

            virtual bool intersects( const SmartPtr<IProceduralCity> &city,
                                     const Sphere3<real_Num> &sphere ) = 0;

            //
            // road functions
            //
            virtual bool intersects( const SmartPtr<IRoad> &srcRoad,
                                     const Sphere3<real_Num> &sphere ) = 0;
            virtual bool intersects( const SmartPtr<IRoad> &srcRoad, const Sphere3<real_Num> &sphere,
                                     SmartPtr<IRoadNode> &collidingNode ) const = 0;
            virtual bool intersects( const SmartPtr<IRoad> &srcRoad, const AABB3<real_Num> &box ) = 0;
            virtual bool intersects( const SmartPtr<IRoad> &srcRoad,
                                     const Polygon2<real_Num> &polygon ) = 0;
            virtual bool intersects( const SmartPtr<IRoad> &srcRoad, const Line2<real_Num> &line,
                                     Vector2<real_Num> &intersectionPoint, bool checkStart ) = 0;
            virtual bool intersects( const SmartPtr<IRoad> &srcRoad, SmartPtr<IRoad> road,
                                     Vector3<real_Num> &intersectionPoint, bool checkStart ) const = 0;
            virtual bool intersects( const SmartPtr<IRoad> &srcRoad, SmartPtr<IRoad> road,
                                     Vector3<real_Num> &intersectionPoint,
                                     Array<SmartPtr<IRoadNode>> &nodes, bool checkStart ) const = 0;
            virtual bool intersects( const SmartPtr<IRoad> &srcRoad, SmartPtr<IRoadNode> node,
                                     Array<SmartPtr<IRoadNode>> &nodes, bool checkStart ) const = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // ICollisionManager_h__
