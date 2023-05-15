#include <FBLuabind/FBLuabindPCH.h>
#include <FBLuabind/ScriptObjectUtil.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>

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

            if( scriptObj->isDerived<ui::IUILabelCheckboxPair>() )
            {
                luabind::detail::convert_to_lua( L,
                                                 static_cast<ui::IUILabelCheckboxPair *>( scriptObj ) );
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
            if( scriptObj->isDerived<ui::IUIText>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<ui::IUIText *>( scriptObj ) );
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
            if( scriptObj->isDerived<scene::SoundContainer>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<scene::SoundContainer *>( scriptObj ) );
                return true;
            }
            //    else if( scriptObj->isDerived( MovementControl2::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<MovementControl2 *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<MovementControl2 *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( MovementControlFPS::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<MovementControlFPS *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<MovementControlFPS *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( InputComponent::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<InputComponent *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<InputComponent *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( EntityMovement2::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<EntityMovement2 *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<EntityMovement2 *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( CombatBodyContainer::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<CombatBodyContainer *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<CombatBodyContainer *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( GraphicsContainer::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<GraphicsContainer *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<GraphicsContainer *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( PhysicsContainer2::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<PhysicsContainer2 *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<PhysicsContainer2 *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( PhysicsResponse2::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<PhysicsResponse2 *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<PhysicsResponse2 *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( SuperFreezeLogic::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<SuperFreezeLogic *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<SuperFreezeLogic *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( WeaponContainer::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<WeaponContainer *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<WeaponContainer *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( ViewportComponent::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<ViewportComponent *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<ViewportComponent *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( EnergyContainer::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<EnergyContainer *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<EnergyContainer *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( Grid2Container::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<Grid2Container *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<Grid2Container *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( MatchLogic::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<MatchLogic *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<MatchLogic *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( AnimatorContainer::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<AnimatorContainer *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<AnimatorContainer *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( FragCounter::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<FragCounter *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<FragCounter *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( Playable::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<Playable *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<Playable *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( AiGoalBased::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<AiGoalBased *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<AiGoalBased *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( ScreenSpaceRotation::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<ScreenSpaceRotation *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<ScreenSpaceRotation *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( CollisionNode::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<CollisionNode *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<CollisionNode *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isExactly( Transformation::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<Transformation *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<Transformation *>( scriptObj ) );
            //    }
            //    else if( scriptObj->isDerived( IGameManager::TYPE_INFO ) )
            //    {
            //        FB_ASSERT( dynamic_cast<IGameManager *>( scriptObj ) );
            //        luabind::detail::convert_to_lua( L, static_cast<IGameManager *>( scriptObj ) );
            //    }
            else
            {
                luabind::detail::convert_to_lua( L, static_cast<scene::IComponent *>( scriptObj ) );
                return true;
            }
        }
        else if( scriptObj->isDerived<scene::IDirector>() )
        {
            if( scriptObj->isDerived<TerrainDirector>() )
            {
                luabind::detail::convert_to_lua( L, static_cast<TerrainDirector *>( scriptObj ) );
                return true;
            }
            else
            {
                luabind::detail::convert_to_lua( L, static_cast<scene::IDirector *>( scriptObj ) );
                return true;
            }
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
