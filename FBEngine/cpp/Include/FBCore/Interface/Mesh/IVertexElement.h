#ifndef __IVertexElement_h__
#define __IVertexElement_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief A description of a single element in a vertex buffer.
     *
     * This class describes the format of a single element in a vertex buffer,
     * including the data type, offset, and source of the element within the buffer.
     */
    class IVertexElement : public ISharedObject
    {
    public:
        /**
         * @brief An enumeration of the types of data that can be stored in a vertex element.
         *
         * This enumeration identifies the base types of the vertex contents, such as
         * floating-point values, integers, or color values.
         */
        enum class VertexElementType
        {
            VET_FLOAT1 = 0,
            VET_FLOAT2 = 1,
            VET_FLOAT3 = 2,
            VET_FLOAT4 = 3,

            VET_SHORT1 = 5,
            ///< @deprecated (see #VertexElementType note)
            VET_SHORT2 = 6,
            VET_SHORT3 = 7,
            ///< @deprecated (see #VertexElementType note)
            VET_SHORT4 = 8,
            VET_UBYTE4 = 9,
            _DETAIL_SWAP_RB = 10,

            // the following are not universally supported on all hardware:
            VET_DOUBLE1 = 12,
            VET_DOUBLE2 = 13,
            VET_DOUBLE3 = 14,
            VET_DOUBLE4 = 15,
            VET_USHORT1 = 16,
            ///< @deprecated (see #VertexElementType note)
            VET_USHORT2 = 17,
            VET_USHORT3 = 18,
            ///< @deprecated (see #VertexElementType note)
            VET_USHORT4 = 19,
            VET_INT1 = 20,
            VET_INT2 = 21,
            VET_INT3 = 22,
            VET_INT4 = 23,
            VET_UINT1 = 24,
            VET_UINT2 = 25,
            VET_UINT3 = 26,
            VET_UINT4 = 27,
            VET_BYTE4 = 28,
            /// signed bytes
            VET_BYTE4_NORM = 29,
            /// signed bytes (normalized to -1..1)
            VET_UBYTE4_NORM = 30,
            /// unsigned bytes (normalized to 0..1)
            VET_SHORT2_NORM = 31,
            /// signed shorts (normalized to -1..1)
            VET_SHORT4_NORM = 32,
            VET_USHORT2_NORM = 33,
            /// unsigned shorts (normalized to 0..1)
            VET_USHORT4_NORM = 34,
            VET_COLOUR = VET_UBYTE4_NORM,
            ///< @deprecated use VET_UBYTE4_NORM
            VET_COLOUR_ARGB = VET_UBYTE4_NORM,
            ///< @deprecated use VET_UBYTE4_NORM
            VET_COLOUR_ABGR = VET_UBYTE4_NORM,
            ///< @deprecated use VET_UBYTE4_NORM
        };

        /** Destructor. */
        ~IVertexElement() override = default;

        /**
         * Copies the data for this element from the given vertex buffer to the given element buffer.
         *
         * @param vertexData The vertex buffer.
         * @param elementData The element buffer.
         */
        virtual void getElementData( void *vertexData, void **elementData ) const = 0;

        /**
         * Copies the data for this element from the given vertex buffer to the given element buffer.
         *
         * @param vertexData The vertex buffer.
         * @param elementData The element buffer.
         */
        virtual void getElementData( void *vertexData, f32 **elementData ) const = 0;

        /**
         * Gets the index of the buffer that this element comes from.
         *
         * @return The buffer index.
         */
        virtual u16 getSource() const = 0;

        /**
         * Sets the index of the buffer that this element comes from.
         *
         * @param source The buffer index.
         */
        virtual void setSource( u16 source ) = 0;

        /**
         * Gets the size of this element.
         *
         * @return The size of this element.
         */
        virtual u32 getSize() const = 0;

        /**
         * Sets the size of this element.
         *
         * @param size The size of this element.
         */
        virtual void setSize( u32 size ) = 0;

        /**
         * Gets the offset of this element from the start of the vertex buffer.
         *
         * @return The offset of this element.
         */
        virtual u32 getOffset() const = 0;

        /**
         * Sets the offset of this element from the start of the vertex buffer.
         *
         * @param offset The offset of this element.
         */
        virtual void setOffset( u32 offset ) = 0;

        /**
         * Gets the semantic of this element.
         *
         * @return The semantic of this element.
         */
        virtual u32 getSemantic() const = 0;

        /**
         * Sets the semantic of this element.
         *
         * @param semantic The semantic of this element.
         */
        virtual void setSemantic( u32 semantic ) = 0;

        /**
         * Gets the type of this element.
         *
         * @return The type of this element.
         */
        virtual VertexElementType getType() const = 0;

        /**
         * Sets the type of this element.
         *
         * @param type The type of this element.
         */
        virtual void setType( VertexElementType type ) = 0;

        /**
         * Gets the index of this element.
         *
         * @return The index of this element.
         */
        virtual u8 getIndex() const = 0;

        /**
         * Sets the index of this element.
         *
         * @param index The index of this element.
         */
        virtual void setIndex( u8 index ) = 0;

        /**
         * Compares this element to another element to see if they are equal.
         *
         * @param other The element to compare to.
         * @return True if the elements are equal, false otherwise.
         */
        virtual bool compare( SmartPtr<IVertexElement> other ) const = 0;

        /**
         * Converts a pointer to the base vertex buffer into a pointer to this element.
         *
         * @tparam T The type of the element.
         * @param pBase Pointer to the base vertex buffer.
         * @param pElem Pointer to the pointer to the element.
         */
        template <typename T>
        void baseVertexPointerToElement( void *pBase, T **pElem ) const
        {
            auto offset = getOffset();
            *pElem = reinterpret_cast<T *>( static_cast<char *>( pBase ) + offset );
        }

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IVertexElement_h__
