#ifndef MaterialState_h__
#define MaterialState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Interface/Graphics/IMaterial.h>

namespace fb
{
    class MaterialState : public BaseState
    {
    public:
        MaterialState();
        ~MaterialState() override;

        render::IMaterial::MaterialType getMaterialType() const;
        void setMaterialType( render::IMaterial::MaterialType materialType );

        FB_CLASS_REGISTER_DECL;

    protected:
        Atomic<render::IMaterial::MaterialType> m_materialType =
            render::IMaterial::MaterialType::Standard;
    };

}  // namespace fb

#endif  // MaterialState_h__
