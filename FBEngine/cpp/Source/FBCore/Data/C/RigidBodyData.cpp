#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/RigidBodyData.h"
#include "FBCore/Base/StringUtil.h"

namespace fb
{
    SaracenRigidBodies MakeSaracenRigidBodies()
    {
        SaracenRigidBodies data;
        data.numRigidBodies = 0;
        return data;
    }

    RigidBodyData::RigidBodyData() :
        position( VectorData() ),
        orientation( VectorData() ),
        scale( VectorData() ),
        id( 0 ),
        reserved1( 0 ),
        reserved2( 0 ),
        reserved3( 0 )
    {
    }

    SaracenRigidBodies::SaracenRigidBodies() : numRigidBodies( 0 )
    {
        for( size_t i = 0; i < 2048; ++i )
        {
            rigidBodies[i] = RigidBodyData();
        }
    }
}  // namespace fb
