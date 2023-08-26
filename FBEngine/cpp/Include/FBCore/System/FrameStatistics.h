#ifndef FB_CFRAMESTATISTICS_H
#define FB_CFRAMESTATISTICS_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IFrameStatistics.h>

#include <FBCore/Core/FixedArray.h>
#include <FBCore/Atomics/AtomicFloat.h>
#include <FBCore/Thread/Threading.h>

namespace fb
{
    class FrameStatistics : public IFrameStatistics
    {
    public:
        FrameStatistics();
        ~FrameStatistics() override;

        /** @copydoc ISharedObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void update() override;

        /** @copydoc ISharedObject::isValid */
        bool isValid() const override;

        /** @copydoc IFrameStatistics::setVisible */
        void setVisible( bool visible ) override;

        /** @copydoc IFrameStatistics::isVisible */
        bool isVisible() const override;

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<render::IOverlay> m_statsOverlay;
        SmartPtr<render::IOverlayElementText> m_fpsText;
        SmartPtr<render::IOverlayElementText> m_renderText;
        SmartPtr<render::IOverlayElementText> m_physicsFPSText;
        SmartPtr<render::IOverlayElementText> m_applicationFPSText;
        FixedArray<atomic_f64, static_cast<u32>( Thread::Task::Count )> m_nextUpdate;
    };
}  // end namespace fb

#endif  // FB_CFRAMESTATISTICS_H
