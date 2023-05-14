#ifndef _FBPropertyGroup_H
#define _FBPropertyGroup_H

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/Property.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{

    /** Stores and manages properties. */
    class FBCore_API Properties : public CSharedObject<ISharedObject>
    {
    public:
        /** Default constructor. */
        Properties();

        /** Copy constructor. */
        Properties( const Properties &other );

        /** Destructor. */
        ~Properties() override;

        /** Sets the name of the property group. */
        void setName( const String &name );

        /** Gets the name of the property group. */
        String getName() const;

        /* Removes all the properties from this property group.
        @param
        Indicates whether or to apply this to the child property groups. A value
        of true will clear all the properties in a property group.
        */
        void clearAll( bool cascade = false );

        /** Adds a property to the property group. */
        void addProperty( const Property &property );

        /** Adds a property to the property group.
        @remarks
        This does not search for a existing property of the same name. If a property of the
        same exists it will overwrite it. Useful for loading properties externally.
        */
        void addProperty( const String &name, const String &value,
                          const String &type = StringUtil::EmptyString, bool readOnly = false );

        /** Sets the a property value as a string. */
        void setProperty( const String &name, const String &value, const String &type );

        /** Sets the a property value as a string. */
        void setProperty( const String &name, const String &value, const String &type, bool readOnly );

        /** Sets the property type. Convenience function to save
        the from having to retrieve the property object. */
        bool setPropertyType( const String &name, const String &type );

        /** Removes a property from the property group. Returns true if successful. */
        bool removeProperty( const String &name );

        /** Finds a property in the property group. */
        bool getProperty( const String &name, Property &property ) const;

        /** Checks if the property group has a property. */
        bool hasProperty( const String &name ) const;

        /** Gets a property in the property group. */
        Property &getPropertyObject( const String &name );

        /** Gets a property in the property group. */
        const Property &getPropertyObject( const String &name ) const;

        /** Check if a property value is equal to the value stored. */
        bool propertyValueEquals( const String &name, const String &value ) const;

        String getProperty( const String &name, String defaultValue = "" ) const;
        bool getPropertyAsBool( const String &name, bool defaultValue = false ) const;
        s32 getPropertyAsInt( const String &name, s32 defaultValue = 0 ) const;
        f32 getPropertyAsFloat( const String &name, f32 defaultValue = 0.0f ) const;
        Vector3F getPropertyAsVector3( const String &name,
                                       Vector3F defaultValue = Vector3F::zero() ) const;
        Vector3D getPropertyAsVector3D( const String &name,
                                        Vector3D defaultValue = Vector3D::zero() ) const;

        /** Sets an array of the properties. */
        void setPropertiesAsArray(const Array<Property>& array);

        /** Gets an array of the properties. */
        Array<Property> getPropertiesAsArray() const;

        /** A copy operator to copy the properties of one object to another. */
        Properties &operator=( const Properties &other );

        /** Gets the children. */
        Array<SmartPtr<Properties>> getChildren() const;

        /** Adds a child property group. */
        void addChild( const SmartPtr<Properties> &propertyGroup );

        /** Removes a child from the property group. */
        void removeChild( const String &name );

        /** Gets the number of children this property group has. */
        u32 getNumChildren() const;

        /** Check if this property group has a child property group. */
        bool hasChild( const String &name ) const;

        /** Get a child property group at a given index. */
        SmartPtr<Properties> getChild( u32 index ) const;

        /** Gets a child property group. Will throw an exception if the property group
        is not found. Use has child to check that the property group has this child. */
        SmartPtr<Properties> getChild( const String &name ) const;

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const String &value, bool readOnly = false );

        /** Sets the a property value. */
        void setProperty( const String &name, const Array<String> &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const char *const &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const bool &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const s32 &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const u32 &value, bool readOnly = false );

#if defined FB_PLATFORM_WIN32
        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const unsigned long &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const unsigned long long &value, bool readOnly = false );
#elif defined FB_PLATFORM_APPLE
        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const unsigned long &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const unsigned long long &value, bool readOnly = false );
#elif defined FB_PLATFORM_LINUX
        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const u64 &value, bool readOnly = false );
#endif

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const f32 &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const f64 &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const Vector2I &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const Vector2F &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const Vector2D &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const Vector3I &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const Vector3F &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const Vector3D &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const QuaternionF &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const QuaternionD &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const ColourI &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, const ColourF &value, bool readOnly = false );

        /** Sets the a property value.
        @param name The name of the property.
        @param value A reference to the value.
        @param readOnly Sets whether or not the property is read only.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        void setProperty( const String &name, SmartPtr<ISharedObject> value, bool readOnly = false );
        void setProperty( const String &name, SmartPtr<render::IMaterial> value, bool readOnly = false );
        void setProperty( const String &name, SmartPtr<render::ITexture> value, bool readOnly = false );
        void setProperty( const String &name, SmartPtr<scene::IComponent> value, bool readOnly = false );
        void setProperty( const String &name, SmartPtr<IMeshResource> value, bool readOnly = false );

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, String &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, bool &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, s32 &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, u32 &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, f32 &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, f64 &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, Vector2I &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, Vector2F &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, Vector3I &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, Vector3F &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, Vector3D &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, ColourI &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, ColourF &value ) const;

        bool getPropertyValue( const String &name, SmartPtr<ISharedObject> &value ) const;
        bool getPropertyValue( const String &name, SmartPtr<render::IMaterial> &value ) const;
        bool getPropertyValue( const String &name, SmartPtr<render::ITexture> &value ) const;
        bool getPropertyValue( const String &name, SmartPtr<scene::IComponent> &value ) const;
        bool getPropertyValue( const String &name, SmartPtr<IMeshResource> &value ) const;

        /** Gets a property value. Returns true if the property was found.
        @param name The name of the property.
        @param value A reference to the value.
        @return Returns true if the property was found. Returns false if the property was not found.
        */
        bool getPropertyValue( const String &name, Array<String> &value ) const;

        static void registerClass();

        FB_CLASS_REGISTER_DECL;

    private:
        /// The name of the property group.
        String m_name;

        /// The map containing the properties.
        Array<Property> m_properties;

        /// An Array of the children of the property group.
        Array<SmartPtr<Properties>> m_children;
    };
}  // end namespace fb

#endif
