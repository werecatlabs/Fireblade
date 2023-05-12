#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/UIComponentUtil.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {

        String UIComponentUtil::getHorizontalAlignmentString(
            CanvasTransform::HorizontalAlignment horizontalAlignment )
        {
            switch( horizontalAlignment )
            {
            case CanvasTransform::HorizontalAlignment::LEFT:
                return String( "Left" );
            case CanvasTransform::HorizontalAlignment::CENTER:
                return String( "Center" );
            case CanvasTransform::HorizontalAlignment::RIGHT:
                return String( "Right" );
            default:
                break;
            }

            return String( "" );
        }

        CanvasTransform::HorizontalAlignment UIComponentUtil::getHorizontalAlignment( const String &str )
        {
            if( str == String( "Left" ) )
            {
                return CanvasTransform::HorizontalAlignment::LEFT;
            }
            if( str == String( "Center" ) )
            {
                return CanvasTransform::HorizontalAlignment::CENTER;
            }
            if( str == String( "Right" ) )
            {
                return CanvasTransform::HorizontalAlignment::RIGHT;
            }

            return CanvasTransform::HorizontalAlignment::LEFT;
        }

        String UIComponentUtil::getHorizontalAlignmentTypesString()
        {
            auto enumValues = String( "" );
            for( size_t i = 0; i < static_cast<size_t>( CanvasTransform::HorizontalAlignment::COUNT );
                 ++i )
            {
                auto eMaterialType = static_cast<CanvasTransform::HorizontalAlignment>( i );
                auto eMaterialTypeStr = getHorizontalAlignmentString( eMaterialType );
                enumValues += eMaterialTypeStr + ";";
            }

            return enumValues;
        }

        String UIComponentUtil::getVerticalAlignmentString(
            CanvasTransform::VerticalAlignment verticalAlignment )
        {
            switch( verticalAlignment )
            {
            case CanvasTransform::VerticalAlignment::TOP:
                return String( "Top" );
            case CanvasTransform::VerticalAlignment::CENTER:
                return String( "Center" );
            case CanvasTransform::VerticalAlignment::BOTTOM:
                return String( "Bottom" );
            default:
                break;
            }

            return String( "" );
        }

        CanvasTransform::VerticalAlignment UIComponentUtil::getVerticalAlignment( const String &str )
        {
            if( str == String( "Top" ) )
            {
                return CanvasTransform::VerticalAlignment::TOP;
            }
            if( str == String( "Center" ) )
            {
                return CanvasTransform::VerticalAlignment::CENTER;
            }
            if( str == String( "Bottom" ) )
            {
                return CanvasTransform::VerticalAlignment::BOTTOM;
            }

            return CanvasTransform::VerticalAlignment::TOP;
        }

        String UIComponentUtil::getVerticalAlignmentTypesString()
        {
            auto enumValues = String( "" );
            for( size_t i = 0; i < static_cast<size_t>( CanvasTransform::VerticalAlignment::COUNT );
                 ++i )
            {
                auto eMaterialType = static_cast<CanvasTransform::VerticalAlignment>( i );
                auto eMaterialTypeStr = getVerticalAlignmentString( eMaterialType );
                enumValues += eMaterialTypeStr + ";";
            }

            return enumValues;
        }
    }  // namespace scene
}  // end namespace fb
