#ifndef __IStateSceneManager_h__
#define __IStateSceneManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    class ITransformManager : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~ITransformManager() override = default;

        virtual SmartPtr<ITransformNode> getRootSceneNode() const = 0;

        virtual void clearScene() = 0;

        virtual SmartPtr<ITransformNode> createNode() = 0;
        virtual SmartPtr<ITransformNode> createNode( const String &name ) = 0;
        virtual void destroyNode( SmartPtr<ITransformNode> node ) = 0;

        virtual String getName() const = 0;
        virtual void setName( const String &name ) = 0;
    };
}  // end namespace fb

#endif  // __IStateSceneManager_h__
