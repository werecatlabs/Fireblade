#ifndef __ISELECTIONMANAGER_H_INCLUDED__
#define __ISELECTIONMANAGER_H_INCLUDED__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /**
     * @brief Interface for managing a selection of shared objects.
     *
     * This interface provides methods for adding and removing objects from a selection, as well as
     * clearing the entire selection and retrieving the current selection as an array.
     */
    class ISelectionManager : public ISharedObject
    {
    public:
        /**
         * @brief Virtual destructor.
         */
        ~ISelectionManager() override = default;

        /**
         * @brief Adds a shared object to the selection.
         *
         * @param object The object to add to the selection.
         */
        virtual void addSelectedObject( SmartPtr<ISharedObject> object ) = 0;

        /**
         * @brief Removes a shared object from the selection.
         *
         * @param object The object to remove from the selection.
         */
        virtual void removeSelectedObject( SmartPtr<ISharedObject> object ) = 0;

        /**
         * @brief Clears the entire selection.
         */
        virtual void clearSelection() = 0;

        /**
         * @brief Returns the current selection as an array of shared objects.
         *
         * @return The current selection as an array of shared objects.
         */
        virtual Array<SmartPtr<ISharedObject>> getSelection() const = 0;
    };

}  // end namespace fb

#endif
