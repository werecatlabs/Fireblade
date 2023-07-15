#ifndef _CameraManager_H
#define _CameraManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/ICameraManager.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Thread/RecursiveMutex.h>

namespace fb
{
    namespace scene
    {
        class CameraManager : public SharedObject<ICameraManager>
        {
        public:
            CameraManager() = default;
            CameraManager( u32 id );
            ~CameraManager() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            bool postEvent( const String &cameraName, const SmartPtr<IInputEvent> &event ) override;

            void addCamera( SmartPtr<IActor> camera ) override;
            bool removeCamera( SmartPtr<IActor> camera ) override;
            bool removeCamera( const String &name ) override;

            SmartPtr<IActor> findCamera( const String &name ) const override;
            void setCurrentCamera( const String &name );
            void setCurrentCamera( SmartPtr<IActor> camera ) override;
            String getCurrentCameraName() const override;
            SmartPtr<IActor> getCurrentCamera() const override;

            Array<SmartPtr<IActor>> getCameras() const override;

            void setFlag( u32 flag, bool value ) override;
            bool getFlag( u32 flag ) const override;

            void setCameraSettings( const String &name, const Properties &properties ) override;

            /** @copydoc ICameraManager::play */
            void play() override;

            /** @copydoc ICameraManager::edit */
            void edit() override;

            /** @copydoc ICameraManager::stop */
            void stop() override;

            /** @copydoc ICameraManager::reset */
            void reset() override;

            SmartPtr<render::ITexture> getEditorTexture() const override;

            void setEditorTexture( SmartPtr<render::ITexture> editorTexture ) override;

        private:
            WeakPtr<render::ITexture> m_editorTexture;

            RecursiveMutex m_camerasMutex;
            Array<SmartPtr<IActor>> m_cameras;
            Array<SmartPtr<IActor>> m_sceneCameras;

            AtomicSmartPtr<IActor> m_selectedCameraCtrl;
        };
    } // namespace scene
}     // namespace fb

#endif
