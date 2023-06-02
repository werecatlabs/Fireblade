#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/Grid3.h>

namespace fb
{

    Grid3::Grid3()
    {
    }

    Grid3::~Grid3()
    {
    }

    void Grid3::initialise( const Vector3F &center, const Vector3F &areaSize,
                            const Vector3F &areaBoundry )
    {
        m_center.X() = center.X();
        m_center.Y() = center.Y();
        m_center.Z() = center.Z();

        m_cellSize.X() = areaSize.X();
        m_cellSize.Y() = areaSize.Y();
        m_cellSize.Z() = areaSize.Z();

        m_gridExtents.X() = areaBoundry.X();
        m_gridExtents.Y() = areaBoundry.Y();
        m_gridExtents.Z() = areaBoundry.Z();

        /*
        m_numNodes = 0;

        m_numBoundingboxes.X() = (int)((m_gridExtents.X() / m_cellSize.X())*2.0f);
        m_numBoundingboxes.Y() = (int)((m_gridExtents.Y() / m_cellSize.Y())*2.0f);
        m_numBoundingboxes.Z() = (int)((m_gridExtents.Z() / m_cellSize.Z())*2.0f);

        f64 as = 200.0;
        f64 invas = 1.0/as;
        m_invAreaSize.X() = 1.0/(f64)m_cellSize.X();
        m_invAreaSize.Y() = 1.0/(f64)m_cellSize.Y();
        m_invAreaSize.Z() = 1.0/(f64)m_cellSize.Z();

        m_maxIndex.X() = m_numBoundingboxes.X() - 1;
        m_maxIndex.Y() = m_numBoundingboxes.Y() - 1;
        m_maxIndex.Z() = m_numBoundingboxes.Z() - 1;*/
    }

    const Vector3F &Grid3::getCenter() const
    {
        return m_center;
    }

    const Vector3F &Grid3::getExtents() const
    {
        return m_gridExtents;
    }

    const Vector3F &Grid3::getCellSize() const
    {
        return m_cellSize;
    }

    Vector3I Grid3::getNumCells() const
    {
        Vector3F numCells = ( m_gridExtents / m_cellSize ) * 2.0f;
        return Vector3I( MathF::Round( numCells.X() ), MathF::Round( numCells.Y() ),
                         MathF::Round( numCells.Z() ) );
    }
}  // end namespace fb
