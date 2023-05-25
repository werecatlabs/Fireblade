#include <FBGraphics/Wrapper/CGraphicsSystem.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        void CGraphicsSystem::load( SmartPtr<ISharedObject> data )
        {
        }

        void CGraphicsSystem::unload( SmartPtr<ISharedObject> data )
        {
        }


void CGraphicsSystem::lock()
        {
            m_mutex.lock();
        }

        void CGraphicsSystem::unlock()
        {
            m_mutex.unlock();
        }

        Array<SmartPtr<IWindow>> CGraphicsSystem::getWindows() const
        {
            return m_windows;
        }

    }  // namespace render
}  // namespace fb
