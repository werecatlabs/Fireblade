#ifndef StateMessageSkyBox_h__
#define StateMessageSkyBox_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageSkyBox : public StateMessage
    {
    public:
        StateMessageSkyBox() = default;
        ~StateMessageSkyBox() override = default;

        SmartPtr<render::IMaterial> getMaterial() const;
        void setMaterial( SmartPtr<render::IMaterial> material );

        String getMaterialName() const;
        void setMaterialName( const String &materialName );

        f32 getDistance() const;
        void setDistance( f32 val );

        bool getEnable() const;
        void setEnable( bool val );

        bool getDrawFirst() const;
        void setDrawFirst( bool val );

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<render::IMaterial> m_material;
        String m_materialName;
        f32 m_distance = 0.0f;
        bool m_enable = true;
        bool m_drawFirst = true;
    };
}  // end namespace fb

#endif  // StateMessageSkyBox_h__
