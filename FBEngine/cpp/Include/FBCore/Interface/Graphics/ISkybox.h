#ifndef ISkybox_h__
#define ISkybox_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        /**
         * Represents a skybox.
         */
        class ISkybox : public ISharedObject
        {
        public:
            /** The different types of faces for the skybox. */
            enum class FaceTypes
            {
                Front, /**< The front face of the skybox. */
                Back,  /**< The back face of the skybox. */
                Left,  /**< The left face of the skybox. */
                Right, /**< The right face of the skybox. */
                Up,    /**< The top face of the skybox. */
                Down,  /**< The bottom face of the skybox. */
                Count  /**< The total number of faces in the skybox. */
            };

            /** Virtual destructor. */
            ~ISkybox() override = default;

            /**
             * Sets the material to use for the skybox.
             *
             * @param material The material to use.
             */
            virtual void setMaterial( SmartPtr<IMaterial> material ) = 0;

            /**
             * Gets the material used by the skybox.
             *
             * @return The material used.
             */
            virtual SmartPtr<IMaterial> getMaterial() const = 0;

            /**
             * Gets the scene manager to use.
             *
             * @return The scene manager.
             */
            virtual SmartPtr<IGraphicsScene> getSceneManager() const = 0;

            /**
             * Sets the scene manager to use.
             *
             * @param sceneManager The scene manager to use.
             */
            virtual void setSceneManager( SmartPtr<IGraphicsScene> sceneManager ) = 0;

            /**
             * Gets the visibility of the skybox.
             *
             * @return True if the skybox is visible, false otherwise.
             */
            virtual bool isVisible() const = 0;

            /**
             * Sets the visibility of the skybox.
             *
             * @param visible Whether or not the skybox should be visible.
             */
            virtual void setVisible( bool visible ) = 0;

            /**
             * Gets the distance of the skybox.
             *
             * @return The distance of the skybox.
             */
            virtual f32 getDistance() const = 0;

            /**
             * Sets the distance of the skybox.
             *
             * @param distance The distance of the skybox.
             */
            virtual void setDistance( f32 distance ) = 0;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // ISkybox_h__
