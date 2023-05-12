#ifndef IPrototype_h__
#define IPrototype_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace core
    {

        /**
         * @brief Interface for a prototype, a design pattern used to create new objects based on an existing
         * prototype instance.
         *
         * @note Cloning functionality should be implemented by the managers of the derived classes.
         *
         * @author Zane Desir
         * @version 1.0
         */
        class IPrototype : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IPrototype() override = default;

            /**
             * Gets the parent prototype.
             *
             * @return The parent prototype.
             */
            virtual SmartPtr<IPrototype> getParentPrototype() const = 0;

            /**
             * Sets the parent prototype.
             *
             * @param prototype The parent prototype.
             */
            virtual void setParentPrototype( SmartPtr<IPrototype> prototype ) = 0;

            /**
             * Gets the data as a properties object.
             *
             * @return The data as a properties object.
             */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /**
             * Sets the data as a properties object.
             *
             * @param properties The properties object.
             */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace core
}  // end namespace fb

#endif  // IPrototype_h__
