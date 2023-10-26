#ifndef _CBillboard_H
#define _CBillboard_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IBillboard.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        //--------------------------------------------
        class CBillboardOgre : public IBillboard
        {
        public:
            CBillboardOgre();
            ~CBillboardOgre();

            void initialise( Ogre::v1::Billboard *bb );

            void setPosition( const Vector3F &position );
            Vector3F getPosition() const;

            void setDimensions( const Vector2F &dimensions );
            Vector2F getDimensions() const;

            void setColour( const ColourF &colour );
            ColourF getColour() const;

            void _getObject( void **ppObject ) const;

            static bool IsFree( void *element );

            virtual void setOrientation( const QuaternionF &orientation );

            virtual QuaternionF getOrientation() const;

            virtual void setScale( const Vector3F &dimensions );

            virtual Vector3F getScale() const;

            virtual void *_getRenderSystemTransform() const;

            virtual void *getRenderData() const;

            virtual void setRenderData( void *val );

        protected:
            Ogre::v1::Billboard *m_bb;
            Vector3F m_position;
        };

        typedef SmartPtr<CBillboardOgre> CBillboardPtr;

    }  // namespace render
}  // end namespace fb

#endif
