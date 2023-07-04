#ifndef _CBillboard_H
#define _CBillboard_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IBillboard.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace render
    {
        class CBillboardOgre : public SharedObject<IBillboard>
        {
        public:
            CBillboardOgre();
            ~CBillboardOgre() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::Billboard *bb );

            void setPosition( const Vector3F &position ) override;
            Vector3F getPosition() const override;

            void setDimensions( const Vector2F &dimensions );
            Vector2F getDimensions() const;

            void setColour( const ColourF &colour ) override;
            ColourF getColour() const override;

            void _getObject( void **ppObject ) const override;

            static bool IsFree( void *element );

            void setOrientation( const QuaternionF &orientation ) override;

            QuaternionF getOrientation() const override;

            void setScale( const Vector3F &dimensions ) override;

            Vector3F getScale() const override;

            void *_getRenderSystemTransform() const override;

            void *getRenderData() const override;

            void setRenderData( void *val ) override;

        protected:
            Ogre::Billboard *m_bb = nullptr;
            Vector3F m_position;
        };
    }  // end namespace render
}  // end namespace fb

#endif
