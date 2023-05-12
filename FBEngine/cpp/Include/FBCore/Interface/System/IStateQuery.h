#ifndef IStateQuery_h__
#define IStateQuery_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    /** Used to query the state of an object. */
    class IStateQuery : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IStateQuery() override = default;

        /** Gets the query type. */
        virtual u32 getType() const = 0;

        /** Sets the query type. */
        virtual void setType( u32 type ) = 0;

        /** Gets a boolean indicating whether the results are ready. */
        virtual bool isResultReady() const = 0;

        /** Gets a boolean indicating whether the results are ready. */
        virtual void setResultReady( bool resultReady ) = 0;

        /** Gets the result code of the query. */
        virtual s32 getResultCode() const = 0;

        /** Sets the result code of the query. */
        virtual void setResultCode( s32 resultCode ) = 0;

        /** Waits until the results of the query are ready. */
        virtual void wait() = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IStateQuery_h__
