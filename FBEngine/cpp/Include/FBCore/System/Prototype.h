#ifndef Prototype_h__
#define Prototype_h__

#include <FBCore/Interface/IPrototype.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    namespace core
    {

        /** Template class to implement the IPrototype interface. */
        template <class T>
        class Prototype : public T
        {
        public:
            /** Constructor. */
            Prototype() = default;

            /** Destructor. */
            virtual ~Prototype() override = default;

            /** @copydoc IPrototype::getParentPrototype */
            virtual SmartPtr<IPrototype> getParentPrototype() const;

            /** @copydoc IPrototype::setParentPrototype */
            virtual void setParentPrototype( SmartPtr<IPrototype> prototype );

            /** Gets the data as a properties object.
            @return The data as a properties object.
            */
            virtual SmartPtr<Properties> getProperties() const;

            /** Sets the data as a properties object.
            @param properties The properties object.
            */
            virtual void setProperties( SmartPtr<Properties> properties );

            /** @copydoc IPrototype::clone */
            virtual SmartPtr<IPrototype> clone();

            FB_CLASS_REGISTER_TEMPLATE_DECL( Prototype, T );

        protected:
            // The parent prototype.
            SmartPtr<IPrototype> m_parentPrototype;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb::core, Prototype, T, T );

        template <class T>
        SmartPtr<IPrototype> Prototype<T>::getParentPrototype() const
        {
            return m_parentPrototype;
        }

        template <class T>
        void Prototype<T>::setParentPrototype( SmartPtr<IPrototype> prototype )
        {
            m_parentPrototype = prototype;
        }

        template <class T>
        SmartPtr<Properties> Prototype<T>::getProperties() const
        {
            static const auto loadedStr = String( "loaded" );
            static const auto referencesStr = String( "references" );
            static const auto weakReferencesStr = String( "weakReferences" );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto properties = factoryManager->make_ptr<Properties>();
            properties->setProperty( loadedStr, Prototype<T>::isLoaded() );
            properties->setProperty( referencesStr, Prototype<T>::getReferences() );
            properties->setProperty( weakReferencesStr, Prototype<T>::getWeakReferences() );
            return properties;
        }

        template <class T>
        void Prototype<T>::setProperties( SmartPtr<Properties> properties )
        {
        }

        template <class T>
        SmartPtr<IPrototype> Prototype<T>::clone()
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto type = this->getTypeInfo();

            SmartPtr<IPrototype> instance = factoryManager->createById( type );

            if( auto properties = this->getProperties() )
            {
                instance->setProperties( properties );
            }

            return instance;
        }

    }  // namespace core
}  // namespace fb

#endif  // Prototype_h__
