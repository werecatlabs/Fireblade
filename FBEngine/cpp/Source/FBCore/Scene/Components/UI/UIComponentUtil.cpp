#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/UIComponentUtil.h>

namespace fb::scene
{

    auto UIComponentUtil::getHorizontalAlignmentString(
        LayoutTransform::HorizontalAlignment horizontalAlignment ) -> String
    {
        switch( horizontalAlignment )
        {
        case LayoutTransform::HorizontalAlignment::LEFT:
            return String( "Left" );
        case LayoutTransform::HorizontalAlignment::CENTER:
            return String( "Center" );
        case LayoutTransform::HorizontalAlignment::RIGHT:
            return String( "Right" );
        default:
            break;
        }

        return String( "" );
    }

    auto UIComponentUtil::getHorizontalAlignment( const String &str )
        -> LayoutTransform::HorizontalAlignment
    {
        if( str == String( "Left" ) )
        {
            return LayoutTransform::HorizontalAlignment::LEFT;
        }
        if( str == String( "Center" ) )
        {
            return LayoutTransform::HorizontalAlignment::CENTER;
        }
        if( str == String( "Right" ) )
        {
            return LayoutTransform::HorizontalAlignment::RIGHT;
        }

        return LayoutTransform::HorizontalAlignment::LEFT;
    }

    auto UIComponentUtil::getHorizontalAlignmentTypesString() -> String
    {
        auto enumValues = String( "" );
        for( size_t i = 0; i < static_cast<size_t>( LayoutTransform::HorizontalAlignment::COUNT ); ++i )
        {
            auto eMaterialType = static_cast<LayoutTransform::HorizontalAlignment>( i );
            auto eMaterialTypeStr = getHorizontalAlignmentString( eMaterialType );
            enumValues += eMaterialTypeStr + ";";
        }

        return enumValues;
    }

    auto UIComponentUtil::getVerticalAlignmentString(
        LayoutTransform::VerticalAlignment verticalAlignment ) -> String
    {
        switch( verticalAlignment )
        {
        case LayoutTransform::VerticalAlignment::TOP:
            return String( "Top" );
        case LayoutTransform::VerticalAlignment::CENTER:
            return String( "Center" );
        case LayoutTransform::VerticalAlignment::BOTTOM:
            return String( "Bottom" );
        default:
            break;
        }

        return String( "" );
    }

    auto UIComponentUtil::getVerticalAlignment( const String &str ) -> LayoutTransform::VerticalAlignment
    {
        if( str == String( "Top" ) )
        {
            return LayoutTransform::VerticalAlignment::TOP;
        }
        if( str == String( "Center" ) )
        {
            return LayoutTransform::VerticalAlignment::CENTER;
        }
        if( str == String( "Bottom" ) )
        {
            return LayoutTransform::VerticalAlignment::BOTTOM;
        }

        return LayoutTransform::VerticalAlignment::TOP;
    }

    auto UIComponentUtil::getVerticalAlignmentTypesString() -> String
    {
        auto enumValues = String( "" );
        for( size_t i = 0; i < static_cast<size_t>( LayoutTransform::VerticalAlignment::COUNT ); ++i )
        {
            auto eMaterialType = static_cast<LayoutTransform::VerticalAlignment>( i );
            auto eMaterialTypeStr = getVerticalAlignmentString( eMaterialType );
            enumValues += eMaterialTypeStr + ";";
        }

        return enumValues;
    }
}  // namespace fb::scene
