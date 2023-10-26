#ifndef _TranslateManipulator_H
#define _TranslateManipulator_H

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/AABB3.h>
#include "FBCore/Math/Cylinder3.h"
#include <FBCore/Math/Line3.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/FixedArray.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    class TranslateManipulator : public ISharedObject
    {
    public:
        enum AxisType
        {
            AT_X = 0,
            AT_Y = 1,
            AT_Z = 2,
            AT_XY = 3,
            AT_XZ = 4,
            AT_YZ = 5,
            AT_LAST = 6
        };

        TranslateManipulator();
        ~TranslateManipulator() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        /** Used to update object. */
        void preUpdate() override;

        /** Used to update object. */
        void update() override;

        /** Used to update object. */
        void postUpdate() override;

        void createRenderObjects();

        bool OnEvent( const SmartPtr<IInputEvent> &event );

        void setEnabled( bool bEnabled );
        bool isEnabled();

        bool isVisible() const;
        void setVisible( bool visible );

        SmartPtr<render::ISceneNode> getSceneNode() const;

        void setSceneNode( SmartPtr<render::ISceneNode> sceneNode );

        Array<Line3<real_Num>> getLines() const;
        Array<Cylinder3<real_Num>> getHandles() const;

        FB_CLASS_REGISTER_DECL;

    private:
        class SelectionManagerListener : public IEventListener
        {
        public:
            SelectionManagerListener() = default;
            SelectionManagerListener( TranslateManipulator *manipulator );
            ~SelectionManagerListener() override;

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

            void addSelectedObject();
            void addSelectedObjects();
            void setSelectedObjects();
            void deselectObjects();
            void deselectAll();

            TranslateManipulator *getOwner() const;
            void setOwner( TranslateManipulator *owner );

        private:
            TranslateManipulator *m_owner = nullptr;
        };

        const Vector3<real_Num> &getPosition() const;
        void setPosition( const Vector3<real_Num> &newpos );

        const Vector3<real_Num> &getRotation() const;
        void setRotation( const Vector3<real_Num> &rotation );

        const Vector3<real_Num> &getScale() const;
        void setScale( const Vector3<real_Num> &scale );

        // ISelectionManagerListener functions
        void OnAddSelectedObject();
        void OnAddSelectedObjects();
        void OnSetSelectedObjects();
        void OnDeselectObjects();
        void OnDeselectAll();

        void updateManipulatorPosition();

        // graphical representation
        SmartPtr<render::ISceneNode> m_sceneNode;
        Array<SmartPtr<render::ISceneNode>> m_node;
        Array<SmartPtr<render::IGraphicsMesh>> m_mesh;
        Array<SmartPtr<render::IGraphicsMesh>> m_coneMesh;

        SmartPtr<SelectionManagerListener> m_selectionManagerListener;

        //! Relative translation of the scene node.
        Vector3<real_Num> m_relativeTranslation;

        //! Relative rotation of the scene node.
        Vector3<real_Num> m_relativeRotation;

        //! Relative scale of the scene node.
        Vector3<real_Num> m_relativeScale;        

        // store the gizmo's position	//should use the scene variable
        Vector3<real_Num> m_vPosition;

        // the axis around which the translatation takes place
        Vector3<real_Num> m_vAxis;

        // The start position of the gizmo's movement
        Vector3<real_Num> m_vStart;     

        Vector2<real_Num> m_prevCursor;
        Vector2<real_Num> m_cursorPos;

        // the scale of the manipulator components
        f32 m_fScale;  

        // bool to indicate whether or not the x axis is selected
        bool m_bXSelected = false;

        // bool to indicate whether or not the y axis is selected
        bool m_bYSelected = false;

        // bool to indicate whether or not the z axis is selected
        bool m_bZSelected = false;

        //
        bool m_bMouseButtonDown;  

        // boolean to know whether or not the gizmo is enabled
        bool m_enabled;          

        // boolean to know whether or not the gizmo should be shown
        bool m_visible;         

        bool m_shouldLogMessages;

        Array<String> m_prevPropValues;
    };
}  // end namespace fb

#endif
