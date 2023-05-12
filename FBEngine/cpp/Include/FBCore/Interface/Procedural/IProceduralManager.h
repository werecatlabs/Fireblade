#ifndef IProceduralManager_h__
#define IProceduralManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralManager : public ISharedObject
        {
        public:
            ~IProceduralManager() override = default;

            virtual void generate() = 0;

            virtual SmartPtr<IAiManager> getAiManager() const = 0;
            virtual void setAiManager( SmartPtr<IAiManager> aiManager ) = 0;

            virtual SmartPtr<ICityGenerator> getCityGenerator() const = 0;
            virtual void setCityGenerator( SmartPtr<ICityGenerator> val ) = 0;

            virtual SmartPtr<ITerrainGenerator> getTerrainGenerator() const = 0;
            virtual void setTerrainGenerator( SmartPtr<ITerrainGenerator> val ) = 0;

            virtual SmartPtr<IProceduralCollision> getCollisionManager() const = 0;
            virtual void setCollisionManager( SmartPtr<IProceduralCollision> val ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralManager_h__
