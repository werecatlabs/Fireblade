#ifndef IGraphicsListener_h__
#define IGraphicsListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    namespace render
    {
        /**
         * The base interface for the graphics system.
         */
        class IGraphicsSystem : public ISharedObject
        {
        public:
            /**
             * The graphics rendering APIs.
             */
            enum class RenderApi
            {
                None,     ///< No API
                DX9,      ///< DirectX 9
                DX11,     ///< DirectX 11
                DX12,     ///< DirectX 12
                GL,       ///< OpenGL
                GL3Plus,  ///< OpenGL 3.0+
                Vulkan,   ///< Vulkan
                Metal,    ///< Apple Metal

                Count  ///< Number of render APIs
            };

            ~IGraphicsSystem() override = default;

            /**
             * Creates a configuration object.
             *
             * @return A new graphics configuration object.
             */
            virtual SmartPtr<IGraphicsSettings> createConfiguration() = 0;

            /**
             * Configures the graphics system with the specified settings.
             *
             * @param config The graphics configuration settings.
             * @return True if the configuration was successful, false otherwise.
             */
            virtual bool configure( SmartPtr<IGraphicsSettings> config ) = 0;

            /**
             * Processes messages from the system message queue.
             */
            virtual void messagePump() = 0;

            /**
             * Gets the debug interface.
             *
             * @return A pointer to the debug interface.
             */
            virtual SmartPtr<IDebug> getDebug() const = 0;

            /**
             * Sets the debug interface.
             *
             * @param debug A pointer to the debug interface.
             */
            virtual void setDebug( SmartPtr<IDebug> debug ) = 0;

            /**
             * Adds a scene manager of the specified type and name to the graphics system.
             *
             * @param type The type of the scene manager to create.
             * @param name The name of the scene manager to create.
             * @return A pointer to the newly created scene manager.
             */
            virtual SmartPtr<IGraphicsScene> addGraphicsScene( const String &type,
                                                             const String &name ) = 0;

            /**
             * Gets the default scene manager.
             *
             * @return A pointer to the default scene manager.
             */
            virtual SmartPtr<IGraphicsScene> getGraphicsScene() const = 0;

            /**
             * Gets a scene manager by name.
             *
             * @param name The name of the scene manager to retrieve.
             * @return A pointer to the specified scene manager, or nullptr if not found.
             */
            virtual SmartPtr<IGraphicsScene> getGraphicsScene( const String &name ) const = 0;

            /**
             * Gets a scene manager by ID.
             *
             * @param id The ID of the scene manager to retrieve.
             * @return A pointer to the specified scene manager, or nullptr if not found.
             */
            virtual SmartPtr<IGraphicsScene> getGraphicsSceneById( hash32 id ) const = 0;

            /** Gets the scene managers. */
            virtual Array<SmartPtr<IGraphicsScene>> getSceneManagers() const = 0;

            /**
             * Gets the overlay manager.
             *
             * @return A pointer to the overlay manager.
             */
            virtual SmartPtr<IOverlayManager> getOverlayManager() const = 0;

            /**
             * Gets the resource group manager.
             *
             * @return A pointer to the resource group manager.
             */
            virtual SmartPtr<IResourceGroupManager> getResourceGroupManager() const = 0;

            /**
             * Gets the material manager.
             *
             * @return A pointer to the material manager.
             */
            virtual SmartPtr<IMaterialManager> getMaterialManager() const = 0;

            /**
             * Gets the texture manager.
             *
             * @return A pointer to the texture manager.
             */
            virtual SmartPtr<ITextureManager> getTextureManager() const = 0;

            /** Gets the instance manager. */
            virtual SmartPtr<IInstanceManager> getInstanceManager() const = 0;

            /** Gets the sprite renderer. */
            virtual SmartPtr<ISpriteRenderer> getSpriteRenderer() const = 0;

            /** Gets the font manager. */
            virtual SmartPtr<IFontManager> getFontManager() const = 0;

            /** Creates a render window. */
            virtual SmartPtr<IWindow> createRenderWindow( const String &name, u32 width, u32 height,
                                                          bool fullScreen,
                                                          const SmartPtr<Properties> &properties ) = 0;

            /** Gets a render window. */
            virtual SmartPtr<IWindow> getRenderWindow(
                const String &name = StringUtil::EmptyString ) const = 0;

            /** Gets the default render window. */
            virtual SmartPtr<IWindow> getDefaultWindow() const = 0;

            /** Gets the default render window. */
            virtual void setDefaultWindow( SmartPtr<IWindow> defaultWindow ) = 0;

            /** Gets the windows. */
            virtual Array<SmartPtr<IWindow>> getWindows() const = 0;

            /** Adds a deferred shading system to a viewport. */
            virtual SmartPtr<IDeferredShadingSystem> addDeferredShadingSystem(
                SmartPtr<IViewport> vp ) = 0;

            /** Removes a deferred shading system from a viewport. */
            virtual void removeDeferredShadingSystem( SmartPtr<IViewport> vp ) = 0;

            /** Gets the deferred shading systems. */
            virtual Array<SmartPtr<IDeferredShadingSystem>> getDeferredShadingSystems() const = 0;

            /** Loads an object via the graphics system.
            @param graphicsObject The object to be loaded.
            @param forceQueue Forces the object to be queued for deferred loading.
            */
            virtual void loadObject( SmartPtr<ISharedObject> graphicsObject,
                                     bool forceQueue = false ) = 0;

            /** Unloads an object via the graphics system.
            @param graphicsObject The object to be unloaded.
            @param forceQueue Forces the object to be queued for deferred unloading.
            */
            virtual void unloadObject( SmartPtr<ISharedObject> graphicsObject,
                                       bool forceQueue = false ) = 0;

            /** Sets up the renderer. */
            virtual void setupRenderer( SmartPtr<IGraphicsScene> sceneManager, SmartPtr<IWindow> window,
                                        SmartPtr<ICamera> camera, String workspaceName, bool val ) = 0;

            /** The task used to render. */
            virtual Thread::Task getStateTask() const = 0;

            /** The current task used to render. */
            virtual Thread::Task getRenderTask() const = 0;

            /**
             * Gets the mesh converter used by this script manager.
             *
             * @return Smart pointer to the mesh converter.
             */
            virtual SmartPtr<IMeshConverter> getMeshConverter() const = 0;

            /**
             * Sets the mesh converter used by this script manager.
             *
             * @param meshConverter Smart pointer to the mesh converter to be set.
             */
            virtual void setMeshConverter( SmartPtr<IMeshConverter> meshConverter ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace render
}  // namespace fb

#endif  // IGraphicsListener_h__
