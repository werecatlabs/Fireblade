#ifndef _ScaleManipulator_H
#define _ScaleManipulator_H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{

    class ScaleManipulator : public ISharedObject
    {
    public:
        enum AxisType
        {
            AT_X = 0,
            AT_Y = 1,
            AT_Z = 2,
            AT_LAST = 3
        };

        ScaleManipulator();
        ScaleManipulator( ISelectionManager *selectionManager );
        ~ScaleManipulator() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void SetEnabled( bool bEnabled );
        bool IsEnabled();

        bool isVisible() const;
        void setVisible( bool isVisible );

        FB_CLASS_REGISTER_DECL;

    private:
        const Vector3F &getPosition() const;
        void setPosition( const Vector3F &newpos );

        const Vector3F &getRotation() const;
        void setRotation( const Vector3F &rotation );

        const Vector3F &getScale() const;
        void setScale( const Vector3F &scale );

        // ISelectionManagerListener functions
        void OnAddSelectedObject();
        void OnAddSelectedObjects();
        void OnSetSelectedObjects();

        void UpdateManipulatorPosition( ISelectionManager *SelectionManager );

        ISelectionManager *m_selectionManager;

        AABB3<f32> Box;  //

        //! Relative translation of the scene node.
        Vector3F RelativeTranslation;

        //! Relative rotation of the scene node.
        Vector3F RelativeRotation;

        //! Relative scale of the scene node.
        Vector3F RelativeScale;

        Vector3F m_vPosition;  // store the gizmo's position	//should use the scene variable
        Vector3F vAxis;        // the axis around which the translatation takes place
        Vector3F m_vStart;     // The start position of the gizmo's movement

        f32 m_fScale;  // the scale of the manipulator components

        bool m_bXSelected;  // bool to indicate whether or not the x axis is selected
        bool m_bYSelected;  // bool to indicate whether or not the y axis is selected
        bool m_bZSelected;  // bool to indicate whether or not the z axis is selected

        bool m_bMouseButtonDown;  //
        bool m_bEnabled;          // boolean to know whether or not the gizmo should be shown

        bool IsVisible;
    };
}  // end namespace fb

#endif
