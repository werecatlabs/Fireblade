#ifndef ICubemap_h__
#define ICubemap_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    namespace render
    {
        /**
         * @brief Interface for a cubemap object.
         */
        class ICubemap : public ISharedObject
        {
        public:
            /**
             * @brief Destructor.
             */
            ~ICubemap() override = default;

            /**
             * @brief Get the name of the texture.
             * @return The texture name.
             */
            virtual String getTextureName() const = 0;

            /**
             * @brief Set the name of the texture.
             * @param textureName The new texture name.
             */
            virtual void setTextureName( const String &textureName ) = 0;

            /**
             * @brief Get the scene manager.
             * @return A smart pointer to the scene manager.
             */
            virtual SmartPtr<IGraphicsScene> getSceneManager() const = 0;

            /**
             * @brief Set the scene manager.
             * @param smgr A smart pointer to the scene manager.
             */
            virtual void setSceneManager( SmartPtr<IGraphicsScene> smgr ) = 0;

            /**
             * @brief Get the visibility mask.
             * @return The visibility mask.
             */
            virtual u32 getVisibilityMask() const = 0;

            /**
             * @brief Set the visibility mask.
             * @param visibilityMask The new visibility mask.
             */
            virtual void setVisibilityMask( u32 visibilityMask ) = 0;

            /**
             * @brief Get the exclusion mask.
             * @return The exclusion mask.
             */
            virtual u32 getExclusionMask() const = 0;

            /**
             * @brief Set the exclusion mask.
             * @param exclusionMask The new exclusion mask.
             */
            virtual void setExclusionMask( u32 exclusionMask ) = 0;

            /**
             * @brief Get the position.
             * @return The position.
             */
            virtual Vector3F getPosition() const = 0;

            /**
             * @brief Set the position.
             * @param position The new position.
             */
            virtual void setPosition( const Vector3F &position ) = 0;

            /**
             * @brief Get the enable state.
             * @return The enable state.
             */
            virtual bool getEnable() const = 0;

            /**
             * @brief Set the enable state.
             * @param enable The new enable state.
             */
            virtual void setEnable( bool enable ) = 0;

            /**
             * @brief Get the update interval.
             * @return The update interval in milliseconds.
             */
            virtual u32 getUpdateInterval() const = 0;

            /**
             * @brief Set the update interval.
             * @param milliseconds The new update interval in milliseconds.
             */
            virtual void setUpdateInterval( u32 milliseconds ) = 0;

            /**
             * @brief Add an excluded object.
             * @param object A smart pointer to the excluded object.
             */
            virtual void addExludedObject( SmartPtr<IGraphicsObject> object ) = 0;

            /**
             * @brief Get the excluded objects.
             * @return An array of smart pointers to the excluded objects.
             */
            virtual Array<SmartPtr<IGraphicsObject>> getExludedObjects() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // ICubemap_h__
