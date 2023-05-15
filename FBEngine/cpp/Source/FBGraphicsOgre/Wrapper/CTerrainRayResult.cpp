#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CTerrainRayResult.h>
#include <FBCore/Interface/Graphics/ITerrain.h>

namespace fb
{
    namespace render
    {
        CTerrainRayResult::CTerrainRayResult() : m_isIntersected( false )
        {
        }

        CTerrainRayResult::~CTerrainRayResult()
        {
        }

        bool CTerrainRayResult::hasIntersected() const
        {
            return m_isIntersected;
        }

        void CTerrainRayResult::setIntersected( bool intersected )
        {
            m_isIntersected = intersected;
        }

        SmartPtr<ITerrain> CTerrainRayResult::getTerrain() const
        {
            return m_terrain;
        }

        void CTerrainRayResult::setTerrain( SmartPtr<ITerrain> terrain )
        {
            m_terrain = terrain;
        }

        Vector3F CTerrainRayResult::getPosition() const
        {
            return m_position;
        }

        void CTerrainRayResult::setPosition( const Vector3F &position )
        {
            m_position = position;
        }
    }  // namespace render
}  // end namespace fb
