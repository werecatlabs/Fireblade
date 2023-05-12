#ifndef __FBData_ComponentData_h__
#define __FBData_ComponentData_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Data/C/InterfaceConfigData.h>
#include <FBCore/Data/C/VectorData.h>
#include "FBCore/Data/C/TransformData.h"

namespace fb
{
    class ComponentData
    {
    public:
        ComponentData();
        ComponentData( const ComponentData &other );

        ComponentData &operator=( const ComponentData &other );

        bool isValid() const
        {
            if( hashName == 0 )
            {
                return false;
            }

            if( rpm > 1e5 )
            {
                return false;
            }

            if( !worldTransform.isValid() )
            {
                return false;
            }

            if( !localTransform.isValid() )
            {
                return false;
            }

            if( !rotation.isValid() )
            {
                return false;
            }

            if( !vec1.isValid() )
            {
                return false;
            }

            if( !vec2.isValid() )
            {
                return false;
            }

            if( !vec3.isValid() )
            {
                return false;
            }

            return true;
        }

        ComponentData toSaracenStruct() const;

        int hashName = 0;

        TransformData worldTransform;
        TransformData localTransform;
        VectorData rotation;

        VectorData vec1;
        VectorData vec2;
        VectorData vec3;

        float rpm = 0.0f;

        float value = 0.0f;
        float value1 = 0.0f;
        float value2 = 0.0f;
        float value3 = 0.0f;

        float value4 = 0.0f;
        float value5 = 0.0f;
        float value6 = 0.0f;
        float value7 = 0.0f;

        int isControl = 0;
        int isVisible = 0;

        int isCrashed = 0;
        int state = 0;
        int isLocalTransform = 0;
        int hasTransform = 0;

        int objectReferenceHash = 0;
        int rendererReferenceHash = 0;
        int classTypeHash = 0;
        int parentId = 0;

        int iValue1 = 0;
        int iValue2 = 0;
        int iValue3 = 0;
        int iValue4 = 0;

        int isTransformDirty = 0;
        int iReserved2 = 0;
        int iReserved3 = 0;
        int iReserved4 = 0;
    };

    struct SaracenModelComponentsData
    {
    public:
        static const int NUM_COMPONENTS = 1024;

        SaracenModelComponentsData();
        SaracenModelComponentsData( const SaracenModelComponentsData &other );

        int hashName = 0;
        int numComponents = 0;
        float value = 0.0f;
        float value1 = 0.0f;

        int reserved0 = 0;
        int reserved1 = 0;
        int reserved2 = 0;
        int reserved3 = 0;

        ComponentData componentData[NUM_COMPONENTS];

        void operator=( const SaracenModelComponentsData &other );
    };

    inline ComponentData::ComponentData() :
        hashName( 0 ),
        rpm( 0.0f ),
        value( 0.0f ),
        value1( 0.0f ),
        value2( 0.0f ),
        value3( 0.0f ),
        isControl( false ),
        isVisible( true ),
        isLocalTransform( false ),
        hasTransform( true ),
        objectReferenceHash( 0 ),
        rendererReferenceHash( 0 ),
        classTypeHash( 0 ),
        parentId( 0 ),
        isTransformDirty( 0 )
    {
        constexpr size_t size = sizeof( ComponentData );
    }
}  // namespace fb

#endif  // SaracenComponentData_h__
