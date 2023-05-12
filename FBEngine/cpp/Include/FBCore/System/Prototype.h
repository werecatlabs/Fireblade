#ifndef Prototype_h__
#define Prototype_h__

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/IPrototype.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Base/Properties.h>

namespace fb
{
    namespace core
    {
        template <class T>
        class Prototype : public CSharedObject<T>
        {
        public:
            Prototype() = default;
            ~Prototype() override = default;

            virtual SmartPtr<IPrototype> getParentPrototype() const
            {
                return m_parentPrototype;
            }

            virtual void setParentPrototype( SmartPtr<IPrototype> prototype )
            {
                m_parentPrototype = prototype;
            }

            /** Gets the data as a properties object.
            @return The data as a properties object.
            */
            virtual SmartPtr<Properties> getProperties() const
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto properties = factoryManager->make_ptr<Properties>();
                properties->setProperty( "loaded", this->isLoaded() );
                properties->setProperty( "references", this->getReferences() );
                properties->setProperty( "weakReferences", this->getWeakReferences() );
                return properties;
            }

            /** Sets the data as a properties object.
            @param properties The properties object.
            */
            virtual void setProperties( SmartPtr<Properties> properties )
            {
            }

            virtual SmartPtr<IPrototype> clone()
            {
                auto applicationManager = IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto type = this->getTypeInfo();

                constexpr bool res = std::is_convertible<IPrototype *, ISharedObject *>();
                if( res )
                {
                }

                SmartPtr<IPrototype> instance = factoryManager->createById( type );

                if( auto properties = this->getProperties() )
                {
                    instance->setProperties( properties );
                }

                return instance;
            }

            FB_CLASS_REGISTER_TEMPLATE_DECL( Prototype, T );

        protected:
            SmartPtr<IPrototype> m_parentPrototype;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, Prototype, T, CSharedObject<T> );
    }  // namespace core
}  // namespace fb

#endif  // Prototype_h__
