#ifndef _IBillboardSet_H
#define _IBillboardSet_H

#include <FBCore/Interface/Graphics/IGraphicsObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    namespace render
    {
        
        /** Manages billboards. */
        class IBillboardSet : public IGraphicsObject
        {
        public:
            ~IBillboardSet() override = default;

            /** Removes all the billboards in this set. */
            virtual void clear() = 0;

            /** Create a billboard that belongs to this set. */
            virtual SmartPtr<IBillboard> createBillboard(
                const Vector3F &position = Vector3F::zero() ) = 0;

            /** Remove a billboard thats part of this set. */
            virtual bool removeBillboard( SmartPtr<IBillboard> billboard ) = 0;

            /** Gets a list of billboards. */
            virtual Array<SmartPtr<IBillboard>> getBillboards() const = 0;

            /** Sets a boolean to cull the billboards in this set individually. */
            virtual void setCullIndividually( bool cullIndividually ) = 0;

            /** Sets whether or not sorting is enabled. */
            virtual void setSortingEnabled( bool sortingEnabled ) = 0;

            /** Sets whether or not accurate facing is used. */
            virtual void setUseAccurateFacing( bool useAccurateFacing ) = 0;

            /** Sets the bounding of the billboard set. */
            virtual void setBounds( const AABB3F &box, f32 radius ) = 0;

            /** Sets the default dimensions of the billboards in this set. */
            virtual void setDefaultDimensions( const Vector3F &dimension ) = 0;

            /** Sets the material name. */
            virtual void setMaterialName( const String &materialName, s32 index = -1 ) = 0;

            /** Gets the material name. */
            virtual String getMaterialName( s32 index = -1 ) const = 0;

            /** Sets the material to use. */
            virtual void setMaterial( SmartPtr<IMaterial> material, s32 index = -1 ) = 0;

            /** Gets the material name being used. */
            virtual SmartPtr<IMaterial> getMaterial( s32 index = -1 ) const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
