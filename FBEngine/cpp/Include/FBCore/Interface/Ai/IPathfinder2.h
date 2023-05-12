#ifndef IPathfinder2_h__
#define IPathfinder2_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    
    class IPathfinder2 : public ISharedObject
    {
    public:
        enum class State
        {
            SEARCH_STATE_NOT_INITIALISED,
            SEARCH_STATE_SEARCHING,
            SEARCH_STATE_SUCCEEDED,
            SEARCH_STATE_FAILED,
            SEARCH_STATE_OUT_OF_MEMORY,
            SEARCH_STATE_INVALID
        };

        /** Virtual destructor. */
        ~IPathfinder2() override = default;

        /** */
        virtual void setGoal( const Vector2I &start, const Vector2I &goal ) = 0;

        /** */
        virtual u32 searchStep() = 0;

        virtual String getClassName() const = 0;
        virtual void setClassName( const String &className ) = 0;
    };
}  // end namespace fb

#endif  // IPathfinder2_h__
