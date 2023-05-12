#ifndef IUITerrainEditor_h__
#define IUITerrainEditor_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        class IUITerrainEditor : public IUIElement
        {
        public:
            static const hash_type selectTerrainTextureHash;

            /** Destructor. */
            ~IUITerrainEditor() override = default;

            virtual SmartPtr<scene::IComponent> getTerrain() const = 0;

            virtual void setTerrain( SmartPtr<scene::IComponent> terrain ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif // IUITerrainEditor_h__
