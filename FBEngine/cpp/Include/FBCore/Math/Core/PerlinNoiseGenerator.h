#ifndef PerlinNoiseGenerator_h__
#define PerlinNoiseGenerator_h__

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    template <class T>
    class PerlinNoiseGenerator : public CSharedObject<ISharedObject>
    {
    public:
        PerlinNoiseGenerator() = default;
        ~PerlinNoiseGenerator() override = default;

        Array<Array<T>> generate();

        Array<Array<T>> generate( T &maxLocalNoiseHeight, T &minLocalNoiseHeight );

        Array<Array<T>> generateNoise( T &maxLocalNoiseHeight, T &minLocalNoiseHeight );

        int getSize() const;
        void setSize( int size );

        int getOctaves() const;
        void setOctaves( int octaves );

        T getScale() const;
        void setScale( T scale );

        T getOffset() const;
        void setOffset( T offset );

        T getPersistance() const;
        void setPersistance( T persistance );

        T getLacunarity() const;
        void setLacunarity( T lacunarity );

    private:
        s32 m_size = 0;
        s32 m_octaves = 0;
        T m_scale = T( 1.0 );
        T m_offset = T( 0.0 );
        T m_persistance = T( 0.0 );
        T m_lacunarity = T( 0.0 );
    };
}  // end namespace fb

#endif  // PerlinNoiseGenerator_h__
