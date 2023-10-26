#include <FBCore/FBCorePCH.h>
#include "FBCore/Manipulators/RotateManipulator.h"
#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, RotateManipulator, ISharedObject );

    const f32 COMPONENT_RADIUS = 0.5f;
    const f32 TRANSLATE_REDUCTION_COEF = 0.02f;
    const f32 SCALE_COEF = 0.2f;

    const Vector3F g_origin( 0.f, 0.f, 0.f );
    const float g_radius = 64.0f;

    RotateManipulator::RotateManipulator( ISelectionManager *selectionManager ) :
        m_selectionManager( selectionManager ),
        m_fScale( 1.f ),
        m_bXSelected( false ),
        m_bYSelected( false ),
        m_bZSelected( false ),
        m_bMouseButtonDown( false ),
        m_bEnabled( false )
    {
    }

    RotateManipulator::RotateManipulator()
    {
    }

    RotateManipulator::~RotateManipulator()
    {
        unload( nullptr );
    }

    void RotateManipulator::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Loading && loadingState != LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Loading );

                m_node.resize( 6 );
                m_mesh.resize( 6 );

                //createRenderObjects();
                setLoadingState( LoadingState::Loaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void RotateManipulator::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            // if (m_msgDispatchListner)
            //{
            //	SmartPtr<ITaskManager>& taskManager = FBSystem::getSingletonPtr()->getTaskManager();
            //	bool result =
            // FBSystem::getSingletonPtr()->getTaskManager()->removeTaskListener(m_msgDispatchListner);
            //	_FB_DEBUG_BREAK_IF(!result);

            //	FB_SAFE_REMOVE(m_msgDispatchListner);
            //}

            for( auto node : m_node )
            {
                if( node )
                {
                    auto creator = node->getCreator();
                    if( creator )
                    {
                        creator->removeSceneNode( node );
                    }
                }
            }

            for( auto mesh : m_mesh )
            {
                if( mesh )
                {
                    auto owner = mesh->getOwner();
                    if( owner )
                    {
                        owner->detachObject( mesh );
                    }
                }
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void RotateManipulator::createRenderObjects()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto sceneManager = graphicsSystem->getGraphicsScene();

        auto sceneRoot = sceneManager->getRootSceneNode();
        m_sceneNode = sceneManager->addSceneNode();
        sceneRoot->addChild( m_sceneNode );

        String name = "CRotateGizmo";
        String temp = name + "_Y";

        // create and set up the nodes, meshes, collision shapes and handles
        m_node[AT_Y] = m_sceneNode->addChildSceneNode( temp );
        m_mesh[AT_Y] = sceneManager->addMesh( temp, "RotationRingVisible.mesh" );
        m_mesh[AT_Y]->setMaterialName( "ManipulatorHandlesRedMat" );
        m_mesh[AT_Y]->setRenderQueueGroup(
            static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
        m_mesh[AT_Y]->setCastShadows( false );
        m_node[AT_Y]->attachObject( m_mesh[AT_Y] );

        temp = name + "_X";

        // create and set up the nodes, meshes, collision shapes and handles
        m_node[AT_X] = m_sceneNode->addChildSceneNode( temp );

        Quaternion<real_Num> orient;
        orient.fromAngleAxis( Math<real_Num>::deg_to_rad() * 90.0f, Vector3<real_Num>::UNIT_Z );

        m_node[AT_X]->setOrientation( m_node[AT_X]->getOrientation() * orient );
        m_mesh[AT_X] = sceneManager->addMesh( temp, "RotationRingVisible.mesh" );
        m_mesh[AT_X]->setMaterialName( "ManipulatorHandlesGreenMat" );
        m_mesh[AT_X]->setRenderQueueGroup(
            static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
        m_mesh[AT_X]->setCastShadows( false );
        m_node[AT_X]->attachObject( m_mesh[AT_X] );

        temp = name + "_Z";

        // create and set up the nodes, meshes, collision shapes and handles
        m_node[AT_Z] = m_sceneNode->addChildSceneNode( temp );

        orient.fromAngleAxis( Math<real_Num>::deg_to_rad() * 90.0f, Vector3<real_Num>::UNIT_X );

        m_node[AT_Z]->setOrientation( m_node[AT_Z]->getOrientation() * orient );
        m_mesh[AT_Z] = sceneManager->addMesh( temp, "RotationRingVisible.mesh" );
        m_mesh[AT_Z]->setMaterialName( "ManipulatorHandlesBlueMat" );
        m_mesh[AT_Z]->setRenderQueueGroup(
            static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
        m_mesh[AT_Z]->setCastShadows( false );
        m_node[AT_Z]->attachObject( m_mesh[AT_Z] );

        m_sceneNode->setVisible( false );

        // m_msgDispatchListner = new TaskListenerAdapter<RotateManipulator>(this,
        // ITaskManager::MainThreadId);
        // FBSystem::getSingletonPtr()->getTaskManager()->addTaskListener(m_msgDispatchListner);

        // EditorEventReceiver* eventReceiver = EditorEventReceiver::getSingletonPtr();
        // eventReceiver->AddEventListener(this);

        // m_selectionManager->AddSelectionManagerListener(this);
    }

    void RotateManipulator::update()
    {
        if( !m_bEnabled )
        {
            return;
        }

        auto applicationManager = core::IApplicationManager::instance();
        auto cameraManager = applicationManager->getCameraManager();

        auto cameraActor = cameraManager->getCurrentCamera();
        if( cameraActor )
        {
            auto camera = cameraActor->getComponent<scene::CameraController>();

            auto cameraActorTransform = cameraActor->getTransform();
            auto cameraLocalTranform = cameraActorTransform->getLocalTransform();
            auto cameraWorldTransform = cameraActorTransform->getWorldTransform();

            auto camPos = cameraWorldTransform.getPosition();
            auto camDist = ( m_vPosition - camPos ).length();
            m_fScale = camDist * SCALE_COEF;

            auto scale = Vector3<real_Num>( m_fScale, m_fScale, m_fScale );
            m_sceneNode->setScale( scale );
        }

        m_sceneNode->updateBounds();
        auto box = m_sceneNode->getWorldAABB();

        // select the material
        if( m_bXSelected )
        {
            m_mesh[AT_X]->setMaterialName( "ManipulatorHandlesYellowMat" );
        }
        else
        {
            m_mesh[AT_X]->setMaterialName( "ManipulatorHandlesGreenMat" );
        }

        if( m_bYSelected )
        {
            m_mesh[AT_Y]->setMaterialName( "ManipulatorHandlesYellowMat" );
        }
        else
        {
            m_mesh[AT_Y]->setMaterialName( "ManipulatorHandlesRedMat" );
        }

        if( m_bZSelected )
        {
            m_mesh[AT_Z]->setMaterialName( "ManipulatorHandlesYellowMat" );
        }
        else
        {
            m_mesh[AT_Z]->setMaterialName( "ManipulatorHandlesBlueMat" );
        }
    }

    //
    // ISelectionManagerListener functions
    //

    void RotateManipulator::OnAddSelectedObject()
    {
    }

    void RotateManipulator::OnAddSelectedObjects()
    {
    }

    void RotateManipulator::OnSetSelectedObjects()
    {
    }

    bool RotateManipulator::OnEvent( const SmartPtr<IInputEvent> &event )
    {
        if( !m_bEnabled )
        {
            return false;
        }

        auto inputResult = false;

        // auto applicationManager = core::IApplicationManager::instance();
        // auto timer = applicationManager->getTimer();
        // auto cameraManager = applicationManager->getCameraManager();
        // auto selectionManager = applicationManager->getSelectionManager();

        // auto cameraActor = cameraManager->getCurrentCamera();
        // auto camera = cameraActor->getComponent<scene::CameraController>();
        // if (camera)
        //{
        //	auto mouseState = event->getMouseState();
        //	if (mouseState)
        //	{
        //		auto mouseStateEvent = (IMouseState::MouseInputEventType)mouseState->getEventType();

        //		if (mouseStateEvent == IMouseState::MOUSE_EVENT_LEFT_RELEASED)
        //		{
        //			m_bXSelected = false;
        //			m_bYSelected = false;
        //			m_bZSelected = false;
        //		}
        //		else if (mouseStateEvent == IMouseState::MOUSE_EVENT_LEFT_BUTTON_PRESSED)
        //		{
        //			auto mousePosition = mouseState->getAbsolutePosition();

        //			auto ray = camera->getCameraToViewportRay(mousePosition);

        //			const auto controlWidth = 0.05f;
        //			auto curControlWidth = controlWidth * m_fScale;

        //			m_bXSelected = false;
        //			m_bYSelected = false;
        //			m_bZSelected = false;

        //			auto position = getPosition();
        //			auto sphere = Sphere3F(position, m_fScale);

        //			auto result = MathUtilF::intersects(ray, sphere);
        //			//if (result.first)
        //			{
        //				const auto tollerance = 0.2f;

        //				auto nodeX = mNode[AT_X];
        //				nodeX->_updateBounds();
        //				auto aabbX = nodeX->getWorldAABB();
        //				auto aabbResultX = MathUtilF::intersects(ray, aabbX);
        //				if (aabbResultX.first)
        //				{
        //					auto lineX = Line3F(
        //						position - Vector3F::unitX() * controlWidth,
        //						position + Vector3F::unitX() * controlWidth);
        //					auto cylinderX = Cylinder3F(lineX, m_fScale * 2.0f, curControlWidth);
        //					auto cylinderResultX = MathUtilF::intersects(ray, cylinderX);
        //					if (cylinderResultX.first)
        //					{
        //						//auto graphicsSystem = applicationManager->getGraphicsSystem();
        //						//auto debug = graphicsSystem->getDebug();
        //						//debug->drawLine(0, getPosition(), intersectionPoint, 0xFFFFFF);

        //						auto intersectionPoint = ray.getOrigin() + (ray.getDirection() *
        // cylinderResultX.second); 						auto intersectionDistance =
        // (intersectionPoint
        // - position).length(); 						if (intersectionDistance > m_fScale * (1.0f -
        // tollerance) &&
        // intersectionDistance < m_fScale * (1.0f + tollerance))
        //						{
        //							m_bXSelected = true;

        //							if (mouseStateEvent == IMouseState::MOUSE_EVENT_LEFT_BUTTON_PRESSED)
        //							{
        //								inputResult = true;
        //							}
        //						}
        //					}
        //				}

        //				auto nodeY = mNode[AT_Y];
        //				nodeY->_updateBounds();
        //				auto aabbY = nodeY->getWorldAABB();
        //				auto aabbResult = MathUtilF::intersects(ray, aabbY);
        //				if (aabbResult.first)
        //				{
        //					auto lineY = Line3F(
        //						position - Vector3F::unitY() * controlWidth,
        //						position + Vector3F::unitY() * controlWidth);
        //					auto cylinderY = Cylinder3F(lineY, m_fScale * 2.0f, curControlWidth);
        //					auto cylinderResultY = MathUtilF::intersects(ray, cylinderY);
        //					if (cylinderResultY.first)
        //					{
        //						auto intersectionPoint = ray.getOrigin() + (ray.getDirection() *
        // cylinderResultY.second); 						auto intersectionDistance =
        // (intersectionPoint
        // - position).length(); 						if (intersectionDistance > m_fScale * (1.0f -
        // tollerance) &&
        // intersectionDistance < m_fScale * (1.0f + tollerance))
        //						{
        //							m_bYSelected = true;

        //							if (mouseStateEvent == IMouseState::MOUSE_EVENT_LEFT_BUTTON_PRESSED)
        //							{
        //								inputResult = true;
        //							}
        //						}
        //					}
        //				}

        //				auto nodeZ = mNode[AT_Z];
        //				nodeZ->_updateBounds();
        //				auto aabbZ = nodeZ->getWorldAABB();
        //				auto aabbResultZ = MathUtilF::intersects(ray, aabbZ);
        //				if (aabbResultZ.first)
        //				{
        //					auto lineZ = Line3F(
        //						position - Vector3F::unitX() * controlWidth,
        //						position + Vector3F::unitX() * controlWidth);
        //					auto cylinderZ = Cylinder3F(lineZ, m_fScale * 2.0f, curControlWidth);
        //					auto cylinderResultZ = MathUtilF::intersects(ray, cylinderZ);
        //					if (cylinderResultZ.first)
        //					{
        //						auto intersectionPoint = ray.getOrigin() + (ray.getDirection() *
        // cylinderResultZ.second); 						auto intersectionDistance =
        // (intersectionPoint
        // - position).length(); 						if (intersectionDistance > m_fScale * (1.0f -
        // tollerance) &&
        // intersectionDistance < m_fScale * (1.0f + tollerance))
        //						{
        //							m_bZSelected = true;

        //							if (mouseStateEvent == IMouseState::MOUSE_EVENT_LEFT_BUTTON_PRESSED)
        //							{
        //								inputResult = true;
        //							}
        //						}
        //					}
        //				}
        //			}
        //		}

        //		if ((mouseStateEvent == IMouseState::MOUSE_EVENT_MOVED) &&
        //			(m_bXSelected || m_bYSelected || m_bZSelected))
        //		{
        //			if (m_shouldLogMessages)
        //			{
        //				LOG_MESSAGE("TranslateManipulator", "CTranslateGizmo: EMIE_MOUSE_MOVED");
        //			}

        //			m_prevCursor = m_cursorPos;

        //			auto mousePosition = mouseState->getAbsolutePosition();
        //			m_cursorPos = mousePosition;

        //			if (m_bXSelected || m_bYSelected || m_bZSelected)
        //			{
        //				auto mouseMove = m_prevCursor - m_cursorPos;

        //				auto xAxis = Vector3F::UNIT_X;
        //				auto yAxis = Vector3F::UNIT_Y;
        //				auto zAxis = Vector3F::UNIT_Z;

        //				auto actor = camera->getActor();
        //				auto worldTransform = actor->getTransform();
        //				auto cameraOrientation = worldTransform->getOrientation();

        //				Matrix3F cameraRotation;

        //				cameraOrientation.toRotationMatrix(cameraRotation);
        //				xAxis = Vector3F(cameraRotation.m_[0], cameraRotation.m_[1],
        // cameraRotation.m_[2]); 				yAxis = Vector3F(cameraRotation.m_[4],
        // cameraRotation.m_[5], cameraRotation.m_[6]); 				zAxis =
        // Vector3F(cameraRotation.m_[8], cameraRotation.m_[9], cameraRotation.m_[10]);

        //				Vector3F translation;

        //				if (m_bXSelected)
        //				{
        //					auto xyPlane = (zAxis.Y() < real_Num(0.0) ? real_Num(-1.0) : real_Num(1.0));
        //					auto yAxis = (xAxis.Z() < real_Num(0.0) ? real_Num(1.0) : real_Num(-1.0));

        //					translation.X() += mouseMove.X() * xAxis.X();
        //					translation.X() += mouseMove.Y() * (1 - xAxis.X()) * xyPlane * yAxis;
        //				}
        //				else if (m_bYSelected)
        //				{
        //					translation.Y() += -mouseMove.Y() * (yAxis.Y() > real_Num(0.0) ?
        // real_Num(1.0)
        //: real_Num(-1.0));
        //				}
        //				else if (m_bZSelected)
        //				{
        //					auto yAxis = (zAxis.Z() < real_Num(0.0) ? real_Num(-1.0) : real_Num(1.0));

        //					translation.Z() += mouseMove.X() * xAxis.Z();
        //					translation.Z() += mouseMove.Y() * (real_Num(1.0) - xAxis.Z()) * yAxis;
        //				}

        //				translation *= m_fScale * TRANSLATE_REDUCTION_COEF;

        //				auto selection = selectionManager->getSelected();

        //				for (auto selected : selection)
        //				{
        //					auto actor = fb::dynamic_pointer_cast<IActor>(selected);
        //					if (actor)
        //					{
        //						auto transform = actor->getTransform();
        //						auto localTransform = transform->getLocalTransform();
        //						auto worldTransform = transform->getWorldTransform();

        //						auto entityPos = localTransform.getPosition();

        //						auto diff = Math<real_Num>::Abs((entityPos -
        // translation).squaredLength());

        //						if (diff > Math<real_Num>::epsilon())
        //						{
        //							auto worldPosition = entityPos - translation;
        //							//localTransform->setPosition(worldPosition);

        //							auto moveLength = (m_prevCursor.X() - m_cursorPos.X());
        //							moveLength += (m_prevCursor.Y() - m_cursorPos.Y());
        //							moveLength *= timer->getDeltaTime() * 20.0f;

        //							auto rotation = localTransform.getRotation();
        //							if (m_bXSelected)
        //							{
        //								rotation.X() += moveLength;
        //							}
        //							else if (m_bYSelected)
        //							{
        //								rotation.Y() += moveLength;
        //							}
        //							else if (m_bZSelected)
        //							{
        //								rotation.Z() += moveLength;
        //							}

        //							localTransform.setRotation(rotation);

        //							//auto localPosition =
        // worldTransform->convertWorldToLocalPosition(worldPosition);
        //							//localTransform->setPosition(localPosition);

        //							actor->updateTransform();
        //						}
        //					}
        //				}

        //				updateManipulatorPosition();
        //				inputResult = true;
        //			}
        //		}
        //	}
        //}

        return inputResult;
    }

    void RotateManipulator::updateManipulatorPosition( ISelectionManager *SelectionManager )
    {
    }

    void RotateManipulator::setEnabled( bool bEnabled )
    {
        if( m_bEnabled != bEnabled )
        {
            m_bEnabled = bEnabled;
            m_sceneNode->setVisible( m_bEnabled );
            updateManipulatorPosition();
        }
    }

    bool RotateManipulator::isEnabled()
    {
        return m_bEnabled;
    }

    const Vector3<real_Num> &RotateManipulator::getPosition() const
    {
        return RelativeTranslation;
    }

    void RotateManipulator::setPosition( const Vector3<real_Num> &newpos )
    {
        RelativeTranslation = newpos;

        if( m_sceneNode )
        {
            m_sceneNode->setPosition( newpos );
        }
    }

    const Vector3<real_Num> &RotateManipulator::getRotation() const
    {
        return RelativeRotation;
    }

    void RotateManipulator::setRotation( const Vector3<real_Num> &rotation )
    {
        RelativeRotation = rotation;
    }

    const Vector3<real_Num> &RotateManipulator::getScale() const
    {
        return RelativeScale;
    }

    void RotateManipulator::setScale( const Vector3<real_Num> &scale )
    {
        RelativeScale = scale;
    }

    bool RotateManipulator::isVisible() const
    {
        return m_isVisible;
    }

    void RotateManipulator::setVisible( bool visible )
    {
        m_isVisible = visible;
    }

    void RotateManipulator::updateManipulatorPosition()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto selectionManager = applicationManager->getSelectionManager();

        auto selection = selectionManager->getSelection();

        if( !selection.empty() )
        {
            auto vAveragePosition = Vector3<real_Num>::zero();

            // for (auto selected : selection)
            //{
            //	auto actor = fb::dynamic_pointer_cast<IActor>(selected);
            //	if (actor)
            //	{
            //		auto worldTransform = actor->getTransform();
            //		if (worldTransform)
            //		{
            //			vAveragePosition += worldTransform->getPosition();
            //		}
            //	}
            // }

            vAveragePosition /= static_cast<real_Num>( selection.size() );
            m_vPosition = vAveragePosition;

            setPosition( vAveragePosition );
            setVisible( true );
        }
        else
        {
            setVisible( false );
        }
    }
}  // end namespace fb
