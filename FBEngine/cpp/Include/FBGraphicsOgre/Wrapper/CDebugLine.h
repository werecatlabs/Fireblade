#ifndef CDebugLine_h__
#define CDebugLine_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IDebugLine.h>
#include <OgreMaterial.h>

namespace fb
{
    namespace render
    {
        class CDebugLine : public IDebugLine
        {
        public:
            CDebugLine();
            ~CDebugLine() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void preUpdate() override;

            Vector3F getVector() const override;
            void setVector( const Vector3F &val ) override;

            Vector3F getPosition() const override;
            void setPosition( const Vector3F &val ) override;

            f64 getLifeTime() const override;
            void setLifeTime( f64 val ) override;

            f64 getMaxLifeTime() const override;
            void setMaxLifeTime( f64 val ) override;

            bool isVisible() const override;
            void setVisible( bool val ) override;

            hash32 getId() const override;
            void setId( hash32 id ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            hash32 m_id = 0;

            Vector3F m_vector = Vector3F::unitY();
            Vector3F m_position = Vector3F::zero();

            RawPtr<DynamicLines> m_dynamicLines;
            RawPtr<Ogre::SceneNode> m_sceneNode;
            Ogre::MaterialPtr m_material;

            f64 m_lifeTime = 0.0;
            f64 m_maxLifeTime = 1.0;

            bool m_isDirty = true;
            bool m_isVisible = false;

            String m_materialName;

            static u32 m_nameExt;
        };
    }  // namespace render
}  // end namespace fb

#endif  // CDebugLine_h__
