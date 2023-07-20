#ifndef _IAI_H
#define _IAI_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** A base ai class.
     */
    class IAi : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IAi() override = default;

        /** Sets an entity flag. */
        virtual void setFlag( u32 entityFlag, bool value ) = 0;

        /** Gets the value of an entity flag. */
        virtual bool getFlag( u32 entityFlag ) = 0;

        /** Sets a debug flag for the entity. */
        virtual void setDebugFlag( u32 debugFlag, bool value ) = 0;

        /** Gets the value of this debug flag. */
        virtual bool getDebugFlag( u32 debugFlag ) = 0;

        /** Sets the value of a log flag for the entity. */
        virtual void setLogFlag( u32 logFlag, bool value ) = 0;

        /** Gets the value of a log flag for the entity. */
        virtual bool getLogFlag( u32 logFlag ) = 0;

        virtual SmartPtr<IFSM> &getFSM() = 0;
        virtual const SmartPtr<IFSM> &getFSM() const = 0;
        virtual void setFSM( SmartPtr<IFSM> &fsm ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif
