#ifndef CanvasTransform_h__
#define CanvasTransform_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {

        class CanvasTransform : public Component
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

            CanvasTransform();
            ~CanvasTransform() override;

            static void updateComponents();

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::preUpdate */
            void updateTransform() override;

            /** @copydoc IComponent::reset */
            void reset() override;

            /** @copydoc IComponent::awake */
            void awake() override;

            /** @copydoc IComponent::destroy */
            void destroy() override;

            /** @copydoc IComponent::start */
            void play();

            /** @copydoc IComponent::edit */
            void edit();

            Vector2F getPosition() const;
            void setPosition( const Vector2F &position );

            Vector2F getSize() const;
            void setSize( const Vector2F &size );

            Vector2F getAnchor() const;
            void setAnchor( const Vector2F &anchor );

            Vector2F getPivot() const;
            void setPivot( const Vector2F &pivot );

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
            struct canvas_transform
            {
                bool dirty = false;

                UIComponent *uiComponent = nullptr;
                CanvasTransform *owner = nullptr;

                Vector2F position = Vector2F::zero();
                Vector2F size = Vector2F( 300.0f, 100.0f );

                Vector2F absolutePosition;
                Vector2F absoluteSize;

                Vector2F anchor = Vector2F::zero();
                Vector2F pivot = Vector2F::UNIT * 0.5f;

                VerticalAlignment verticalAlignment = VerticalAlignment::TOP;
                HorizontalAlignment horizontalAlignment = HorizontalAlignment::LEFT;
            };

            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType );

            SmartPtr<UIComponent> m_uiComponent;

            //Vector2F m_absolutePosition;
            //Vector2F m_absoluteSize;

            //Vector2F m_position;
            //Vector2F m_relativePosition;
            //Vector2F m_size;
            //Vector2F m_anchor;
            //Vector2F m_pivot = Vector2F::UNIT * 0.5f;

            //VerticalAlignment m_verticalAlignment = VerticalAlignment::TOP;
            //HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::LEFT;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CanvasTransform_h__
