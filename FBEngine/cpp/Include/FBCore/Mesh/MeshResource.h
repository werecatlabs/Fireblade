#ifndef CMeshResource__H
#define CMeshResource__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Mesh/IMeshResource.h>
#include <FBCore/Resource/Resource.h>

#include "FBCore/Atomics/AtomicFloat.h"

namespace fb
{

    /// Mesh resource implementation.
    class MeshResource : public Resource<IMeshResource>
    {
    public:
        MeshResource();
        ~MeshResource() override;

        /** @copydoc IResource::save */
        void save() override;

        /** @copydoc IResource::import */
        void import() override;

        /** @copydoc IResource::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IResource::reload */
        void reload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IResource::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IMeshResource::hasSkeleton */
        bool hasSkeleton() const;

        /** @copydoc IMeshResource::getSkeletonName */
        String getSkeletonName() const;

        /** @copydoc IMeshResource::getNumLodLevels */
        u32 getNumLodLevels() const;

        /** @copydoc IMeshResource::isEdgeListBuilt */
        bool isEdgeListBuilt() const;

        /** @copydoc IMeshResource::hasVertexAnimation */
        bool hasVertexAnimation() const;

        /** @copydoc IResource::getProperties */
        SmartPtr<Properties> getProperties() const override;

        /** @copydoc IResource::setProperties */
        void setProperties( SmartPtr<Properties> properties ) override;

        /** @copydoc IResource::getStateContext */
        SmartPtr<IStateContext> getStateContext() const override;

        /** @copydoc IResource::setStateContext */
        void setStateContext( SmartPtr<IStateContext> stateContext ) override;

        /** @copydoc IObject::_getObject */
        void _getObject( void **ppObject ) const override;

        /** @copydoc IMeshResource::getScale */
        f32 getScale() const override;

        /** @copydoc IMeshResource::setScale */
        void setScale( f32 val ) override;

        /** @copydoc IMeshResource::getMaterialNaming */
        MaterialNaming getMaterialNaming() const override;

        /** @copydoc IMeshResource::setMaterialNaming */
        void setMaterialNaming( MaterialNaming val ) override;

        /** @copydoc IMeshResource::getConstraints */
        bool getConstraints() const override;

        /** @copydoc IMeshResource::setConstraints */
        void setConstraints( bool val ) override;

        /** @copydoc IMeshResource::getAnimation */
        bool getAnimation() const override;

        /** @copydoc IMeshResource::setAnimation */
        void setAnimation( bool val ) override;

        /** @copydoc IMeshResource::getVisibility */
        bool getVisibility() const override;

        /** @copydoc IMeshResource::setVisibility */
        void setVisibility( bool val ) override;

        /** @copydoc IMeshResource::getCameras */
        bool getCameras() const override;

        /** @copydoc IMeshResource::setCameras */
        void setCameras( bool val ) override;

        /** @copydoc IMeshResource::getLights */
        bool getLights() const override;

        /** @copydoc IMeshResource::setLights */
        void setLights( bool val ) override;

        /** @copydoc IMeshResource::getLightmapUVs */
        bool getLightmapUVs() const override;

        /** @copydoc IMeshResource::setLightmapUVs */
        void setLightmapUVs( bool val ) override;

        /** @copydoc IMeshResource::getMesh */
        SmartPtr<IMesh> getMesh() const override;

        /** @copydoc IMeshResource::setMesh */
        void setMesh( SmartPtr<IMesh> val ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<IStateContext> m_stateContext;
        SmartPtr<IMesh> m_mesh;

        atomic_f32 m_scale = 1.0f;

        MaterialNaming m_materialNaming = MaterialNaming::MaterialName;

        atomic_bool m_constraints = true;
        atomic_bool m_animation = true;
        atomic_bool m_visibility = true;
        atomic_bool m_cameras = true;
        atomic_bool m_lights = true;

        atomic_bool m_lightmapUVs = false;

        atomic_bool m_hasSharedVertexData = false;
    };
}  // end namespace fb

#endif
