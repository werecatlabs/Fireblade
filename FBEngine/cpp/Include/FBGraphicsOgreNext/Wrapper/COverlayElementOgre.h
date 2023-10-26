#ifndef _COverlayElementOgreNext_H
#define _COverlayElementOgreNext_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/COverlayElement.h>
#include <FBCore/Interface/Graphics/IOverlayElement.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        template <class T>
        class COverlayElementOgre : public COverlayElement<T>
        {
        public:
            COverlayElementOgre();
            ~COverlayElementOgre() override;

            virtual void _getObject( void **ppObject ) const override
            {
                *ppObject = m_element;
            }

            /** @copydoc IObject::isValid */
            bool isValid() const override
            {
                if( T::isLoaded() )
                {
                    if( auto parent = COverlayElement<T>::getParent() )
                    {
                        auto children = COverlayElement<T>::getChildren();
                        for( auto child : children )
                        {
                            if( !child->isValid() )
                            {
                                return false;
                            }
                        }

                        return true;
                    }

                    return true;
                }

                return false;
            }

            Ogre::v1::OverlayElement *getElement() const
            {
                return m_element;
            }

            void setElement( Ogre::v1::OverlayElement *element )
            {
                m_element = element;
            }

            virtual void setupMaterial( SmartPtr<IMaterial> material )
            {
            }

            SmartPtr<Properties> getProperties() const;

            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_TEMPLATE_DECL( COverlayElementOgre, T );

        protected:
            class StateListenerOgre : public COverlayElement<T>::ElementStateListener
            {
            public:
                StateListenerOgre() = default;
                ~StateListenerOgre() override = default;

                virtual void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;

                virtual void handleStateChanged( SmartPtr<IState> &state ) override;

                virtual void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            virtual void createStateContext();

            Atomic<Ogre::v1::OverlayElement *> m_element = nullptr;
        };

        template <class T>
        COverlayElementOgre<T>::COverlayElementOgre() : COverlayElement<T>()
        {
        }

        template <class T>
        COverlayElementOgre<T>::~COverlayElementOgre()
        {
        }

    }  // end namespace render
}  // end namespace fb

#endif
