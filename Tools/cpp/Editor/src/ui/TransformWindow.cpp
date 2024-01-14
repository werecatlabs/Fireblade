#include <GameEditorPCH.h>
#include "ui/TransformWindow.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{

    TransformWindow::TransformWindow() = default;

    TransformWindow::~TransformWindow()
    {
        unload( nullptr );
    }

    void TransformWindow::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto parent = getParent();

            auto parentWindow = ui->addElementByType<ui::IUIWindow>();
            FB_ASSERT( parentWindow );

            setParentWindow( parentWindow );
            parentWindow->setLabel( "TransformWindow" );

            if( getShowWorldTransform() )
            {
                parentWindow->setSize( Vector2F( 0.0f, 150.0f ) );
            }
            else
            {
                parentWindow->setSize( Vector2F( 0.0f, 85.0f ) );
            }

            parentWindow->setHasBorder( true );

            if( parent )
            {
                parent->addChild( parentWindow );
            }

            // m_actorEnabled = ui->addElementByType<ui::IUILabelTogglePair>();
            // parentWindow->addChild( m_actorEnabled );

            m_localPosition = ui->addElementByType<ui::IUIVector3>();
            m_localRotation = ui->addElementByType<ui::IUIVector3>();
            m_localScale = ui->addElementByType<ui::IUIVector3>();

            if( getShowWorldTransform() )
            {
                m_position = ui->addElementByType<ui::IUIVector3>();
                m_rotation = ui->addElementByType<ui::IUIVector3>();
                m_scale = ui->addElementByType<ui::IUIVector3>();
            }

            m_localPosition->setLabel( "Local Position" );
            m_localRotation->setLabel( "Local Rotation" );
            m_localScale->setLabel( "Local Scale" );

            if( getShowWorldTransform() )
            {
                m_position->setLabel( "Position" );
                m_rotation->setLabel( "Rotation" );
                m_scale->setLabel( "Scale" );
            }

            auto localPositionListener = fb::make_ptr<VectorListener>();
            localPositionListener->setOwner( this );
            localPositionListener->setVectorUI( m_localPosition );
            localPositionListener->setType( scene::ITransform::Type::LocalPosition );
            m_localPosition->addObjectListener( localPositionListener );

            auto localRotationListener = fb::make_ptr<VectorListener>();
            localRotationListener->setOwner( this );
            localRotationListener->setVectorUI( m_localRotation );
            localRotationListener->setType( scene::ITransform::Type::LocalRotation );
            m_localRotation->addObjectListener( localRotationListener );

            auto localScaleListener = fb::make_ptr<VectorListener>();
            localScaleListener->setOwner( this );
            localScaleListener->setVectorUI( m_localScale );
            localScaleListener->setType( scene::ITransform::Type::LocalScale );
            m_localScale->addObjectListener( localScaleListener );

            if( getShowWorldTransform() )
            {
                auto positionListener = fb::make_ptr<VectorListener>();
                positionListener->setOwner( this );
                positionListener->setVectorUI( m_position );
                positionListener->setType( scene::ITransform::Type::Position );
                m_position->addObjectListener( positionListener );

                auto rotationListener = fb::make_ptr<VectorListener>();
                rotationListener->setOwner( this );
                rotationListener->setVectorUI( m_rotation );
                rotationListener->setType( scene::ITransform::Type::Rotation );
                m_rotation->addObjectListener( rotationListener );

                auto scaleListener = fb::make_ptr<VectorListener>();
                scaleListener->setOwner( this );
                scaleListener->setVectorUI( m_scale );
                scaleListener->setType( scene::ITransform::Type::Scale );
                m_scale->addObjectListener( scaleListener );
            }

            parentWindow->addChild( m_localPosition );
            parentWindow->addChild( m_localRotation );
            parentWindow->addChild( m_localScale );

            if( getShowWorldTransform() )
            {
                parentWindow->addChild( m_position );
                parentWindow->addChild( m_rotation );
                parentWindow->addChild( m_scale );
            }

            FB_ASSERT( parentWindow->isValid() );
            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TransformWindow::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            if( m_localPosition )
            {
                ui->removeElement( m_localPosition );
                m_localPosition = nullptr;
            }

            if( m_localRotation )
            {
                ui->removeElement( m_localRotation );
                m_localRotation = nullptr;
            }

            if( m_localScale )
            {
                ui->removeElement( m_localScale );
                m_localScale = nullptr;
            }

            if( m_position )
            {
                ui->removeElement( m_position );
                m_position = nullptr;
            }

            if( m_rotation )
            {
                ui->removeElement( m_rotation );
                m_rotation = nullptr;
            }

            if( m_scale )
            {
                ui->removeElement( m_scale );
                m_scale = nullptr;
            }

            if( auto parentWindow = getParentWindow() )
            {
                ui->removeElement( parentWindow );
                setParentWindow( nullptr );
            }

            EditorWindow::unload( nullptr );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TransformWindow::updateSelection()
    {
        auto transform = getTransform();
        if( transform )
        {
            auto localPosition = transform->getLocalPosition();
            if( m_localPosition )
            {
                m_localPosition->setValue( localPosition );
            }

            auto localRotation = transform->getLocalRotation();
            if( m_localRotation )
            {
                m_localRotation->setValue( localRotation );
            }

            auto localScale = transform->getLocalScale();
            if( m_localScale )
            {
                m_localScale->setValue( localScale );
            }

            auto position = transform->getPosition();
            if( m_position )
            {
                m_position->setValue( position );
            }

            auto rotation = transform->getRotation();
            if( m_rotation )
            {
                m_rotation->setValue( rotation );
            }

            auto scale = transform->getScale();
            if( m_scale )
            {
                m_scale->setValue( scale );
            }
        }
    }

    auto TransformWindow::getTransform() const -> SmartPtr<scene::ITransform>
    {
        return m_transform;
    }

    void TransformWindow::setTransform( SmartPtr<scene::ITransform> transform )
    {
        m_transform = transform;
    }

    auto TransformWindow::getShowWorldTransform() const -> bool
    {
        return m_showWorldTransform;
    }

    void TransformWindow::setShowWorldTransform( bool showWorldTransform )
    {
        m_showWorldTransform = showWorldTransform;
    }

    void TransformWindow::VectorListener::unload( SmartPtr<ISharedObject> data )
    {
        m_vectorUI = nullptr;
    }

    auto TransformWindow::VectorListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                       const Array<Parameter> &arguments,
                                                       SmartPtr<ISharedObject> sender,
                                                       SmartPtr<ISharedObject> object,
                                                       SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::handleValueChanged )
        {
            handleValueChanged();
        }

        return {};
    }

    void TransformWindow::VectorListener::handleValueChanged()
    {
        if( auto owner = getOwner() )
        {
            auto transform = owner->getTransform();
            auto value = m_vectorUI->getValue();

            switch( m_type )
            {
            case scene::ITransform::Type::LocalPosition:
            {
                transform->setLocalPosition( value );
                transform->setDirty( true );
            }
            break;
            case scene::ITransform::Type::LocalRotation:
            {
                transform->setLocalRotation( value );
                transform->setDirty( true );
            }
            break;
            case scene::ITransform::Type::LocalScale:
            {
                transform->setLocalScale( value );
                transform->setDirty( true );
            }
            break;
            case scene::ITransform::Type::Position:
            {
                transform->setPosition( value );
                transform->setLocalDirty( true );
            }
            break;
            case scene::ITransform::Type::Rotation:
            {
                transform->setRotation( value );
                transform->setLocalDirty( true );
            }
            break;
            case scene::ITransform::Type::Scale:
            {
                transform->setScale( value );
                transform->setLocalDirty( true );
            }
            break;
            default:
            {
            }
            break;
            };
        }
    }

    auto TransformWindow::VectorListener::getOwner() const -> TransformWindow *
    {
        return m_owner;
    }

    void TransformWindow::VectorListener::setOwner( TransformWindow *owner )
    {
        m_owner = owner;
    }

    auto TransformWindow::VectorListener::getVectorUI() const -> SmartPtr<ui::IUIVector3>
    {
        return m_vectorUI.lock();
    }

    void TransformWindow::VectorListener::setVectorUI( SmartPtr<ui::IUIVector3> val )
    {
        m_vectorUI = val;
    }

    auto TransformWindow::VectorListener::getType() const -> scene::ITransform::Type
    {
        return m_type;
    }

    void TransformWindow::VectorListener::setType( scene::ITransform::Type val )
    {
        m_type = val;
    }

}  // namespace fb::editor
