#ifndef IProceduralScene_h__
#define IProceduralScene_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralScene : public ISharedObject
        {
        public:
            ~IProceduralScene() override = default;

            virtual String getName() const = 0;
            virtual void setName( const String &name ) = 0;

            virtual void addCity( SmartPtr<IProceduralCity> city ) = 0;
            virtual void removeCity( SmartPtr<IProceduralCity> city ) = 0;
            virtual Array<SmartPtr<IProceduralCity>> getCities() const = 0;
            virtual void setCities( Array<SmartPtr<IProceduralCity>> cities ) = 0;

            virtual void addTerrain( SmartPtr<IProceduralTerrain> terrain ) = 0;
            virtual void removeTerrain( SmartPtr<IProceduralTerrain> terrain ) = 0;
            virtual Array<SmartPtr<IProceduralTerrain>> getTerrains() const = 0;
            virtual void setTerrains( Array<SmartPtr<IProceduralTerrain>> terrains ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralScene_h__
