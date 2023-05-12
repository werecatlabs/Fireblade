#ifndef ICityGenerator_h__
#define ICityGenerator_h__

#include <FBCore/Interface/Procedural/IProceduralGenerator.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    namespace procedural
    {

        /**
         * @brief Interface for generating city layouts procedurally.
         *
         * This class defines the interface for generating city layouts using procedural generation techniques.
         * It is derived from the `IProceduralGenerator` class and adds additional methods for configuring and
         * controlling the city generation process.
         */
        class ICityGenerator : public IProceduralGenerator
        {
        public:
            /**
             * @brief Destructor.
             */
            virtual ~ICityGenerator() override = default;

            /**
             * @brief Generates the city layout.
             *
             * This method generates the city layout using the configured generator modules and settings.
             * The generation process may take some time, and can be monitored using the `isFinished` method.
             */
            void generate() override = 0;

            /**
             * @brief Returns whether the city generation process has finished.
             *
             * This method returns whether the city generation process has finished, and all cities have been generated.
             * The `generate` method may take some time to complete, and this method can be used to check whether it is safe
             * to access the generated cities.
             *
             * @return True if the generation process has finished, false otherwise.
             */
            virtual bool isFinished() const = 0;

            /**
             * @brief Returns the terrain generator module.
             *
             * This method returns the terrain generator module used by the city generator to generate the terrain mesh.
             * If no terrain generator has been set, this method returns nullptr.
             *
             * @return The terrain generator module, or nullptr if no generator has been set.
             */
            virtual SmartPtr<ITerrainGenerator> getTerrainGenerator() const = 0;

            /**
             * @brief Sets the terrain generator module.
             *
             * This method sets the terrain generator module used by the city generator to generate the terrain mesh.
             * The terrain generator is used to generate the base terrain mesh, which is then modified and built upon
             * by the other generator modules.
             *
             * @param terrainGenerator The terrain generator module to set.
             */
            virtual void setTerrainGenerator( SmartPtr<ITerrainGenerator> terrainGenerator ) = 0;

            /**
             * @brief Returns the block generator module.
             *
             * This method returns the block generator module used by the city generator to generate the city blocks.
             * If no block generator has been set, this method returns nullptr.
             *
             * @return The block generator module, or nullptr if no generator has been set.
             */
            virtual SmartPtr<IBlockGenerator> getBlockGenerator() const = 0;

            /**
             * @brief Sets the block generator module.
             *
             * This method sets the block generator module used by the city generator to generate the city blocks.
             * The block generator is used to generate the basic structure and layout of the city blocks.
             *
             * @param blockGenerator The block generator module to set.
             */
            virtual void setBlockGenerator( SmartPtr<IBlockGenerator> blockGenerator ) = 0;

            /**
             * @brief Returns the procedural world module.
             *
             * This method returns the procedural world module used by the city generator to build the city world.
             * If no procedural world has been set, this method returns nullptr.
             *
             * @return The procedural world module, or nullptr if no world has been set.
             */
            virtual SmartPtr<IProceduralWorld> getProceduralWorld() const = 0;

            /**
             * @brief Sets the procedural world module.
             */
            virtual void setProceduralWorld( SmartPtr<IProceduralWorld> proceduralWorld ) = 0;

            virtual SmartPtr<IMeshGenerator> getMeshGenerator() const = 0;
            virtual void setMeshGenerator( SmartPtr<IMeshGenerator> meshGenerator ) = 0;

            virtual SmartPtr<IRoadGenerator> getRoadGenerator() const = 0;
            virtual void setRoadGenerator( SmartPtr<IRoadGenerator> roadGenerator ) = 0;

            virtual const Array<SmartPtr<IProceduralCity>> getCities() const = 0;
            virtual void removeCity( SmartPtr<IProceduralCity> city ) = 0;
            virtual void addCity( SmartPtr<IProceduralCity> city ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // ICityGenerator_h__
