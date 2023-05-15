#ifndef ScriptObjectUtil_h__
#define ScriptObjectUtil_h__


#include <luabind/luabind.hpp>
#include <FBCore/Interface/Graphics/IAnimationController.h>

namespace fb
{
    class ScriptObjectUtil
    {
    public:
        static bool toLua( lua_State *L, ISharedObject *scriptObj );

        template <class T>
        static void apply( lua_State *L, SmartPtr<T> ptr )
        {
            luabind::detail::value_converter().apply( L, ptr );

            //check for known types
           
            if(ptr->template isDerived<render::IAnimationController>())
            {
                auto p = fb::static_pointer_cast<render::IAnimationController>( ptr );
                luabind::detail::value_converter().apply(L, p);
            }
            /*
            if(ptr->isDerived(IAnimationStateController::TYPE_INFO))
            {
                AnimationStateControllerPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IAnimationTextureCtrl::TYPE_INFO))
            {
                AnimationTextureCtrlPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(EntityMessageDispatcher::TYPE_INFO))
            {
                EntityMsgDispatcherPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IPhysicsBody2::TYPE_INFO))
            {
                PhysicsBody2Ptr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IPhysicsShape2::TYPE_INFO))
            {
                if(ptr->isDerived(IBoxShape2::TYPE_INFO))
                {
                    BoxShape2Ptr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(ISphereShape2::TYPE_INFO))
                {
                    SphereShape2Ptr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else
                {
                    PhysicsShape2Ptr p = ptr;
                    detail::value_converter().apply(L, p);
                }
            }
            else if(ptr->isDerived(IEntity::TYPE_INFO))
            {
                SmartPtr<scene::IActor> p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IEntityManager::TYPE_INFO))
            {
                EntityManagerPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(EntityMessage::TYPE_INFO))
            {
                MessagePtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(InputManager::TYPE_INFO))
            {
                InputManagerPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IInputEvent::TYPE_INFO))
            {
                InputEventPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IGameInput::TYPE_INFO))
            {
                GameInputPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IFlashPlayer::TYPE_INFO))
            {
                FlashPlayerPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IFlashMovie::TYPE_INFO))
            {
                FlashMoviePtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IVideo::TYPE_INFO))
            {
                VideoPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IVideoManager::TYPE_INFO))
            {
                VideoManagerPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(ICombatAttackData::TYPE_INFO))
            {
                CombatAttackDataPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IStateManager::TYPE_INFO))
            {
                SmartPtr<IStateManager> p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IBuilder::TYPE_INFO))
            {
                if(ptr->isDerived(EntityBuilder::TYPE_INFO))
                {
                    EntityBuilderPtr p =ptr;
                    detail::value_converter().apply(L, p);
                }
                else
                {
                    BuilderPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
            }
            else if(ptr->isDerived(IFlashInstance::TYPE_INFO))
            {
                if(ptr->isDerived(IFlashText::TYPE_INFO))
                {
                    FlashTextPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else
                {
                    FlashInstancePtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
            }
            else if(ptr->isDerived(IComponent::TYPE_INFO))
            {
                if(ptr->isDerived(SoundContainer::TYPE_INFO))
                {
                    SoundContainerPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(MovementControl2::TYPE_INFO))
                {
                    MovementCtrl2Ptr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(InputComponent::TYPE_INFO))
                {
                    Inputboost::shared_ptr<IComponent> p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(EntityMovement2::TYPE_INFO))
                {
                    EntityMovement2Ptr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(CombatBodyContainer::TYPE_INFO))
                {
                    CombatBodyContainerPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(GraphicsContainer::TYPE_INFO))
                {
                    GraphicsContainerPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(PhysicsContainer2::TYPE_INFO))
                {
                    PhysicsContainer2Ptr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(PhysicsResponse2::TYPE_INFO))
                {
                    PhysicsResponse2Ptr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(SuperFreezeLogic::TYPE_INFO))
                {
                    SuperFreezeLogicPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(WeaponContainer::TYPE_INFO))
                {
                    WeaponContainerPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(ViewportConstraint::TYPE_INFO))
                {
                    ViewportConstraintPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(EnergyContainer::TYPE_INFO))
                {
                    EnergyContainerPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(Grid2Container::TYPE_INFO))
                {
                    Grid2ContainerPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(MatchRoundLogic::TYPE_INFO))
                {
                    MatchRoundLogicPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(AnimatorContainer::TYPE_INFO))
                {
                    AnimatorContainerPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(FragCounter::TYPE_INFO))
                {
                    FragCounterPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(Playable::TYPE_INFO))
                {
                    PlayablePtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(AiGoalBased::TYPE_INFO))
                {
                    AiGoalBasedPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(ScreenSpacePosition::TYPE_INFO))
                {
                    ScreenSpacePositionPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(ScreenSpaceRotation::TYPE_INFO))
                {
                    ScreenSpaceRotationPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(CollisionNode::TYPE_INFO))
                {
                    CollisionNodePtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else if(ptr->isDerived(IGameSceneManager::TYPE_INFO))
                {
                    GameSceneManagerPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else
                {
                    boost::shared_ptr<IComponent> p = ptr;
                    detail::value_converter().apply(L, p);
                }
            }
            else if(ptr->isDerived(IWeapon::TYPE_INFO))
            {
                if(ptr->isDerived(IWeaponFireArm2::TYPE_INFO))
                {
                    WeaponFireArm2Ptr p = ptr;
                    detail::value_converter().apply(L, p);
                }
                else
                {
                    WeaponPtr p = ptr;
                    detail::value_converter().apply(L, p);
                }
            }
            else if(ptr->isDerived(IAiGoal::TYPE_INFO))
            {
                boost::shared_ptr<IAiGoal> p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IAiCompositeGoal::TYPE_INFO))
            {
                AiCompositeGoalPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(AiGoalEvaluator::TYPE_INFO))
            {
                boost::shared_ptr<IAiGoalEvaluator> p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(AiGoalEvaluator::TYPE_INFO))
            {
                boost::shared_ptr<IAiGoalEvaluator> p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(MessageData::TYPE_INFO))
            {
                MessageDataPtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IScene::TYPE_INFO))
            {
                ScenePtr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IPhysicsMaterial2::TYPE_INFO))
            {
                PhysicsMaterial2Ptr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else if(ptr->isDerived(IProjectile2::TYPE_INFO))
            {
                Projectile2Ptr p = ptr;
                detail::value_converter().apply(L, p);
            }
            else
            {
                detail::value_converter().apply(L, ptr);
            }
            */
        }
    };

    template <class T>
    class CastUtil
    {
    public:
        static void down_cast( lua_State *L, SmartPtr<T> const &ptr );
    };
}  // namespace fb

#endif  // ScriptObjectUtil_h__
