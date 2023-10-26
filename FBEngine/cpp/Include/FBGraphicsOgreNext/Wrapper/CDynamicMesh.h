#ifndef _CDynamicMesh_H_
#define _CDynamicMesh_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IDynamicMesh.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsObjectOgreNext.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        
        class CDynamicMesh : public CGraphicsObjectOgreNext<IDynamicMesh>
        {
        public:
            CDynamicMesh( SmartPtr<render::IGraphicsScene> creator );
            ~CDynamicMesh();

            //
            // IGraphicsObject functions
            //
            void setName( const String &name );
            String getName() const;
            u32 getId() const;

            void setMaterialName( const String &materialName, s32 index = -1 );
            String getMaterialName( s32 index = -1 ) const;

            void setCastShadows( bool castShadows );
            bool getCastShadows() const;

            void setRecieveShadows( bool recieveShadows );
            bool getRecieveShadows() const;

            void setVisible( bool isVisible );
            bool isVisible() const;

            void setRenderQueueGroup( u8 renderQueue );

            void setVisibilityFlags( u32 flags );
            u32 getVisibilityFlags() const;

            SmartPtr<IGraphicsObject> clone( const String &name = StringUtil::EmptyString ) const;

            void _getObject( void **ppObject ) const;

            const String &getComponentType() const;

            //
            // IDynamicMesh functions
            //
            virtual void setMesh( SmartPtr<IMesh> mesh );
            virtual SmartPtr<IMesh> getMesh() const;

            void setSubMesh( SmartPtr<ISubMesh> subMesh );
            SmartPtr<ISubMesh> getSubMesh() const;

            void setDirty(bool dirty);

            void setOwner( SmartPtr<ISceneNode> owner );

        protected:
            DynamicMesh *m_dynamicMesh = nullptr;

            SmartPtr<render::IGraphicsScene> m_creator;
            String m_name;
        };

    }  // end namespace render
}  // end namespace fb

#endif
