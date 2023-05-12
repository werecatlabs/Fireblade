#ifndef RenderWindowState_h__
#define RenderWindowState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Base/FixedArray.h>

namespace fb
{
    class RenderWindowState : public BaseState
    {
    public:
        RenderWindowState();
        ~RenderWindowState() override;

        SmartPtr<IState> clone() const override;

        Vector2I getSize() const;

        void setSize( const Vector2I &size );

        u32 getColourDepth() const;

        void setColourDepth( u32 colourDepth );

        bool isFullscreen() const;

        void setFullscreen( bool fullscreen );

        bool isVisible() const;

        void setVisible( bool visible );

        String getLabel() const;

        void setLabel( const String &label );

        String getWindowHandle() const;

        void setWindowHandle( const String &windowHandle );

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector2I m_size;

        u32 m_colourDepth = 0;
        bool m_fullscreen = false;
        bool m_visible = true;

        String m_label;
        String m_windowHandle;
    };
} // end namespace fb

#endif  // RenderWindowState_h__
