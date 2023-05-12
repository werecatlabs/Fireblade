#ifndef IStandardObject_h__
#define IStandardObject_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IScriptObject : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IScriptObject() override = default;

        /** Gets an object call script functions. */
        virtual SmartPtr<IScriptInvoker> &getInvoker() = 0;

        /** Gets an object call script functions. */
        virtual const SmartPtr<IScriptInvoker> &getInvoker() const = 0;

        /** Sets an object call script functions. */
        virtual void setInvoker( SmartPtr<IScriptInvoker> invoker ) = 0;

        /** Gets an object to receive script calls. */
        virtual SmartPtr<IScriptReceiver> &getReceiver() = 0;

        /** Gets an object to receive script calls. */
        virtual const SmartPtr<IScriptReceiver> &getReceiver() const = 0;

        /** Sets an object to receive script calls. */
        virtual void setReceiver( SmartPtr<IScriptReceiver> receiver ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif  // IStandardObject_h__
