#ifndef IPathNode2_h__
#define IPathNode2_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{

    class IPathNode2 : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IPathNode2() override = default;

        virtual Vector2I getPosition() const = 0;
        virtual void setPosition( const Vector2I &position ) = 0;

        virtual f32 getCost( SmartPtr<IPathNode2> successor ) = 0;
    };

}  // end namespace fb

#endif  // IPathNode2_h__
