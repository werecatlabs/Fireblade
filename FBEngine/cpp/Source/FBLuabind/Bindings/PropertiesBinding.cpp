#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/PropertiesBinding.h"
#include "FBLuabind/SmartPtrConverter.h"

namespace fb
{

    PropertiesBinding::PropertiesBinding()
    {
        m_properties = boost::make_shared<Properties>();
    }

    PropertiesBinding::~PropertiesBinding()
    {
    }

    void PropertiesBinding::test()
    {
        //m_properties->setProperty("name", "val");
    }

    void PropertiesBinding::setProperty( const char *name, const char *val )
    {
        //m_properties->setProperty<String>( name, val );
    }

}  // namespace fb
