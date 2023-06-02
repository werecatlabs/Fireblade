#ifndef CanvasTransformSystem_h__
#define CanvasTransformSystem_h__

#include <FBCore/Interface/Scene/ISystem.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>

namespace fb
{
    namespace scene
    {

        class CanvasTransformSystem : public ISystem
        {
        public:
            struct canvas_transform
            {
                bool dirty = false;

                UIComponent *uiComponent = nullptr;
                LayoutTransform *owner = nullptr;

                Vector2F position = Vector2F::zero();
                Vector2F size = Vector2F( 300.0f, 100.0f );

                Vector2F absolutePosition = Vector2F::zero();
                Vector2F absoluteSize = Vector2F::zero();

                Vector2F anchor = Vector2F::zero();
                Vector2F pivot = Vector2F::UNIT * 0.5f;

                LayoutTransform::VerticalAlignment verticalAlignment = LayoutTransform::VerticalAlignment::TOP;
                LayoutTransform::HorizontalAlignment horizontalAlignment = LayoutTransform::HorizontalAlignment::LEFT;
            };

            CanvasTransformSystem();
            ~CanvasTransformSystem();

            virtual void update() override;

        protected:
            Array<bool> m_dirty;

            Array<UIComponent *> m_uiComponent;
            Array<LayoutTransform *> m_owner;

            Array<Vector2F> m_position;
            Array<Vector2F> m_size;

            Array<Vector2F> m_absolutePosition;
            Array<Vector2F> m_absoluteSize;

            Array<Vector2F> m_anchor;
            Array<Vector2F> m_pivot;

            Array<LayoutTransform::VerticalAlignment> m_verticalAlignments;
            Array<LayoutTransform::HorizontalAlignment> m_horizontalAlignments;
        };

    }  // namespace scene
}  // namespace fb

#endif  // CanvasTransformSystem_h__
