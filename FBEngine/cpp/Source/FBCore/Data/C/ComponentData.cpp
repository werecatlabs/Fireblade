#include "FBCore/FBCorePCH.h"
#include "FBCore/Data/C/ComponentData.h"
#include "FBCore/Base/StringUtil.h"

namespace fb
{
    ComponentData MakeSaracenComponentData()
    {
        using namespace fb;
        ComponentData data;

        static auto EMPTY_STR = "";

        data.hashName = 0;

        data.localTransform = MakeSaracenTransform();
        data.worldTransform = MakeSaracenTransform();

        data.rpm = 0.0f;
        data.value = 0.0f;
        data.value1 = 0.0f;
        data.value2 = 0.0f;
        data.value3 = 0.0f;
        data.isControl = false;
        data.isVisible = true;
        data.isLocalTransform = false;
        data.hasTransform = true;

        data.objectReferenceHash = 0;
        data.rendererReferenceHash = 0;
        data.classTypeHash = 0;
        data.parentId = 0;

        return data;
    }

    SaracenModelComponentsData MakeSaracenModelComponentsData()
    {
        using namespace fb;
        SaracenModelComponentsData data;
        data.hashName = 0;
        data.numComponents = 0;
        data.value = 0.0f;
        data.value1 = 0.0f;

        data.reserved0 = 0;
        data.reserved1 = 0;
        data.reserved2 = 0;
        data.reserved3 = 0;

        return data;
    }

    void SaracenModelComponentsData::operator=( const SaracenModelComponentsData &other )
    {
        hashName = other.hashName;
        numComponents = other.numComponents;
        value = other.value;
        value1 = other.value1;

        reserved0 = other.reserved0;
        reserved1 = other.reserved1;
        reserved2 = other.reserved2;
        reserved3 = other.reserved3;

        for( size_t i = 0; i < static_cast<size_t>( numComponents ); ++i )
        {
            ComponentData &data = componentData[i];
            const ComponentData &otherData = other.componentData[i];

            data = otherData;
        }
    }

    SaracenModelComponentsData::SaracenModelComponentsData( const SaracenModelComponentsData &other )
    {
        *this = other;
    }

    SaracenModelComponentsData::SaracenModelComponentsData()
    {
        hashName = 0;
        numComponents = 0;
        value = 0.0f;
        value1 = 0.0f;

        reserved0 = 0;
        reserved1 = 0;
        reserved2 = 0;
        reserved3 = 0;
    }

    ComponentData::ComponentData( const ComponentData &other )
    {
        *this = other;
    }

    ComponentData &ComponentData::operator=( const ComponentData &other )
    {
        hashName = other.hashName;

        worldTransform = other.worldTransform;
        localTransform = other.localTransform;
        rotation = other.rotation;

        vec1 = other.vec1;
        vec2 = other.vec2;
        vec3 = other.vec3;

        rpm = other.rpm;

        value = other.value;
        value1 = other.value1;
        value2 = other.value2;
        value3 = other.value3;
        value4 = other.value4;
        value5 = other.value5;
        value6 = other.value6;
        value7 = other.value7;

        isControl = other.isControl;
        isVisible = other.isVisible;

        isCrashed = other.isCrashed;
        state = other.state;
        isLocalTransform = other.isLocalTransform;
        hasTransform = other.hasTransform;

        objectReferenceHash = other.objectReferenceHash;
        rendererReferenceHash = other.rendererReferenceHash;
        classTypeHash = other.classTypeHash;
        parentId = other.parentId;

        iValue1 = other.iValue1;
        iValue2 = other.iValue2;
        iValue3 = other.iValue3;
        iValue4 = other.iValue4;

        isTransformDirty = other.isTransformDirty;

        return *this;
    }

    ComponentData ComponentData::toSaracenStruct() const
    {
        return *this;
    }
}  // namespace fb
