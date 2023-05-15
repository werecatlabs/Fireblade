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
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>

#include "ImGuiInputManager.h"
#include "ImGuiProfilerWindow.h"

namespace fb
{
    namespace ui
    {
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
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
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

        size_t ImGuiManager::messagePump( SmartPtr<ISharedObject> data )
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

        fb::SmartPtr<fb::ui::IUIApplication> ImGuiManager::addApplication()
        {
            auto applicationManager = core::IApplicationManager::instance();
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

        SmartPtr<IUIElement> ImGuiManager::addElement( hash64 type )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto typeManager = TypeManager::instance();
            auto eType = typeManager->getDataTypeEnum<UITypes>( type );

            auto element = SmartPtr<IUIElement>();

            switch( eType )
            {
            case UITypes::Application:
            {
                auto application = factoryManager->make_ptr<ImGuiApplication>();
                application->load( nullptr );
                return application;
            }
            break;
            case UITypes::Button:
            {
                element = factoryManager->make_ptr<ImGuiButton>();
                element->load( nullptr );
            }
            break;
            case UITypes::Toolbar:
            {
                element = factoryManager->make_ptr<ImGuiToolbar>();
                element->load( nullptr );
            }
            break;
            case UITypes::None:
            default:
            {
                // todo refactor
                const auto ABOUT_TYPEINFO = IUIAbout::typeInfo();
                const auto APPLICATION_TYPEINFO = IUIApplication::typeInfo();
                const auto Checkbox_TYPEINFO = IUICheckbox::typeInfo();
                const auto EventWindow_TYPEINFO = IUIEventWindow::typeInfo();

                const auto FileBrowser_TYPEINFO = IUIFileBrowser::typeInfo();
                const auto RENDERWINDOW_TYPEINFO = IUIRenderWindow::typeInfo();
                const auto WINDOW_TYPEINFO = IUIWindow::typeInfo();
                const auto IMAGE_TYPEINFO = IUIImage::typeInfo();
                const auto InputManager_TYPEINFO = IUIInputManager::typeInfo();

                const auto MENU_TYPEINFO = IUIMenu::typeInfo();
                const auto MENUBAR_TYPEINFO = IUIMenubar::typeInfo();
                const auto MENUITEM_TYPEINFO = IUIMenuItem::typeInfo();
                const auto LabelCheckbox_TYPEINFO = IUILabelCheckboxPair::typeInfo();
                const auto LabelTextInput_TYPEINFO = IUILabelTextInputPair::typeInfo();
                const auto TREECTRL_TYPEINFO = IUITreeCtrl::typeInfo();
                const auto TREENODE_TYPEINFO = IUITreeNode::typeInfo();
                const auto Property_Grid_TYPEINFO = IUIPropertyGrid::typeInfo();
                const auto TEXT_TYPEINFO = IUIText::typeInfo();
                const auto TERRAIN_TYPEINFO = IUITerrainEditor::typeInfo();
                const auto Dropdown_TYPEINFO = IUIDropdown::typeInfo();
                const auto Vector2_TYPEINFO = IUIVector2::typeInfo();
                const auto Vector3_TYPEINFO = IUIVector3::typeInfo();
                const auto Vector4_TYPEINFO = IUIVector4::typeInfo();

                const auto ProfilerWindow_TYPEINFO = ui::IUIProfilerWindow::typeInfo();

                if( type == APPLICATION_TYPEINFO )
                {
                    auto application = factoryManager->make_ptr<ImGuiApplication>();
                    application->load( nullptr );
                    return application;
                }
                if( type == Checkbox_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiLabelCheckboxPair>();
                    element->load( nullptr );
                }
                if( type == ABOUT_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiAbout>();
                    element->load( nullptr );
                }
                if( type == ProfilerWindow_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiProfilerWindow>();
                    element->load( nullptr );
                }
                if( type == EventWindow_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiEventWindow>();
                    element->load( nullptr );
                }
                if( type == RENDERWINDOW_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiRenderWindow>();
                    element->load( nullptr );
                }
                if( type == WINDOW_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiWindow<ui::IUIWindow>>();
                    element->load( nullptr );
                }
                if( type == IMAGE_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiImage>();
                    element->load( nullptr );
                }
                if( type == InputManager_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiInputManager>();
                    element->load( nullptr );
                }
                if( type == MENU_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiMenu>();
                    element->load( nullptr );
                }
                if( type == MENUBAR_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiMenubar>();
                    element->load( nullptr );
                }
                if( type == MENUITEM_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiMenuItem>();
                    element->load( nullptr );
                }
                if( type == TREECTRL_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiTreeCtrl>();
                    element->load( nullptr );
                }
                if( type == TREENODE_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiTreeNode>();
                    element->load( nullptr );
                }
                if( type == TEXT_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiText>();
                    element->load( nullptr );
                }
                if( type == LabelCheckbox_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiLabelCheckboxPair>();
                    element->load( nullptr );
                }
                if( type == LabelTextInput_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiLabelTextInputPair>();
                    element->load( nullptr );
                }
                if( type == FileBrowser_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiFileBrowser>();
                    element->load( nullptr );
                }
                if( type == Property_Grid_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiPropertyGrid>();
                    element->load( nullptr );
                }
                if( type == Dropdown_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiDropdown>();
                    element->load( nullptr );
                }
                if( type == Vector2_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiVector2>();
                    element->load( nullptr );
                }
                if( type == Vector3_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiVector3>();
                    element->load( nullptr );
                }
                if( type == Vector4_TYPEINFO )
                {
                    element = factoryManager->make_ptr<ImGuiVector4>();
                    element->load( nullptr );
                }
                if( type == TERRAIN_TYPEINFO )
                {
                    element = fb::make_ptr<ImGuiTerrainEditor>();
                    element->load( nullptr );
                }
            }
            break;
            }

            addElement( type, element );
            return element;
        }

        SmartPtr<IUIElement> ImGuiManager::addElement( SmartPtr<IUIElement> parent, u8 type )
        {
            return nullptr;
        }

        SmartPtr<IUIElement> ImGuiManager::addElement( SmartPtr<IUIElement> parent, u8 type,
                                                       Properties &properties )
        {
            return nullptr;
        }

        void ImGuiManager::removeElement( SmartPtr<IUIElement> p )
        {
            if( isLoaded() )
            {
                if( p )
                {
                    auto applicationManager = core::IApplicationManager::instance();
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

        SmartPtr<IUICursor> ImGuiManager::getCursor() const
        {
            return nullptr;
        }

        SmartPtr<IUIElement> ImGuiManager::findElement( const String &id ) const
        {
            return nullptr;
        }

        SmartPtr<IUIApplication> ImGuiManager::getApplication() const
        {
            return m_application;
        }

        void ImGuiManager::setApplication( SmartPtr<IUIApplication> application )
        {
            m_application = application;
        }

        Array<SmartPtr<IUIWindow>> ImGuiManager::getWindows() const
        {
            const auto WINDOW_TYPEINFO = IUIWindow::typeInfo();
            if( auto p = getElementsPtr( WINDOW_TYPEINFO ) )
            {
                auto &elements = *p;
                return Array<SmartPtr<IUIWindow>>( elements.begin(), elements.end() );
            }

            return Array<SmartPtr<IUIWindow>>();
        }

        void ImGuiManager::setWindows( Array<SmartPtr<IUIWindow>> val )
        {
            //m_windows = val;
        }

        Array<SmartPtr<IUIFileBrowser>> ImGuiManager::getFileBrowsers() const
        {
            const auto FileBrowser_TYPEINFO = IUIFileBrowser::typeInfo();
            if( auto p = getElementsPtr( FileBrowser_TYPEINFO ) )
            {
                auto &elements = *p;
                return Array<SmartPtr<IUIFileBrowser>>( elements.begin(), elements.end() );
            }

            return Array<SmartPtr<IUIFileBrowser>>();
        }

        void ImGuiManager::setFileBrowsers( Array<SmartPtr<IUIFileBrowser>> val )
        {
            //m_fileBrowsers = val;
        }

        Array<SmartPtr<IUIRenderWindow>> ImGuiManager::getRenderWindows() const
        {
            const auto RENDERWINDOW_TYPEINFO = IUIRenderWindow::typeInfo();
            if( auto p = getElementsPtr( RENDERWINDOW_TYPEINFO ) )
            {
                auto &elements = *p;
                return Array<SmartPtr<IUIRenderWindow>>( elements.begin(), elements.end() );
            }

            return Array<SmartPtr<IUIRenderWindow>>();
        }

        void ImGuiManager::setRenderWindows( Array<SmartPtr<IUIRenderWindow>> val )
        {
            //m_renderWindows = val;
        }

        bool ImGuiManager::isDragging() const
        {
            return m_dragging;
        }

        void ImGuiManager::setDragging( bool dragging )
        {
            m_dragging = dragging;
        }

        SmartPtr<IUIWindow> ImGuiManager::getMainWindow() const
        {
            return m_uiWindow;
        }

        void ImGuiManager::setMainWindow( SmartPtr<IUIWindow> uiWindow )
        {
            m_uiWindow = uiWindow;
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

        SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> ImGuiManager::getElementsPtr( u32 type ) const
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

        Array<SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>>> ImGuiManager::getElementsByType() const
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

        bool ImGuiManager::InputListener::inputEvent( SmartPtr<IInputEvent> event )
        {
            return false;
        }

        bool ImGuiManager::InputListener::updateEvent( const SmartPtr<IInputEvent> &event )
        {
            return false;
        }

        void ImGuiManager::InputListener::setPriority( s32 priority )
        {
        }

        s32 ImGuiManager::InputListener::getPriority() const
        {
            return 0;
        }

        void ImGuiManager::InputListener::setOwner( ImGuiManager *owner )
        {
            m_owner = owner;
        }

        ImGuiManager *ImGuiManager::InputListener::getOwner() const
        {
            return m_owner;
        }

        void ImGuiManager::WindowListener::setOwner( ImGuiManager *owner )
        {
            m_owner = owner;
        }

        ImGuiManager *ImGuiManager::WindowListener::getOwner() const
        {
            return m_owner;
        }
    }  // end namespace ui
}  // end namespace fb
