#ifndef TextureState_h__
#define TextureState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{
    class TextureState : public BaseState
    {
    public:
        TextureState() = default;
        ~TextureState() override = default;

        SmartPtr<IState> clone();

        Vector2I getSize() const;
        void setSize( const Vector2I &size );

        FB_CLASS_REGISTER_DECL;

    private:
        Vector2I m_size = Vector2I( 1024, 1024 );
    };
}  // end namespace fb

#endif  // TextureState_h__
