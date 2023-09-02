#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/TargetViewCameraCtrl.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {

        TargetViewCameraCtrl::TargetViewCameraCtrl()
        {
            // FSMStandard(MVCS_INITIALIZING)
        }

        TargetViewCameraCtrl::~TargetViewCameraCtrl()
        {
        }

        void TargetViewCameraCtrl::initialise( SmartPtr<render::ISceneNode> cameraSceneNode )
        {
            m_cameraSceneNode = cameraSceneNode;
            m_cameraSceneNode->setFixedYawAxis( true );

            m_coords.X() = 30.0f;
            m_coords.Z() = 1.0f;
        }

        void TargetViewCameraCtrl::update()
        {
            // updateFSM();
        }

        void TargetViewCameraCtrl::setPosition( const Vector3F &position )
        {
        }

        Vector3F TargetViewCameraCtrl::getPosition() const
        {
            return Vector3F::zero();
        }

        void TargetViewCameraCtrl::setTargetPosition( const Vector3F &position )
        {
        }

        Vector3F TargetViewCameraCtrl::getTargetPosition() const
        {
            return Vector3F::zero();
        }

        void TargetViewCameraCtrl::OnEnterState( u8 state )
        {
        }

        void TargetViewCameraCtrl::OnUpdateState()
        {
        }

        void TargetViewCameraCtrl::OnLeaveState( u8 state )
        {
        }

        Vector3F TargetViewCameraCtrl::sphericalToCartesian( const Vector3F &coords )
        {
            Vector3F cart;
            Vector3F coordinates = coords;

            WrapCoords( coordinates );

            // due to model orientation the x and the z are swapped
            cart.Z() = coords.X() * MathF::Sin( coordinates.Z() ) * MathF::Cos( coordinates.Y() );
            cart.X() = coords.X() * MathF::Sin( coordinates.Z() ) * MathF::Sin( coordinates.Y() );
            cart.Y() = coords.X() * MathF::Cos( coordinates.Z() );

            return cart;
        }

        Vector3F TargetViewCameraCtrl::CalcSphericalCoords( const Vector3F &cameraDir )
        {
            Vector3F cart;
            cart.X() = cameraDir.length();
            cart.Z() = MathF::ACos( MathF::Abs( cameraDir.Y() ) / MathF::Abs( cart.X() ) );
            //	cart.Y() = atan( cart.X() / cart.Z() );

            Vector3F vector = -cameraDir;
            cart.Y() = MathF::ATan2( vector.X(), vector.Z() );

            WrapCoords( cart );

            return cart;
        }

        void TargetViewCameraCtrl::WrapCoords( Vector3F &coords )
        {
            if( coords.Y() >= MathF::pi() )
            {
                coords.Y() += -2 * MathF::pi();
            }
            else if( coords.Y() <= -MathF::pi() )
            {
                coords.Y() += 2 * MathF::pi();
            }

            if( coords.Z() >= ( MathF::pi() - MathF::epsilon() ) )
            {
                coords.Z() = ( MathF::pi() - MathF::epsilon() );
            }
            else if( coords.Z() <= MathF::epsilon() )
            {
                coords.Z() = MathF::epsilon();
            }
        }
    }  // namespace scene
}  // end namespace fb
