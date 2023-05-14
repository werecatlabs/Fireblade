#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/Property.h>
#include <FBCore/Base/StringUtil.h>
#include <rttr/registration>

namespace fb
{

    Property::Property() : m_readOnly( false )
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
        m_type( StringUtil::EmptyString ),
        m_readOnly( false )
    {
        static String defaultType( "string" );
        m_type = defaultType;
    }

    Property::Property( const String &name, const String &value, const String &type ) :
        m_label( StringUtil::EmptyString ),
        m_name( name ),
        m_value( value ),
        m_type( type ),
        m_readOnly( false )
    {
    }

    Property::Property( const String &name, const String &value, const String &label,
                        const String &type ) :
        m_label( label ),
        m_name( name ),
        m_value( value ),
        m_type( type ),
        m_readOnly( false )
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

    Property::~Property()
    {
    }

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

    bool Property::isReadOnly() const
    {
        return m_readOnly;
    }

    void Property::setAttribute( const String &name, const String &value )
    {
        m_attributes[name] = value;
    }

    String Property::getAttribute( const String &name ) const
    {
        auto it = m_attributes.find( name );
        if( it != m_attributes.end() )
            return it->second;

        return StringUtil::EmptyString;
    }

    Array<Pair<String, String>> Property::getAttributes() const
    {
        Array<std::pair<String, String>> attributes;

        for( auto attribute : m_attributes )
        {
            attributes.push_back( Pair<String, String>( attribute.first, attribute.second ) );
        }

        return attributes;
    }

    Pair<String, String> Property::getAttributeByIndex( u32 index )
    {
        auto it = m_attributes.begin();
        std::advance( it, index );
        return Pair<String, String>( it->first, it->second );
    }

    u32 Property::getNumAttributes() const
    {
        return static_cast<u32>( m_attributes.size() );
    }

    Any Property::getAny() const
    {
        return m_any;
    }

    void Property::setAny( const Any &any )
    {
        m_any = any;
    }

    Property &Property::operator=( const Property &other )
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

    String Property::getLabel() const
    {
        return m_label;
    }

    String Property::getName() const
    {
        return m_name;
    }

    String Property::getValue() const
    {
        return m_value;
    }

    bool Property::getValueAsBool() const
    {
        return StringUtil::parseBool( m_value );
    }

    s32 Property::getValueAsInt() const
    {
        return StringUtil::parseInt( m_value );
    }

    f32 Property::getValueAsFloat() const
    {
        return StringUtil::parseFloat( m_value );
    }

    Vector3F Property::getValueAsVector3f() const
    {
        return StringUtil::parseVector3<f32>( m_value );
    }

    Vector4F Property::getValueAsVector4f() const
    {
        return StringUtil::parseVector4<f32>( m_value );
    }

    String Property::getType() const
    {
        return m_type;
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
