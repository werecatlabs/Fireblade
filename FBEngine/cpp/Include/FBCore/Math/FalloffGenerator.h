#ifndef FalloffGenerator_h__
#define FalloffGenerator_h__

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    template <class T>
    class FalloffGenerator : public CSharedObject<ISharedObject>
    {
    public:
        FalloffGenerator() = default;
        ~FalloffGenerator() override = default;

        Array<Array<T>> generate();
        T evaluate( T value );

        void setFalloffDirection( T falloffDirection );
        void setFalloffRange( T falloffRange );
        void setSize( s32 size );

        T getFalloffDirection() const;
        T getFalloffRange() const;
        s32 getSize() const;

    private:
        T m_falloffDirection = T( 1.0 );
        T m_falloffRange = T( 1.0 );
        s32 m_size = 0;
    };
}  // end namespace fb

#endif  // LookupTable_h__
