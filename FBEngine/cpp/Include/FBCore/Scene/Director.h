#ifndef __FB_CDirector_h__
#define __FB_CDirector_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IDirector.h>
#include <FBCore/Resource/Resource.h>

namespace fb
{
    namespace scene
    {
        /** Director implementation. */
        class Director : public Resource<IDirector>
        {
        public:
            /** Constructor. */
            Director();

            /** Destructor. */
            ~Director() override;

            /** @copydoc IDirector::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IDirector::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IDirector::save */
            void save() override;

            /** @copydoc IDirector::toData */
            SmartPtr<ISharedObject> toData() const override;

            /** @copydoc IDirector::fromData */
            void fromData( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IDirector::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IDirector::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IDirector::setProperties */
            SmartPtr<IDirector> getParent() const override;

            /** @copydoc IDirector::setProperties */
            void setParent( SmartPtr<IDirector> parent ) override;

            /** @copydoc IDirector::addChild */
            void addChild( SmartPtr<IDirector> child ) override;

            /** @copydoc IDirector::removeChild */
            void removeChild( SmartPtr<IDirector> child ) override;

            /** @copydoc IDirector::removeChildren */
            void removeChildren() override;

            /** @copydoc IDirector::findChild */
            SmartPtr<IDirector> findChild( const String &name ) override;

            /** @copydoc IDirector::getChildren */
            Array<SmartPtr<IDirector>> getChildren() const override;

            /** @copydoc IDirector::getChildrenPtr */
            SharedPtr<ConcurrentArray<SmartPtr<IDirector>>> getChildrenPtr() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            // The director children.
            SharedPtr<ConcurrentArray<SmartPtr<IDirector>>> m_children;

            // String pair values.
            SmartPtr<Properties> m_properties;
        };
    }  // namespace scene
}  // namespace fb

#endif  // __FB_CDirector_h__
