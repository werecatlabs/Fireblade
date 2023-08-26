#ifndef LookupTable_h__
#define LookupTable_h__

#include <FBCore/FBCoreTypes.h>
#include "FBCore/Core/Array.h"
#include "FBCore/Interface/Memory/ISharedObject.h"

namespace fb
{
    class LookupTable : public ISharedObject
    {
    public:
        LookupTable();
        ~LookupTable() override;

        void setPoints( const Array<std::pair<real_Num, real_Num>> &points );
        void setPoints( const Array<real_Num> &keys, const Array<real_Num> &points );
        void setPoints( const Array<real_Num> &points );

        real_Num interpolate( real_Num t );

    private:
        Array<std::pair<real_Num, real_Num>> m_points;
    };
}  // end namespace fb

#endif  // LookupTable_h__
