#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Manipulator.h>
#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/Scene/Components/Camera.h>
#include <FBCore/Interface/Graphics/IDebug.h>
#include <FBCore/Interface/Graphics/IDebugLine.h>
#include <FBCore/Interface/Graphics/IGraphicsMesh.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/Input/IInputEvent.h>
#include <FBCore/Interface/Input/IMouseState.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/ICameraManager.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/System/ISelectionManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUIWindow.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/ApplicationUtil.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, Manipulator, ISharedObject );

    Manipulator::Manipulator() = default;

    Manipulator::~Manipulator() = default;

    void Manipulator::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Loading && loadingState != LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::ApplicationManager::instance();
                auto selectionManager = applicationManager->getSelectionManager();

                if( selectionManager )
                {
                    m_selectionManagerListener = fb::make_ptr<SelectionManagerListener>();
                    m_selectionManagerListener->setOwner( this );
                    selectionManager->addObjectListener( m_selectionManagerListener );
                }

                setLoadingState( LoadingState::Loaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Manipulator::unload( SmartPtr<ISharedObject> data )
    {
    }

    void Manipulator::update()
    {
    }

    auto Manipulator::handleEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        return false;
    }

    void Manipulator::setEnabled( bool bEnabled )
    {
        if( m_enabled != bEnabled )
        {
            m_enabled = bEnabled;
            updateManipulatorPosition();
        }
    }

    auto Manipulator::isEnabled() -> bool
    {
        return m_enabled;
    }

    auto Manipulator::isVisible() const -> bool
    {
        return m_visible;
    }

    void Manipulator::setVisible( bool visible )
    {
        m_visible = visible;
    }

    auto Manipulator::getPosition() const -> Vector3<real_Num>
    {
        return m_relativeTranslation;
    }

    void Manipulator::setPosition( const Vector3<real_Num> &position )
    {
        m_relativeTranslation = position;
    }

    auto Manipulator::getRotation() const -> Vector3<real_Num>
    {
        return m_relativeRotation;
    }

    void Manipulator::setRotation( const Vector3<real_Num> &rotation )
    {
        m_relativeRotation = rotation;
    }

    auto Manipulator::getScale() const -> Vector3<real_Num>
    {
        return m_relativeScale;
    }

    void Manipulator::setScale( const Vector3<real_Num> &scale )
    {
        m_relativeScale = scale;
    }

    void Manipulator::updateManipulatorPosition()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto selectionManager = applicationManager->getSelectionManager();

        auto selection = selectionManager->getSelection();

        if( !selection.empty() )
        {
            auto vAveragePosition = Vector3<real_Num>::zero();

            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
                    if( actor )
                    {
                        auto worldTransform = actor->getTransform();
                        if( worldTransform )
                        {
                            vAveragePosition += worldTransform->getPosition();
                        }
                    }
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

    Manipulator::SelectionManagerListener::~SelectionManagerListener()
    {
        m_owner = nullptr;
    }

    auto Manipulator::SelectionManagerListener::handleEvent( IEvent::Type eventType,
                                                             hash_type eventValue,
                                                             const Array<Parameter> &arguments,
                                                             SmartPtr<ISharedObject> sender,
                                                             SmartPtr<ISharedObject> object,
                                                             SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::addSelectedObject )
        {
            addSelectedObject();
        }
        if( eventValue == IEvent::addSelectedObjects )
        {
            addSelectedObjects();
        }
        if( eventValue == IEvent::deselectObjects )
        {
            deselectObjects();
        }

        return {};
    }

    void Manipulator::SelectionManagerListener::addSelectedObject()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    void Manipulator::SelectionManagerListener::addSelectedObjects()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    void Manipulator::SelectionManagerListener::setSelectedObjects()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    void Manipulator::SelectionManagerListener::deselectObjects()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    void Manipulator::SelectionManagerListener::deselectAll()
    {
        if( auto owner = getOwner() )
        {
            owner->updateManipulatorPosition();
        }
    }

    auto Manipulator::SelectionManagerListener::getOwner() const -> SmartPtr<Manipulator>
    {
        return m_owner;
    }

    void Manipulator::SelectionManagerListener::setOwner( SmartPtr<Manipulator> owner )
    {
        m_owner = owner;
    }

}  // namespace fb
