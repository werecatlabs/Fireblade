#ifndef StateMessageMaterial_h__
#define StateMessageMaterial_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{

    class StateMessageMaterial : public StateMessage
    {
    public:
        StateMessageMaterial() = default;
        ~StateMessageMaterial() override = default;

        SmartPtr<render::IMaterial> getMaterial() const;
        void setMaterial( SmartPtr<render::IMaterial> material );

        s32 getIndex() const;
        void setIndex( s32 index );

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<render::IMaterial> m_material;
        s32 m_index = -1;
    };

}  // end namespace fb

#endif  // StateMessageMaterial_h__
