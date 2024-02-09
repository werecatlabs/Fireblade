#ifndef FB_Grid3_H
#define FB_Grid3_H

#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{

    /** */
    class FBCore_API Grid3
    {
    public:
        /** */
        Grid3();

        /** */
        ~Grid3();

        /** */
        void initialise( const Vector3<real_Num> &center, const Vector3<real_Num> &cellSize, const Vector3<real_Num> &halfExtents );

        /** */
        void calculatePosition( const Vector3I &cellIdx, Vector3<real_Num> &position );

        /** */
        void calculateGridSlot( const Vector3<real_Num> &position, Vector3I &cellIndex ) const;

        /** */
        AABB3<real_Num> getCellBox( const Vector3I &cellIdx ) const;

        /** */
        const Vector3<real_Num> &getCenter() const;

        /** */
        const Vector3<real_Num> &getExtents() const;

        /** */
        const Vector3<real_Num> &getCellSize() const;

        /** */
        Vector3I getNumCells() const;

    private:
        /// The grid's m_center position.
        Vector3<real_Num> m_center;

        /// The boundaries of the grid.
        Vector3<real_Num> m_gridExtents;

        /// The size of each cell.
        Vector3<real_Num> m_cellSize;
    };

    inline void Grid3::calculatePosition( const Vector3I &cellIdx, Vector3<real_Num> &position )
    {
        position.X() = ( cellIdx.X() * m_cellSize.X() ) - m_gridExtents.X();
        position.Y() = ( cellIdx.Y() * m_cellSize.Y() ) - m_gridExtents.Y();
        position.Z() = ( cellIdx.Z() * m_cellSize.Z() ) - m_gridExtents.Z();
    }

    inline void Grid3::calculateGridSlot( const Vector3<real_Num> &position, Vector3I &cellIndex ) const
    {
        Vector3<real_Num> offset = ( position - m_center ) + m_gridExtents;
        Vector3<real_Num> result = offset / m_cellSize;
        cellIndex.set( static_cast<s32>( result.X() ), static_cast<s32>( result.Y() ),
                       static_cast<s32>( result.Z() ) );
    }

    inline AABB3<real_Num> Grid3::getCellBox( const Vector3I &cellIdx ) const
    {
        Vector3<real_Num> position;
        position.X() = ( cellIdx.X() * m_cellSize.X() ) - m_gridExtents.X();
        position.Y() = ( cellIdx.Y() * m_cellSize.Y() ) - m_gridExtents.Y();
        position.Z() = ( cellIdx.Z() * m_cellSize.Z() ) - m_gridExtents.Z();

        auto halfCellSize = m_cellSize * 0.5f;
        auto minimum = position - halfCellSize;
        auto maximum = position + halfCellSize;

        return AABB3<real_Num>( minimum, maximum );
    }

}  // end namespace fb

#endif
