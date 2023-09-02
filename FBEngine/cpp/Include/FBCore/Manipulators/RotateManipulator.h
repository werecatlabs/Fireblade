#ifndef _RotateManipulator_H
#define _RotateManipulator_H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/FixedArray.h>

namespace fb
{

    class RotateManipulator : public ISharedObject
    {
    public:
        enum AxisType
        {
            AT_X = 0,
            AT_Y = 1,
            AT_Z = 2,
            AT_LAST = 3
        };

        RotateManipulator();
        RotateManipulator( ISelectionManager *selectionManager );
        ~RotateManipulator() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void update() override;

        bool OnEvent( const SmartPtr<IInputEvent> &event );

        void setEnabled( bool bEnabled );
        bool isEnabled();

        bool isVisible() const;
        void setVisible( bool visible );

    private:
        void createRenderObjects();

        void updateManipulatorPosition();

        const Vector3<real_Num> &getPosition() const;
        virtual void setPosition( const Vector3<real_Num> &newpos );

        virtual const Vector3<real_Num> &getRotation() const;
        virtual void setRotation( const Vector3<real_Num> &rotation );

        virtual const Vector3<real_Num> &getScale() const;
        virtual void setScale( const Vector3<real_Num> &scale );

        // ISelectionManagerListener functions
        void OnAddSelectedObject();
        void OnAddSelectedObjects();
        void OnSetSelectedObjects();

        void updateManipulatorPosition( ISelectionManager *SelectionManager );

        // TaskListenerAdapter<RotateManipulator>* m_msgDispatchListner;

        SmartPtr<render::ISceneNode> m_sceneNode;
        Array<SmartPtr<render::ISceneNode>> m_node;
        Array<SmartPtr<render::IGraphicsMesh>> m_mesh;

        ISelectionManager *m_selectionManager;

        AABB3<real_Num> Box;  //

        //! Relative translation of the scene node.
        Vector3<real_Num> RelativeTranslation;

        //! Relative rotation of the scene node.
        Vector3<real_Num> RelativeRotation;

        //! Relative scale of the scene node.
        Vector3<real_Num> RelativeScale;

        Vector3<real_Num> m_vPosition;  // store the gizmo's position	//should use the scene variable
        Vector3<real_Num> vAxis;        // the axis around which the translatation takes place
        Vector3<real_Num> m_vStart;     // The start position of the gizmo's movement

        Vector3<real_Num> m_intersectionPoint;

        Vector2<real_Num> m_prevCursor;
        Vector2<real_Num> m_cursorPos;

        f32 m_fScale;  // the scale of the manipulator components

        bool m_bXSelected;  // bool to indicate whether or not the x axis is selected
        bool m_bYSelected;  // bool to indicate whether or not the y axis is selected
        bool m_bZSelected;  // bool to indicate whether or not the z axis is selected

        bool m_bMouseButtonDown;  //
        bool m_bEnabled;          // boolean to know whether or not the gizmo should be shown

        bool m_isVisible;
        bool m_shouldLogMessages = false;
    };
}  // end namespace fb

#endif
