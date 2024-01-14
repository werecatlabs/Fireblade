#ifndef __LayoutTransform_h__
#define __LayoutTransform_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {
        /** Component for ui component transforms. */
        class LayoutTransform : public Component
        {
        public:
            /** Enum for horizontal alignment. */
            enum class HorizontalAlignment
            {
                LEFT,
                CENTER,
                RIGHT,
                CUSTOM,

                COUNT
            };

            /** Enum for vertical alignment. */
            enum class VerticalAlignment
            {
                TOP,
                CENTER,
                BOTTOM,
                CUSTOM,

                COUNT
            };

            /** Constructor. */
            LayoutTransform();

            /** Destructor. */
            ~LayoutTransform() override;

            /** @copydoc Component::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::updateTransform */
            void updateTransform() override;

            Vector2F getPosition() const;
            void setPosition( const Vector2F &position );

            Vector2F getSize() const;
            void setSize( const Vector2F &size );

            Vector2F getAnchor() const;
            void setAnchor( const Vector2F &anchor );

            Vector2F getAnchorMin() const;

            void setAnchorMin( const Vector2F &anchorMin );

            Vector2F getAnchorMax() const;

            void setAnchorMax( const Vector2F &anchorMax );

            /** @copydoc Component::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Component::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            void setHorizontalAlignment( HorizontalAlignment gha );

            HorizontalAlignment getHorizontalAlignment() const;

            void setVerticalAlignment( VerticalAlignment gva );

            VerticalAlignment getVerticalAlignment() const;

            Vector2F getAbsolutePosition() const;

            void setAbsolutePosition( const Vector2F &absolutePosition );

            Vector2F getAbsoluteSize() const;

            void setAbsoluteSize( const Vector2F &absoluteSize );

            SmartPtr<UIComponent> getLayout() const;

            void setLayout( SmartPtr<UIComponent> layout );

            FB_CLASS_REGISTER_DECL;

        protected:
            /** @copydoc Component::handleComponentEvent */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            /**
             * @brief Sets up the canvas for the UIComponent.
             *
             * This method is responsible for initializing the canvas for the UIComponent.
             * It should be implemented by derived classes if additional setup is needed.
             */
            virtual void setupCanvas();

            /** The ui component transform. */
            SmartPtr<UIComponent> m_uiComponent;

            /**
             * @brief A smart pointer to the UIComponent object representing the canvas on which the UIComponent is drawn.
             */
            SmartPtr<UIComponent> m_layout;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // __LayoutTransform_h__
