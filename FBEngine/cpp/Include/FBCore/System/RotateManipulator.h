#ifndef _RotateManipulator_H
#define _RotateManipulator_H

#include <FBCore/System/Manipulator.h>

namespace fb
{

    /** The RotateManipulator is used to rotate objects in the scene.
     *  It is a gizmo consisting of three circles, one for each axis.
     *  The user can click and drag on the circles to rotate the object.
     *  The circles are drawn in the x-y, y-z and x-z planes.
     */
    class RotateManipulator : public Manipulator
    {
    public:
        /** Constructor. */
        RotateManipulator();

        /** Destructor. */
        ~RotateManipulator() override;

        /** Loads the manipulator.
         *  @param data The data to load.
         */
        void load( SmartPtr<ISharedObject> data ) override;

        /** Unloads the manipulator.
         *  @param data The data to unload.
         */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** Updates the manipulator.
         */
        void update() override;

        /** Handles an input event.
         *  @param event The input event to handle.
         *  @return True if the event was handled, false otherwise.
         */
        bool handleEvent( const SmartPtr<IInputEvent> &event );

        FB_CLASS_REGISTER_DECL;

    private:
        // the axis around which the translatation takes place
        Vector3<real_Num> vAxis;

        // The start position of the gizmo's movement
        Vector3<real_Num> m_vStart;

        // The current position of the gizmo's movement
        Vector3<real_Num> m_intersectionPoint;

        // The current position of the gizmo's movement
        Vector2<real_Num> m_prevCursor;

        // The current position of the gizmo's movement
        Vector2<real_Num> m_cursorPos;

        // The tolerance for the intersection test
        f32 m_tollerance = 0.2f;

        // The width of the control circles
        f32 m_controlWidth = 0.05f;

        // Controls the size of the circles
        f32 m_cameraDistanceScale = 0.2f;

        // The control circles
        Array<SmartPtr<render::IDebugCircle>> m_circles;

        // The control circles orientations
        Array<Quaternion<real_Num>> m_orientations;

        // To know which control circles are selected
        Array<bool> m_selected;
    };
}  // end namespace fb

#endif
