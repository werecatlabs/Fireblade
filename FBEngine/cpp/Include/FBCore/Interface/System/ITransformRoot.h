#ifndef __IStateRoot_h__
#define __IStateRoot_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{
    class ITransformRoot : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~ITransformRoot() override = default;

        virtual SmartPtr<ITransformManager> createSceneManager( const String &typeName,
                                                                 const String &instanceName ) = 0;
        virtual void destroySceneManager( SmartPtr<ITransformManager> sm ) = 0;
        virtual SmartPtr<ITransformManager> getSceneManager( const String &instanceName ) const = 0;

        virtual bool hasSceneManager( const String &instanceName ) const = 0;
        virtual Array<SmartPtr<ITransformManager>> getSceneManagers() const = 0;

        static SmartPtr<ITransformRoot> instance();
        static void setInstance( SmartPtr<ITransformRoot> instance );

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // __IStateRoot_h__
