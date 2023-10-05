#include <FBCore/FBCorePCH.h>
#include <FBCore/Manipulators/TranslateManipulator.h>
#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/FBCore.h>

namespace fb
{
    f32 COMPONENT_RADIUS = 0.1f;
    f32 TRANSLATE_REDUCTION_COEF = 0.02f;
    f32 SCALE_COEF = 0.2f;

    TranslateManipulator::TranslateManipulator()
    {
    }

    TranslateManipulator::~TranslateManipulator()
    {
        unload( nullptr );
    }

    void TranslateManipulator::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Loading && loadingState != LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Loading );

                m_node.resize( 6 );
                m_mesh.resize( 6 );
                m_coneMesh.resize( 3 );

                createRenderObjects();
                setLoadingState( LoadingState::Loaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TranslateManipulator::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                auto selectionManager = applicationManager->getSelectionManager();

                //if( selectionManager )
                //{
                //    if( m_selectionManagerListener )
                //    {
                //        selectionManager->removeListener( m_selectionManagerListener );
                //        m_selectionManagerListener = nullptr;
                //    }
                //}

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

                for( auto coneMesh : m_coneMesh )
                {
                    if( coneMesh )
                    {
                        auto owner = coneMesh->getOwner();
                        if( owner )
                        {
                            owner->detachObject( coneMesh );
                        }
                    }
                }

                auto count = 0;
                for( auto node : m_node )
                {
                    if( count == AT_X )
                    {
                        int stop = 0;
                        stop = 0;
                    }

                    if( node )
                    {
                        if( auto creator = node->getCreator() )
                        {
                            creator->removeSceneNode( node );
                        }
                    }

                    ++count;
                }

                if( auto node = getSceneNode() )
                {
                    node->removeChildren();

                    if( auto creator = node->getCreator() )
                    {
                        creator->removeSceneNode( node );
                    }

                    setSceneNode( nullptr );
                }

                m_mesh.clear();
                m_coneMesh.clear();
                m_node.clear();

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TranslateManipulator::createRenderObjects()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto sceneManager = graphicsSystem->getGraphicsScene();
            auto selectionManager = applicationManager->getSelectionManager();

            auto sceneRoot = sceneManager->getRootSceneNode();
            m_sceneNode = sceneManager->addSceneNode();
            sceneRoot->addChild( m_sceneNode );

            auto name = String( "CTranslateGizmo" );
            auto temp = name + "_Y";

            // create and set up the nodes, meshes, collision shapes and handles
            m_node[AT_Y] = m_sceneNode->addChildSceneNode( temp );
            m_mesh[AT_Y] = sceneManager->addMesh( temp + "_Line", "MoveArrowLineVisible.mesh" );
            m_mesh[AT_Y]->setMaterialName( "ManipulatorHandlesRedMat" );
            m_coneMesh[AT_Y] = sceneManager->addMesh( temp + "_Cone", "MoveArrowConeVisible.mesh" );
            m_coneMesh[AT_Y]->setMaterialName( "ManipulatorHandlesRedMat" );
            m_mesh[AT_Y]->setRenderQueueGroup(
                static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
            m_coneMesh[AT_Y]->setRenderQueueGroup(
                static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
            m_mesh[AT_Y]->setCastShadows( false );
            m_coneMesh[AT_Y]->setCastShadows( false );
            m_node[AT_Y]->attachObject( m_mesh[AT_Y] );
            m_node[AT_Y]->attachObject( m_coneMesh[AT_Y] );

            temp = name + "_X";

            m_node[AT_X] = m_sceneNode->addChildSceneNode( temp );

            Quaternion<real_Num> orient;
            orient.fromAngleAxis( MathF::deg_to_rad() * 270.0f, Vector3<real_Num>::UNIT_Z );

            m_node[AT_X]->setOrientation( m_node[AT_X]->getOrientation() * orient );
            m_mesh[AT_X] = sceneManager->addMesh( temp + "_Line", "MoveArrowLineVisible.mesh" );
            m_mesh[AT_X]->setMaterialName( "ManipulatorHandlesGreenMat" );
            m_coneMesh[AT_X] = sceneManager->addMesh( temp + "_Cone", "MoveArrowConeVisible.mesh" );
            m_coneMesh[AT_X]->setMaterialName( "ManipulatorHandlesGreenMat" );
            m_mesh[AT_X]->setRenderQueueGroup(
                static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
            m_coneMesh[AT_X]->setRenderQueueGroup(
                static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
            m_mesh[AT_X]->setCastShadows( false );
            m_coneMesh[AT_Y]->setCastShadows( false );
            m_node[AT_X]->attachObject( m_mesh[AT_X] );
            m_node[AT_X]->attachObject( m_coneMesh[AT_X] );

            temp = name + "_Z";

            m_node[AT_Z] = m_sceneNode->addChildSceneNode( temp );

            orient.fromAngleAxis( MathF::deg_to_rad() * 90.0f, Vector3<real_Num>::UNIT_X );

            m_node[AT_Z]->setOrientation( orient );
            m_mesh[AT_Z] = sceneManager->addMesh( temp + "_Line", "MoveArrowLineVisible.mesh" );
            m_mesh[AT_Z]->setMaterialName( "ManipulatorHandlesBlueMat" );
            m_coneMesh[AT_Z] = sceneManager->addMesh( temp + "_Cone", "MoveArrowConeVisible.mesh" );
            m_coneMesh[AT_Z]->setMaterialName( "ManipulatorHandlesBlueMat" );
            m_mesh[AT_Z]->setRenderQueueGroup(
                static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
            m_coneMesh[AT_Z]->setRenderQueueGroup(
                static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
            m_mesh[AT_Z]->setCastShadows( false );
            m_coneMesh[AT_Z]->setCastShadows( false );
            m_node[AT_Z]->attachObject( m_mesh[AT_Z] );
            m_node[AT_Z]->attachObject( m_coneMesh[AT_Z] );

            temp = name + "_XY";

            m_node[AT_XY] = m_sceneNode->addChildSceneNode( temp );
            m_mesh[AT_XY] = sceneManager->addMesh( temp, "MoveTwoAxisVisible.mesh" );
            m_mesh[AT_XY]->setMaterialName( "ManipulatorHandlesRedMat" );
            m_mesh[AT_XY]->setRenderQueueGroup(
                static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
            m_mesh[AT_XY]->setCastShadows( false );
            m_node[AT_XY]->attachObject( m_mesh[AT_XY] );

            temp = name + "_XZ";

            m_node[AT_XZ] = m_sceneNode->addChildSceneNode( temp );

            orient.fromAngleAxis( MathF::deg_to_rad() * 90.0f, Vector3F::UNIT_X );

            m_node[AT_XZ]->setOrientation( m_node[AT_XZ]->getOrientation() * orient );
            m_mesh[AT_XZ] = sceneManager->addMesh( temp, "MoveTwoAxisVisible.mesh" );
            m_mesh[AT_XZ]->setMaterialName( "ManipulatorHandlesGreenMat" );
            m_mesh[AT_XZ]->setRenderQueueGroup(
                static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
            m_mesh[AT_XZ]->setCastShadows( false );
            m_node[AT_XZ]->attachObject( m_mesh[AT_XZ] );

            temp = name + "_YZ";

            m_node[AT_YZ] = m_sceneNode->addChildSceneNode( temp );

            orient.fromAngleAxis( MathF::deg_to_rad() * 270.0f, Vector3F::UNIT_Y );

            m_node[AT_YZ]->setOrientation( m_node[AT_YZ]->getOrientation() * orient );
            m_mesh[AT_YZ] = sceneManager->addMesh( temp, "MoveTwoAxisVisible.mesh" );
            m_mesh[AT_YZ]->setMaterialName( "ManipulatorHandlesBlueMat" );
            m_mesh[AT_YZ]->setRenderQueueGroup(
                static_cast<u32>( render::IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_9 ) );
            m_mesh[AT_YZ]->setCastShadows( false );
            m_node[AT_YZ]->attachObject( m_mesh[AT_YZ] );

            if( selectionManager )
            {
                m_selectionManagerListener = fb::make_ptr<SelectionManagerListener>( this );
                selectionManager->addObjectListener( m_selectionManagerListener );
            }

            m_sceneNode->setVisible( false );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TranslateManipulator::preUpdate()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto timer = applicationManager->getTimer();

        auto task = Thread::getCurrentTask();
        auto t = timer->getTime();
        auto dt = timer->getDeltaTime();

        switch( task )
        {
        case Thread::Task::Application:
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
                auto cameraWorldTransform = cameraActor->getTransform();

                auto camPos = cameraWorldTransform->getPosition();
                auto camDist = ( m_vPosition - camPos ).length();
                if( camDist < 1.0f )
                {
                    camDist = 1.0f;
                }

                auto scale = Vector3F( 1.f, 1.f, 1.f );
                scale *= camDist * SCALE_COEF;
                m_fScale = scale.length();

                m_sceneNode->setScale( scale );
            }

            // select the material
            if( m_bXSelected )
            {
                m_mesh[AT_X]->setMaterialName( "ManipulatorHandlesYellowMat" );
                m_coneMesh[AT_X]->setMaterialName( "ManipulatorHandlesYellowMat" );
            }
            else
            {
                m_mesh[AT_X]->setMaterialName( "ManipulatorHandlesGreenMat" );
                m_coneMesh[AT_X]->setMaterialName( "ManipulatorHandlesGreenMat" );
            }

            if( m_bYSelected )
            {
                m_mesh[AT_Y]->setMaterialName( "ManipulatorHandlesYellowMat" );
                m_coneMesh[AT_Y]->setMaterialName( "ManipulatorHandlesYellowMat" );
            }
            else
            {
                m_mesh[AT_Y]->setMaterialName( "ManipulatorHandlesRedMat" );
                m_coneMesh[AT_Y]->setMaterialName( "ManipulatorHandlesRedMat" );
            }

            if( m_bZSelected )
            {
                m_mesh[AT_Z]->setMaterialName( "ManipulatorHandlesYellowMat" );
                m_coneMesh[AT_Z]->setMaterialName( "ManipulatorHandlesYellowMat" );
            }
            else
            {
                m_mesh[AT_Z]->setMaterialName( "ManipulatorHandlesBlueMat" );
                m_coneMesh[AT_Z]->setMaterialName( "ManipulatorHandlesBlueMat" );
            }
        }
        break;
        default:
        {
        }
        }
    }

    void TranslateManipulator::update()
    {
    }

    void TranslateManipulator::postUpdate()
    {
    }

    void TranslateManipulator::OnAddSelectedObject()
    {
        updateManipulatorPosition();
    }

    void TranslateManipulator::OnAddSelectedObjects()
    {
        updateManipulatorPosition();
    }

    void TranslateManipulator::OnSetSelectedObjects()
    {
        updateManipulatorPosition();
    }

    void TranslateManipulator::OnDeselectObjects()
    {
        updateManipulatorPosition();
    }

    void TranslateManipulator::OnDeselectAll()
    {
    }

    bool TranslateManipulator::OnEvent( const SmartPtr<IInputEvent> &event )
    {
        if( !isLoaded() )
        {
            return false;
        }

        if( !m_bEnabled )
        {
            return false;
        }

        if( event->getEventType() != IInputEvent::EventType::Mouse )
        {
            return false;
        }

        auto applicationManager = core::IApplicationManager::instance();
        auto cameraManager = applicationManager->getCameraManager();
        auto selectionManager = applicationManager->getSelectionManager();

        auto bResult = false;

        m_prevCursor = m_cursorPos;

        auto mouseState = event->getMouseState();
        if( mouseState )
        {
            auto mousePosition = mouseState->getAbsolutePosition();

            m_cursorPos.X() = mousePosition.X();
            m_cursorPos.Y() = mousePosition.Y();

            auto cameraActor = cameraManager->getCurrentCamera();
            if( !cameraActor )
            {
                return false;
            }

            auto camera = cameraActor->getComponent<scene::CameraController>();
            SmartPtr<scene::Transform> cameraWorldTransform = cameraActor->getTransform();

            auto ray = camera->getCameraToViewportRay( mousePosition );

            // Y Axis
            if( !m_bMouseButtonDown )
            {
                auto node = m_node[AT_Y];
                auto aabb = node->getWorldAABB();
                auto result = MathUtil<real_Num>::intersects( ray, aabb );
                if( result.first )
                {
                    m_bYSelected = true;
                    m_vAxis = Vector3<real_Num>( 0.f, 1.f, 0.f );
                }
                else
                {
                    m_bYSelected = false;
                }
            }

            // x axis
            if( !m_bMouseButtonDown )
            {
                auto node = m_node[AT_X];
                auto aabb = node->getWorldAABB();
                auto result = MathUtil<real_Num>::intersects( ray, aabb );
                if( result.first )
                {
                    m_bXSelected = true;
                    m_vAxis = Vector3<real_Num>( 1.f, 0.f, 0.f );
                }
                else
                {
                    m_bXSelected = false;
                }
            }

            // z axis
            if( !m_bMouseButtonDown )
            {
                auto node = m_node[AT_Z];
                auto aabb = node->getWorldAABB();
                auto result = MathUtil<real_Num>::intersects( ray, aabb );
                if( result.first )
                {
                    m_bZSelected = true;
                    m_vAxis = Vector3<real_Num>( 0.f, 0.f, -1.f );
                }
                else
                {
                    m_bZSelected = false;
                }
            }

            // Mouse event
            auto mouseStateEvent = mouseState->getEventType();
            if( mouseStateEvent == IMouseState::Event::LeftReleased )
            {
                if( m_shouldLogMessages )
                {
                    FB_LOG_MESSAGE( "TranslateManipulator", "TranslateManipulator: MouseButtonUp" );
                }

                m_bMouseButtonDown = false;
                bResult = false;

                auto selection = selectionManager->getSelection();

                Array<String> keyValues;

                for( auto selected : selection )
                {
                    auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
                    if( actor )
                    {
                        auto transform = actor->getTransform();

                        auto position = transform->getLocalPosition();

                        auto positionStr = StringUtil::toString( position );
                        keyValues.push_back( positionStr );
                    }
                }

                // CSetGroupKeyValueCmd* setGroupKeyValueCmd = new CSetGroupKeyValueCmd(selectedEntities,
                // "Position", keyValues); setGroupKeyValueCmd->Execute();
                // CommandHistory::getSingletonPtr()->Add( setGroupKeyValueCmd );
                // DropReference(setGroupKeyValueCmd);
            }
            else if( mouseStateEvent == IMouseState::Event::LeftPressed )
            {
                if( m_shouldLogMessages )
                {
                    FB_LOG_MESSAGE( "TranslateManipulator", "CTranslateGizmo: MouseButtonDown" );
                }

                m_bMouseButtonDown = true;
                if( m_bXSelected || m_bYSelected || m_bZSelected )
                {
                    bResult = true;
                }
            }
            else if( ( mouseStateEvent == IMouseState::Event::Moved ) && m_bMouseButtonDown )
            {
                if( m_shouldLogMessages )
                {
                    FB_LOG_MESSAGE( "TranslateManipulator", "CTranslateGizmo: EMIE_MOUSE_MOVED" );
                }

                if( m_bXSelected || m_bYSelected || m_bZSelected )
                {
                    auto mouseMove = m_prevCursor - m_cursorPos;

                    auto xAxis = Vector3<real_Num>::UNIT_X;
                    auto yAxis = Vector3<real_Num>::UNIT_Y;
                    auto zAxis = Vector3<real_Num>::UNIT_Z;

                    auto actor = camera->getActor();
                    auto worldTransform = actor->getTransform();
                    auto cameraOrientation = worldTransform->getOrientation();

                    Matrix3<real_Num> cameraRotation;

                    cameraOrientation.toRotationMatrix( cameraRotation );
                    xAxis = Vector3<real_Num>( cameraRotation.m_[0], cameraRotation.m_[1],
                                               cameraRotation.m_[2] );
                    yAxis = Vector3<real_Num>( cameraRotation.m_[4], cameraRotation.m_[5],
                                               cameraRotation.m_[6] );
                    zAxis = Vector3<real_Num>( cameraRotation.m_[8], cameraRotation.m_[9],
                                               cameraRotation.m_[10] );

                    Vector3<real_Num> translation;

                    if( m_bXSelected )
                    {
                        auto xyPlane =
                            ( zAxis.Y() < static_cast<real_Num>( 0.0 ) ? static_cast<real_Num>( -1.0 )
                                                                       : static_cast<real_Num>( 1.0 ) );
                        auto yAxis =
                            ( xAxis.Z() < static_cast<real_Num>( 0.0 ) ? static_cast<real_Num>( 1.0 )
                                                                       : static_cast<real_Num>( -1.0 ) );

                        translation.X() += mouseMove.X() * xAxis.X();
                        translation.X() += mouseMove.Y() * ( 1 - xAxis.X() ) * xyPlane * yAxis;
                    }
                    else if( m_bYSelected )
                    {
                        translation.Y() += -mouseMove.Y() * ( yAxis.Y() > static_cast<real_Num>( 0.0 )
                                                                  ? static_cast<real_Num>( 1.0 )
                                                                  : static_cast<real_Num>( -1.0 ) );
                    }
                    else if( m_bZSelected )
                    {
                        auto yAxis =
                            ( zAxis.Z() < static_cast<real_Num>( 0.0 ) ? static_cast<real_Num>( -1.0 )
                                                                       : static_cast<real_Num>( 1.0 ) );

                        translation.Z() += mouseMove.X() * xAxis.Z();
                        translation.Z() +=
                            mouseMove.Y() * ( static_cast<real_Num>( 1.0 ) - xAxis.Z() ) * yAxis;
                    }

                    translation *= m_fScale * TRANSLATE_REDUCTION_COEF;

                    auto selection = selectionManager->getSelection();

                    for( auto selected : selection )
                    {
                        auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
                        if( actor )
                        {
                            SmartPtr<scene::Transform> transform = actor->getTransform();

                            auto entityPos = transform->getLocalPosition();

                            auto diff =
                                Math<real_Num>::Abs( ( entityPos - translation ).squaredLength() );

                            if( diff > Math<real_Num>::epsilon() )
                            {
                                auto localPosition = entityPos - translation;
                                transform->setLocalPosition( localPosition );

                                // auto localPosition =
                                // worldTransform->convertWorldToLocalPosition(worldPosition);
                                // localTransform->setPosition(localPosition);

                                actor->updateTransform();
                            }
                        }
                    }

                    updateManipulatorPosition();
                    bResult = true;
                }
            }
        }

        return bResult;
    }

    void TranslateManipulator::updateManipulatorPosition()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto selectionManager = applicationManager->getSelectionManager();

        auto selection = selectionManager->getSelection();

        if( !selection.empty() )
        {
            auto vAveragePosition = Vector3<real_Num>::zero();

            for( auto selected : selection )
            {
                auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
                if( actor )
                {
                    // auto worldTransform = actor->getTransform();
                    // if (worldTransform)
                    //{
                    //	vAveragePosition += worldTransform->getPosition();
                    // }
                }
            }

            vAveragePosition /= static_cast<f32>( selection.size() );
            m_vPosition = vAveragePosition;

            setPosition( vAveragePosition );
            setVisible( true );
        }
        else
        {
            setVisible( false );
        }
    }

    void TranslateManipulator::setEnabled( bool bEnabled )
    {
        if( m_bEnabled != bEnabled )
        {
            m_bEnabled = bEnabled;
            m_sceneNode->setVisible( m_bEnabled );
            updateManipulatorPosition();
        }
    }

    bool TranslateManipulator::isEnabled()
    {
        return m_bEnabled;
    }

    const Vector3<real_Num> &TranslateManipulator::getPosition() const
    {
        return RelativeTranslation;
    }

    void TranslateManipulator::setPosition( const Vector3<real_Num> &newpos )
    {
        RelativeTranslation = newpos;

        if( m_sceneNode )
        {
            m_sceneNode->setPosition( newpos );
        }
    }

    const Vector3<real_Num> &TranslateManipulator::getRotation() const
    {
        return RelativeRotation;
    }

    void TranslateManipulator::setRotation( const Vector3<real_Num> &rotation )
    {
        RelativeRotation = rotation;
    }

    const Vector3<real_Num> &TranslateManipulator::getScale() const
    {
        return RelativeScale;
    }

    void TranslateManipulator::setScale( const Vector3<real_Num> &scale )
    {
        RelativeScale = scale;
    }

    bool TranslateManipulator::isVisible() const
    {
        return m_isVisible;
    }

    SmartPtr<render::ISceneNode> TranslateManipulator::getSceneNode() const
    {
        return m_sceneNode;
    }

    void TranslateManipulator::setSceneNode( SmartPtr<render::ISceneNode> sceneNode )
    {
        m_sceneNode = sceneNode;
    }

    void TranslateManipulator::setVisible( bool visible )
    {
        m_isVisible = visible;
    }

    TranslateManipulator::SelectionManagerListener::SelectionManagerListener(
        TranslateManipulator *manipulator ) :
        m_owner( manipulator )
    {
    }

    TranslateManipulator::SelectionManagerListener::~SelectionManagerListener()
    {
        m_owner = nullptr;
    }

    Parameter TranslateManipulator::SelectionManagerListener::handleEvent(
        IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
        SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
    {
        return Parameter();
    }

    void TranslateManipulator::SelectionManagerListener::addSelectedObject()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    void TranslateManipulator::SelectionManagerListener::addSelectedObjects()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    void TranslateManipulator::SelectionManagerListener::setSelectedObjects()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    void TranslateManipulator::SelectionManagerListener::deselectObjects()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    void TranslateManipulator::SelectionManagerListener::deselectAll()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    TranslateManipulator *TranslateManipulator::SelectionManagerListener::getOwner() const
    {
        return m_owner;
    }

    void TranslateManipulator::SelectionManagerListener::setOwner( TranslateManipulator *owner )
    {
        m_owner = owner;
    }
}  // end namespace fb
