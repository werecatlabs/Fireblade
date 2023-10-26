#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/Wrapper/CBillboardOgre.h"
#include <OgreBillboard.h>

namespace fb
{
    namespace render
    {

        //--------------------------------------------
        CBillboardOgre::CBillboardOgre() : m_bb( NULL )
        {
        }

        //--------------------------------------------
        CBillboardOgre::~CBillboardOgre()
        {
        }

        //--------------------------------------------
        void CBillboardOgre::initialise( Ogre::v1::Billboard *bb )
        {
            m_bb = bb;
        }

        //--------------------------------------------
        void CBillboardOgre::setPosition( const Vector3F &position )
        {
            //
            m_position = position;
            // m_bb->setPosition(Ogre::Vector3(m_position));
        }

        //--------------------------------------------
        Vector3F CBillboardOgre::getPosition() const
        {
            return m_position;
        }

        //--------------------------------------------
        void CBillboardOgre::setDimensions( const Vector2F &dimensions )
        {
            // m_bb->setDimensions(dimensions.X(), dimensions.Y());
        }

        //--------------------------------------------
        Vector2F CBillboardOgre::getDimensions() const
        {
            return Vector2F::ZERO;
        }

        //--------------------------------------------
        void CBillboardOgre::setColour( const ColourF &colour )
        {
            m_bb->setColour( Ogre::ColourValue( colour.r, colour.g, colour.b, colour.a ) );
        }

        //--------------------------------------------
        ColourF CBillboardOgre::getColour() const
        {
            return ColourF::Red;
        }

        //--------------------------------------------
        void CBillboardOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_bb;
        }

        //--------------------------------------------
        bool CBillboardOgre::IsFree( void *element )
        {
            // CBillboardPtr billboard = *(static_cast<CBillboardPtr*>(element));
            // if( billboard->getReferenceCount() == 2 )
            //	return true;

            return false;
        }

        void CBillboardOgre::setOrientation( const QuaternionF &orientation )
        {
        }

        QuaternionF CBillboardOgre::getOrientation() const
        {
            return QuaternionF::identity();
        }

        void CBillboardOgre::setScale( const Vector3F &dimensions )
        {
        }

        Vector3F CBillboardOgre::getScale() const
        {
            return Vector3F::zero();
        }

        void *CBillboardOgre::_getRenderSystemTransform() const
        {
            return nullptr;
        }

        void *CBillboardOgre::getRenderData() const
        {
            return nullptr;
        }

        void CBillboardOgre::setRenderData( void *val )
        {
        }

    }  // namespace render
}  // namespace fb
