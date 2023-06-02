#ifndef IMaterialTechnique_h__
#define IMaterialTechnique_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Graphics/IMaterialNode.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace render
    {

        /**
         * @brief An interface for a material technique.
         */
        class IMaterialTechnique : public IMaterialNode
        {
        public:
            /** Virtual destructor. */
            ~IMaterialTechnique() override = default;

            /**
             * @brief Gets the scheme hash value.
             * @return A 32-bit unsigned integer representing the scheme hash value.
             */
            virtual hash32 getScheme() const = 0;

            /**
             * @brief Sets the scheme hash value.
             * @param scheme A 32-bit unsigned integer representing the scheme hash value.
             */
            virtual void setScheme( hash32 scheme ) = 0;

            /**
             * @brief Gets the number of material passes in the technique.
             * @return A 32-bit unsigned integer representing the number of material passes.
             */
            virtual u32 getNumPasses() const = 0;

            /**
             * @brief Creates a new material pass.
             * @return A smart pointer to the new material pass.
             */
            virtual SmartPtr<IMaterialPass> createPass() = 0;

            /**
             * @brief Adds a material pass to the technique.
             * @param pass A smart pointer to the material pass to add.
             */
            virtual void addPass( SmartPtr<IMaterialPass> pass ) = 0;

            /**
             * @brief Removes a material pass from the technique.
             * @param pass A smart pointer to the material pass to remove.
             */
            virtual void removePass( SmartPtr<IMaterialPass> pass ) = 0;

            /**
             * @brief Removes all material passes from the technique.
             */
            virtual void removePasses() = 0;

            /**
             * @brief Gets an array of all material passes in the technique.
             * @return An array of smart pointers to the material passes.
             */
            virtual Array<SmartPtr<IMaterialPass>> getPasses() const = 0;

            /**
             * @brief Sets the material passes in the technique.
             * @param passes An array of smart pointers to the material passes.
             */
            virtual void setPasses( Array<SmartPtr<IMaterialPass>> passes ) = 0;

            /**
             * @brief Gets a shared pointer to an array of all material passes in the technique.
             * @return A shared pointer to the array of smart pointers to the material passes.
             */
            virtual SharedPtr<Array<SmartPtr<IMaterialPass>>> getPassesPtr() const = 0;

            /**
             * @brief Sets a shared pointer to an array of all material passes in the technique.
             * @param ptr A shared pointer to the array of smart pointers to the material passes.
             */
            virtual void setPassesPtr( SharedPtr<Array<SmartPtr<IMaterialPass>>> ptr ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // IMaterialTechnique_h__
