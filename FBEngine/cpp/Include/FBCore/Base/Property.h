#ifndef __FBProperty_H__
#define __FBProperty_H__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/Any.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/Map.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/ColourF.h>
#include <FBCore/Base/ColourI.h>
#include <FBCore/Base/UtilityTypes.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector4.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    /** A property class. Initially designed for key pair values.
    Contains functionality to store additional information.
    */
    class FBCore_API Property
    {
    public:
        /** Default constructor. */
        Property();

        /** Copy constructor. */
        Property( const Property &other );

        /** Constructs a property object with a key value pair. */
        Property( const String &name, const String &value );

        /** Constructs a property object with a key value pair. */
        Property( const String &name, const String &value, const String &type );

        /** Constructs a property object with a key value pair. */
        Property( const String &name, const String &value, const String &label, const String &type );

        /** Constructs a property object with a key value pair. */
        Property( const String &name, const String &value, const String &label, const String &type,
                  bool readyOnly );

        /** Destructor. */
        ~Property();

        /** Sets the name of the property. */
        void setName( const String &name );

        /** Gets the name of the property. */
        String getName() const;

        /** Sets the label of the property. */
        String getLabel() const;

        /** Gets the label of the property. */
        void setLabel( const String &label );

        /** Sets the value of the property. */
        void setValue( const String &value );

        /** Gets the value of the property. */
        String getValue() const;

        /** Gets the value of the property as a boolean. */
        bool getValueAsBool() const;

        /** Gets the value of the property as an integer. */
        s32 getValueAsInt() const;

        /** Gets the value of the property as a float. */
        f32 getValueAsFloat() const;

        /** Gets the value of the property as a vector3. */
        Vector3F getValueAsVector3f() const;

        /** Gets the value of the property as vector4. */
        Vector4F getValueAsVector4f() const;

        /** Sets the type of the property. */
        void setType( const String &type );

        /** Gets the type of the property. */
        String getType() const;

        /** Sets the property to read only. */
        void setReadOnly( bool readOnly );

        /** Gets if the property to read only. */
        bool isReadOnly() const;

        /** Sets an attribute value. */
        void setAttribute( const String &name, const String &value );

        /** Gets an attribute value. */
        String getAttribute( const String &name ) const;

        /** Gets the attributes. */
        Array<Pair<String, String>> getAttributes() const;

        /** Gets the attributes. */
        Pair<String, String> getAttributeByIndex( u32 index );

        /** Gets the number of attributes. */
        u32 getNumAttributes() const;

        /** Assignment operator. */
        Property &operator=( const Property &other );

        /** Gets an any value. */
        Any getAny() const;

        /** Sets an any value. */
        void setAny( const Any &any );

        static void registerClass();

    private:
        String m_label;
        String m_name;
        String m_value;
        String m_type;
        bool m_readOnly = false;

        Any m_any;

        using Attributes = std::map<String, String>;
        Attributes m_attributes;
    };
}  // end namespace fb

#endif
