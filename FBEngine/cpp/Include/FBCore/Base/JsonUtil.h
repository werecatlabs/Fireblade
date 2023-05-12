#ifndef JsonUtil_h__
#define JsonUtil_h__

#include <FBCore/Base/Properties.h>
#include <FBCore/Base/DataBinding.h>
#include <FBCore/Base/DataMacros.h>
#include <FBCore/Base/Array.h>
#include <cJSON.h>

namespace fb
{
    class JsonUtil
    {
    public:
        template <class X>
        static void bind( cJSON *json, bool from, Array<X> *ANI, const char *tagname )
        {
            if( from )
            {
                auto arr = cJSON_GetObjectItem( json, tagname );
                if( !arr )
                {
                    return;
                }

                if( arr->type == cJSON_Array )
                {
                    for( int i = 0, length = cJSON_GetArraySize( arr ); i < length; i++ )
                    {
                        cJSON *child = cJSON_GetArrayItem( arr, i );
                        X ani;
                        TXB_binding( &ani, child, 1 );
                        ANI->push_back( ani );
                    }
                }
            }
            else
            {
                auto arr = cJSON_CreateArray();
                cJSON_AddItemToObject( json, tagname, arr );
                for( int i = 0; i < static_cast<int>( ANI->size() ); i++ )
                {
                    auto ani = &ANI->at( i );
                    auto jani = cJSON_CreateObject();
                    TXB_binding( ani, jani, 0 );
                    cJSON_AddItemToArray( arr, jani );
                }
            }
        }
    };
}  // end namespace fb

#endif  // JsonUtil_h__
