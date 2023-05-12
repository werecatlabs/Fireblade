#ifndef _IRenderTarget_H
#define _IRenderTarget_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace render
    {

        /**
         * An interface for managing a render target, which is a destination for rendering operations.
         * This class is an extension of the ISharedObject interface.
         */
        class IRenderTarget : public ISharedObject
        {
        public:
            /**
             * An enumeration representing the frame buffer to be used for rendering operations.
             * - Front: Represents the front frame buffer.
             * - Back: Represents the back frame buffer.
             * - Auto: Represents automatic selection of the frame buffer.
             * - Count: Represents the number of items in the enumeration (not a valid value to be used).
             */
            enum class FrameBuffer
            {
                Front,
                Back,
                Auto,
                Count
            };

            /**
             * Struct containing statistics about the current render target.
             */
            struct RenderTargetStats
            {
                f32 lastFPS = 0.0f;      ///< The last measured FPS (frames per second).
                f32 avgFPS = 0.0f;       ///< The average FPS since the last reset.
                f32 bestFPS = 0.0f;      ///< The best (highest) FPS since the last reset.
                f32 worstFPS = 0.0f;     ///< The worst (lowest) FPS since the last reset.
                u32 bestFrameTime = 0;   ///< The best (shortest) frame time since the last reset.
                u32 worstFrameTime = 0;  ///< The worst (longest) frame time since the last reset.
                u32 triangleCount = 0;   ///< The number of triangles rendered since the last reset.
                u32 batchCount = 0;      ///< The number of batches rendered since the last reset.
            };

            /**
             * Struct containing information about a RenderTarget event.
             */
            struct RenderTargetEvent
            {
                IRenderTarget *source;  ///< The source of the event being raised.
            };

            /**
             * Struct containing information about a RenderTarget Viewport-specific event.
             */
            struct RenderTargetViewportEvent
            {
                IViewport *source;  ///< The source of the event being raised.
            };

            /**
             * An interface for classes that wish to be notified of changes to a render target.
             * This class is a nested class of the `IRenderTarget` interface and extends the `ISharedObject` interface.
             * Implementations of this interface can be registered with an `IRenderTarget` object to receive notifications.
             */
            class Listener : public ISharedObject
            {
            public:
                /**
                 * Virtual destructor for the `IRenderTarget::Listener` class, to ensure that destructors of derived classes are called correctly.
                 * The destructor is marked as `override` to ensure that it overrides the base class destructor.
                 */
                virtual ~Listener() override = default;

                /**
                 * Called before the render target is updated.
                 * @param evt A reference to a `RenderTargetEvent` object containing information about the event.
                 */
                virtual void preRenderTargetUpdate( const RenderTargetEvent &evt ) = 0;

                /**
                 * Called after the render target is updated.
                 * @param evt A reference to a `RenderTargetEvent` object containing information about the event.
                 */
                virtual void postRenderTargetUpdate( const RenderTargetEvent &evt ) = 0;

                /**
                 * Called before a viewport is updated.
                 * @param evt A reference to a `RenderTargetViewportEvent` object containing information about the event.
                 */
                virtual void preViewportUpdate( const RenderTargetViewportEvent &evt ) = 0;

                /**
                 * Called after a viewport is updated.
                 * @param evt A reference to a `RenderTargetViewportEvent` object containing information about the event.
                 */
                virtual void postViewportUpdate( const RenderTargetViewportEvent &evt ) = 0;

                /**
                 * Called when a viewport is added to the render target.
                 * @param evt A reference to a `RenderTargetViewportEvent` object containing information about the event.
                 */
                virtual void viewportAdded( const RenderTargetViewportEvent &evt ) = 0;

                /**
                 * Called when a viewport is removed from the render target.
                 * @param evt A reference to a `RenderTargetViewportEvent` object containing information about the event.
                 */
                virtual void viewportRemoved( const RenderTargetViewportEvent &evt ) = 0;
            };

            /**
             * Virtual destructor for the `IRenderTarget` class, to ensure that destructors of derived classes are called correctly.
             * The destructor is marked as `override` to ensure that it overrides the base class destructor.
             */
            virtual ~IRenderTarget() override = default;

            /**
             * Adds a listener for the events related to changes to the render target.
             * @param listener - a `SmartPtr` to the `Listener` object that will be added to the list of listeners.
             */
            virtual void addRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener ) = 0;

            /**
             * Removes a listener for the events related to changes to the render target.
             * @param listener - a `SmartPtr` to the `Listener` object that will be removed from the list of listeners.
             */
            virtual void removeRenderTargetListener( SmartPtr<IRenderTarget::Listener> listener ) = 0;

            /**
             * Gets the list of all the listeners for the events related to changes to the render target.
             * @return An `Array` of `SmartPtr` to the `Listener` objects that are currently registered.
             */
            virtual Array<SmartPtr<IRenderTarget::Listener>> getRenderTargetListeners() const = 0;

            /**
             * Swaps the buffers of the render target.
             */
            virtual void swapBuffers() = 0;

            /**
             * Sets the priority of the render target.
             * @param priority - an unsigned 8-bit integer that represents the priority to be set.
             */
            virtual void setPriority( u8 priority ) = 0;

            /**
             * Gets the priority of the render target.
             * @return An unsigned 8-bit integer that represents the priority of the render target.
             */
            virtual u8 getPriority() const = 0;

            /**
             * Gets the active state of the render target.
             * @return A boolean that represents whether or not the render target is currently active.
             */
            virtual bool isActive() const = 0;

            /**
             * Sets the active state of the render target.
             * @param state - a boolean that represents whether or not the render target should be set as active.
             */
            virtual void setActive( bool state ) = 0;

            /**
             * Sets whether or not this render target should be automatically updated.
             * @param autoupdate - a boolean that represents whether or not the render target should be automatically updated.
             */
            virtual void setAutoUpdated( bool autoupdate ) = 0;

            /**
             * Gets whether or not this render target is automatically updated.
             * @return A boolean that represents whether or not the render target is automatically updated.
             */
            virtual bool isAutoUpdated() const = 0;

            /** Copies the current contents of the render target.*/
            virtual void copyContentsToMemory( void *buffer, u32 size,
                                               FrameBuffer bufferId = FrameBuffer::Auto ) = 0;

            /** Gets the size of the render target. */
            virtual Vector2I getSize() const = 0;

            /** Sets the size of the render target. */
            virtual void setSize( const Vector2I &size ) = 0;

            /** Gets the colour depth of the render target. */
            virtual u32 getColourDepth() const = 0;

            /** Sets the colour depth of the render target. */
            virtual void setColourDepth( u32 colourDepth ) = 0;

            /** Adds a viewport to the render target. */
            virtual SmartPtr<IViewport> addViewport( hash32 id, SmartPtr<ICamera> camera,
                                                     s32 ZOrder = -1, f32 left = 0.0f, f32 top = 0.0f,
                                                     f32 width = 1.0f, f32 height = 1.0f ) = 0;

            /** Returns the number of viewports attached to this target.*/
            virtual u32 getNumViewports() const = 0;

            /** Retrieves a pointer to the viewport with the given index. */
            virtual SmartPtr<IViewport> getViewport( u32 index ) = 0;

            /** Retrieves a pointer to the viewport with the given index. */
            virtual SmartPtr<IViewport> getViewportById( hash32 id ) = 0;

            /** Retrieves a pointer to the viewport with the given zorder. */
            virtual SmartPtr<IViewport> getViewportByZOrder( s32 zorder ) const = 0;

            /** Retrieves a boolean if the render target has a viewport with the given zorder. */
            virtual bool hasViewportWithZOrder( s32 zorder ) const = 0;

            /** Gets the viewports. */
            virtual Array<SmartPtr<IViewport>> getViewports() const = 0;

            /** Removes a viewport.
             */
            virtual void removeViewport( SmartPtr<IViewport> vp ) = 0;

            /** Removes all viewports on this target.
             */
            virtual void removeAllViewports() = 0;

            /** Gets the render target statistics. */
            virtual RenderTargetStats getRenderTargetStats() const = 0;

            /**
             * Gets a pointer to the underlying graphics object used by this class.
             * The type of the object returned is dependent on the graphics library used.
             * @param ppObject - a pointer to a void pointer which will be set to the underlying object
             */
            virtual void _getObject( void **ppObject ) const = 0;

            /**
             * Returns the texture used by this object.
             * @return a SmartPtr<ITexture> object representing the texture used by this object
             */
            virtual SmartPtr<ITexture> getTexture() const = 0;

            /**
             * Sets the texture that will be used by this object.
             * @param texture - a pointer object representing the texture to be set
             */
            virtual void setTexture( SmartPtr<ITexture> texture ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif
