#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/Wrapper/CBillboardOgre.h"
#include <OgreBillboard.h>

namespace fb::render
{

    //--------------------------------------------
    CBillboardOgre::CBillboardOgre() : m_bb( nullptr )
    {
    }

    //--------------------------------------------
    CBillboardOgre::~CBillboardOgre() = default;

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
    auto CBillboardOgre::getPosition() const -> Vector3F
    {
        return m_position;
    }

    //--------------------------------------------
    void CBillboardOgre::setDimensions( const Vector2F &dimensions )
    {
        // m_bb->setDimensions(dimensions.X(), dimensions.Y());
    }

    //--------------------------------------------
    auto CBillboardOgre::getDimensions() const -> Vector2F
    {
        return Vector2F::ZERO;
    }

    //--------------------------------------------
    void CBillboardOgre::setColour( const ColourF &colour )
    {
        m_bb->setColour( Ogre::ColourValue( colour.r, colour.g, colour.b, colour.a ) );
    }

    //--------------------------------------------
    auto CBillboardOgre::getColour() const -> ColourF
    {
        return ColourF::Red;
    }

    //--------------------------------------------
    void CBillboardOgre::_getObject( void **ppObject ) const
    {
        *ppObject = m_bb;
    }

    //--------------------------------------------
    auto CBillboardOgre::IsFree( void *element ) -> bool
    {
        // CBillboardPtr billboard = *(static_cast<CBillboardPtr*>(element));
        // if( billboard->getReferenceCount() == 2 )
        //	return true;

        return false;
    }

    void CBillboardOgre::setOrientation( const QuaternionF &orientation )
    {
    }

    auto CBillboardOgre::getOrientation() const -> QuaternionF
    {
        return QuaternionF::identity();
    }

    void CBillboardOgre::setScale( const Vector3F &dimensions )
    {
    }

    auto CBillboardOgre::getScale() const -> Vector3F
    {
        return Vector3F::zero();
    }

    auto CBillboardOgre::_getRenderSystemTransform() const -> void *
    {
        return nullptr;
    }

    auto CBillboardOgre::getRenderData() const -> void *
    {
        return nullptr;
    }

    void CBillboardOgre::setRenderData( void *val )
    {
    }

}  // namespace fb::render
