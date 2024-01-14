#ifndef SkyboxComponent_h__
#define SkyboxComponent_h__

#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace scene
    {

        /**
         * @brief Skybox component.
         *
         * This class represents a skybox component that can be attached to a game object in the scene.
         * A skybox is used to represent the background/environment surrounding the scene and is often used in 3D rendering.
         * The skybox can be loaded with multiple textures representing different faces of the environment.
         */
        class Skybox : public Component
        {
        public:
            /**
             * @brief Default constructor for Skybox.
             */
            Skybox();

            /**
             * @brief Destructor for Skybox.
             */
            ~Skybox() override;

            /**
             * @brief Loads data for the Skybox component.
             *
             * This function is part of the IObject interface.
             * It loads the shared object data associated with the Skybox.
             *
             * @param data The shared object data to be loaded.
             */
            void load( SmartPtr<ISharedObject> data ) override;

            /**
             * @brief Unloads data for the Skybox component.
             *
             * This function is part of the IObject interface.
             * It unloads the shared object data associated with the Skybox.
             *
             * @param data The shared object data to be unloaded.
             */
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * @brief Updates the Skybox component based on dirty flags.
             *
             * This function is part of the Component class and gets called when the component is marked dirty.
             * The flags parameter specifies the types of changes that occurred, and oldFlags represent the previous state.
             *
             * @param flags The flags indicating the changes that occurred.
             * @param oldFlags The previous state's flags.
             */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /**
             * @brief Get the properties associated with the Skybox component.
             *
             * @return A smart pointer to the properties of the Skybox component.
             */
            SmartPtr<Properties> getProperties() const override;

            /**
             * @brief Set the properties for the Skybox component.
             *
             * @param properties A smart pointer to the properties to be set for the Skybox component.
             */
            void setProperties( SmartPtr<Properties> properties ) override;

            /**
             * @brief Get the material used by the Skybox component.
             *
             * @return A smart pointer to the material used by the Skybox component.
             */
            SmartPtr<render::IMaterial> getMaterial() const;

            /**
             * @brief Get the distance of the Skybox from the camera.
             *
             * @return The distance of the Skybox from the camera in floating-point value.
             */
            f32 getDistance() const;

            /**
             * @brief Set the distance of the Skybox from the camera.
             *
             * @param distance The distance of the Skybox from the camera in floating-point value.
             */
            void setDistance( f32 distance );

            /**
             * @brief Get the array of textures used by the Skybox.
             *
             * @return An array of smart pointers to textures used by the Skybox.
             */
            Array<SmartPtr<render::ITexture>> getTextures() const;

            /**
             * @brief Set the array of textures used by the Skybox.
             *
             * @param textures An array of smart pointers to textures to be used by the Skybox.
             */
            void setTextures( Array<SmartPtr<render::ITexture>> textures );

            /**
             * @brief Get the texture at a specific index in the Skybox.
             *
             * @param index The index of the texture to retrieve.
             * @return A smart pointer to the texture at the specified index.
             */
            SmartPtr<render::ITexture> getTexture( u8 index ) const;

            /**
             * @brief Set the texture at a specific index in the Skybox.
             *
             * @param index The index of the texture to set.
             * @param texture A smart pointer to the texture to be set at the specified index.
             */
            void setTexture( SmartPtr<render::ITexture> texture, u8 index );

            /**
             * @brief Set the texture at a specific index in the Skybox.
             */
            void setTextureByName( const String &textureName, u8 index );

            /** @copydoc Component::updateMaterials */
            void updateMaterials() override;

            /**
            @brief Get the swap left and right flag.
            @return True if the left and right textures are swapped, false otherwise.
            */
            bool getSwapLeftRight() const;

            /**
            @brief Set the swap left and right flag.
            */
            void setSwapLeftRight( bool swapLeftRight );

            /**
             * @brief Declaration of class registration for the Skybox component.
             */
            FB_CLASS_REGISTER_DECL;

        protected:
            /**
             * @brief Listener class for handling material shared events.
             *
             * This class listens to events related to the material used by the Skybox component.
             * It implements the IEventListener interface to handle such events.
             */
            class MaterialSharedListener : public IEventListener
            {
            public:
                /**
                 * @brief Default constructor for MaterialSharedListener.
                 */
                MaterialSharedListener() = default;

                /**
                 * @brief Destructor for MaterialSharedListener.
                 */
                ~MaterialSharedListener() override = default;

                /**
                 * @brief Handles an event received by the listener.
                 *
                 * @param eventType The type of the event received.
                 * @param eventValue The value associated with the event.
                 * @param arguments An array of parameters associated with the event.
                 * @param sender The shared object that sent the event.
                 * @param object The shared object associated with the event.
                 * @param event The event object itself.
                 * @return The result of handling the event.
                 */
                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                /**
                 * @brief Handles the loading state change event for the shared object.
                 *
                 * @param sharedObject The shared object whose loading state changed.
                 * @param oldState The previous loading state.
                 * @param newState The new loading state.
                 */
                void loadingStateChanged( ISharedObject *sharedObject, LoadingState oldState,
                                          LoadingState newState );

                /**
                 * @brief Destroys the object pointed to by the given pointer.
                 *
                 * @param ptr A pointer to the object to be destroyed.
                 * @return True if the object was successfully destroyed, false otherwise.
                 */
                bool destroy( void *ptr );

                /**
                 * @brief Get the owner of the MaterialSharedListener.
                 *
                 * @return A smart pointer to the owner Skybox component.
                 */
                SmartPtr<Skybox> getOwner() const;

                /**
                 * @brief Set the owner of the MaterialSharedListener.
                 *
                 * @param owner A smart pointer to the Skybox component that owns this listener.
                 */
                void setOwner( SmartPtr<Skybox> owner );

            private:
                WeakPtr<Skybox> m_owner = nullptr;
            };

            /**
             * @brief Handles component events for the Skybox component.
             *
             * This function is part of the Component class and is called when an event specific to the component is triggered.
             * It handles the events based on the current state of the component's finite state machine.
             *
             * @param state The current state of the finite state machine for the component
             */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            /**
             * @brief Sets up the material used by the Skybox component.
             */
            void setupMaterial();

            /**
             * @brief A mutex used to provide thread-safety for the Skybox component.
             *
             * This mutable recursive mutex is used to protect critical sections of code
             * that may be accessed concurrently by multiple threads. The 'mutable' keyword
             * allows the mutex to be modified even in const member functions, as it does
             * not affect the logical state of the object.
             */
            mutable RecursiveMutex m_mutex;

            /**
             * @brief An array of smart pointers to textures used by the Skybox.
             *
             * This array contains smart pointers to render::ITexture objects representing
             * the textures used by the Skybox component. The textures are used to render
             * the different faces of the skybox that surround the scene.
             */
            Array<SmartPtr<render::ITexture>> m_textures;

            /**
             * @brief A smart pointer to the MaterialSharedListener for the Skybox component.
             *
             * This smart pointer points to the MaterialSharedListener object associated with
             * the Skybox component. The MaterialSharedListener is responsible for handling events
             * related to the material used by the Skybox, such as loading state changes.
             */
            SmartPtr<MaterialSharedListener> m_materialSharedListener;

            /**
             * @brief A smart pointer to the material used by the Skybox component.
             *
             * This smart pointer points to the render::IMaterial object representing the material
             * used by the Skybox component. The material defines the visual appearance and properties
             * of the Skybox, including how the textures are blended and rendered in the scene.
             */
            SmartPtr<render::IMaterial> m_material;

            /**
             * @brief The distance of the Skybox from the camera in the scene.
             *
             * This floating-point variable represents the distance between the Skybox and the camera
             * in the scene. It is typically used to control the rendering order and depth of the Skybox,
             * ensuring that it appears in the background of the scene and does not intersect with other
             * objects at close distances.
             * The default value is 50000.0 units.
             */
            f32 m_distance = 50000.0f;

            // If true, swap left and right textures
            bool m_swapLeftRight = false;
        };
    }  // namespace scene
}  // namespace fb

#endif  // SkyboxComponent_h__
