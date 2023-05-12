#ifndef __FB_CDirector_h__
#define __FB_CDirector_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IDirector.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Resource/CResource.h>

namespace fb
{
    namespace scene
    {
        /** Director implementation. */
        class CDirector : public CResource<IDirector>
        {
        public:
            /** Constructor. */
            CDirector();

            /** Destructor. */
            ~CDirector() override;

            /** @copydoc IDirector::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IDirector::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IDirector::toData */
            SmartPtr<IData> toData() const;

            /** @copydoc IDirector::fromData */
            void fromData( SmartPtr<IData> data ) override;

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
            SmartPtr<IActor> findChild( const String &name ) override;

            /** @copydoc IDirector::getChildren */
            Array<SmartPtr<IActor>> getChildren() const;

            /** @copydoc IDirector::getChildrenPtr */
            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> getChildrenPtr() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            SharedPtr<ConcurrentArray<SmartPtr<IActor>>> m_children;

            /**
             * \brief String pair values.
             */
            SmartPtr<Properties> m_properties;
        };
    }  // namespace scene
}  // namespace fb

#endif  // __FB_CDirector_h__
