#include <FBCore/FBCorePCH.h>

#include <FBCore/Data/OSMUtil.h>
#include <FBCore/Data/DefineDataStructures.h>
#include "FBCore/Base/Handle.h"
#include <FBCore/Base/Properties.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    //--------------------------------------------
    bool OSMUtil::isBuilding( SmartPtr<IData> data, const String &id )
    {
        auto osmData = data->getDataAsType<data::osm>();
        for( auto n : osmData->node )
        {
            if( id == n.id )
            {
                for( auto tag : n.tag )
                {
                    if( tag.k == "addr:postcode" )
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool OSMUtil::isRoad( SmartPtr<IData> data, const String &id )
    {
        auto osmData = data->getDataAsType<data::osm>();
        for( auto n : osmData->way )
        {
            if( id == n.id )
            {
                for( auto tag : n.tag )
                {
                    if( tag.k == "surface" )
                    {
                        if( tag.v == "asphalt" )
                        {
                            return true;
                        }
                    }

                    if( tag.k == "highway" )
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    String OSMUtil::getTagValue( SmartPtr<IData> data, const String &id, const String &key )
    {
        auto osmData = data->getDataAsType<data::osm>();
        for( auto n : osmData->way )
        {
            if( id == n.id )
            {
                for( auto tag : n.tag )
                {
                    if( tag.k == key )
                    {
                        return tag.v;
                    }
                }
            }
        }

        return "";
    }

    data::node_data OSMUtil::getNodeFromDataById( SmartPtr<IData> data, const String &id )
    {
        auto osmData = data->getDataAsType<data::osm>();
        for( auto n : osmData->node )
        {
            if( id == n.id )
            {
                return n;
            }
        }

        return data::node_data();
    }

    bool OSMUtil::getProperties( SmartPtr<IData> data, SmartPtr<Properties> properties,
                                 const String &id )
    {
        auto osmData = data->getDataAsType<data::osm>();
        for( auto n : osmData->node )
        {
            if( id == n.id )
            {
                for( auto tag : n.tag )
                {
                    properties->addProperty( tag.k, tag.v );
                }

                return true;
            }
        }

        return false;
    }
}  // namespace fb
