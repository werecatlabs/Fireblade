namespace fb
{
    //-------------------------------------------------
    inline void Grid3::calculatePosition( const Vector3I &cellIdx, Vector3F &position )
    {
        position.X() = ( cellIdx.X() * m_cellSize.X() ) - m_gridExtents.X();
        position.Y() = ( cellIdx.Y() * m_cellSize.Y() ) - m_gridExtents.Y();
        position.Z() = ( cellIdx.Z() * m_cellSize.Z() ) - m_gridExtents.Z();
    }


    //-------------------------------------------------
    inline void Grid3::calculateGridSlot( const Vector3F &position, Vector3I &cellIndex ) const
    {
        Vector3F offset = ( position - m_center ) + m_gridExtents;
        Vector3F result = offset / m_cellSize;
        cellIndex.set( static_cast<s32>(result.X()), static_cast<s32>(result.Y()),
                       static_cast<s32>(result.Z()) );
    }


    //-------------------------------------------------
    inline AABB3F Grid3::getCellBox( const Vector3I &cellIdx ) const
    {
        Vector3F position;
        position.X() = ( cellIdx.X() * m_cellSize.X() ) - m_gridExtents.X();
        position.Y() = ( cellIdx.Y() * m_cellSize.Y() ) - m_gridExtents.Y();
        position.Z() = ( cellIdx.Z() * m_cellSize.Z() ) - m_gridExtents.Z();

        auto halfCellSize = m_cellSize * 0.5f;
        auto minimum = position - halfCellSize;
        auto maximum = position + halfCellSize;

        return AABB3F( minimum, maximum );
    }
} // end namespace fb
