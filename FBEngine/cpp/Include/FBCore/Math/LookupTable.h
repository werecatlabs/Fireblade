#ifndef LookupTable_h__
#define LookupTable_h__

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/FBCoreTypes.h>
#include <vector>

namespace fb
{
    class LookupTable : public CSharedObject<ISharedObject>
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
