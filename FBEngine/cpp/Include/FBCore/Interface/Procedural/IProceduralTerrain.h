#ifndef IProceduralTerrain_h__
#define IProceduralTerrain_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Procedural/IProceduralObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralTerrain : public IProceduralObject
        {
        public:
            ~IProceduralTerrain() override = default;

            /** Get the size of the terrain. */
            virtual Vector3<real_Num> getSize() const = 0;

            /** Get the size of the terrain. */
            virtual void setSize( const Vector3<real_Num> &size ) = 0;

            /** Get the size of the terrain. */
            virtual Vector2I getHeightmapResolution() const = 0;

            /** Get the size of the terrain. */
            virtual void setHeightmapResolution( const Vector2I &resolution ) = 0;

            /** Get the size of the terrain. */
            virtual Vector2I getAlphamapResolution() const = 0;

            /** Get the size of the terrain. */
            virtual void setAlphamapResolution( const Vector2I &resolution ) = 0;

            /** Get the size of the terrain. */
            virtual Vector2I getDetailResolution() const = 0;

            /** Get the size of the terrain. */
            virtual void setDetailResolution( const Vector2I &resolution ) = 0;

            /** Get the size of the terrain. */
            virtual s32 getDetailResolutionPerPatch() const = 0;

            /** Get the size of the terrain. */
            virtual void setDetailResolutionPerPatch( s32 resolutionPerPatch ) = 0;

            /** Gets height data. */
            virtual Array<f32> getHeightData() const = 0;

            /** Sets height data. */
            virtual void setHeightData( const Array<f32> &heightData ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralTerrain_h__
