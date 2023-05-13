#ifndef CMaterialTechnique_h__
#define CMaterialTechnique_h__

#include <FBGraphics/Wrapper/CMaterialNode.h>
#include <FBCore/Interface/Graphics/IMaterialTechnique.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace render
    {
        class CMaterialTechnique : public CMaterialNode<IMaterialTechnique>
        {
        public:
            CMaterialTechnique();
            ~CMaterialTechnique() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            hash32 getScheme() const override;
            void setScheme( hash32 scheme ) override;

            u32 getNumPasses() const;

            SmartPtr<IMaterialPass> createPass() override;
            void addPass( SmartPtr<IMaterialPass> pass ) override;
            void removePass( SmartPtr<IMaterialPass> pass ) override;
            void removePasses() override;

            Array<SmartPtr<IMaterialPass>> getPasses() const override;

            void setPasses( Array<SmartPtr<IMaterialPass>> passes );

            SharedPtr<Array<SmartPtr<IMaterialPass>>> getPassesPtr() const;

            void setPassesPtr( SharedPtr<Array<SmartPtr<IMaterialPass>>> ptr );

            /** @copydoc IObject::toJson */
            String toJson() const override;

            /** @copydoc IObject::toData */
            SmartPtr<IData> toData() const override;

            /** @copydoc IObject::fromData */
            void fromData( SmartPtr<IData> data ) override;

            /** @copydoc IResource::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IResource::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialTechniqueStateListener : public MaterialNodeStateListener
            {
            public:
                MaterialTechniqueStateListener() = default;
                ~MaterialTechniqueStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };
            
            hash32 m_scheme = 0;
            SharedPtr<Array<SmartPtr<IMaterialPass>>> m_passes;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CTechnique_h__
