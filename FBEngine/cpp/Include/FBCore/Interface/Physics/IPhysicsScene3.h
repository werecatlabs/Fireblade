#ifndef IPhysicsWorld3_h__
#define IPhysicsWorld3_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Ray3.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief Interface for a physics scene.
         */
        class IPhysicsScene3 : public ISharedObject
        {
        public:
            /**
             * @brief Destructor.
             */
            ~IPhysicsScene3() override = default;

            /**
             * @brief Removes everything from the world.
             */
            virtual void clear() = 0;

            /**
             * @brief Adds a body to the scene.
             * @param body A shared pointer to the body to be added.
             */
            virtual void addActor( SmartPtr<IPhysicsBody3> body ) = 0;

            /**
             * @brief Removes a body from the scene.
             * @param body A shared pointer to the body to be removed.
             */
            virtual void removeActor( SmartPtr<IPhysicsBody3> body ) = 0;

            /**
             * @brief Sets the size of the scene.
             * @param size A vector representing the size of the scene.
             */
            virtual void setSize( const Vector3<real_Num> &size ) = 0;

            /**
             * @brief Gets the size of the scene.
             * @return A vector representing the size of the scene.
             */
            virtual Vector3<real_Num> getSize() const = 0;

            /**
             * @brief Performs a ray intersection test.
             * @param start The starting position of the ray.
             * @param direction The direction of the ray.
             * @param hitPos A vector where the hit position will be stored.
             * @param hitNormal A vector where the hit normal will be stored.
             * @param collisionType The collision type.
             * @param collisionMask The collision mask.
             * @return True if there was an intersection, false otherwise.
             */
            virtual bool rayTest( const Vector3<real_Num> &start, const Vector3<real_Num> &direction, Vector3<real_Num> &hitPos,
                                  Vector3<real_Num> &hitNormal, u32 collisionType = 0,
                                  u32 collisionMask = 0 ) = 0;

            /**
             * @brief Performs a line intersection test.
             * @param start The start point of the line.
             * @param end The end point of the line.
             * @param hitPos The hit position of the intersection.
             * @param hitNormal The surface normal of the intersection.
             * @param object A shared pointer to the object that intersects.
             * @param collisionType The collision type.
             * @param collisionMask The collision mask.
             * @return True if there was an intersection, false otherwise.
             */
            virtual bool intersects( const Vector3<real_Num> &start, const Vector3<real_Num> &end, Vector3<real_Num> &hitPos,
                                     Vector3<real_Num> &hitNormal, SmartPtr<ISharedObject> &object,
                                     u32 collisionType = 0, u32 collisionMask = 0 ) = 0;

            /**
             * @brief Casts a ray and returns all the hits.
             * @param origin The origin of the ray.
             * @param dir The direction of the ray.
             * @param hits An array where all the hits will be stored.
             * @return True if there was at least one hit, false otherwise.
             */
            virtual bool castRay( const Vector3<real_Num> &origin, const Vector3<real_Num> &dir,
                                  Array<SmartPtr<IRaycastHit>> &hits ) = 0;

            /**
             * @brief Casts a ray and returns the first hit.
             * @param ray The ray to be cast.
             * @param hit The hit information to be stored.
             * @return True if there was a hit, false otherwise.
             */
            virtual bool castRay( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit ) = 0;

            /**
             * @brief Casts a dynamic ray and returns the first hit.
             * @param ray The ray to be cast.
             * @param hit The hit information to be stored.
             * @return True if there was a hit, false otherwise.
             */
            virtual bool castRayDynamic( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit ) = 0;

            /**
             * @brief Sets the gravity vector of the scene.
             * @param vec A vector representing the new gravity.
             */
            virtual void setGravity( const Vector3<real_Num> &vec ) = 0;

            /**
             * @brief Gets the gravity vector of the scene.
             * @return A vector representing the gravity of the scene.
             */
            virtual Vector3<real_Num> getGravity() const = 0;

            /**
             * @brief Simulates the physics scene for the given time.
             * @param elapsedTime The elapsed time to simulate.
             * @param scratchMemBlock A pointer to a block of scratch memory for the simulation.
             * @param scratchMemBlockSize The size of the scratch memory block in bytes.
             * @param controlSimulation Whether to control the simulation or let it run automatically.
             */
            virtual void simulate( real_Num elapsedTime, void *scratchMemBlock, u32 scratchMemBlockSize,
                                   bool controlSimulation ) = 0;

            /**
             * @brief Fetches the simulation results.
             * @param block Whether to block until the simulation results are available.
             * @param errorState A pointer to an error state variable to be set if there was an error.
             * @return True if the results were fetched successfully, false otherwise.
             */
            virtual bool fetchResults( bool block, u32 *errorState ) = 0;

            /**
             * @brief Gets the state object.
             * @return The state object. Can be null.
             */
            virtual SmartPtr<IStateContext> getStateContext() const = 0;

            /**
             * @brief Sets the state object.
             * @param stateContext The state object. Can be null.
             */
            virtual void setStateContext( SmartPtr<IStateContext> stateContext ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsWorld3_h__
