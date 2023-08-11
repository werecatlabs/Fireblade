#include <FBCore/FBCorePCH.h>
#include "FBCore/Graphics/CGraphicsMesh.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        CGraphicsMesh::CGraphicsMesh()
        {
        }

        CGraphicsMesh::~CGraphicsMesh()
        {
        }

        void CGraphicsMesh::setMaterialName( const String &materialName, s32 index /*= -1 */ )
        {
        }

        String CGraphicsMesh::getMaterialName( s32 index /*= -1 */ ) const
        {
            return String();
        }

        void CGraphicsMesh::setMaterial( SmartPtr<IMaterial> material, s32 index /*= -1 */ )
        {
        }

        SmartPtr<IMaterial> CGraphicsMesh::getMaterial( s32 index /*= -1 */ ) const
        {
            return nullptr;
        }

        void CGraphicsMesh::setHardwareAnimationEnabled( bool enabled )
        {
        }

        void CGraphicsMesh::checkVertexProcessing()
        {
        }

        SmartPtr<IAnimationController> CGraphicsMesh::getAnimationController()
        {
            return nullptr;
        }

        String CGraphicsMesh::getMeshName() const
        {
            return String();
        }

        void CGraphicsMesh::setMeshName( const String &meshName )
        {
        }

    } // namespace render
}     // namespace fb
