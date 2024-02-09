#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Core/VectorUtil.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Interface/System/IFactory.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/Mesh/IMeshResource.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Interface/Sound/ISound.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <rttr/registration>
#include <rttr/detail/type/type_converter.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, Properties, ISharedObject );

    static u32 NumPropertyGroups = 0;

    Properties::Properties()
    {
#ifdef DEBUG_PROPGRP
        NumPropertyGroups++;
#endif
    }

    Properties::Properties( const Properties &other )
    {
        m_name = other.m_name;
        m_properties = other.m_properties;
        m_children = other.m_children;

#ifdef DEBUG_PROPGRP
        NumPropertyGroups++;
#endif
    }

    Properties::~Properties()
    {
#ifdef DEBUG_PROPGRP
        NumPropertyGroups--;
        FB_LOG_MESSAGE( "Properties", "Properties destroyed name: %s Num groups: %i", m_name.c_str(),
                        NumPropertyGroups );
#endif
    }

    void Properties::clearAll( bool cascade )
    {
        m_properties.clear();

        if( cascade )
        {
            for( auto &i : m_children )
            {
                i->clearAll( cascade );
            }
        }
    }

    void Properties::setName( const String &name )
    {
        m_name = name;
    }

    auto Properties::getName() const -> String
    {
        return m_name;
    }

    void Properties::addProperty( const Property &property )
    {
        auto name = property.getName();
        if( hasProperty( name ) )
        {
            for( auto &currentProperty : m_properties )
            {
                if( currentProperty.getName() == name )
                {
                    currentProperty = property;
                }
            }
        }
        else
        {
            m_properties.push_back( property );
        }
    }

    auto Properties::hasProperty( const String &name ) const -> bool
    {
        for( auto &currentProperty : m_properties )
        {
            if( currentProperty.getName() == name )
            {
                return true;
            }
        }

        return false;
    }

    void Properties::addProperty( const String &name, const String &value, const String &type,
                                  bool readOnly )
    {
        // check if the property exists
        if( !hasProperty( name ) )
        {
            Property property;
            property.setName( name );
            property.setValue( value );
            property.setType( type );
            property.setReadOnly( readOnly );

            m_properties.push_back( property );
        }
        else
        {
            Property &property = getPropertyObject( name );
            property.setValue( value );

            if( type.length() > 0 )
            {
                property.setType( type );
            }
        }
    }

    void Properties::setProperty( const String &name, const String &value, const String &type )
    {
        // check if the property exists
        if( !hasProperty( name ) )
        {
            Property property( name, value, type );
            m_properties.push_back( property );
        }
        else
        {
            auto &property = getPropertyObject( name );
            property.setValue( value );

            if( type.length() > 0 )
            {
                property.setType( type );
            }
        }
    }

    void Properties::setProperty( const String &name, const String &value, const String &type,
                                  bool readOnly )
    {
        // check if the property exists
        if( !hasProperty( name ) )
        {
            Property property;
            property.setName( name );
            property.setValue( value );
            property.setType( type );
            property.setReadOnly( readOnly );

            m_properties.push_back( property );
        }
        else
        {
            Property &property = getPropertyObject( name );
            property.setValue( value );

            if( type.length() > 0 )
            {
                property.setType( type );
            }

            property.setReadOnly( readOnly );
        }
    }

    auto Properties::setPropertyType( const String &name, const String &type ) -> bool
    {
        if( !hasProperty( name ) )
        {
            return false;
        }

        Property &property = getPropertyObject( name );
        property.setType( type );
        return true;
    }

    auto Properties::removeProperty( const String &name ) -> bool
    {
        auto count = static_cast<size_t>( 0 );
        for( auto &property : m_properties )
        {
            if( property.getName() == name )
            {
                break;
            }

            ++count;
        }

        auto it = m_properties.begin();
        std::advance( it, count );
        if( it != m_properties.end() )
        {
            m_properties.erase( it );
            return true;
        }

        return false;
    }

    auto Properties::propertyValueEquals( const String &name, const String &value ) const -> bool
    {
        for( auto &property : m_properties )
        {
            if( property.getName() == name )
            {
                return property.getValue() == value;
            }
        }

        return false;
    }

    auto Properties::operator=( const Properties &other ) -> Properties &
    {
        if( this != &other )
        {
            m_name = other.m_name;
            m_properties = other.m_properties;
            m_children = other.m_children;
        }

        return *this;
    }

    auto Properties::getChildren() const -> Array<SmartPtr<Properties>>
    {
        return m_children;
    }

    void Properties::addChild( const SmartPtr<Properties> &properties )
    {
        m_children.push_back( properties );
    }

    void Properties::removeChild( const String &name )
    {
        for( u32 i = 0; i < m_children.size(); ++i )
        {
            SmartPtr<Properties> &properties = m_children[i];
            if( properties->getName() == name )
            {
                auto it = m_children.begin();
                std::advance( it, i );
                m_children.erase( it );
                return;
            }
        }
    }

    auto Properties::getNumChildren() const -> u32
    {
        return static_cast<u32>( m_children.size() );
    }

    auto Properties::getChild( u32 index ) const -> SmartPtr<Properties>
    {
        FB_ASSERT( index < m_children.size() );
        return m_children[index];
    }

    auto Properties::getChild( const String &name ) const -> SmartPtr<Properties>
    {
        for( auto &child : m_children )
        {
            const auto &childName = child->m_name;
            if( name == childName )
            {
                return child;
            }
        }

        return nullptr;
    }

    auto Properties::getChildrenByName( const String &name ) const -> Array<SmartPtr<Properties>>
    {
        Array<SmartPtr<Properties>> children;
        children.reserve( m_children.size() );

        for( auto &child : m_children )
        {
            if( child )
            {
                const auto &childName = child->m_name;
                if( name == childName )
                {
                    children.push_back( child );
                }
            }
        }

        return children;
    }

    auto Properties::hasChild( const String &name ) const -> bool
    {
        for( const auto &child : m_children )
        {
            const auto &childName = child->m_name;
            if( name == childName )
            {
                return true;
            }
        }

        return false;
    }

    auto Properties::getProperty( const String &name, Property &property ) const -> bool
    {
        for( auto &currentProperty : m_properties )
        {
            if( currentProperty.getName() == name )
            {
                property = currentProperty;
                return true;
            }
        }

        return false;
    }

    auto Properties::getPropertyObject( const String &name ) -> Property &
    {
        for( auto &currentProperty : m_properties )
        {
            if( currentProperty.getName() == name )
            {
                return currentProperty;
            }
        }

        FB_EXCEPTION( "Could not get property" );
    }

    auto Properties::getPropertyObject( const String &name ) const -> const Property &
    {
        for( auto &currentProperty : m_properties )
        {
            if( currentProperty.getName() == name )
            {
                return currentProperty;
            }
        }

        FB_EXCEPTION( "Could not get property" );
    }

    void Properties::setPropertiesAsArray( const Array<Property> &array )
    {
        m_properties = array;
    }

    auto Properties::getPropertiesAsArray() const -> Array<Property>
    {
        return m_properties;
    }

    auto Properties::getProperty( const String &name, String defaultValue ) const -> String
    {
        String value;
        if( getPropertyValue( name, value ) )
        {
            return value;
        }

        return defaultValue;
    }

    auto Properties::getPropertyAsBool( const std::string &name, bool defaultValue /*= false*/ ) const
        -> bool
    {
        auto value = getProperty( name, "" );
        return StringUtil::parseBool( value, defaultValue );
    }

    auto Properties::getPropertyAsInt( const std::string &name, int defaultValue /*= 0*/ ) const -> s32
    {
        auto value = getProperty( name, "" );
        return StringUtil::parseInt( value, defaultValue );
    }

    auto Properties::getPropertyAsFloat( const std::string &name, float defaultValue /*= 0.0f*/ ) const
        -> f32
    {
        auto value = getProperty( name, "" );
        return StringUtil::parseFloat( value, defaultValue );
    }

    auto Properties::getPropertyAsVector3( const std::string &name,
                                           Vector3F defaultValue /*= Vector3<real_Num>::ZERO*/ ) const
        -> Vector3F
    {
        auto value = getProperty( name, "" );
        return StringUtil::parseVector3( value, defaultValue );
    }

    auto Properties::getPropertyAsVector3D( const std::string &name,
                                            Vector3D defaultValue /*= Vector3<real_Num>::ZERO*/ ) const
        -> Vector3D
    {
        auto value = getProperty( name, "" );
        return StringUtil::parseVector3<f64>( value, defaultValue );
    }

    void Properties::setProperty( const String &name, const String &value, bool readOnly )
    {
        auto type = String( "string" );
        setProperty( name, value, type, readOnly );
    }

    void Properties::setProperty( const String &name, const Array<String> &value, bool readOnly )
    {
        auto arrayStr = StringUtil::toString( value );
        auto type = String( "string_array" );
        setProperty( name, arrayStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const char *const &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "string" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const bool &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "bool" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const s32 &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "int" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const u32 &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "u32" );
        setProperty( name, valueStr, type, readOnly );
    }

#if defined FB_PLATFORM_WIN32

    void Properties::setProperty( const String &name, const unsigned long &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "unsigned long" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const unsigned long long &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "unsigned long long" );
        setProperty( name, valueStr, type, readOnly );
    }
#elif defined FB_PLATFORM_APPLE

    void Properties::setProperty( const String &name, const unsigned long &value, bool readOnly )
    {
        setProperty( name, StringUtil::toString( value ), String( "unsigned long" ), readOnly );
    }

    void Properties::setProperty( const String &name, const unsigned long long &value, bool readOnly )
    {
        setProperty( name, StringUtil::toString( value ), String( "unsigned long long" ), readOnly );
    }
#elif defined FB_PLATFORM_LINUX

    void Properties::setProperty( const String &name, const u64 &value, bool readOnly )
    {
        setProperty( name, StringUtil::toString( value ), String( "unsigned long long" ), readOnly );
    }
#endif

    void Properties::setProperty( const String &name, const f32 &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "float" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const f64 &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "double" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const Vector2I &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "Vector2i" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const Vector2F &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "Vector2" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const Vector2D &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "Vector2d" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const Vector3I &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "vector3i" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const Vector3F &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "vector3" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const Vector3D &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "vector3d" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const QuaternionF &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "quaternion" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const QuaternionD &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "quaterniond" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const Transform3F &value, bool readOnly )
    {
        auto p = value.getPosition();
        auto r = value.getRotation();
        auto s = value.getScale();

        auto valueStr = StringUtil::toString( p ) + ";" + StringUtil::toString( r ) + ";" +
                        StringUtil::toString( s );

        auto type = String( "transform" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const Transform3D &value, bool readOnly )
    {
        auto p = value.getPosition();
        auto r = value.getRotation();
        auto s = value.getScale();

        auto valueStr = StringUtil::toString( p ) + ";" + StringUtil::toString( r ) + ";" +
                        StringUtil::toString( s );

        auto type = String( "transformd" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const ColourI &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "colour" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setProperty( const String &name, const ColourF &value, bool readOnly )
    {
        auto valueStr = StringUtil::toString( value );
        auto type = String( "colourf" );
        setProperty( name, valueStr, type, readOnly );
    }

    void Properties::setPropertyAsEnum( const String &name, s32 value, const Array<String> &values,
                                        bool readOnly )
    {
        setPropertyAsEnum( name, StringUtil::toString( value ), values, readOnly );
    }

    void Properties::setPropertyAsEnum( const String &name, const String &value,
                                        const Array<String> &values, bool readOnly )
    {
        auto type = String( "enum" );
        setProperty( name, value, type, readOnly );

        auto &property = getPropertyObject( name );
        auto valueStr = StringUtil::toString( values );
        property.setAttribute( "enum", valueStr );
    }

    void Properties::setPropertyAsButton( const String &name, const String &value, bool readOnly )
    {
        setProperty( name, value, "button", readOnly );
    }

    void Properties::setButtonPressed( const String &name, bool value )
    {
        if( hasProperty( name ) )
        {
            auto &button = getPropertyObject( name );
            button.setAttribute( "click", StringUtil::toString( value ) );
        }
    }

    auto Properties::isButtonPressed( const String &name ) const -> bool
    {
        if( hasProperty( name ) )
        {
            auto &resetButton = getPropertyObject( name );
            if( resetButton.getAttribute( "click" ) == "true" )
            {
                return true;
            }
        }

        return false;
    }

    void Properties::setProperty( const String &name, SmartPtr<ISharedObject> value, bool readOnly )
    {
        if( value )
        {
            auto handle = value->getHandle();
            auto uuid = handle->getUUID();
            setProperty( name, uuid, "resource", false );

            if( value->isDerived<IResource>() )
            {
                auto resource = fb::static_pointer_cast<IResource>( value );
                auto resourceTypeName = resource->getResourceTypeByName();

                auto &property = getPropertyObject( name );
                property.setAttribute( "resourceType", resourceTypeName );
            }
        }
        else
        {
            setProperty( name, "", "resource", false );

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", "None" );
        }
    }

    void Properties::setProperty( const String &name, SmartPtr<render::IMaterial> value, bool readOnly )
    {
        if( value )
        {
            auto handle = value->getHandle();
            auto uuid = handle->getUUID();
            setProperty( name, uuid, "resource", false );

            auto resourceTypeName = value->getResourceTypeByName();

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", resourceTypeName );
        }
        else
        {
            setProperty( name, "", "resource", false );

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", "Material" );
        }
    }

    void Properties::setProperty( const String &name, SmartPtr<render::ITexture> value, bool readOnly )
    {
        if( value )
        {
            auto handle = value->getHandle();
            auto uuid = handle->getUUID();
            setProperty( name, uuid, "resource", false );

            auto resourceTypeName = value->getResourceTypeByName();

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", resourceTypeName );
        }
        else
        {
            setProperty( name, "", "resource", false );

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", "Texture" );
        }
    }

    void Properties::setProperty( const String &name, SmartPtr<scene::IActor> value, bool readOnly )
    {
        if( value )
        {
            auto handle = value->getHandle();
            auto uuid = handle->getUUID();
            setProperty( name, uuid, "resource", false );

            auto resourceTypeName = value->getResourceTypeByName();

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", resourceTypeName );
        }
        else
        {
            setProperty( name, "", "resource", false );

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", "Component" );
        }
    }

    void Properties::setProperty( const String &name, SmartPtr<scene::IComponent> value, bool readOnly )
    {
        if( value )
        {
            auto handle = value->getHandle();
            auto uuid = handle->getUUID();
            setProperty( name, uuid, "resource", false );

            auto resourceTypeName = value->getResourceTypeByName();

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", resourceTypeName );
        }
        else
        {
            setProperty( name, "", "resource", false );

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", "Component" );
        }
    }

    void Properties::setProperty( const String &name, SmartPtr<IMeshResource> value, bool readOnly )
    {
        if( value )
        {
            auto handle = value->getHandle();
            auto uuid = handle->getUUID();
            setProperty( name, uuid, "resource", false );

            auto resourceTypeName = value->getResourceTypeByName();

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", resourceTypeName );
        }
        else
        {
            setProperty( name, "", "resource", false );

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", "Component" );
        }
    }

    void Properties::setProperty( const String &name, SmartPtr<ISound> value, bool readOnly )
    {
        if( value )
        {
            auto handle = value->getHandle();
            auto uuid = handle->getUUID();
            setProperty( name, uuid, "resource", false );

            auto resourceTypeName = value->getResourceTypeByName();

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", resourceTypeName );
        }
        else
        {
            setProperty( name, "", "resource", false );

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", "Component" );
        }
    }

    void Properties::setProperty( const String &name, Array<SmartPtr<scene::IComponent>> value,
                                  bool readOnly )
    {
        if( value.size() > 0 )
        {
            Array<String> uuids;
            for( auto &component : value )
            {
                auto handle = component->getHandle();
                auto uuid = handle->getUUID();
                uuids.push_back( uuid );
            }

            auto valueStr = StringUtil::toString( uuids );
            setProperty( name, valueStr, "resource", false );

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", "Component" );
        }
        else
        {
            setProperty( name, "", "resource", false );

            auto &property = getPropertyObject( name );
            property.setAttribute( "resourceType", "Component" );
        }
    }

    auto Properties::getPropertyValue( const String &name, String &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            value = property.getValue();
            return true;
        }

        for( auto &child : m_children )
        {
            if( child->getPropertyValue( name, value ) )
            {
                return true;
            }
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, bool &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            value = property.getValueAsBool();
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, s32 &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            value = property.getValueAsInt();
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, u32 &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            value = static_cast<u32>( property.getValueAsInt() );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, f32 &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            value = property.getValueAsFloat();
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, f64 &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            value = static_cast<f64>( property.getValueAsFloat() );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, Vector2I &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseVector2<s32>( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, Vector2F &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseVector2<f32>( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, Vector2D &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseVector2<f64>( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, Vector3I &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseVector3<s32>( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, Vector3F &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseVector3<f32>( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, Vector3D &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseVector3<f64>( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, QuaternionF &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseQuaternion<f32>( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, QuaternionD &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseQuaternion<f64>( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, Transform3F &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            auto splitStr = StringUtil::split( str, ";", 3, false );

            if( splitStr.size() >= 3 )
            {
                auto p = StringUtil::parseVector3<f32>( splitStr[0] );
                auto r = StringUtil::parseVector3<f32>( splitStr[1] );
                auto s = StringUtil::parseVector3<f32>( splitStr[2] );

                value.setPosition( p );
                value.setRotation( r );
                value.setScale( s );
            }

            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, Transform3D &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            auto splitStr = StringUtil::split( str, ";", 3, false );

            if( splitStr.size() >= 3 )
            {
                auto p = StringUtil::parseVector3<f64>( splitStr[0] );
                auto r = StringUtil::parseVector3<f64>( splitStr[1] );
                auto s = StringUtil::parseVector3<f64>( splitStr[2] );

                value.setPosition( p );
                value.setRotation( r );
                value.setScale( s );
            }

            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, ColourI &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseColour( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, ColourF &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            value = StringUtil::parseColourf( str );
            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, SmartPtr<ISharedObject> &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto uuid = property.getValue();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            value = resourceDatabase->getObject( uuid );

            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, SmartPtr<render::IMaterial> &value ) const
        -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto uuid = property.getValue();
            if( !StringUtil::isNullOrEmpty( uuid ) )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto resourceDatabase = applicationManager->getResourceDatabase();
                FB_ASSERT( resourceDatabase );

                auto resource = resourceDatabase->getObject( uuid );
                if( resource )
                {
                    if( resource->isDerived<render::IMaterial>() )
                    {
                        value = fb::static_pointer_cast<render::IMaterial>( resource );
                    }
                }

                return true;
            }
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, SmartPtr<render::ITexture> &value ) const
        -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto uuid = property.getValue();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            auto resource = resourceDatabase->getObject( uuid );
            if( resource )
            {
                if( resource->isDerived<render::ITexture>() )
                {
                    value = fb::static_pointer_cast<render::ITexture>( resource );
                }
            }

            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, SmartPtr<scene::IActor> &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto uuid = property.getValue();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            auto resource = resourceDatabase->getObject( uuid );
            if( resource )
            {
                if( resource->isDerived<scene::IActor>() )
                {
                    value = fb::static_pointer_cast<scene::IActor>( resource );
                }
            }

            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, SmartPtr<scene::IComponent> &value ) const
        -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto uuid = property.getValue();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            auto resource = resourceDatabase->getObject( uuid );
            if( resource )
            {
                if( resource->isDerived<scene::IComponent>() )
                {
                    value = fb::static_pointer_cast<scene::IComponent>( resource );
                }
            }

            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, SmartPtr<IMeshResource> &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto uuid = property.getValue();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            auto resource = resourceDatabase->getObject( uuid );
            if( resource )
            {
                if( resource->isDerived<IMeshResource>() )
                {
                    value = fb::static_pointer_cast<IMeshResource>( resource );
                }
            }

            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, SmartPtr<ISound> &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto uuid = property.getValue();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            auto resource = resourceDatabase->getObject( uuid );
            if( resource )
            {
                if( resource->isDerived<ISound>() )
                {
                    value = fb::static_pointer_cast<ISound>( resource );
                }
            }

            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name,
                                       Array<SmartPtr<scene::IComponent>> &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto valueStr = property.getValue();
            auto uuids = Array<String>();
            StringUtil::parseArray( valueStr, uuids );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            FB_ASSERT( resourceDatabase );

            for( auto &uuid : uuids )
            {
                auto resource = resourceDatabase->getObject( uuid );
                if( resource )
                {
                    if( resource->isDerived<scene::IComponent>() )
                    {
                        value.push_back( fb::static_pointer_cast<scene::IComponent>( resource ) );
                    }
                }
            }

            return true;
        }

        return false;
    }

    auto Properties::getPropertyValue( const String &name, Array<String> &value ) const -> bool
    {
        if( hasProperty( name ) )
        {
            const auto &property = getPropertyObject( name );
            auto str = property.getValue();
            StringUtil::parseArray( str, value );
            return true;
        }

        return false;
    }

    void Properties::registerClass()
    {
        using namespace fb;
        using namespace rttr;
        using namespace rttr::detail;

        registration::class_<Properties>( "Properties" )
            .property( "name", &Properties::m_name )
            .property( "properties", &Properties::m_properties )
            .property( "children", &Properties::m_children );
    }

}  // end namespace fb
