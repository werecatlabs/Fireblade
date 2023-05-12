#ifndef IWater_h__
#define IWater_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for a water object.
         */
        class IWater : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IWater() override = default;

            /**
             * Gets the scene manager associated with this water object.
             *
             * @return A shared pointer to the scene manager.
             */
            virtual SmartPtr<IGraphicsScene> getSceneManager() const = 0;

            /**
             * Sets the scene manager associated with this water object.
             *
             * @param sceneMgr A shared pointer to the scene manager.
             */
            virtual void setSceneManager( SmartPtr<IGraphicsScene> sceneMgr ) = 0;

            /**
             * Gets the camera associated with this water object.
             *
             * @return A shared pointer to the camera.
             */
            virtual SmartPtr<ICamera> getCamera() const = 0;

            /**
             * Sets the camera associated with this water object.
             *
             * @param camera A shared pointer to the camera.
             */
            virtual void setCamera( SmartPtr<ICamera> camera ) = 0;

            /**
             * Gets the viewport associated with this water object.
             *
             * @return A shared pointer to the viewport.
             */
            virtual SmartPtr<IViewport> getViewport() const = 0;

            /**
             * Sets the viewport associated with this water object.
             *
             * @param viewport A shared pointer to the viewport.
             */
            virtual void setViewport( SmartPtr<IViewport> viewport ) = 0;

            /**
             * Gets the position of the node relative to its parent.
             *
             * @return The position vector.
             */
            virtual Vector3F getPosition() const = 0;

            /**
             * Sets the position of the node relative to its parent.
             *
             * @param position The position vector.
             */
            virtual void setPosition( const Vector3F &position ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IWater_h__
