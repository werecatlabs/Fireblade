#ifndef CTerrainRayResult_h__
#define CTerrainRayResult_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/ITerrainRayResult.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace render
    {
        class CTerrainRayResult : public CSharedObject<ITerrainRayResult>
        {
        public:
            CTerrainRayResult();
            ~CTerrainRayResult() override;

            bool hasIntersected() const override;
            void setIntersected( bool intersected ) override;

            SmartPtr<ITerrain> getTerrain() const override;
            void setTerrain( SmartPtr<ITerrain> terrain ) override;

            Vector3F getPosition() const override;
            void setPosition( const Vector3F &position ) override;

        protected:
            Vector3F m_position;
            SmartPtr<ITerrain> m_terrain;
            bool m_isIntersected;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CTerrainRayResult_h__
