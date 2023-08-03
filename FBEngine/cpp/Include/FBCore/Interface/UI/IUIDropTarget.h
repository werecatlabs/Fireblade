#ifndef IUIDropTarget_h__
#define IUIDropTarget_h__

#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIDropTarget
         * @brief Interface for a drop target, responsible for handling drop events in the user interface
         */
        class IUIDropTarget : public IEventListener
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIDropTarget() override = default;

            ///**
            // * @brief Handles a drop event
            // * @param position The position of the drop event in the UI
            // * @param src The source UI element involved in the drop event
            // * @param dst The destination UI element involved in the drop event
            // * @param data The data associated with the drop event
            // * @return true if the drop event is handled successfully, false otherwise
            // */
            //virtual bool handleDrop( const Vector2I &position, SmartPtr<IUIElement> src,
            //                         SmartPtr<IUIElement> dst, const String &data ) = 0;

            /*
            void onDragEnter( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDragLeave( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDragMove( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDrop( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropLeave( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropMove( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropEnter( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropCancel( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropFinish( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropAccept( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropReject( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropComplete( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropFail( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropSuccess( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropError( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropStart( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropStop( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;

            void onDropUpdate( SmartPtr<ui::IUIDragData> dragData, const Vector2i &position ) override;
            */

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIDropTarget_h__
