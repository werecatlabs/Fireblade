#ifndef IDirector_h__
#define IDirector_h__

#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Core/ConcurrentArray.h>

namespace fb
{
    namespace scene
    {

        /**
         * Interface for a director class. This class implements the builder pattern. The IDirector
         * class contains properties to construct objects.
         *
         * @author Zane Desir
         * @version 1.0
         */
        class IDirector : public IResource
        {
        public:
            /** Virtual destructor. */
            ~IDirector() override = default;

            /**
             * Gets the data as a properties object. The properties object can be used to set up the
             * construction of an object.
             *
             * @return The data as a properties object.
             */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /**
             * Sets the data as a properties object. The properties object can be used to set up the
             * construction of an object.
             *
             * @param properties The properties object.
             */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            /** Gets the parent. */
            virtual SmartPtr<IDirector> getParent() const = 0;

            /** Set the parent. Used internally */
            virtual void setParent( SmartPtr<IDirector> parent ) = 0;

            /** Adds a child to this actor. */
            virtual void addChild( SmartPtr<IDirector> child ) = 0;

            /** Removes a child of this actor. */
            virtual void removeChild( SmartPtr<IDirector> child ) = 0;

            /** Removes all children. */
            virtual void removeChildren() = 0;

            /** Return the child with the name provided. Return null if a child is not found. */
            virtual SmartPtr<IActor> findChild( const String &name ) = 0;

            /** Gets children. */
            virtual Array<SmartPtr<IActor>> getChildren() const = 0;

            /** Gets children. */
            virtual SharedPtr<ConcurrentArray<SmartPtr<IActor>>> getChildrenPtr() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // namespace fb

#endif  // ITransform_h__
