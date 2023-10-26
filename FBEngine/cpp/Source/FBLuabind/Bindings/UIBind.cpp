#include <FBLuabind/FBLuabindPCH.h>
#include <FBLuabind/Bindings/UIBind.h>
#include <FBLuabind/SmartPtrConverter.h>
#include <FBLuabind/ParamConverter.h>
#include <FBCore/FBCore.h>
#include <luabind/luabind.hpp>

namespace fb
{
    using namespace ui;

    void _setVisible( IUIElement *element, bool isVisible )
    {
        element->setVisible( isVisible );
    }

    void _setEnabled( IUIElement *element, bool isEnabled )
    {
        element->setEnabled( isEnabled );
    }

    void _setEnabledCascade( IUIElement *element, bool isEnabled, bool cascade )
    {
        element->setEnabled( isEnabled, cascade );
    }

    void bindGUIManager( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<IUIDragSource, IEventListener>( "IUIDragSource" )];

        module( L )[class_<IUIDropTarget, IEventListener>( "IUIDropTarget" )];

        module( L )[class_<IUIElement, core::IPrototype, SmartPtr<IUIElement>>( "IUIElement" )
                        .def( "handleEvent", &IUIElement::handleEvent )
                        .def( "setName", &IUIElement::setName )
                        .def( "getName", &IUIElement::getName )
                        .def( "getElementId", &IUIElement::getElementId )
                        .def( "setElementId", &IUIElement::setElementId )
                        .def( "setPosition", &IUIElement::setPosition )
                        .def( "getPosition", &IUIElement::getPosition )
                        .def( "getAbsolutePosition", &IUIElement::getAbsolutePosition )
                        .def( "setSize", &IUIElement::setSize )
                        .def( "getSize", &IUIElement::getSize )
                        .def( "setVisible", &IUIElement::setVisible )
                        .def( "isVisible", &IUIElement::isVisible )
                        .def( "setEnabled", &IUIElement::setEnabled )
                        .def( "isEnabled", &IUIElement::isEnabled )
                        .def( "getParent", &IUIElement::getParent )
                        .def( "setParent", &IUIElement::setParent )
                        .def( "getChildren", &IUIElement::getChildren )
                        .def( "addChild", &IUIElement::addChild )
                        .def( "removeChild", &IUIElement::removeChild )
                        .def( "remove", &IUIElement::remove )
                        .def( "removeAllChildren", &IUIElement::removeAllChildren )
                        .def( "destroyAllChildren", &IUIElement::destroyAllChildren )
                        .def( "hasChildById", &IUIElement::hasChildById )
                        .def( "findChildById", &IUIElement::findChildById )
                        .def( "getLayout", &IUIElement::getLayout )
                        .def( "setLayout", &IUIElement::setLayout )
                        .def( "getContainer", &IUIElement::getContainer )
                        .def( "setContainer", &IUIElement::setContainer )
                        .def( "getOwner", &IUIElement::getOwner )
                        .def( "setOwner", &IUIElement::setOwner )
                        .def( "getDragSource", &IUIElement::getDragSource )
                        .def( "setDragSource", &IUIElement::setDragSource )
                        .def( "getDropTarget", &IUIElement::getDropTarget )
                        .def( "setDropTarget", &IUIElement::setDropTarget )
                        .def( "getOrder", &IUIElement::getOrder )
                        .def( "setOrder", &IUIElement::setOrder )
                        .def( "getSameLine", &IUIElement::getSameLine )
                        .def( "setSameLine", &IUIElement::setSameLine )
                        .def( "getColour", &IUIElement::getColour )
                        .def( "setColour", &IUIElement::setColour )
                        .def( "getHandleInputEvents", &IUIElement::getHandleInputEvents )
                        .def( "setHandleInputEvents", &IUIElement::setHandleInputEvents )
                        .def( "updateZOrder", &IUIElement::updateZOrder )
                        .def( "getStateObject", &IUIElement::getStateObject )
                        .def( "setStateObject", &IUIElement::setStateObject )
                        .def( "getStateListener", &IUIElement::getStateListener )
                        .def( "setStateListener", &IUIElement::setStateListener )
                        .scope[def( "typeInfo", IUIElement::typeInfo )]];

        module( L )[class_<IUIApplication, ISharedObject, SmartPtr<IUIElement>>( "IUIApplication" )
                        .def( "handleWindowEvent", &IUIApplication::handleWindowEvent )
                        .def( "handleInputEvent", &IUIApplication::handleInputEvent )
                        .def( "getMenubar", &IUIApplication::getMenubar )
                        .def( "setMenubar", &IUIApplication::setMenubar )
                        .def( "getToolbar", &IUIApplication::getToolbar )
                        .def( "setToolbar", &IUIApplication::setToolbar )
                        .def( "getWindowSize", &IUIApplication::getWindowSize )
                        .def( "setWindowSize", &IUIApplication::setWindowSize )
                        .scope[def( "typeInfo", IUIApplication::typeInfo )]];

        module( L )[class_<IUIButton, IUIElement, SmartPtr<IUIElement>>( "IUIButton" )
                        .def( "getLabel", &IUIButton::getLabel )
                        .def( "setLabel", &IUIButton::setLabel )
                        .def( "setTextSize", &IUIButton::setTextSize )
                        .def( "getTextSize", &IUIButton::getTextSize )
                        .scope[def( "typeInfo", IUIButton::typeInfo )]];

        module( L )[class_<IUICheckbox, IUIElement, SmartPtr<IUIElement>>( "IUICheckbox" )
                        .def( "setValue", &IUICheckbox::setValue )
                        .def( "getValue", &IUICheckbox::getValue )
                        .scope[def( "typeInfo", IUICheckbox::typeInfo )]];

        module( L )[class_<IUIColourPicker, IUIElement, SmartPtr<IUIColourPicker>>( "IUIColourPicker" )
                        .def( "setColour", &IUIColourPicker::setColour )
                        .def( "getColour", &IUIColourPicker::getColour )
                        .def( "setGradient", &IUIColourPicker::setGradient )
                        .def( "getGradient", &IUIColourPicker::getGradient )
                        //.def( "setColourFormat", &IUIColourPicker::setColourFormat )
                        //.def( "getColourFormat", &IUIColourPicker::getColourFormat )
                        .enum_( "ColourFormat" )[value( "RGB", IUIColourPicker::ColourFormat::RGB ),
                                                 value( "HSL", IUIColourPicker::ColourFormat::HSL ),
                                                 value( "HEX", IUIColourPicker::ColourFormat::HEX )]
                        .scope[def( "typeInfo", IUIColourPicker::typeInfo )]];

        module( L )[class_<IUICollapsingHeader, IUIElement, SmartPtr<IUICollapsingHeader>>(
                        "IUICollapsingHeader" )
                        .scope[def( "typeInfo", IUICollapsingHeader::typeInfo )]];

        module( L )[class_<IUIDropdown, bases<IUIElement>, SmartPtr<IUIElement>>( "IUIDropdown" )
                        .def( "getOptions", &IUIDropdown::getOptions )
                        .def( "setOptions", &IUIDropdown::setOptions )
                        .def( "getSelectedOption", &IUIDropdown::getSelectedOption )
                        .def( "setSelectedOption", &IUIDropdown::setSelectedOption )
                        .scope[def( "typeInfo", IUIDropdown::typeInfo )]];

        module( L )[class_<IUIImage, IUIElement, SmartPtr<IUIElement>>( "IUIImage" )
                        .def( "setTexture", &IUIImage::setTexture )
                        .def( "getTexture", &IUIImage::getTexture )
                        .scope[def( "typeInfo", IUIImage::typeInfo )]];

        module(
            L )[class_<IUILabelCheckboxPair, IUIElement, SmartPtr<IUIElement>>( "IUILabelCheckboxPair" )
                    .def( "getLabel", &IUILabelCheckboxPair::getLabel )
                    .def( "setLabel", &IUILabelCheckboxPair::setLabel )
                    .def( "getValue", &IUILabelCheckboxPair::getValue )
                    .def( "setValue", &IUILabelCheckboxPair::setValue )];

        module( L )[class_<IUILabelTextInputPair, IUIElement, SmartPtr<IUILabelTextInputPair>>(
                        "IUILabelTextInputPair" )
                        .def( "getLabel", &IUILabelTextInputPair::getLabel )
                        .def( "setLabel", &IUILabelTextInputPair::setLabel )
                        .def( "getValue", &IUILabelTextInputPair::getValue )
                        .def( "setValue", &IUILabelTextInputPair::setValue )
                        .scope[def( "typeInfo", IUILabelTextInputPair::typeInfo )]];

        module( L )[class_<IUISpinner, IUIElement, SmartPtr<IUIElement>>( "IUISpinner" )
                        .def( "setText", &IUISpinner::setText )
                        .def( "getText", &IUISpinner::getText )
                        .def( "incrementValue", &IUISpinner::incrementValue )
                        .def( "decrementValue", &IUISpinner::decrementValue )
                        .def( "setValue", &IUISpinner::setValue )
                        .def( "getValue", &IUISpinner::getValue )
                        .def( "setMinValue", &IUISpinner::setMinValue )
                        .def( "getMinValue", &IUISpinner::getMinValue )
                        .def( "setMaxValue", &IUISpinner::setMaxValue )
                        .def( "getMaxValue", &IUISpinner::getMaxValue )
                        .scope[def( "typeInfo", IUISpinner::typeInfo )]];

        module( L )[class_<IUITabBar, IUIElement, SmartPtr<IUITabBar>>( "IUITabBar" )
                        .def( "addTabItem", &IUITabBar::addTabItem )
                        .def( "removeTabItem", &IUITabBar::removeTabItem )
                        .scope[def( "typeInfo", IUITabBar::typeInfo )]];

        module( L )[class_<IUITabItem, IUIElement, SmartPtr<IUITabItem>>( "IUITabItem" )
                        .def( "getLabel", &IUITabItem::getLabel )
                        .def( "setLabel", &IUITabItem::setLabel )
                        .scope[def( "typeInfo", IUITabItem::typeInfo )]];

        module( L )[class_<IUIText, IUIElement, SmartPtr<IUIElement>>( "IUIText" )
                        .def( "getText", &IUIText::getText )
                        .def( "setText", &IUIText::setText )

                        .def( "getTextSize", &IUIText::getTextSize )
                        .def( "setTextSize", &IUIText::setTextSize )

                        .scope[def( "typeInfo", IUIText::typeInfo )]];

        module( L )[class_<IUITextEntry, IUIText, SmartPtr<IUIElement>>( "IUITextEntry" )
                        .scope[def( "typeInfo", IUITextEntry::typeInfo )]];

        module( L )[class_<IUITreeCtrl, IUIElement, SmartPtr<IUIElement>>( "IUITreeCtrl" )
                        .def( "clear", &IUITreeCtrl::clear )
                        .def( "addRoot", &IUITreeCtrl::addRoot )
                        .def( "addNode", &IUITreeCtrl::addNode )
                        .def( "expand", &IUITreeCtrl::expand )
                        .def( "getTreeNodes", &IUITreeCtrl::getTreeNodes )
                        .def( "setTreeNodes", &IUITreeCtrl::setTreeNodes )
                        .def( "getSelectTreeNodes", &IUITreeCtrl::getSelectTreeNodes )
                        .def( "setSelectTreeNodes", &IUITreeCtrl::setSelectTreeNodes )
                        .def( "addSelectTreeNode", &IUITreeCtrl::addSelectTreeNode )
                        .def( "clearSelectTreeNodes", &IUITreeCtrl::clearSelectTreeNodes )
                        .def( "getSelectTreeNode", &IUITreeCtrl::getSelectTreeNode )
                        .def( "setSelectTreeNode", &IUITreeCtrl::setSelectTreeNode )
                        .def( "getRoot", &IUITreeCtrl::getRoot )
                        .def( "setRoot", &IUITreeCtrl::setRoot )
                        .def( "getDragSourceElement", &IUITreeCtrl::getDragSourceElement )
                        .def( "setDragSourceElement", &IUITreeCtrl::setDragSourceElement )
                        .def( "getDropDestinationElement", &IUITreeCtrl::getDropDestinationElement )
                        .def( "setDropDestinationElement", &IUITreeCtrl::setDropDestinationElement )
                        .def( "isMultiSelect", &IUITreeCtrl::isMultiSelect )
                        .def( "setMultiSelect", &IUITreeCtrl::setMultiSelect )
                        .scope[def( "typeInfo", IUITreeCtrl::typeInfo )]
                        .enum_( "constants" )[luabind::value( "clearHash", IUITreeCtrl::clearHash )]

        ];

        module( L )[class_<IUIMenu, IUIElement, SmartPtr<IUIElement>>( "IUIMenu" )
                        .def( "addMenuItem", &IUIMenu::addMenuItem )
                        .def( "removeMenuItem", &IUIMenu::removeMenuItem )
                        .def( "getMenuItems", &IUIMenu::getMenuItems )
                        .def( "setMenuItems", &IUIMenu::setMenuItems )
                        .def( "setCursorPosition", &IUIMenu::setCursorPosition )
                        .def( "getCursorPosition", &IUIMenu::getCursorPosition )
                        .def( "setNumListItems", &IUIMenu::setNumListItems )
                        .def( "getNumListItems", &IUIMenu::getNumListItems )
                        .def( "setCurrentItemIndex", &IUIMenu::setCurrentItemIndex )
                        .def( "getCurrentItemIndex", &IUIMenu::getCurrentItemIndex )
                        .def( "incrementCursor", &IUIMenu::incrementCursor )
                        .def( "decrementCursor", &IUIMenu::decrementCursor )
                        .def( "getNumMenuItems", &IUIMenu::getNumMenuItems )
                        .def( "getLabel", &IUIMenu::getLabel )
                        .def( "setLabel", &IUIMenu::setLabel )
                        .scope[def( "typeInfo", IUIMenu::typeInfo )]];

        module( L )[class_<IUIMenuItem, IUIElement, SmartPtr<IUIElement>>( "IUIMenuItem" )
                        //.def( "getMenuItemType", &IUIMenuItem::getMenuItemType )
                        //.def( "setMenuItemType", &IUIMenuItem::setMenuItemType )
                        .def( "getText", &IUIMenuItem::getText )
                        .def( "setText", &IUIMenuItem::setText )
                        .def( "getHelp", &IUIMenuItem::getHelp )
                        .def( "setHelp", &IUIMenuItem::setHelp )
                        .scope[def( "typeInfo", IUIMenuItem::typeInfo )]];

        module( L )[class_<IUIMenubar, IUIElement, SmartPtr<IUIMenubar>>( "IUIMenubar" )
                        .def( "addMenu", &IUIMenubar::addMenu )
                        .def( "removeMenu", &IUIMenubar::removeMenu )
                        .def( "getMenus", &IUIMenubar::getMenus )
                        .def( "setMenus", &IUIMenubar::setMenus )
                        .scope[def( "typeInfo", IUIMenubar::typeInfo )]];

        module( L )[class_<IUIWindow, IUIElement, SmartPtr<IUIElement>>( "IUIWindow" )
                        .def( "setLabel", &IUIWindow::setLabel )
                        .def( "getLabel", &IUIWindow::getLabel )
                        .def( "setContextMenu", &IUIWindow::setContextMenu )
                        .def( "getContextMenu", &IUIWindow::getContextMenu )
                        .def( "hasBorder", &IUIWindow::hasBorder )
                        .def( "setHasBorder", &IUIWindow::setHasBorder )
                        .def( "isDocked", &IUIWindow::isDocked )
                        .def( "setDocked", &IUIWindow::setDocked )
                        .scope[def( "typeInfo", IUIWindow::typeInfo )]];

        module( L )[class_<IUICharacterSelect, IUIElement, SmartPtr<IUIElement>>( "IUICharacterSelect" )
                        .def( "setCursorPosition", &IUICharacterSelect::setCursorPosition )
                        .def( "getCursorPosition", &IUICharacterSelect::getCursorPosition )

                        .def( "setNumListItems", &IUICharacterSelect::setNumListItems )
                        .def( "getNumListItems", &IUICharacterSelect::getNumListItems )

                        .def( "setCurrentItemIndex", &IUICharacterSelect::setCurrentItemIndex )
                        .def( "getCurrentItemIndex", &IUICharacterSelect::getCurrentItemIndex )

                        .def( "increamentCursor", &IUICharacterSelect::increamentCursor )
                        .def( "decrementCursor", &IUICharacterSelect::decrementCursor )

                        .def( "getNumMenuItems", &IUICharacterSelect::getNumMenuItems )
                        .def( "getCurrentCursorIndex", &IUICharacterSelect::getCurrentCursorIndex )];

        module( L )[class_<IUIManager, ISharedObject, SmartPtr<IUIManager>>( "IUIManager" )
                        .def( "messagePump", &IUIManager::messagePump )
                        .def( "addApplication", &IUIManager::addApplication )
                        .def( "removeApplication", &IUIManager::removeApplication )
                        .def( "getApplication", &IUIManager::getApplication )
                        .def( "setApplication", &IUIManager::setApplication )

                        .def( "addElement", &IUIManager::addElement )
                        .def( "removeElement", &IUIManager::removeElement )
                        .def( "clear", &IUIManager::clear )
                        .def( "getCursor", &IUIManager::getCursor )
                        .def( "findElement", &IUIManager::findElement )
                        .def( "isDragging", &IUIManager::isDragging )
                        .def( "setDragging", &IUIManager::setDragging )];
    }

}  // end namespace fb
