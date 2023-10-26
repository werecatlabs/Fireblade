#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CVideoTextureOgreNext.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        CVideoTextureOgreNext::CVideoTextureOgreNext()
        {
        }

        CVideoTextureOgreNext::~CVideoTextureOgreNext()
        {
        }

        void CVideoTextureOgreNext::_copyFrameData( void *frameData, const Vector2I &size )
        {
            // if(Thread::getCurrentTask() == Thread::Task::TASK_ID_RENDER)
            //{
            //	u8* texData = (u8*)m_texture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_DISCARD);
            //	u8* videoData = (u8*)frameData;
            //
            //	int width = OgreVideoTextureUtil::CalculateNearest2Pow(size.X());
            //	int height =  OgreVideoTextureUtil::CalculateNearest2Pow(size.Y());

            //	Memory::Memcpy(texData, videoData, width*height*4);
            //	//Memory::Memcpy(texData, videoData, width*height*4);
            //	m_texture->getBuffer()->unlock();
            //}
            // else
            //{
            //
            //	u8* texData= (u8*)m_texture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_DISCARD);
            //	u8* videoData= (u8*)frameData;
            //	Memory::Memcpy(texData, videoData, size.X()*size.Y()*4);
            //	//Memory::Memcpy(texData, videoData, size.X()*size.Y()*4);
            //	m_texture->getBuffer()->unlock();
            //}
        }

        void CVideoTextureOgreNext::_getObject( void **object )
        {
            //*object = m_texture.getPointer();
        }

        void CVideoTextureOgreNext::_getObject( void **ppObject ) const
        {
        }

        Vector2I CVideoTextureOgreNext::getSize() const
        {
            return m_size;
        }

        void CVideoTextureOgreNext::initialise( const String &name, const Vector2I &size )
        {
            // int width = OgreVideoTextureUtil::CalculateNearest2Pow(size.X());
            // int height = OgreVideoTextureUtil::CalculateNearest2Pow(size.Y());

            // Ogre::TexturePtr t = Ogre::TextureManager::getSingleton().createManual(name.c_str(),
            //	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            //	Ogre::TEX_TYPE_2D, width, height, 1, 0, Ogre::PF_A8R8G8B8, Ogre::TU_DYNAMIC_WRITE_ONLY);

            // t->load(true);
            //
            // Ogre::PixelFormat format = t->getFormat();
            // if (format != Ogre::PF_A8R8G8B8)
            //{
            //	Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Pixel format is not PF_A8R8G8B8
            // which is what was requested!");
            // }

            // u8* texData=(u8*) t->getBuffer()->lock(Ogre::HardwareBuffer::HBL_DISCARD);
            // memset(texData,0x0,width*height*4);
            // t->getBuffer()->unlock();

            // m_texture = t;
        }

        void CVideoTextureOgreNext::update( const s32 &task, const time_interval &t,
                                        const time_interval &dt )
        {
        }

        void CVideoTextureOgreNext::setSize( const Vector2I &size )
        {
            m_size = size;
        }

        SmartPtr<IRenderTarget> CVideoTextureOgreNext::getRenderTarget() const
        {
            return nullptr;
        }

        void CVideoTextureOgreNext::copyToTexture( SmartPtr<ITexture> &target )
        {
        }

        void CVideoTextureOgreNext::copyData( void *data, const Vector2I &size )
        {
            // if(Thread::getCurrentTask() == Thread::Task::TASK_ID_RENDER)
            //{
            //	u8* texData = (u8*)m_texture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_DISCARD);
            //	u8* videoData = (u8*)data;

            //	int width = OgreVideoTextureUtil::CalculateNearest2Pow(size.X());
            //	int height =  OgreVideoTextureUtil::CalculateNearest2Pow(size.Y());

            //	Memory::Memcpy(texData, videoData, width*height*4);
            //	//Memory::Memcpy(texData, videoData, width*height*4);
            //	m_texture->getBuffer()->unlock();
            //}
            // else
            //{
            //
            //	u8* texData= (u8*)m_texture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_DISCARD);
            //	u8* videoData= (u8*)data;
            //	Memory::Memcpy(texData, videoData, size.X()*size.Y()*4);
            //	//Memory::Memcpy(texData, videoData, size.X()*size.Y()*4);
            //	m_texture->getBuffer()->unlock();
            //}
        }

        Vector2I CVideoTextureOgreNext::getActualSize() const
        {
            return Vector2I();
        }
    }  // end namespace render
}  // end namespace fb
