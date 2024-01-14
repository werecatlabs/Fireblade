#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/VertexDeclaration.h>
#include <FBCore/FBCore.h>
#include <FBCore/Mesh/VertexElement.h>

namespace fb
{
    VertexDeclaration::VertexDeclaration()
    {
        m_elements.reserve( 4 );
    }

    VertexDeclaration::~VertexDeclaration()
    {
        unload( nullptr );
    }

    void VertexDeclaration::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            for( auto e : m_elements )
            {
                if( e )
                {
                    e->unload( nullptr );
                }
            }

            m_elements.clear();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto VertexDeclaration::addElement( u16 source, u32 offset, VertexElementSemantic elementSemantic,
                                        IVertexElement::VertexElementType elementType,
                                        u32 index /*= 0*/ ) -> SmartPtr<IVertexElement>
    {
#ifdef _DEBUG
        for( u32 i = 0; i < m_elements.size(); ++i )
        {
            const SmartPtr<IVertexElement> &vertexElement = m_elements[i];
            if( vertexElement->getSemantic() == static_cast<u32>( elementSemantic ) &&
                vertexElement->getIndex() == index )
            {
                FB_ASSERT( false );
            }
        }
#endif

        u32 size = 0;

        switch( elementType )
        {
        case IVertexElement::VertexElementType::VET_FLOAT1:
            size = sizeof( float ) * 1;
            break;
        case IVertexElement::VertexElementType::VET_FLOAT2:
            size = sizeof( float ) * 2;
            break;
        case IVertexElement::VertexElementType::VET_FLOAT3:
            size = sizeof( float ) * 3;
            break;
        case IVertexElement::VertexElementType::VET_FLOAT4:
            size = sizeof( float ) * 4;
            break;
        /// alias to more specific colour type - use the current rendersystem's colour packing
        // case IVertexElement::VET_COLOUR:
        //	size = sizeof(u32);
        //	break;
        case IVertexElement::VertexElementType::VET_SHORT1:
            size = sizeof( u16 );
            break;
        case IVertexElement::VertexElementType::VET_SHORT2:
            size = sizeof( u16 ) * 2;
            break;
        case IVertexElement::VertexElementType::VET_SHORT3:
            size = sizeof( u16 ) * 3;
            break;
        case IVertexElement::VertexElementType::VET_SHORT4:
            size = sizeof( u16 ) * 4;
            break;
        case IVertexElement::VertexElementType::VET_UBYTE4:
            size = sizeof( u32 ) * 4;
            break;
#if FB_ENDIAN == FB_ENDIAN_LITTLE
        case IVertexElement::VertexElementType::VET_COLOUR_ARGB:
            size = sizeof( float ) * 4;
            break;
#else
        case IVertexElement::VertexElementType::VET_COLOUR_ABGR:
            size = sizeof( float ) * 4;
            break;
#endif
        }

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto element = factoryManager->make_ptr<VertexElement>();
        element->setSource( source );
        element->setSize( size );
        element->setOffset( offset );
        element->setSemantic( static_cast<u32>( elementSemantic ) );
        element->setType( elementType );
        element->setIndex( index );

        m_elements.emplace_back( element );
        return element;
    }

    auto VertexDeclaration::getSize( u16 source ) const -> u32
    {
        u32 sz = 0;

        for( auto &element : m_elements )
        {
            if( element->getSource() == source )
            {
                sz += element->getSize();
            }
        }

        return sz;
    }

    auto VertexDeclaration::findElementBySemantic( VertexElementSemantic elementSemantic, u32 index )
        -> SmartPtr<IVertexElement>
    {
        for( auto &element : m_elements )
        {
            if( element->getSemantic() == static_cast<u32>( elementSemantic ) &&
                element->getIndex() == index )
            {
                return element;
            }
        }

        return nullptr;
    }

    auto VertexDeclaration::findElementsBySource( u16 source ) const -> Array<SmartPtr<IVertexElement>>
    {
        Array<SmartPtr<IVertexElement>> retList;
        retList.reserve( m_elements.size() );

        for( auto element : m_elements )
        {
            if( element->getSource() == source )
            {
                retList.push_back( element );
            }
        }
        return retList;
    }

    auto VertexDeclaration::clone() const -> SmartPtr<IVertexDeclaration>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto vertexDec = factoryManager->make_ptr<VertexDeclaration>();
        vertexDec->m_elements = m_elements;

        return vertexDec;
    }

    auto VertexDeclaration::getElements() const -> const Array<SmartPtr<IVertexElement>> &
    {
        return m_elements;
    }

    auto VertexDeclaration::getElements() -> Array<SmartPtr<IVertexElement>> &
    {
        return m_elements;
    }

    auto VertexDeclaration::compare( SmartPtr<IVertexDeclaration> other ) const -> bool
    {
        auto otherElements = other->getElements();
        if( m_elements.size() != otherElements.size() )
        {
            return false;
        }

        for( auto element : m_elements )
        {
            for( auto otherElement : otherElements )
            {
                if( !element->compare( otherElement ) )
                {
                    return false;
                }
            }
        }

        return true;
    }
}  // end namespace fb
