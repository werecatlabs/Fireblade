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

            /** @copydoc IMaterial::setDirty */
            void setDirty( bool dirty );

            /** @copydoc IMaterial::isDirty */
            bool isDirty() const;

            /** @copydoc IObject::toJson */
            String toJson() const;

            /** @copydoc IObject::toData */
            SmartPtr<ISharedObject> toData() const override;

            /** @copydoc IObject::fromData */
            void fromData( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IResource::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IResource::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialStateListener : public IStateListener
            {
            public:
                MaterialStateListener();
                MaterialStateListener( CFontOgreNext *material );
                ~MaterialStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CFontOgreNext *getOwner() const;
                void setOwner( CFontOgreNext *owner );

            protected:
                CFontOgreNext *m_owner = nullptr;
            };

            void createMaterialByType();

            String m_name;

            SmartPtr<IMaterialNode> m_root;
            SmartPtr<IStateContext> m_stateContext;
            SmartPtr<IStateListener> m_stateListener;

            SmartPtr<IMaterialTechnique> m_technique;

            Array<SmartPtr<IMaterialTechnique>> m_techniques;

            hash32 m_rendererType = 0;

            s32 m_id = 0;

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
