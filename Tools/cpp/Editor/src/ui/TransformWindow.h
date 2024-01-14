#ifndef TransformWindow_h__
#define TransformWindow_h__

#include <GameEditorPrerequisites.h>
#include "ui/EditorWindow.h"
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/Scene/ITransform.h>

namespace fb
{
    namespace editor
    {
        class TransformWindow : public EditorWindow
        {
        public:
            TransformWindow();
            ~TransformWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void updateSelection() override;

            SmartPtr<scene::ITransform> getTransform() const;
            void setTransform( SmartPtr<scene::ITransform> transform );

            bool getShowWorldTransform() const;
            void setShowWorldTransform( bool showWorldTransform );

        protected:
            class VectorListener : public IEventListener
            {
            public:
                VectorListener() = default;
                ~VectorListener() override = default;

                void unload( SmartPtr<ISharedObject> data ) override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                void handleValueChanged();

                TransformWindow *getOwner() const;
                void setOwner( TransformWindow *owner );

                SmartPtr<ui::IUIVector3> getVectorUI() const;
                void setVectorUI( SmartPtr<ui::IUIVector3> val );

                scene::ITransform::Type getType() const;
                void setType( scene::ITransform::Type val );

            private:
                scene::ITransform::Type m_type = scene::ITransform::Type::None;
                TransformWindow *m_owner = nullptr;
                WeakPtr<ui::IUIVector3> m_vectorUI;
            };

            SmartPtr<ui::IUILabelTogglePair> m_actorEnabled;

            SmartPtr<ui::IUIVector3> m_localPosition;
            SmartPtr<ui::IUIVector3> m_localRotation;
            SmartPtr<ui::IUIVector3> m_localScale;

            SmartPtr<ui::IUIVector3> m_position;
            SmartPtr<ui::IUIVector3> m_rotation;
            SmartPtr<ui::IUIVector3> m_scale;

            SmartPtr<scene::ITransform> m_transform;

            bool m_showWorldTransform = false;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // TransformWindow_h__
