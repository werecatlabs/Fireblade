#ifndef CMaterialTechniqueOgre_h__
#define CMaterialTechniqueOgre_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Graphics/CMaterialTechnique.h>
#include <FBCore/Interface/Graphics/IMaterialTechnique.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace render
    {
        class CMaterialTechniqueOgre : public CMaterialTechnique
        {
        public:
            CMaterialTechniqueOgre();
            ~CMaterialTechniqueOgre() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::Technique *technique );

            /** @copydoc IResource::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IResource::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            Ogre::Technique *getTechnique() const;

            void setTechnique( Ogre::Technique *technique );

            void addPass( SmartPtr<IMaterialPass> pass );

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialTextureOgreStateListener : public MaterialTechniqueStateListener
            {
            public:
                MaterialTextureOgreStateListener() = default;
                ~MaterialTextureOgreStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            Ogre::Technique *m_technique = nullptr;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CTechnique_h__
