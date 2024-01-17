#ifndef __CameraManager_H
#define __CameraManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/ICameraManager.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Thread/RecursiveMutex.h>

namespace fb
{
    namespace scene
    {
        /** Implementation of a camera manager. */
        class CameraManager : public ICameraManager
        {
        public:
            /** Constructor. */
            CameraManager();

            /** Constructor. */
            explicit CameraManager( u32 id );

            /** Destructor. */
            ~CameraManager() override;

            /** @copydoc ICameraManager::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ICameraManager::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ICameraManager::update */
            void update() override;

            /** @copydoc ICameraManager::addCamera */
            void addCamera( SmartPtr<IActor> camera ) override;

            /** @copydoc ICameraManager::removeCamera */
            bool removeCamera( SmartPtr<IActor> camera ) override;

            /** @copydoc ICameraManager::findCamera */
            SmartPtr<IActor> findCamera( const String &name ) const override;

            /** @copydoc ICameraManager::getCameras */
            Array<SmartPtr<IActor>> getCameras() const override;

            /** @copydoc ICameraManager::reset */
            void reset() override;

            /** @copydoc ICameraManager::getEditorTexture */
            SmartPtr<render::ITexture> getEditorTexture() const override;

            /** @copydoc ICameraManager::setEditorTexture */
            void setEditorTexture( SmartPtr<render::ITexture> editorTexture ) override;

            /** @copydoc ICameraManager::getEditorCamera */
            SmartPtr<IActor> getEditorCamera() const override;

            /** @copydoc ICameraManager::setEditorCamera */
            void setEditorCamera( SmartPtr<IActor> editorCamera ) override;

            /** @copydoc ICameraManager::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc ICameraManager::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc ICameraManager::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            FB_CLASS_REGISTER_DECL;

        private:
            /** The editor texture */
            SmartPtr<render::ITexture> m_editorTexture;

            /** The editor camera. */
            SmartPtr<IActor> m_editorCamera;

            /** The scene cameras. */
            Array<SmartPtr<IActor>> m_cameras;

            /** The camera manager mutex. */
            mutable RecursiveMutex m_mutex;
        };
    } // namespace scene
}     // namespace fb

#endif
