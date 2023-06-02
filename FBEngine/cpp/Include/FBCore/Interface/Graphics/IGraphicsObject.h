#ifndef _FB_IGraphicsObject_H_
#define _FB_IGraphicsObject_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    namespace render
    {
        
        /** Interface for a class that can be attached to a scene node.
         */
        class IGraphicsObject : public ISharedObject
        {
        public:
            enum class RenderQueueGroupID
            {
                /// Use this queue for objects which must be rendered first e.g. backgrounds
                RENDER_QUEUE_BACKGROUND = 0,
                /// First queue (after backgrounds), used for skyboxes if rendered first
                RENDER_QUEUE_SKIES_EARLY = 5,
                RENDER_QUEUE_1 = 10,
                RENDER_QUEUE_2 = 20,
                RENDER_QUEUE_WORLD_GEOMETRY_1 = 25,
                RENDER_QUEUE_3 = 30,
                RENDER_QUEUE_4 = 40,
                /// The default render queue
                RENDER_QUEUE_MAIN = 50,
                RENDER_QUEUE_6 = 60,
                RENDER_QUEUE_7 = 70,
                RENDER_QUEUE_WORLD_GEOMETRY_2 = 75,
                RENDER_QUEUE_8 = 80,
                RENDER_QUEUE_9 = 90,
                /// Penultimate queue(before overlays), used for skyboxes if rendered last
                RENDER_QUEUE_SKIES_LATE = 95,
                /// Use this queue for objects which must be rendered last e.g. overlays
                RENDER_QUEUE_OVERLAY = 100,
                /// Final possible render queue, don't exceed this
                RENDER_QUEUE_MAX = 105,
                RENDER_QUEUE_COUNT
            };

            /// To get all the properties of a GraphicsObject.
            static const u32 AllProperties; /* 0x01*/

            static const u32 OverlayFlag;
            static const u32 UiFlag;
            static const u32 SceneFlag;

            static const hash_type STATE_MESSAGE_RENDER_QUEUE;
            static const hash_type STATE_MESSAGE_DIRECTION;

            /** Virtual destructor. */
            ~IGraphicsObject() override = default;

            /** Sets whether or not this object cast shadows. */
            virtual void setCastShadows( bool castShadows ) = 0;

            /** Gets a boolean indicating whether or not the object should cast shadows. */
            virtual bool getCastShadows() const = 0;

            /** Sets whether or not this object receives shadows. */
            virtual void setReceiveShadows( bool receiveShadows ) = 0;

            /** Gets whether or not this object receives shadows. */
            virtual bool getReceiveShadows() const = 0;

            /** Tells this object whether to be visible or not, if it has a renderable component. */
            virtual void setVisible( bool visible ) = 0;

            /** Return a boolean indicating whether or not this object is visible. */
            virtual bool isVisible() const = 0;

            /** Sets the render queue group this entity will be rendered through. */
            virtual void setZOrder( u32 zOrder ) = 0;

            /** Gets the render queue group this entity will be rendered through. */
            virtual u32 getZOrder() const = 0;

            /** Sets the visibility flags for this object.
            @remarks
            As well as a simple true/false value for visibility (as seen in setVisible),
            you can also set visibility flags which when 'and'ed with the SceneManager's
            visibility mask can also make an object invisible.
            */
            virtual void setVisibilityFlags( u32 flags ) = 0;

            /** Returns the visibility flags relevant for this object.
             */
            virtual u32 getVisibilityFlags() const = 0;

            /** Clones this graphics object.  */
            virtual SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const = 0;

            /** Gets a pointer to the underlying graphics object. This is dependent on the graphics
             * library used. */
            virtual void _getObject( void **ppObject ) const = 0;

            /** Sets a flag of the graphics object. */
            virtual void setFlag( u32 flag, bool value ) = 0;

            /** Gets a value of a flag. */
            virtual bool getFlag( u32 flag ) const = 0;

            /** Gets the scene node this is attached to.
            @return A pointer to the scene node.
            */
            virtual SmartPtr<ISceneNode> getOwner() const = 0;

            /** Sets the scene node this is attached to.
            @param sceneNode A pointer to the scene node.
            */
            virtual void setOwner( SmartPtr<ISceneNode> sceneNode ) = 0;

            /** Get the local bounding box data.
            @return The bounding data in a bounding box object.
            */
            virtual AABB3F getLocalAABB() const = 0;

            /** Set the local bounding box data.
            @param aabb The bounding box data.
            */
            virtual void setLocalAABB( const AABB3F &aabb ) = 0;

            /** To handle event. */
            virtual void attachToParent( SmartPtr<ISceneNode> parent ) = 0;

            /** To handle event. */
            virtual void detachFromParent( SmartPtr<ISceneNode> parent ) = 0;

            /** Used to know if the object is attached to a scene node. */
            virtual bool isAttached() const = 0;

            /** Used to know if the object is attached to a scene node. */
            virtual void setAttached( bool attached ) = 0;

            /** Get the render technique. */
            virtual hash32 getRenderTechnique() const = 0;

            /** Set the render technique. */
            virtual void setRenderTechnique( hash32 renderTechnique ) = 0;

            /** Sets the render queue group. */
            virtual void setRenderQueueGroup( u32 queueID ) = 0;

            /** Gets the render queue group. */
            virtual u32 getRenderQueueGroup() const = 0;

            /** Gets the data as a properties object.
            @return The data as a properties object.
            */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /** Sets the data as a properties object.
            @param properties The properties object.
            */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif
