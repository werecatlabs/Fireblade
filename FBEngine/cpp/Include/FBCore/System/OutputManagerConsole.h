#ifndef OutputManagerStd_h__
#define OutputManagerStd_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IOutputManager.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{

    /**
     * @brief Implementation of the output manager that writes to the console.
     *
     * This class is an implementation of the output manager interface that writes output to the console.
     */
    class OutputManagerConsole : public CSharedObject<IOutputManager>
    {
    public:
        /** Constructor. */
        OutputManagerConsole();

        /** Destructor. */
        ~OutputManagerConsole() override;

        /**
         * @brief Outputs a string value to the console.
         *
         * This method outputs a string value to the console.
         *
         * @param outputStr The string value to output.
         */
        void output( const String &outputStr ) override;
    };

}  // end namespace fb

#endif  // OutputManagerStd_h__
