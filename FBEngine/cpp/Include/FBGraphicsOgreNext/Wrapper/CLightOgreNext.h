#ifndef __CLightOgreNext_H
#define __CLightOgreNext_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/Light.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace render
    {

        /** Implementation of the ILight interface for OgreNext. */
        class CLightOgreNext : public Light
        {
        public:
            /** Constructor. */
            CLightOgreNext();

            /** Constructor. */
            CLightOgreNext( const CLightOgreNext &other ) = delete;

            /** Constructor. */
            CLightOgreNext( SmartPtr<IGraphicsScene> creator );

            /** Destructor. */
            ~CLightOgreNext() override;

            /** @copydoc Light::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Light::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Light::clone */
            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;

            /** @copydoc Light::_getObject */
            void _getObject( void **ppObject ) const override;

            /** Gets the Ogre light object. */
            Ogre::Light *getLight() const;

            FB_CLASS_REGISTER_DECL;

        protected:
            class CLightStateListener : public IStateListener
            {
            public:
                CLightStateListener();
                CLightStateListener( CLightOgreNext *light );
                ~CLightStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                void setOwner( CLightOgreNext *light );

                CLightOgreNext *getOwner() const;

                CLightOgreNext *m_owner = nullptr;
            };

            // The Ogre light object.
            Ogre::Light *m_light = nullptr;

            // Used to generate a unique name for the Ogre light object.
            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
