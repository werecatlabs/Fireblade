#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include "FBGraphicsOgre/Ogre/ShadowFrameListener.h"
#include "FBGraphicsOgre/Ogre/ScreenSpaceEffect.h"
#include "FBGraphicsOgre/Ogre/SSEffectRenderer.h"
#include "FBGraphicsOgre/Ogre/SSEffectClear.h"
#include <Ogre.h>

namespace fb
{
    namespace render
    {

        ShadowFrameListener::ShadowFrameListener( ScreenSpaceEffect *effectClear, Ogre::TexturePtr tex,
                                                  u32 nextUpdateInterval ) :
            m_effectClear( effectClear ),
            m_tex( tex ),
            m_nextUpdate( 0 ),
            m_nextUpdateInterval( nextUpdateInterval )
        {
            float floatMax = 1000.0f * 1000.0f;
            int maxValue = *reinterpret_cast<int *>( &floatMax );

            int channels = 2;
            int size = m_tex->getWidth() * m_tex->getHeight() * channels;
            data = new float[size];
            // memset((void*)data,maxValue,m_tex->getWidth()*m_tex->getHeight()*sizeof(float)*2);
            for( int i = 0; i < size; i += channels )
            {
                data[i] = 1000.0f;
                data[i + 1] = 1000.0f * 1000.0f;
                // data[i+2] = 1.0f;
            }

            box = new Ogre::PixelBox( m_tex->getWidth(), m_tex->getHeight(), m_tex->getDepth(),
                                      m_tex->getFormat(), data );
        }

        ShadowFrameListener::~ShadowFrameListener()
        {
            delete[] data;
        }

        bool ShadowFrameListener::frameStarted( const Ogre::FrameEvent &evt )
        {
            m_effectClear->update();
            return true;
        }

    }  // namespace render
}  // end namespace fb
