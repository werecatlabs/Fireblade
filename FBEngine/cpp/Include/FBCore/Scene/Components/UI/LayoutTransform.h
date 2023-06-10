#ifndef CanvasTransform_h__
#define CanvasTransform_h__

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

                COUNT
            };

            /** Enum for vertical alignment. */
            enum class VerticalAlignment
            {
                TOP,
                CENTER,
                BOTTOM,

                COUNT
            };

            /** Constructor. */
            LayoutTransform();

            /** Destructor. */
            ~LayoutTransform() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::preUpdate */
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

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            void setHorizontalAlignment( HorizontalAlignment gha );

            HorizontalAlignment getHorizontalAlignment() const;

            void setVerticalAlignment( VerticalAlignment gva );

            VerticalAlignment getVerticalAlignment() const;

            Vector2F getAbsolutePosition() const;

            void setAbsolutePosition( const Vector2F &absolutePosition );

            Vector2F getAbsoluteSize() const;

            void setAbsoluteSize( const Vector2F &absoluteSize );

            FB_CLASS_REGISTER_DECL;

        protected:
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType );

            SmartPtr<UIComponent> m_uiComponent;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CanvasTransform_h__
