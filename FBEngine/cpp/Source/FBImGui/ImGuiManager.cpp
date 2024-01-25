#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiManager.h>
#include <FBImGui/ImGuiApplication.h>
#include <FBImGui/ImGuiButton.h>
#include <FBImGui/ImGuiFileBrowser.h>
#include <FBImGui/ImGuiImage.h>
#include <FBImGui/ImGuiRenderWindow.h>
#include <FBImGui/ImGuiMenu.h>
#include <FBImGui/ImGuiMenuBar.h>
#include <FBImGui/ImGuiMenuItem.h>
#include <FBImGui/ImGuiTreeCtrl.h>
#include <FBImGui/ImGuiTreeNode.h>
#include <FBImGui/ImGuiText.h>
#include <FBImGui/ImGuiTextEntry.h>
#include <FBImGui/ImGuiWindow.h>
#include <FBImGui/ImGuiPropertyGrid.h>
#include <FBImGui/ImGuiDropdown.h>
#include <FBImGui/ImGuiLabelCheckboxPair.h>
#include <FBImGui/ImGuiLabelDropdownPair.h>
#include <FBImGui/ImGuiLabelSliderPair.h>
#include <FBImGui/ImGuiLabelTextInputPair.h>
#include <FBImGui/ImGuiVector2.h>
#include <FBImGui/ImGuiVector3.h>
#include <FBImGui/ImGuiVector4.h>
#include <FBImGui/ImGuiToolbar.h>
#include <FBImGui/ImGuiTerrainEditor.h>
#include "FBImGui/ImGuiEventWindow.h"
#include "FBImGui/ImGuiAbout.h"
#include "FBImGui/ImGuiTabBar.h"
#include "FBImGui/ImGuiTabItem.h"
#include <FBCore/FBCore.h>

#include "ImGuiCollapsingHeader.h"
#include "ImGuiColourPicker.h"
#include "ImGuiInputManager.h"
#include "ImGuiProfilerWindow.h"
#include "ImGuiProfileWindow.h"

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiManager, IUIManager );

    ImGuiManager::ImGuiManager()
    {
        auto typeManager = TypeManager::instance();
        typeManager->setDataTypeEnum<IUIApplication>( UITypes::Application );
        typeManager->setDataTypeEnum<IUIButton>( UITypes::Button );
        typeManager->setDataTypeEnum<IUIToolbar>( UITypes::Toolbar );

        auto size = 32000;
        //m_treeNodes.reserve( size );
        //m_textElements.reserve( size );
    }

    ImGuiManager::~ImGuiManager()
    {
        unload( nullptr );
    }

    void ImGuiManager::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto factoryManager = fb::make_ptr<FactoryManager>();
            setFactoryManager( factoryManager );

            ApplicationUtil::addFactory<ImGuiAbout>( factoryManager );
            ApplicationUtil::addFactory<ImGuiApplication>( factoryManager );
            ApplicationUtil::addFactory<ImGuiButton>( factoryManager );
            ApplicationUtil::addFactory<ImGuiCollapsingHeader>( factoryManager );
            ApplicationUtil::addFactory<ImGuiColourPicker>( factoryManager );
            ApplicationUtil::addFactory<ImGuiDropdown>( factoryManager );
            ApplicationUtil::addFactory<ImGuiEventWindow>( factoryManager );
            ApplicationUtil::addFactory<ImGuiFileBrowser>( factoryManager );
            ApplicationUtil::addFactory<ImGuiImage>( factoryManager );
            ApplicationUtil::addFactory<ImGuiInputManager>( factoryManager );

            ApplicationUtil::addFactory<ImGuiLabelCheckboxPair>( factoryManager );
            ApplicationUtil::addFactory<ImGuiLabelDropdownPair>( factoryManager );
            ApplicationUtil::addFactory<ImGuiLabelSliderPair>( factoryManager );
            ApplicationUtil::addFactory<ImGuiLabelTextInputPair>( factoryManager );

            ApplicationUtil::addFactory<ImGuiInputManager>( factoryManager );
            ApplicationUtil::addFactory<ImGuiMenu>( factoryManager );
            ApplicationUtil::addFactory<ImGuiMenuBar>( factoryManager );
            ApplicationUtil::addFactory<ImGuiMenuItem>( factoryManager );

            ApplicationUtil::addFactory<ImGuiProfilerWindow>( factoryManager );
            ApplicationUtil::addFactory<ImGuiProfileWindow>( factoryManager );

            ApplicationUtil::addFactory<ImGuiPropertyGrid>( factoryManager );
            ApplicationUtil::addFactory<ImGuiRenderWindow>( factoryManager );
            ApplicationUtil::addFactory<ImGuiTabBar>( factoryManager );
            ApplicationUtil::addFactory<ImGuiTabItem>( factoryManager );

            ApplicationUtil::addFactory<ImGuiTerrainEditor>( factoryManager );
            ApplicationUtil::addFactory<ImGuiText>( factoryManager );
            ApplicationUtil::addFactory<ImGuiTextEntry>( factoryManager );
            ApplicationUtil::addFactory<ImGuiToolbar>( factoryManager );

            ApplicationUtil::addFactory<ImGuiTreeCtrl>( factoryManager );
            ApplicationUtil::addFactory<ImGuiTreeNode>( factoryManager );

            ApplicationUtil::addFactory<ImGuiVector2>( factoryManager );
            ApplicationUtil::addFactory<ImGuiVector3>( factoryManager );
            ApplicationUtil::addFactory<ImGuiVector4>( factoryManager );
            ApplicationUtil::addFactory<ImGuiWindow>( factoryManager );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ImGuiManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                auto elementsByType = getElementsByType();
                for( auto p : elementsByType )
                {
                    auto &elements = *p;
                    for( auto element : elements )
                    {
                        if( element )
                        {
                            element->unload( data );
                        }
                    }
                }

                elementsByType.clear();
                m_elements.clear();

                if( m_uiWindow )
                {
                    m_uiWindow->unload( data );
                    m_uiWindow = nullptr;
                }

                if( m_application )
                {
                    m_application->unload( data );
                    m_application = nullptr;
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto ImGuiManager::messagePump( SmartPtr<ISharedObject> data ) -> size_t
    {
        auto application = fb::static_pointer_cast<ImGuiApplication>( getApplication() );
        if( application )
        {
            return application->messagePump( data );
        }

        return 0;
    }

    void ImGuiManager::handleWindowEvent( SmartPtr<render::IWindowEvent> event )
    {
    }

    auto ImGuiManager::addApplication() -> SmartPtr<IUIApplication>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto application = factoryManager->make_ptr<ImGuiApplication>();
        application->load( nullptr );
        return application;
    }

    void ImGuiManager::removeApplication( SmartPtr<IUIApplication> application )
    {
        application->unload( nullptr );
    }

    auto ImGuiManager::addElement( hash64 type ) -> SmartPtr<IUIElement>
    {
        FB_ASSERT( isLoaded() );

        auto applicationManager = core::ApplicationManager::instance();

        auto element = SmartPtr<IUIElement>();

        auto typeManager = TypeManager::instance();
        auto eType = typeManager->getDataTypeEnum<UITypes>( type );
        switch( eType )
        {
        case UITypes::Application:
        {
            auto factoryManager = applicationManager->getFactoryManager();
            auto application = factoryManager->make_ptr<ImGuiApplication>();
            application->load( nullptr );
            return application;
        }
        break;
        case UITypes::Button:
        {
            auto factoryManager = applicationManager->getFactoryManager();
            element = factoryManager->make_ptr<ImGuiButton>();
        }
        break;
        case UITypes::Toolbar:
        {
            auto factoryManager = applicationManager->getFactoryManager();
            element = factoryManager->make_ptr<ImGuiToolbar>();
        }
        break;
        case UITypes::None:
        default:
        {
            auto factoryManager = getFactoryManager();

            const auto WINDOW_TYPEINFO = IUIWindow::typeInfo();

            // Special case for window
            if( type == WINDOW_TYPEINFO )
            {
                element = factoryManager->make_ptr<ImGuiWindow>();
            }
            else
            {
                element = factoryManager->make_object<IUIElement>( type );
            }
        }
        break;
        };

        if (element)
        {
            element->load( nullptr );
            addElement( type, element );
            return element;
        }

        return nullptr;
    }

    auto ImGuiManager::addElement( SmartPtr<IUIElement> parent, u8 type ) -> SmartPtr<IUIElement>
    {
        return nullptr;
    }

    auto ImGuiManager::addElement( SmartPtr<IUIElement> parent, u8 type, Properties &properties )
        -> SmartPtr<IUIElement>
    {
        return nullptr;
    }

    void ImGuiManager::removeElement( SmartPtr<IUIElement> p )
    {
        if( isLoaded() )
        {
            if( p )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                if( auto parent = p->getParent() )
                {
                    parent->removeChild( p );
                }

                graphicsSystem->unloadObject( p );

                auto type = p->getTypeInfo();
                removeElement( type, p );
            }
        }
    }

    void ImGuiManager::clear()
    {
    }

    void ImGuiManager::reloadCurrentLayout()
    {
    }

    auto ImGuiManager::getCursor() const -> SmartPtr<IUICursor>
    {
        return nullptr;
    }

    auto ImGuiManager::findElement( const String &id ) const -> SmartPtr<IUIElement>
    {
        return nullptr;
    }

    auto ImGuiManager::getApplication() const -> SmartPtr<IUIApplication>
    {
        return m_application;
    }

    void ImGuiManager::setApplication( SmartPtr<IUIApplication> application )
    {
        m_application = application;
    }

    auto ImGuiManager::getWindows() const -> Array<SmartPtr<IUIWindow>>
    {
        const auto WINDOW_TYPEINFO = IUIWindow::typeInfo();
        if( auto p = getElementsPtr( WINDOW_TYPEINFO ) )
        {
            auto &elements = *p;
            return Array<SmartPtr<IUIWindow>>( elements.begin(), elements.end() );
        }

        return {};
    }

    void ImGuiManager::setWindows( Array<SmartPtr<IUIWindow>> val )
    {
        //m_windows = val;
    }

    auto ImGuiManager::getFileBrowsers() const -> Array<SmartPtr<IUIFileBrowser>>
    {
        const auto FileBrowser_TYPEINFO = IUIFileBrowser::typeInfo();
        if( auto p = getElementsPtr( FileBrowser_TYPEINFO ) )
        {
            auto &elements = *p;
            return Array<SmartPtr<IUIFileBrowser>>( elements.begin(), elements.end() );
        }

        return {};
    }

    void ImGuiManager::setFileBrowsers( Array<SmartPtr<IUIFileBrowser>> val )
    {
        //m_fileBrowsers = val;
    }

    auto ImGuiManager::getRenderWindows() const -> Array<SmartPtr<IUIRenderWindow>>
    {
        const auto RENDERWINDOW_TYPEINFO = IUIRenderWindow::typeInfo();
        if( auto p = getElementsPtr( RENDERWINDOW_TYPEINFO ) )
        {
            auto &elements = *p;
            return Array<SmartPtr<IUIRenderWindow>>( elements.begin(), elements.end() );
        }

        return {};
    }

    void ImGuiManager::setRenderWindows( Array<SmartPtr<IUIRenderWindow>> val )
    {
        //m_renderWindows = val;
    }

    auto ImGuiManager::isDragging() const -> bool
    {
        return m_dragging;
    }

    void ImGuiManager::setDragging( bool dragging )
    {
        m_dragging = dragging;
    }

    auto ImGuiManager::getMainWindow() const -> SmartPtr<IUIWindow>
    {
        return m_uiWindow;
    }

    void ImGuiManager::setMainWindow( SmartPtr<IUIWindow> uiWindow )
    {
        m_uiWindow = uiWindow;
    }

    void ImGuiManager::_getObject( void **ppObject )
    {
    }

    auto ImGuiManager::getFactoryManager() const -> SmartPtr<IFactoryManager>
    {
        return m_factoryManager;
    }

    void ImGuiManager::setFactoryManager( SmartPtr<IFactoryManager> factoryManager )
    {
        m_factoryManager = factoryManager;
    }

    void ImGuiManager::addElement( u32 type, SmartPtr<IUIElement> node )
    {
        auto p = getElementsPtr( type );
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IUIElement>>>();
            setElementsPtr( type, p );
        }

        if( p )
        {
            auto &elements = *p;
            elements.push_back( node );
        }
    }

    void ImGuiManager::removeElement( u32 type, SmartPtr<IUIElement> node )
    {
        auto typeManager = TypeManager::instance();
        auto classHierarchy = typeManager->getClassHierarchyId( type );

        for( auto id : classHierarchy )
        {
            auto p = getElementsPtr( id );
            if( p )
            {
                auto &elements = *p;

                auto elementsArray = Array<SmartPtr<IUIElement>>( elements.begin(), elements.end() );
                auto it = std::find( elementsArray.begin(), elementsArray.end(), node );
                if( it != elementsArray.end() )
                {
                    elementsArray.erase( it );

                    auto newElements = fb::make_shared<ConcurrentArray<SmartPtr<IUIElement>>>(
                        elementsArray.begin(), elementsArray.end() );
                    setElementsPtr( id, newElements );
                }
            }
        }
    }

    auto ImGuiManager::getElementsPtr( u32 type ) const
        -> SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>>
    {
        RecursiveMutex::ScopedLock lock( m_elementsMutex );

        auto it = m_elements.find( type );
        if( it != m_elements.end() )
        {
            return it->second;
        }

        return nullptr;
    }

    void ImGuiManager::setElementsPtr( u32 type, SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> p )
    {
        RecursiveMutex::ScopedLock lock( m_elementsMutex );
        m_elements[type] = p;
    }

    auto ImGuiManager::getElementsByType() const
        -> Array<SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>>>
    {
        RecursiveMutex::ScopedLock lock( m_elementsMutex );

        Array<SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>>> elements;
        for( auto &entry : m_elements )
        {
            auto &p = entry.second;
            elements.push_back( p.load() );
        }

        return elements;
    }

    auto ImGuiManager::InputListener::inputEvent( SmartPtr<IInputEvent> event ) -> bool
    {
        return false;
    }

    auto ImGuiManager::InputListener::updateEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        return false;
    }

    void ImGuiManager::InputListener::setPriority( s32 priority )
    {
    }

    auto ImGuiManager::InputListener::getPriority() const -> s32
    {
        return 0;
    }

    void ImGuiManager::InputListener::setOwner( ImGuiManager *owner )
    {
        m_owner = owner;
    }

    auto ImGuiManager::InputListener::getOwner() const -> ImGuiManager *
    {
        return m_owner;
    }

    void ImGuiManager::WindowListener::setOwner( ImGuiManager *owner )
    {
        m_owner = owner;
    }

    auto ImGuiManager::WindowListener::getOwner() const -> ImGuiManager *
    {
        return m_owner;
    }
}  // namespace fb::ui
