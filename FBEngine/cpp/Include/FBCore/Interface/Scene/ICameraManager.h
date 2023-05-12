#ifndef _ICameraControllerMgr_H
#define _ICameraControllerMgr_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace scene
    {

        /**
         * @brief A class to manage camera controllers.
         */
        class ICameraManager : public ISharedObject
        {
        public:
            /**
             * @brief The possible states of the camera manager.
             */
            enum class State
            {
                None,
                Edit,
                Play,
                Reset,

                Count
            };

            /**
             * @brief The reserved flag for cameras.
             */
            static const u32 CameraFlagReserved;

            /**
             * @brief Virtual destructor.
             */
            ~ICameraManager() override = default;

            /**
             * @brief Posts an input event to the camera manager.
             * @param cameraName The name of the camera.
             * @param event The input event to be posted.
             * @return True if the event was handled, false if not.
             */
            virtual bool postEvent( const String &cameraName, const SmartPtr<IInputEvent> &event ) = 0;

            /**
             * @brief Adds a camera to the camera manager.
             * @param camera The camera to add.
             */
            virtual void addCamera( SmartPtr<scene::IActor> camera ) = 0;

            /**
             * @brief Removes a camera from the camera manager.
             * @param camera The camera to remove.
             * @return True if the camera is successfully removed, false if not.
             */
            virtual bool removeCamera( SmartPtr<scene::IActor> camera ) = 0;

            /**
             * @brief Removes a camera from the camera manager.
             * @param name The name of the camera to remove.
             * @return True if the camera is successfully removed, false if not.
             */
            virtual bool removeCamera( const String &name ) = 0;

            /**
             * @brief Finds a camera with the given name.
             * @param name The name of the camera to find.
             * @return The camera object. Will return null if the camera is not found.
             */
            virtual SmartPtr<scene::IActor> findCamera( const String &name ) const = 0;

            /**
             * @brief Gets the current camera's name.
             * @return The name of the current camera.
             */
            virtual String getCurrentCameraName() const = 0;

            /**
             * @brief Sets the current camera.
             * @param camera The camera to set as the current camera.
             */
            virtual void setCurrentCamera( SmartPtr<scene::IActor> camera ) = 0;

            /**
             * @brief Gets the current camera.
             * @return The current camera.
             */
            virtual SmartPtr<scene::IActor> getCurrentCamera() const = 0;

            /**
             * @brief Gets a list of the cameras in the camera manager.
             * @return An array of cameras.
             */
            virtual Array<SmartPtr<scene::IActor>> getCameras() const = 0;

            /**
             * @brief Sets the value of a flag.
             * @param flag The flag to set.
             * @param value The value to set the flag to.
             */
            virtual void setFlag( u32 flag, bool value ) = 0;

            /**
             * @brief Gets the value of a flag.
             * @param flag The flag to get the value of.
             * @return The value of the flag.
             */
            virtual bool getFlag( u32 flag ) const = 0;

            /**
             * @brief Sets the settings of a camera that the camera manager contains.
             * @param name The name of the camera.
             * @param properties The properties to set.
             */
            virtual void setCameraSettings( const String &name, const Properties &properties ) = 0;

            /**
             * @brief Triggers the play event, transitioning the camera manager state to play mode.
             */
            virtual void play() = 0;

            /**
             * @brief Triggers the edit event, transitioning the camera manager state to edit mode.
             */
            virtual void edit() = 0;

            /**
             * @brief Triggers the stop event, stopping the current camera's movement.
             */
            virtual void stop() = 0;

            /**
             * @brief Triggers the reset event, resetting the current camera to its initial position.
             */
            virtual void reset() = 0;

            /**
             * @brief Gets the editor texture used by the camera manager.
             * @return The editor texture used by the camera manager.
             */
            virtual SmartPtr<render::ITexture> getEditorTexture() const = 0;

            /**
             * @brief Sets the editor texture used by the camera manager.
             * @param editorTexture The editor texture to use.
             */
            virtual void setEditorTexture( SmartPtr<render::ITexture> editorTexture ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // namespace fb

#endif
