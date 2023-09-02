#ifndef __IApplication_h__
#define __IApplication_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    namespace core
    {

        /**
         * Interface for an application class.
         *
         * This class is used as a base class for implementing a game or application.
         *
         * @author Zane Desir
         * @version 1.0
         */
        class IApplication : public ISharedObject
        {
        public:
            /**
             * Virtual destructor.
             */
            ~IApplication() override = default;

            /**
             * Starts the main loop of the application.
             *
             * This method should be implemented to initialize the application and start the main game loop. The loop should
             * run continuously until the application is closed or exited.
             */
            virtual void run() = 0;

            /**
             * Gets the finite state machine used by the application.
             *
             * This method returns a shared pointer to the FSM used by the application. The FSM is used to control the flow of
             * the application and manage the game states.
             *
             * @return A shared pointer to the FSM used by the application.
             */
            virtual SmartPtr<IFSM> getFSM() const = 0;

            /**
             * Sets the finite state machine used by the application.
             *
             * This method sets the FSM used by the application to the specified FSM.
             *
             * @param fsm The new FSM to use for the application.
             */
            virtual void setFSM( SmartPtr<IFSM> fsm ) = 0;

            virtual SmartPtr<scene::IActor> createDefaultCubemap( bool addToScene = true ) = 0;
            virtual SmartPtr<scene::IActor> createDefaultSky( bool addToScene = true ) = 0;

            virtual SmartPtr<scene::IActor> createDefaultCamera( bool addToScene = true ) = 0;

            virtual SmartPtr<scene::IActor> createDefaultCube( bool addToScene = true ) = 0;
            virtual SmartPtr<scene::IActor> createDefaultCubeMesh( bool addToScene = true ) = 0;

            virtual SmartPtr<scene::IActor> createDefaultGround( bool addToScene = true ) = 0;

            virtual SmartPtr<scene::IActor> createDefaultTerrain( bool addToScene = true ) = 0;

            virtual SmartPtr<scene::IActor> createDefaultConstraint() = 0;

            virtual SmartPtr<scene::IActor> createDirectionalLight( bool addToScene = true ) = 0;
            virtual SmartPtr<scene::IActor> createPointLight( bool addToScene = true ) = 0;

            virtual SmartPtr<scene::IActor> createDefaultPlane( bool addToScene = true ) = 0;

            virtual SmartPtr<scene::IActor> createDefaultVehicle( bool addToScene = true ) = 0;
            virtual SmartPtr<scene::IActor> createDefaultCar( bool addToScene = true ) = 0;
            virtual SmartPtr<scene::IActor> createDefaultTruck( bool addToScene = true ) = 0;
            virtual SmartPtr<scene::IActor> createVehicleFromDatabase( s32 id ) = 0;

            virtual SmartPtr<scene::IActor> createDefaultParticleSystem( bool addToScene = true ) = 0;

            virtual SmartPtr<render::IMaterial> createDefaultMaterial() = 0;
            virtual void createDefaultMaterials() = 0;

            virtual void createRigidStaticMesh( SmartPtr<scene::IActor> actor, bool recursive ) = 0;
            virtual void createRigidDynamicMesh( SmartPtr<scene::IActor> actor, bool recursive ) = 0;

            virtual IResource::ResourceType getResourceType( const String &resourceTypeName ) = 0;

            virtual String getResourceTypeByName( IResource::ResourceType resourceType ) = 0;

            virtual SmartPtr<Properties> importScene( const String &filePath ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace core
}  // end namespace fb

#endif  // __IApplication_h__
