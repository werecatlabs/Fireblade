namespace fb
{
    //-------------------------------------------------
    inline void Grid2::calculatePosition( const Vector2I &cellIndex, Vector2F &position ) const
    {
        position.X() = ( ( static_cast<f32>(cellIndex.X()) * m_cellSize.X() ) + m_center.X() ) -
                       m_halfExtents.X();
        position.Y() = ( ( static_cast<f32>(cellIndex.Y()) * m_cellSize.Y() ) + m_center.Y() ) -
                       m_halfExtents.Y();
    }


    //-------------------------------------------------
    inline void Grid2::calculateCell( const Vector2F &position, Vector2I &cellIndex ) const
    {
        Vector2F offset = ( position - m_center ) + m_halfExtents;
        Vector2F result = offset / m_cellSize;

        const f32 tollerance = 0.0015f;
        cellIndex.set( static_cast<s32>(result.X() + tollerance),
                       static_cast<s32>(result.Y() + tollerance) );
    }


    //-------------------------------------------------
    inline void Grid2::calculateCellRound( const Vector2F &position, Vector2I &cellIndex ) const
    {
        Vector2F offset = ( position - m_center ) + m_halfExtents;
        Vector2F result = offset / m_cellSize;
        cellIndex.set( MathF::Round( result.X() ), MathF::Round( result.Y() ) );
    }
} // end namespace fb
