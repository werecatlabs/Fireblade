#ifndef __CFont_h__
#define __CFont_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IFont.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Graphics/ResourceGraphics.h>
#include <OgreFont.h>

namespace fb
{
    namespace render
    {
        class CFontOgreNext : public ResourceGraphics<IFont>
        {
        public:
            static const hash_type SET_TEXTURE_HASH;
            static const hash_type FRAGMENT_FLOAT_HASH;
            static const hash_type FRAGMENT_VECTOR2F_HASH;
            static const hash_type FRAGMENT_Vector3f_HASH;
            static const hash_type FRAGMENT_VECTOR4F_HASH;
            static const hash_type FRAGMENT_COLOUR_HASH;

            CFontOgreNext();
            ~CFontOgreNext() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IStateContext> getStateContext() const override;
            void setStateContext( SmartPtr<IStateContext> stateContext ) override;

            String getName() const;
            void setName( const String &name );

            hash32 getId() const;

            hash32 getRendererType() const;
            void setRendererType( hash32 rendererType );

            /** @copydoc IResource::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IResource::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            hash32 m_rendererType = 0;

            s32 m_id = 0;

            String m_name;
            String font_type;
            String font_source;
            u32 font_size = 12;
            u32 font_resolution = 96;

            Ogre::FontPtr m_font;

            static u32 m_nameExt;
        };
    } // end namespace render
}     // end namespace fb

#endif  // IMaterial_h__
