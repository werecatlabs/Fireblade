#ifndef IProceduralCity_h__
#define IProceduralCity_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralCity : public ISharedObject
        {
        public:
            ~IProceduralCity() override = default;

            virtual void load( SmartPtr<IData> data ) = 0;

            virtual SmartPtr<IRoadNetwork> getRoadNetwork() const = 0;
            virtual void setRoadNetwork( SmartPtr<IRoadNetwork> val ) = 0;

            virtual Array<SmartPtr<IProceduralCityCenter>> getCityCenters() const = 0;
            virtual void setCityCenters( Array<SmartPtr<IProceduralCityCenter>> cityCenters ) = 0;
            virtual void addCenter( SmartPtr<IProceduralCityCenter> center ) = 0;
            virtual void removeCenter( SmartPtr<IProceduralCityCenter> center ) = 0;

            virtual void addBlock( SmartPtr<ICityBlock> block ) = 0;
            virtual void removeBlock( SmartPtr<ICityBlock> block ) = 0;
            virtual Array<SmartPtr<ICityBlock>> getBlocks() const = 0;

            virtual Vector2<real_Num> getSize() const = 0;
            virtual void setSize( const Vector2<real_Num> &val ) = 0;

            virtual Vector2<real_Num> getRelativeCoordinates( const Vector2<real_Num> &lat_long ) = 0;

            virtual SmartPtr<IRoad> getRoadByName( const String &name ) = 0;
            virtual Array<SmartPtr<IRoad>> getRoads() const = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralCity_h__
