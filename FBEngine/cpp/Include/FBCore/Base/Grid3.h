#ifndef FB_Grid3_H
#define FB_Grid3_H

#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    //-------------------------------------------------
    /** */
    class FBCore_API Grid3
    {
    public:
        /** */
        Grid3();

        /** */
        ~Grid3();

        /** */
        void initialise( const Vector3F &center, const Vector3F &cellSize, const Vector3F &halfExtents );

        /** */
        void calculatePosition( const Vector3I &cellIdx, Vector3F &position );

        /** */
        void calculateGridSlot( const Vector3F &position, Vector3I &cellIndex ) const;

        /** */
        AABB3F getCellBox( const Vector3I &cellIdx ) const;

        /** */
        const Vector3F &getCenter() const;

        /** */
        const Vector3F &getExtents() const;

        /** */
        const Vector3F &getCellSize() const;

        /** */
        Vector3I getNumCells() const;

    private:
        /// The grid's m_center position.
        Vector3F m_center;

        /// The boundaries of the grid.
        Vector3F m_gridExtents;

        /// The size of each cell.
        Vector3F m_cellSize;
    };
}  // end namespace fb

#include <FBCore/Base/Grid3.inl>

#endif
