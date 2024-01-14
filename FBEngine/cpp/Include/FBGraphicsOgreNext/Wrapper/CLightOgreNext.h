#ifndef __CLightOgreNext_H
#define __CLightOgreNext_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/Light.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace render
    {

        class CLightOgreNext : public Light
        {
        public:
            CLightOgreNext();
            CLightOgreNext( const CLightOgreNext &other ) = delete;
            CLightOgreNext( SmartPtr<IGraphicsScene> creator );
            ~CLightOgreNext() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;

            void _getObject( void **ppObject ) const override;

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

            Ogre::Light *m_light = nullptr;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
