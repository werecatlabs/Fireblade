#ifndef IOutputManager_h__
#define IOutputManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief Interface for controlling program output and writing to different destinations.
     *
     * This interface provides a common way to control program output and write to different destinations.
     * It includes a method for outputting a string value.
     */
    class IOutputManager : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IOutputManager() override = default;

        /**
         * @brief Outputs a string value.
         *
         * This method outputs a string value to the destination controlled by the output manager.
         *
         * @param val The string value to output.
         */
        virtual void output( const String &outputStr ) = 0;
    };

}  // end namespace fb

#endif  // IOutputManager_h__
