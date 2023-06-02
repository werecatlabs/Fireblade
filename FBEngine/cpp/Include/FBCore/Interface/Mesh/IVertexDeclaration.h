#ifndef __IVertexDeclaration_h__
#define __IVertexDeclaration_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Interface/Mesh/IVertexElement.h>

namespace fb
{

    /**
     * @brief An interface for a vertex declaration. Defines the layout of vertex buffer data.
     */
    class IVertexDeclaration : public ISharedObject
    {
    public:
        /**
         * @brief Vertex element semantics, used to identify the meaning of vertex buffer contents.
         */
        enum class VertexElementSemantic
        {
            /** Position, 3 reals per vertex. */
            VES_POSITION = 1,
            /** Blending weights. */
            VES_BLEND_WEIGHTS = 2,
            /** Blending indices. */
            VES_BLEND_INDICES = 3,
            /** Normal, 3 reals per vertex. */
            VES_NORMAL = 4,
            /** Diffuse colours. */
            VES_DIFFUSE = 5,
            /** Specular colours. */
            VES_SPECULAR = 6,
            /** Texture coordinates. */
            VES_TEXTURE_COORDINATES = 7,
            /** Binormal (Y axis if normal is Z). */
            VES_BINORMAL = 8,
            /** Tangent (X axis if normal is Z). */
            VES_TANGENT = 9,
            /** Other vertex element semantics. */
            VES_OTHER = 10
        };

        /** Virtual destructor. */
        ~IVertexDeclaration() override = default;

        /**
         * @brief Adds a new vertex element to the vertex declaration.
         * @param source The source vertex buffer to bind the element to.
         * @param offset The offset in bytes to the start of the element data.
         * @param elementSemantic The semantic of the vertex element.
         * @param elementType The type of data in the vertex element.
         * @param index The index of the element if there are multiple with the same semantic.
         * @return A smart pointer to the newly created vertex element.
         */
        virtual SmartPtr<IVertexElement> addElement( u16 source, u32 offset,
                                                     VertexElementSemantic elementSemantic,
                                                     IVertexElement::VertexElementType elementType,
                                                     u32 index = 0 ) = 0;

        /**
         * @brief Gets the total size of the vertex declaration in bytes.
         * @param source The vertex buffer source to get the size for.
         * @return The size of the vertex declaration in bytes.
         */
        virtual u32 getSize( u16 source = 0 ) const = 0;

        /**
         * @brief Finds the vertex element with the given semantic and index.
         * @param elementSemantic The semantic of the vertex element to find.
         * @param index The index of the element if there are multiple with the same semantic.
         * @return A smart pointer to the found vertex element, or nullptr if it doesn't exist.
         */
        virtual SmartPtr<IVertexElement> findElementBySemantic( VertexElementSemantic elementSemantic,
                                                                u32 index = 0 ) = 0;

        /**
         * @brief Finds all the vertex elements that use a given vertex buffer source.
         * @param source The vertex buffer source to find elements for.
         * @return An array of smart pointers to the vertex elements that use the given source.
         */
        virtual Array<SmartPtr<IVertexElement>> findElementsBySource( u16 source ) const = 0;

        /**
         * @brief Creates a copy of the vertex declaration.
         * @return A smart pointer to the cloned vertex declaration.
         */
        virtual SmartPtr<IVertexDeclaration> clone() const = 0;

        /**
         * @brief Gets an array of all the vertex elements in the vertex declaration.
         * @return A reference to the array
         */
        virtual Array<SmartPtr<IVertexElement>> &getElements() = 0;

        /**
         * @brief Gets an array of all the vertex elements in the vertex declaration.
         * @return A reference to the array
         */
        virtual const Array<SmartPtr<IVertexElement>> &getElements() const = 0;

        /** @brief Compare a vertex declaration. */
        virtual bool compare( SmartPtr<IVertexDeclaration> other ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IVertexDeclaration_h__
