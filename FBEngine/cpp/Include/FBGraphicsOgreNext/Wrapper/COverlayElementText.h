#ifndef _COverlayElementText_H
#define _COverlayElementText_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IOverlayElementText.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayElementOgre.h>
#include <FBCore/State/States/BaseState.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace render
    {
        class COverlayElementText : public COverlayElementOgre<IOverlayElementText>
        {
        public:
            COverlayElementText();
            ~COverlayElementText() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            bool isContainer() const override;

            // IOverlayElementText functions
            void setFontName( const String &fontName ) override;

            void setCharHeight( f32 charHeight ) override;

            void setAlignment( u8 alignment ) ;
            u8 getAlignment() const ;

            void setSpaceWidth( f32 width ) override;
            f32 getSpaceWidth() const override;

            bool isValid() const override;

            Ogre::v1::TextAreaOverlayElement *getElementText() const;
            void setElementText( Ogre::v1::TextAreaOverlayElement *elementText );

            FB_CLASS_REGISTER_DECL;

        protected:
            class StateListener : public StateListenerOgre
            {
            public:
                StateListener() = default;
                ~StateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            void createStateContext() override;

            Ogre::v1::TextAreaOverlayElement *m_elementText = nullptr;

            u8 m_horizontalAlignment = 0;
            u8 m_verticalAlignment = 0;
            ColourF m_colour = ColourF::White;

            String m_name;
        };
    } // end namespace render
}     // end namespace fb

#endif
