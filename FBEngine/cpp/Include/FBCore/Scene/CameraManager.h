#ifndef _CameraManager_H
#define _CameraManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/ICameraManager.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Thread/RecursiveMutex.h>

namespace fb
{
    namespace scene
    {
        /** Implementation of the camera manager. */
        class CameraManager : public ICameraManager
        {
        public:
            CameraManager();
            explicit CameraManager( u32 id );
            ~CameraManager() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            void addCamera( SmartPtr<IActor> camera ) override;
            bool removeCamera( SmartPtr<IActor> camera ) override;

            SmartPtr<IActor> findCamera( const String &name ) const override;

            Array<SmartPtr<IActor>> getCameras() const override;

            /** @copydoc ICameraManager::play */
            void play() override;

            /** @copydoc ICameraManager::edit */
            void edit() override;

            /** @copydoc ICameraManager::stop */
            void stop() override;

            /** @copydoc ICameraManager::reset */
            void reset() override;

            /** @copydoc ICameraManager::getEditorTexture */
            SmartPtr<render::ITexture> getEditorTexture() const override;

            /** @copydoc ICameraManager::setEditorTexture */
            void setEditorTexture( SmartPtr<render::ITexture> editorTexture ) override;

            SmartPtr<IActor> getEditorCamera() const;

            void setEditorCamera( SmartPtr<IActor> editorCamera );

            /** @copydoc CGraphicsObjectOgreNext<ICamera>::getProperties */
            SmartPtr<Properties> getProperties() const;

            /** @copydoc CGraphicsObjectOgreNext<ICamera>::setProperties */
            void setProperties( SmartPtr<Properties> properties );

            /** @copydoc CGraphicsObjectOgreNext<ICamera>::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const;

            FB_CLASS_REGISTER_DECL;

        private:
            /** The editor texture */
            SmartPtr<render::ITexture> m_editorTexture;

            SmartPtr<IActor> m_editorCamera;

            /** The scene cameras. */
            Array<SmartPtr<IActor>> m_cameras;

            /** The camera manager mutex. */
            mutable RecursiveMutex m_mutex;
        };
    }  // namespace scene
}  // namespace fb

#endif
