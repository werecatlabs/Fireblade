#ifndef FB_Grid2d_H
#define FB_Grid2d_H

#include <FBCore/Math/Vector2.h>

namespace fb
{
    /** Represents a 2d grid. */
    class FBCore_API Grid2
    {
    public:
        /** Default constructor*/
        Grid2();

        /** Constructor. */
        Grid2( const Vector2F &center, const Vector2F &cellSize, const Vector2F &halfExtents );

        /** Destructor*/
        ~Grid2();

        /** Calculates the position of the cell index passed. */
        void calculatePosition( const Vector2I &cellIndex, Vector2F &position ) const;

        /** Calculates the cell the position pass is in. */
        void calculateCell( const Vector2F &position, Vector2I &cellIndex ) const;

        /** Calculates the cell by rounding up. */
        void calculateCellRound( const Vector2F &position, Vector2I &cellIndex ) const;

        /** Sets the center of the grid. */
        void setCenter( const Vector2F &center );

        /** Gets the center of the grid. */
        const Vector2F &getCenter() const;

        /** Sets the extents of the grid. */
        void setExtents( const Vector2F &halfExtents );

        /** Gets the extents of the grid. */
        const Vector2F &getExtents() const;

        /** Sets the cell size. */
        void setCellSize( const Vector2F &cellSize );

        /** Gets the cell size. */
        const Vector2F &getCellSize() const;

        /** Gets the number of the cells. */
        Vector2I getNumCells() const;

    private:
        /// The grid's center position.
        Vector2F m_center;

        /// The boundaries of the grid.
        Vector2F m_halfExtents;

        /// The size of each cell.
        Vector2F m_cellSize;
    };

    inline void Grid2::calculatePosition( const Vector2I &cellIndex, Vector2F &position ) const
    {
        position.X() = ( ( static_cast<f32>( cellIndex.X() ) * m_cellSize.X() ) + m_center.X() ) -
                       m_halfExtents.X();
        position.Y() = ( ( static_cast<f32>( cellIndex.Y() ) * m_cellSize.Y() ) + m_center.Y() ) -
                       m_halfExtents.Y();
    }

    inline void Grid2::calculateCell( const Vector2F &position, Vector2I &cellIndex ) const
    {
        Vector2F offset = ( position - m_center ) + m_halfExtents;
        Vector2F result = offset / m_cellSize;

        const f32 tollerance = 0.0015f;
        cellIndex.set( static_cast<s32>( result.X() + tollerance ),
                       static_cast<s32>( result.Y() + tollerance ) );
    }

    inline void Grid2::calculateCellRound( const Vector2F &position, Vector2I &cellIndex ) const
    {
        Vector2F offset = ( position - m_center ) + m_halfExtents;
        Vector2F result = offset / m_cellSize;
        cellIndex.set( MathF::Round( result.X() ), MathF::Round( result.Y() ) );
    }

}  // namespace fb

#endif
