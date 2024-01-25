#ifndef CMaterialTechniqueOgreNext_h__
#define CMaterialTechniqueOgreNext_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IMaterialTechnique.h>
#include <FBCore/Graphics/MaterialTechnique.h>

namespace fb
{
    namespace render
    {
        class CMaterialTechniqueOgreNext : public MaterialTechnique
        {
        public:
            CMaterialTechniqueOgreNext();
            ~CMaterialTechniqueOgreNext() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::Technique *technique );

            hash32 getScheme() const override;
            void setScheme( hash32 val ) override;

            SmartPtr<IMaterialPass> createPass() override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            Ogre::Technique *getTechnique() const;

            void setTechnique( Ogre::Technique *technique );

            FB_CLASS_REGISTER_DECL;

        protected:
            Ogre::Technique *m_technique = nullptr;
            hash32 m_scheme = 0;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CTechnique_h__
