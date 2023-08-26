#ifndef StateMessageBase_h__
#define StateMessageBase_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{
    class StateMessage : public IStateMessage
    {
    public:
        static const hash_type SET_OBJECT;
        static const hash_type SET_MESH;
        static const hash_type SET_CUBEMAP;
        static const hash_type SET_TEXTURES;

        StateMessage() = default;
        ~StateMessage() override = default;

        void unload( SmartPtr<ISharedObject> data ) override;

        hash_type getType() const override;
        void setType( hash_type type ) override;

        SmartPtr<ISharedObject> getSender() const override;
        void setSender( SmartPtr<ISharedObject> object ) override;

        SmartPtr<IStateContext> getStateObject() const override;
        void setStateObject( SmartPtr<IStateContext> object ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        hash_type m_type = 0;
        AtomicWeakPtr<ISharedObject> m_sender;
        AtomicWeakPtr<IStateContext> m_stateObject;
    };
}  // end namespace fb

#endif  // StateMessageBase_h__
