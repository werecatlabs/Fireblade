#ifndef IRoadConnectionData_h__
#define IRoadConnectionData_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace procedural
    {
        class IRoadConnectionData : public ISharedObject
        {
        public:
            ~IRoadConnectionData() override = default;

            virtual void setRoad( SmartPtr<IRoad> road ) = 0;
            virtual SmartPtr<IRoad> getRoad() const = 0;

            virtual void setMarker( s32 marker ) = 0;
            virtual int getMarker() const = 0;

            virtual void setConnection( s32 connection ) = 0;
            virtual int getConnection() const = 0;

            virtual void setTransform( Transform3<real_Num> transform ) = 0;
            virtual Transform3<real_Num> getTransform() const = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IRoadConnection_h__
