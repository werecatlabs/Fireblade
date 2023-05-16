#ifndef _IViewport_H
#define _IViewport_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/ColourUtil.h>
#include <FBCore/Base/ColourF.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace render
    {
        /**
         * Interface for a viewport that represents a region on a render target. A viewport is essentially a
         * camera view into the 3D world, and a region on the screen where the rendered scene is displayed.
         * This class is an extension of the `ISharedObject` interface.
         */
        class IViewport : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IViewport() override = default;

            /**
             * Sets the camera to use for rendering to this viewport.
             * @param camera The camera to use for rendering to this viewport.
             */
            virtual void setCamera( SmartPtr<ICamera> camera ) = 0;

            /**
             * Retrieves a pointer to the camera for this viewport.
             * @return A pointer to the camera for this viewport.
             */
            virtual SmartPtr<ICamera> getCamera() const = 0;

            /**
             * Gets the Z-Order of this viewport, which is used to determine the order in which
             * multiple viewports are rendered.
             * @return The Z-Order of this viewport.
             */
            virtual s32 getZOrder() const = 0;

            /**
             * Sets the Z-Order of this viewport, which is used to determine the order in which
             * multiple viewports are rendered.
             * @param zorder The Z-Order to set.
             */
            virtual void setZOrder( s32 zorder ) = 0;

            /**
             * Gets the position of the viewport, which is expressed as a value between 0.0 and 1.0.
             * @return The position of the viewport.
             */
            virtual Vector2F getPosition() const = 0;

            /**
             * Sets the position of the viewport, which is expressed as a value between 0.0 and 1.0.
             * @param position The position to set.
             */
            virtual void setPosition( const Vector2F &position ) = 0;

            /**
             * Gets the size of the viewport, which is expressed as a value between 0.0 and 1.0.
             * @return The size of the viewport.
             */
            virtual Vector2F getSize() const = 0;

            /**
             * Sets the size of the viewport, which is expressed as a value between 0.0 and 1.0.
             * @param size The size to set.
             */
            virtual void setSize( const Vector2F &size ) = 0;

            /**
             * Gets one of the relative dimensions of the viewport, which is expressed as a value between 0.0 and 1.0.
             * @return The left value of the viewport.
             */
            virtual f32 getLeft() const = 0;

            /**
             * Gets one of the relative dimensions of the viewport, which is expressed as a value between 0.0 and 1.0.
             * @return The top value of the viewport.
             */
            virtual f32 getTop() const = 0;

            /**
             * Gets one of the relative dimensions of the viewport, which is expressed as a value between 0.0 and 1.0.
             * @return The width value of the viewport.
             */
            virtual f32 getWidth() const = 0;

            /** Gets one of the relative dimensions of the viewport, a value
            between 0.0 and 1.0. */
            virtual f32 getHeight() const = 0;

            /**
             * Gets the actual pixel value of the left-most position of the viewport on the render target.
             *
             * @return An integer representing the actual pixel value of the left-most position of the viewport.
             */
            virtual s32 getActualLeft() const = 0;

            /**
             * Gets the actual pixel value of the top-most position of the viewport on the render target.
             *
             * @return An integer representing the actual pixel value of the top-most position of the viewport.
             */
            virtual s32 getActualTop() const = 0;

            /**
             * Gets the actual pixel value of the width of the viewport.
             *
             * @return An integer representing the actual pixel value of the width of the viewport.
             */
            virtual s32 getActualWidth() const = 0;

            /**
             * Gets the actual pixel value of the height of the viewport.
             *
             * @return An integer representing the actual pixel value of the height of the viewport.
             */
            virtual s32 getActualHeight() const = 0;

            /**
             * Sets the dimensions of the viewport after its creation. The new dimensions are specified using
             * a left offset, top offset, width and height.
             *
             * @param left The left-most offset of the viewport on the render target in normalized device coordinates.
             * @param top The top-most offset of the viewport on the render target in normalized device coordinates.
             * @param width The width of the viewport in normalized device coordinates.
             * @param height The height of the viewport in normalized device coordinates.
             */
            virtual void setDimensions( f32 left, f32 top, f32 width, f32 height ) = 0;

            /**
             * Sets the initial background colour of the viewport (before rendering).
             *
             * @param colour The new background colour of the viewport.
             */
            virtual void setBackgroundColour( const ColourF &colour ) = 0;

            /**
             * Gets the background colour of the viewport.
             *
             * @return The background colour of the viewport.
             */
            virtual ColourF getBackgroundColour() const = 0;

            /**
             * Determines whether to clear the viewport before rendering.
             *
             * @param clear Whether to clear the viewport.
             * @param buffers Which buffers to clear (colour, depth, stencil).
             */
            virtual void setClearEveryFrame( bool clear, u32 buffers = IGraphicsScene::FBT_COLOUR |
                                                 IGraphicsScene::FBT_DEPTH ) = 0;

            /**
             * Determines if the viewport is cleared before every frame.
             *
             * @return True if the viewport is cleared before every frame; false otherwise.
             */
            virtual bool getClearEveryFrame() const = 0;

            /**
             * Gets which buffers are to be cleared each frame.
             *
             * @return Which buffers are to be cleared each frame.
             */
            virtual u32 getClearBuffers() const = 0;

            /** Set the material scheme which the viewport should use.
             *  @param schemeName The name of the material scheme to set.
             */
            virtual void setMaterialScheme( const String &schemeName ) = 0;

            /** Get the material scheme which the viewport should use.
             *  @return The name of the material scheme currently in use.
             */
            virtual String getMaterialScheme() const = 0;

            /** Access to actual dimensions (based on target size).
             *  @param left Output parameter to store the left coordinate of the viewport.
             *  @param top Output parameter to store the top coordinate of the viewport.
             *  @param width Output parameter to store the width of the viewport.
             *  @param height Output parameter to store the height of the viewport.
             */
            virtual void getActualDimensions( s32 &left, s32 &top, s32 &width, s32 &height ) const = 0;

            /** To know if the viewport is updated.
             *  @return True if the viewport has been updated, false otherwise.
             */
            virtual bool _isUpdated() const = 0;

            /** Clears the updated flag. */
            virtual void _clearUpdatedFlag() = 0;

            /** Gets the number of rendered faces in the last update.
             *  @return The number of faces rendered in the last update.
             */
            virtual u32 _getNumRenderedFaces() const = 0;

            /** Gets the number of rendered batches in the last update.
             *  @return The number of batches rendered in the last update.
             */
            virtual u32 _getNumRenderedBatches() const = 0;

            /** Sets if the scene should be rendered. */
            virtual void setRenderSceneEnabled( bool enabled ) = 0;

            /** Gets if the scene should be rendered. */
            virtual bool getRenderSceneEnabled() const = 0;

            /** To know if overlay objects should be rendered. */
            virtual void setOverlaysEnabled( bool enabled ) = 0;

            /** Gets if overlay objects should be rendered. */
            virtual bool getOverlaysEnabled() const = 0;

            /** Sets if ui objects should be rendered. */
            virtual void setEnableUI( bool enabled ) = 0;

            /** Gets if ui objects should be rendered. */
            virtual bool getEnableUI() const = 0;

            /** Tells this viewport whether it should display skies. */
            virtual void setSkiesEnabled( bool enabled ) = 0;

            /** Returns whether or not skies (created in the SceneManager) are displayed in this
            viewport. */
            virtual bool getSkiesEnabled() const = 0;

            /** Tells this viewport whether it should display shadows. */
            virtual void setShadowsEnabled( bool enabled ) = 0;

            /** Returns whether or not shadows (defined in the SceneManager) are displayed in this
            viewport. */
            virtual bool getShadowsEnabled() const = 0;

            /** Sets a per-viewport visibility mask. */
            virtual void setVisibilityMask( u32 mask ) = 0;

            /** Gets a per-viewport visibility mask. */
            virtual u32 getVisibilityMask() const = 0;

            /** Sets whether this viewport should be automatically updated
                if Ogre's rendering loop or RenderTarget::update is being used.
            */
            virtual void setAutoUpdated( bool autoupdate ) = 0;

            /** Gets whether this viewport is automatically updated if
                Ogre's rendering loop or RenderTarget::update is being used.
            */
            virtual bool isAutoUpdated() const = 0;

            /** Gets a pointer to the underlying object.
             */
            virtual void _getObject( void **ppObject ) const = 0;

            /** Gets the background texture.
             */
            virtual SmartPtr<ITexture> getBackgroundTexture() const = 0;

            /** Sets the background texture.
             */
            virtual void setBackgroundTexture( SmartPtr<ITexture> texture ) = 0;

            /** Gets the background texture name.
             */
            virtual String getBackgroundTextureName() const = 0;

            /** Sets the background texture name.
             */
            virtual void setBackgroundTextureName( const String &textureName ) = 0;

            /** Gets the render target.
             */
            virtual SmartPtr<IRenderTarget> getRenderTarget() const = 0;

            /** Sets the render target.
             */
            virtual void setRenderTarget( SmartPtr<IRenderTarget> renderTarget ) = 0;

            /** Gets if the viewport is active.
             */
            virtual bool isActive() const = 0;

            /** Sets if the viewport is active.
             */
            virtual void setActive( bool active ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    } // end namespace render
}     // end namespace fb

#endif
