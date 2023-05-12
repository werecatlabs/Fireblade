#ifndef IProceduralGenerator_h__
#define IProceduralGenerator_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        /**
         * @brief An interface for procedural generation objects.
         *
         * This interface defines methods for generating procedural content,
         * as well as accessing and setting input and output objects and a parent
         * procedural generator object.
         *
         * This interface inherits from the `ISharedObject` interface.
         */
        class IProceduralGenerator : public ISharedObject
        {
        public:
            /**
             * @brief Destructor.
             */
            ~IProceduralGenerator() override = default;

            /**
             * @brief Gets the file path associated with this generator.
             *
             * @return The file path as a `String`.
             */
            virtual String getFilePath() const = 0;

            /**
             * @brief Sets the file path associated with this generator.
             *
             * @param filePath The file path to set as a `String`.
             */
            virtual void setFilePath( const String &filePath ) = 0;

            /**
             * @brief Generates the procedural content.
             *
             * This method generates the procedural content based on the input
             * and output objects and any parent procedural generator object.
             */
            virtual void generate() = 0;

            /**
             * @brief Gets the input object associated with this generator.
             *
             * @return A smart pointer to an `IProceduralInput` object.
             */
            virtual SmartPtr<IProceduralInput> getInput() const = 0;

            /**
             * @brief Sets the input object associated with this generator.
             *
             * @param input A smart pointer to an `IProceduralInput` object.
             */
            virtual void setInput( SmartPtr<IProceduralInput> input ) = 0;

            /**
             * @brief Gets the output object associated with this generator.
             *
             * @return A smart pointer to an `IProceduralOutput` object.
             */
            virtual SmartPtr<IProceduralOutput> getOutput() const = 0;

            /**
             * @brief Sets the output object associated with this generator.
             *
             * @param output A smart pointer to an `IProceduralOutput` object.
             */
            virtual void setOutput( SmartPtr<IProceduralOutput> output ) = 0;

            /**
             * @brief Gets the parent procedural generator object.
             *
             * @return A smart pointer to an `IProceduralGenerator` object.
             */
            virtual SmartPtr<IProceduralGenerator> getParent() const = 0;

            /**
             * @brief Sets the parent procedural generator object.
             *
             * @param parent A smart pointer to an `IProceduralGenerator` object.
             */
            virtual void setParent( SmartPtr<IProceduralGenerator> parent ) = 0;
        };

    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralGenerator_h__
