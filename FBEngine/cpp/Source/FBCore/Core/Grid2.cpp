#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/Grid2.h>

namespace fb
{

    Grid2::Grid2()
    {
    }

    Grid2::Grid2( const Vector2F &center, const Vector2F &areaSize, const Vector2F &areaBoundry )
    {
        m_center.X() = center.X();
        m_center.Y() = center.Y();

        m_cellSize.X() = areaSize.X();
        m_cellSize.Y() = areaSize.Y();

        m_halfExtents.X() = areaBoundry.X();
        m_halfExtents.Y() = areaBoundry.Y();

        /*m_numNodes = 0;

        m_numBoundingboxes.X() = (int)((m_areaBoundry.X() / m_areaSize.X())*2.0f);
        m_numBoundingboxes.Y() = (int)((m_areaBoundry.Y() / m_areaSize.Y())*2.0f);

        f64 as = 200.0;
        f64 invas = 1.0/as;
        m_invAreaSize.X() = 1.0/(f64)m_areaSize.X();
        m_invAreaSize.Y() = 1.0/(f64)m_areaSize.Y();

        m_maxIndex.X() = m_numBoundingboxes.X() - 1;
        m_maxIndex.Y() = m_numBoundingboxes.Y() - 1;*/
    }

    Grid2::~Grid2()
    {
    }

    void Grid2::setCenter( const Vector2F &center )
    {
        m_center = center;
    }

    const Vector2F &Grid2::getCenter() const
    {
        return m_center;
    }

    void Grid2::setExtents( const Vector2F &halfExtents )
    {
        m_halfExtents = halfExtents;
    }

    const Vector2F &Grid2::getExtents() const
    {
        return m_halfExtents;
    }

    void Grid2::setCellSize( const Vector2F &cellSize )
    {
        m_cellSize = cellSize;
    }

    const Vector2F &Grid2::getCellSize() const
    {
        return m_cellSize;
    }

    Vector2I Grid2::getNumCells() const
    {
        Vector2F numCells = ( m_halfExtents / m_cellSize ) * 2.0f;
        return Vector2I( MathF::Round( numCells.X() ), MathF::Round( numCells.Y() ) );
    }
}  // end namespace fb
