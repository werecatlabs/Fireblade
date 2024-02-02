#ifndef INode_h__
#define INode_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace render
    {

        /** Interface for a node in a scene graph. */
        class INode : public ISharedObject
        {
        public:
            /** Destructor. */
            ~INode() override = default;

            /** Returns the name of the node. */
            virtual String getName() const = 0;

            /** Gets this node's parent (NULL if this is the root).
             */
            virtual SmartPtr<INode> getParent() const = 0;

            virtual QuaternionF getOrientation() const = 0;
            virtual void setOrientation( const QuaternionF &q ) = 0;

            virtual Vector3F getPosition() const = 0;
            virtual void setPosition( const Vector3F &pos ) = 0;

            virtual Vector3F getScale() const = 0;
            virtual void setScale( const Vector3F &scale ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace render
}  // namespace fb

#endif  // INode_h__
