#ifndef IScript_h__
#define IScript_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief The IScript class is an abstract base class for a script.
     *
     * It inherits from the ISharedObject class and provides functions to retrieve and
     * set information about the script, such as its classes.
     */
    class IScript : public ISharedObject
    {
    public:
        /**
         * @brief Destructor.
         */
        ~IScript() override = default;

        /**
         * @brief Gets the classes of the script.
         * @return An array of smart pointers to IScriptClass objects.
         */
        virtual Array<SmartPtr<IScriptClass>> getClasses() const = 0;

        /**
         * @brief Sets the classes of the script.
         * @param classes An array of smart pointers to IScriptClass objects.
         */
        virtual void setClasses( const Array<SmartPtr<IScriptClass>> &classes ) = 0;

        /**
         * @brief Macro used to register the class with the script system.
         *
         * This macro is used to register the class with the script system. It should be
         * placed in the class definition file.
         */
        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IScript_h__
