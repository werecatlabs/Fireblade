#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/GraphicsMeshState.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, GraphicsMeshState, GraphicsObjectState );

    GraphicsMeshState::GraphicsMeshState() = default;

    GraphicsMeshState::~GraphicsMeshState() = default;

    auto GraphicsMeshState::clone() const -> SmartPtr<IState>
    {
        auto state = fb::make_ptr<GraphicsMeshState>();

        //state->visibilityMask = visibilityMask;
        //state->visible = visible;
        //state->castShadows = castShadows;

        return state;
    }
}  // end namespace fb
