#include <FBCore/FBCorePCH.h>
#include <FBCore/Animation/KeyFrame.h>
#include <FBCore/Math/Math.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/ITimer.h>

namespace fb
{
    KeyFrame::KeyFrame() = default;

    KeyFrame::~KeyFrame() = default;

    auto KeyFrame::getTime() const -> f32
    {
        return m_time;
    }

    void KeyFrame::setTime( f32 time )
    {
        m_time = time;
    }
}  // namespace fb
