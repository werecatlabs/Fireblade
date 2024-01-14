#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/Property.h>
#include <FBCore/Core/StringUtil.h>
#include <rttr/registration>

namespace fb
{

    Property::Property()
    {
    }

    Property::Property( const Property &other )
    {
        *this = other;
    }

    Property::Property( const String &name, const String &value ) :
        m_label( StringUtil::EmptyString ),
        m_name( name ),
        m_value( value ),
        m_type( StringUtil::EmptyString )

    {
        static String defaultType( "string" );
        m_type = defaultType;
    }

    Property::Property( const String &name, const String &value, const String &type ) :
        m_label( StringUtil::EmptyString ),
        m_name( name ),
        m_value( value ),
        m_type( type )

    {
    }

    Property::Property( const String &name, const String &value, const String &label,
                        const String &type ) :
        m_label( label ),
        m_name( name ),
        m_value( value ),
        m_type( type )

    {
    }

    Property::Property( const String &name, const String &value, const String &label, const String &type,
                        bool readyOnly ) :
        m_label( label ),
        m_name( name ),
        m_value( value ),
        m_type( type ),
        m_readOnly( readyOnly )
    {
    }

    Property::~Property() = default;

    void Property::setName( const String &name )
    {
        m_name = name;
    }

    void Property::setValue( const String &value )
    {
        m_value = value;
    }

    void Property::setType( const String &type )
    {
        m_type = type;
    }

    void Property::setReadOnly( bool readOnly )
    {
        m_readOnly = readOnly;
    }

    auto Property::isReadOnly() const -> bool
    {
        return m_readOnly;
    }

    void Property::setAttribute( const String &name, const String &value )
    {
        m_attributes[name] = value;
    }

    auto Property::getAttribute( const String &name ) const -> const String &
    {
        auto it = m_attributes.find( name );
        if( it != m_attributes.end() )
        {
            return it->second;
        }

        return StringUtil::EmptyString;
    }

    auto Property::getAttributes() const -> Array<Pair<String, String>>
    {
        Array<std::pair<String, String>> attributes;

        for( auto attribute : m_attributes )
        {
            attributes.emplace_back( attribute.first, attribute.second );
        }

        return attributes;
    }

    auto Property::getAttributeByIndex( u32 index ) -> Pair<String, String>
    {
        auto it = m_attributes.begin();
        std::advance( it, index );
        return Pair<String, String>( it->first, it->second );
    }

    auto Property::getNumAttributes() const -> u32
    {
        return static_cast<u32>( m_attributes.size() );
    }

    auto Property::getAny() const -> Any
    {
        return m_any;
    }

    void Property::setAny( const Any &any )
    {
        m_any = any;
    }

    auto Property::operator=( const Property &other ) -> Property &
    {
        if( &other != this )
        {
            m_name = other.m_name;
            m_value = other.m_value;
            m_label = other.m_label;
            m_type = other.m_type;
            m_readOnly = other.m_readOnly;

            m_attributes = other.m_attributes;
        }

        return *this;
    }

    void Property::setLabel( const String &label )
    {
        m_label = label;
    }

    auto Property::getValueAsBool() const -> bool
    {
        return StringUtil::parseBool( m_value );
    }

    auto Property::getValueAsInt() const -> s32
    {
        return StringUtil::parseInt( m_value );
    }

    auto Property::getValueAsFloat() const -> f32
    {
        return StringUtil::parseFloat( m_value );
    }

    auto Property::getValueAsVector3f() const -> Vector3F
    {
        return StringUtil::parseVector3<f32>( m_value );
    }

    auto Property::getValueAsVector4f() const -> Vector4F
    {
        return StringUtil::parseVector4<f32>( m_value );
    }

    void Property::registerClass()
    {
        using namespace fb;
        using namespace rttr;

        registration::class_<Property>( "Property" )
            .property( "label", &Property::m_label )
            .property( "name", &Property::m_name )
            .property( "value", &Property::m_value )
            .property( "type", &Property::m_type )
            .property( "readOnly", &Property::m_readOnly )
            .property( "attributes", &Property::m_attributes );
    }

}  // end namespace fb
