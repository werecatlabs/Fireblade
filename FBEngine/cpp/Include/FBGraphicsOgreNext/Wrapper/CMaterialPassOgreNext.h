#ifndef CMaterialPassOgreNext_h__
#define CMaterialPassOgreNext_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IMaterialPass.h>
#include <FBCore/Graphics/MaterialPass.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace render
    {

        class CMaterialPassOgreNext : public MaterialPass
        {
        public:
            CMaterialPassOgreNext();
            ~CMaterialPassOgreNext() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::initialise */
            void initialise( Ogre::Pass *pass );

            /** @copydoc IMaterialPass::toJson */
            SmartPtr<IMaterialTexture> createTextureUnit() override;

            void setTexture( SmartPtr<ITexture> texture, u32 layerIdx = 0 );

            /** @copydoc IMaterialPass::toJson */
            void setTexture( const String &fileName, u32 layerIdx = 0 ) override;

            Ogre::Pass *getPass() const;
            void setPass( Ogre::Pass *pass );

            Ogre::HlmsDatablock *getDataBlock() const;

            void setupMaterial();

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialPassStateListener : public IStateListener
            {
            public:
                MaterialPassStateListener();
                MaterialPassStateListener( CMaterialPassOgreNext *material );
                ~MaterialPassStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CMaterialPassOgreNext *getOwner() const;
                void setOwner( CMaterialPassOgreNext *owner );

            protected:
                CMaterialPassOgreNext *m_owner = nullptr;
            };

            void createTextureSlots();

            Ogre::Pass *m_pass = nullptr;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CPass_h__
