#ifndef IMeshResource_h__
#define IMeshResource_h__

#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{

    /**
     * Interface for a mesh resource.
     */
    class IMeshResource : public IResource
    {
    public:
        /**
         * Enumeration for different types of material naming.
         */
        enum class MaterialNaming
        {
            MaterialName,
            TextureName,
            FileNameMaterial,

            Count
        };

        /** Destructor. */
        ~IMeshResource() override = default;

        /** Get the scaling factor for the mesh. */
        virtual f32 getScale() const = 0;

        /** Set the scaling factor for the mesh. */
        virtual void setScale( f32 scale ) = 0;

        /** Get the current material naming convention. */
        virtual MaterialNaming getMaterialNaming() const = 0;

        /** Set the material naming convention. */
        virtual void setMaterialNaming( MaterialNaming materialNaming ) = 0;

        /** Check if there are any constraints on the mesh. */
        virtual bool getConstraints() const = 0;

        /** Set the constraints on the mesh. */
        virtual void setConstraints( bool constraints ) = 0;

        /** Check if there is any animation on the mesh. */
        virtual bool getAnimation() const = 0;

        /** Set the animation flag for the mesh. */
        virtual void setAnimation( bool animation ) = 0;

        /** Check if the mesh is visible. */
        virtual bool getVisibility() const = 0;

        /** Set the visibility flag for the mesh. */
        virtual void setVisibility( bool visibility ) = 0;

        /** Check if there are any cameras attached to the mesh. */
        virtual bool getCameras() const = 0;

        /** Set the cameras flag for the mesh. */
        virtual void setCameras( bool cameras ) = 0;

        /** Check if there are any lights attached to the mesh. */
        virtual bool getLights() const = 0;

        /** Set the lights flag for the mesh. */
        virtual void setLights( bool lights ) = 0;

        /** Check if there are any lightmap UVs in the mesh. */
        virtual bool getLightmapUVs() const = 0;

        /** Set the lightmap UVs flag for the mesh. */
        virtual void setLightmapUVs( bool lightmapUVs ) = 0;

        /** Get a smart pointer to the mesh object. */
        virtual SmartPtr<IMesh> getMesh() const = 0;

        /** Set a smart pointer to the mesh object. */
        virtual void setMesh( SmartPtr<IMesh> mesh ) = 0;

        /** Get the path to the mesh file. */
        virtual String getMeshPath() const = 0;

        /** Set the path to the mesh file. */
        virtual void setMeshPath( const String &meshPath ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IMeshResource_h__
