#ifndef _CDynamicMesh_H_
#define _CDynamicMesh_H_

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IDynamicMesh.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.h>
#include <DynamicMesh.h>

namespace fb
{
    namespace render
    {

        class CDynamicMesh : public CGraphicsObjectOgre<IDynamicMesh>
        {
        public:
            CDynamicMesh() = default;
            CDynamicMesh( SmartPtr<IGraphicsScene> creator );
            ~CDynamicMesh() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;
                        
            void setMesh( SmartPtr<IMesh> mesh );
            SmartPtr<IMesh> getMesh() const;

            void setSubMesh( SmartPtr<ISubMesh> subMesh ) override;
            SmartPtr<ISubMesh> getSubMesh() const override;

            void setDirty( bool dirty ) override;

            void setOwner( SmartPtr<ISceneNode> owner ) override;

            void _getObject( void **ppObject ) const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            DynamicMesh *m_dynamicMesh = nullptr;
            Array<DynamicMesh *> m_subMeshes;
        };
    }  // end namespace render
}  // end namespace fb

#endif
