#ifndef TerrainState_h__
#define TerrainState_h__

#include <FBCore/State/States/GraphicsObjectState.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    class TerrainState : public GraphicsObjectState
    {
    public:
        TerrainState();

        ~TerrainState() override;

        Transform3<real_Num> getWorldTransform() const;

        void setWorldTransform( const Transform3<real_Num> &worldTransform );

        FB_CLASS_REGISTER_DECL;

    protected:
        Transform3<real_Num> m_worldTransform;
    };
}  // end namespace fb

#endif  // TerrainState_h__
