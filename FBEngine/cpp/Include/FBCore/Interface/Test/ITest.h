#ifndef ITest_h__
#define ITest_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/IObject.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    /**
     * @brief An interface for a test class.
     */
    class ITest : public ISharedObject
    {
    public:
        /**
         * @brief Destructor for ITest.
         */
        ~ITest() override = default;

        /**
         * @brief Runs the test.
         */
        virtual void run() = 0;

        /**
         * @brief Reports the results of the test.
         */
        virtual void report() = 0;

        /**
         * @brief Returns whether the test is enabled.
         * @return True if the test is enabled, false otherwise.
         */
        virtual bool isEnabled() const = 0;

        /**
         * @brief Sets whether the test is enabled.
         * @param enabled The new enabled state of the test.
         */
        virtual void setEnabled( bool enabled ) = 0;

        /**
         * @brief Gets the name of the test.
         * @return The name of the test.
         */
        virtual String getName() const = 0;

        /**
         * @brief Sets the name of the test.
         * @param name The new name of the test.
         */
        virtual void setName( const String &name ) = 0;
    };
} // end namespace fb

#endif  // ITest_h__
