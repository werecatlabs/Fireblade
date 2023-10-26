#include <FBCore/FBCorePCH.h>
#include "FBCore/Manipulators/ScaleManipulator.h"
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ScaleManipulator, ISharedObject );

    const f32 COMPONENT_RADIUS = 0.5f;
    const f32 TRANSLATE_REDUCTION_COEF = 0.02f;
    const f32 SCALE_COEF = 0.2f;

    ScaleManipulator::ScaleManipulator( ISelectionManager *selectionManager ) :
        m_fScale( 1.f ),
        m_bXSelected( false ),
        m_bYSelected( false ),
        m_bZSelected( false ),
        m_bMouseButtonDown( false ),
        m_bEnabled( false )
    {
    }

    ScaleManipulator::ScaleManipulator()
    {
    }

    ScaleManipulator::~ScaleManipulator()
    {
        unload( nullptr );
    }

    void ScaleManipulator::load( SmartPtr<ISharedObject> data )
    {
    }

    void ScaleManipulator::unload( SmartPtr<ISharedObject> data )
    {
    }

    //
    // ISelectionManagerListener functions
    //

    void ScaleManipulator::OnAddSelectedObject()
    {
    }

    void ScaleManipulator::OnAddSelectedObjects()
    {
    }

    void ScaleManipulator::OnSetSelectedObjects()
    {
    }

    //
    // IEventListener functions
    //

    //
    // bool ScaleManipulator::OnEvent(const InputEvent& event)
    //{

    //	return false;
    //}

    void ScaleManipulator::UpdateManipulatorPosition( ISelectionManager *SelectionManager )
    {
    }

    void ScaleManipulator::SetEnabled( bool bEnabled )
    {
        m_bEnabled = bEnabled;
    }

    bool ScaleManipulator::IsEnabled()
    {
        return m_bEnabled;
    }

    const Vector3F &ScaleManipulator::getPosition() const
    {
        return RelativeTranslation;
    }

    void ScaleManipulator::setPosition( const Vector3F &newpos )
    {
        RelativeTranslation = newpos;
    }

    const Vector3F &ScaleManipulator::getRotation() const
    {
        return RelativeRotation;
    }

    void ScaleManipulator::setRotation( const Vector3F &rotation )
    {
        RelativeRotation = rotation;
    }

    const Vector3F &ScaleManipulator::getScale() const
    {
        return RelativeScale;
    }

    void ScaleManipulator::setScale( const Vector3F &scale )
    {
        RelativeScale = scale;
    }

    bool ScaleManipulator::isVisible() const
    {
        return IsVisible;
    }

    void ScaleManipulator::setVisible( bool isVisible )
    {
        IsVisible = isVisible;
    }
}  // end namespace fb
