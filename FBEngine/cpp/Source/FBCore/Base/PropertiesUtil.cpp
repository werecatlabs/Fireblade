#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/PropertiesUtil.h>
#include <FBCore/Base/Data.h>

namespace fb
{

    void PropertiesUtil::bind( Properties *properties, bool from, Array<data::property> *ANI,
                               [[maybe_unused]] const char *tagname )
    {
        if( from )
        {
            for( auto &ani : *ANI )
            {
                properties->setProperty( ani.name, ani.value );
            }
        }
        else
        {
            auto propertiesArray = properties->getPropertiesAsArray();
            for( auto &ani : propertiesArray )
            {
                data::property property;
                property.name = ani.getName();
                property.value = ani.getValue();
                ANI->push_back( property );
            }
        }
    }
}  // end namespace fb
