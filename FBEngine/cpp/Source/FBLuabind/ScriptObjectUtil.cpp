#include <FBLuabind/FBLuabindPCH.h>
#include <FBLuabind/ScriptObjectUtil.h>
#include <FBCore/FBCore.h>

using namespace luabind;

namespace fb
{

    bool ScriptObjectUtil::toLua( lua_State *L, ISharedObject *scriptObj )
    {
        using namespace fb::render;
        using namespace fb::physics;

        if( scriptObj->isDerived<IResource>() )
        {
            if( scriptObj->isDerived<IMaterial>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<IMaterial *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ITexture>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ITexture *>( scriptObj ) );
                return true;
            }

            luabind::detail::convert_to_lua( L, static_cast<IResource *>( scriptObj ) );
            return true;
        }

        if( scriptObj->isDerived<scene::IEditor>() )
        {
            luabind::detail::convert_to_lua( L, static_cast<scene::IEditor *>( scriptObj ) );
            return true;
        }
        if( scriptObj->isDerived<ui::IUIElement>() )
        {
            if( scriptObj->isDerived<ui::IUIButton>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUIButton *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUICheckbox>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUICheckbox *>( scriptObj ) );
                return true;
            }

            if( scriptObj->isDerived<ui::IUIDropdown>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUIDropdown *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUIImage>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUIImage *>( scriptObj ) );
                return true;
            }

            if( scriptObj->isDerived<ui::IUILabelTogglePair>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUILabelTogglePair *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUILabelTextInputPair>() )
            {
                luabind::detail::convert_to_lua( L,
                                                 static_cast<ui::IUILabelTextInputPair *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUIMenu>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUIMenu *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUIMenuItem>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUIMenuItem *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUIMenubar>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUIMenubar *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUITabBar>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUITabBar *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUITabItem>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUITabItem *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUIText>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUIText *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUITextEntry>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUITextEntry *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUITreeCtrl>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUITreeCtrl *>( scriptObj ) );
                return true;
            }
            if( scriptObj->isDerived<ui::IUIWindow>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUIWindow *>( scriptObj ) );
                return true;
            }

            luabind::detail::convert_to_lua( L, static_cast<ui::IUIElement *>( scriptObj ) );
            return true;
        }
        else if( scriptObj->isDerived<IPhysicsShape2>() )
        {
            if( scriptObj->isDerived<IBoxShape2>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<IBoxShape2 *>( scriptObj ) );
                return true;
            }
            else if( scriptObj->isDerived<ISphereShape2>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ISphereShape2 *>( scriptObj ) );
                return true;
            }
            else
            {
                luabind::detail::convert_to_lua( L, static_cast<IPhysicsShape2 *>( scriptObj ) );
                return true;
            }
        }
        else if( scriptObj->isDerived<IPhysicsShape3>() )
        {
            if( scriptObj->isDerived<IBoxShape3>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<IBoxShape3 *>( scriptObj ) );
                return true;
            }
            else if( scriptObj->isDerived<ISphereShape3>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ISphereShape3 *>( scriptObj ) );
                return true;
            }
            else
            {
                luabind::detail::convert_to_lua( L, static_cast<IPhysicsShape3 *>( scriptObj ) );
                return true;
            }
        }
        else if( scriptObj->isDerived<IPhysicsBody2D>() )
        {
            if( scriptObj->isDerived<IRigidBody2>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<IRigidBody2 *>( scriptObj ) );
                return true;
            }
            //else if( scriptObj->isDerived<IStaticBody2>() )
            //{
            //    luabind::detail::convert_to_lua( L, static_cast<IStaticBody2 *>( scriptObj ) );
            //    return true;
            //}
            else
            {
                luabind::detail::convert_to_lua( L, static_cast<IPhysicsBody2D *>( scriptObj ) );
                return true;
            }
        }
        else if( scriptObj->isDerived<IPhysicsBody3>() )
        {
            if( scriptObj->isDerived<IRigidBody3>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<IRigidBody3 *>( scriptObj ) );
                return true;
            }
            else if( scriptObj->isDerived<IRigidStatic3>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<IRigidStatic3 *>( scriptObj ) );
                return true;
            }
            else
            {
                luabind::detail::convert_to_lua( L, static_cast<IPhysicsBody3 *>( scriptObj ) );
                return true;
            }
        }
        else if( scriptObj->isDerived<scene::IComponent>() )
        {
            if( scriptObj->isDerived<scene::Material>() )
            {
                FB_ASSERT( dynamic_cast<scene::Material *>( scriptObj ) );
                luabind::detail::convert_to_lua( L, static_cast<scene::Material *>( scriptObj ) );
            }
            else
            {
                luabind::detail::convert_to_lua( L, static_cast<scene::IComponent *>( scriptObj ) );
                return true;
            }
        }
        else if( scriptObj->isDerived<scene::IDirector>() )
        {
            //if( scriptObj->isDerived<TerrainDirector>() )
            //{
            //    luabind::detail::convert_to_lua( L, static_cast<TerrainDirector *>( scriptObj ) );
            //    return true;
            //}
            //else
            //{
            //    luabind::detail::convert_to_lua( L, static_cast<scene::IDirector *>( scriptObj ) );
            //    return true;
            //}
        }
        else if( scriptObj->isDerived<editor::FileSelection>() )
        {
            luabind::detail::convert_to_lua( L, static_cast<editor::FileSelection *>( scriptObj ) );
            return true;
        }

        return false;
    }

    template <>
    void CastUtil<physics::IPhysicsShape2>::down_cast( lua_State *L,
                                                       SmartPtr<physics::IPhysicsShape2> const &ptr )
    {
        if( ptr->isDerived<physics::IBoxShape2>() )
        {
            SmartPtr<physics::IBoxShape2> p = ptr;
            detail::value_converter().apply( L, p );
        }
        else if( ptr->isDerived<physics::ISphereShape2>() )
        {
            SmartPtr<physics::ISphereShape2> p = ptr;
            detail::value_converter().apply( L, p );
        }
        else
        {
            detail::value_converter().apply( L, ptr );
        }
    }

    template class CastUtil<physics::IPhysicsShape2>;

}  // namespace fb
