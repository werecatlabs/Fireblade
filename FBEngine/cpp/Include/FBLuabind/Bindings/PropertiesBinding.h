#ifndef PropertiesBinding_h__
#define PropertiesBinding_h__

#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Script/IScriptObject.h>

namespace fb
{

    class PropertiesBinding : public IScriptObject
    {
    public:
        PropertiesBinding();

        ~PropertiesBinding();

        void test();

        void setProperty( const char *name, const char *val );

        boost::shared_ptr<Properties> getProperties() const
        {
            return m_properties;
        }
        void setProperties( boost::shared_ptr<Properties> val )
        {
            m_properties = val;
        }

        boost::shared_ptr<Properties> m_properties;
    };

    typedef SmartPtr<PropertiesBinding> PropertiesBindingPtr;

}  // end namespace fb

#endif  // PropertiesBinding_h__
