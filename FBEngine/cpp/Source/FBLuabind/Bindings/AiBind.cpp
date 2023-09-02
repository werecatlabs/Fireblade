#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/AiBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include <FBCore/FBCore.h>
#include "FBLuabind/FBLuabindTypes.h"
#include <luabind/operator.hpp>
#include "FBLuabind/ParamConverter.h"

namespace fb
{

    //AiCompositeGoalPtr _createCompositeGoal(IFactory* factory, lua_Integer typeId)
    //{
    //	return factory->createById((hash32)typeId);
    //}

    /*
    AiGoalBuilderPtr _createAiGoalBuilder()
    {
        return AiGoalBuilderPtr(new AiGoalBuilder, true);
    }
    */

    template <class B>
    void _setOwner( IAiGoal *goal, SmartPtr<B> owner )
    {
        //goal->setOwner( owner.get() );
    }

    template <class B>
    void _setOwnerEvaluator( IAiGoalEvaluator *goal, SmartPtr<B> owner )
    {
        goal->setOwner( owner.get() );
    }

    //struct AiScriptedGoalWrapper : public AiScriptedGoal, luabind::wrap_base
    //{
    //    AiScriptedGoalWrapper()
    //    {
    //    }

    //    void start()
    //    {
    //        setState( (u32)IAiGoal::State::Executing );
    //        call<void>( "start" );
    //    }

    //    void update( const s32 &task, const time_interval &t, const time_interval &dt )
    //    {
    //        call<void>( "update", (lua_Number)t, (lua_Number)dt );
    //    }

    //    void finish()
    //    {
    //        setState( (u32)IAiGoal::State::Finished );
    //        call<void>( "finish" );
    //    }

    //    static void default_start( AiScriptedGoal *ptr )
    //    {
    //        ptr->AiScriptedGoal::start();
    //    }
    //};

    void bindAi( lua_State *L )
    {
        using namespace luabind;

        //module( L )[class_<IAiGoal, bases<IScriptObject>, boost::shared_ptr<IAiGoal>>( "IAiGoal" )
        //                .def( "start", &IAiGoal::start )
        //                .def( "finish", &IAiGoal::finish )

        //                .def( "setState", &IAiGoal::setState )
        //                .def( "getState", &IAiGoal::getState )

        //                //.def( "getOwner", &IAiGoal::getOwner )

        //                .def( "setOwner", _setOwner<scene::IActor> )];

        //module( L )[class_<IAiCompositeGoal, IAiGoal, boost::shared_ptr<IAiGoal>>( "AiCompositeGoal" )
        //                .def( "addGoal", &IAiCompositeGoal::addGoal )
        //                .def( "removeGoal", &IAiCompositeGoal::removeGoal )
        //                .def( "getGoals", &IAiCompositeGoal::getGoals )
        //                .def( "removeAllGoals", &IAiCompositeGoal::removeAllGoals )
        //                .def( "removeGoalsByType", &IAiCompositeGoal::removeGoalsByType )
        //                .def( "hasGoalType", &IAiCompositeGoal::hasGoalType )

        //                .def( "addGoalEvaluator", &IAiCompositeGoal::addGoalEvaluator )
        //                .def( "removeGoalEvaluator", &IAiCompositeGoal::removeGoalEvaluator )
        //                .def( "getEvaluators", &IAiCompositeGoal::getEvaluators )

        //                .def( "hasGoals", &IAiCompositeGoal::hasGoals )];

        //module( L )[class_<IAiGoalEvaluator, IScriptObject, boost::shared_ptr<IAiGoalEvaluator>>(
        //                "AiGoalEvaluator" )
        //                .def( "activateGoal", &IAiGoalEvaluator::activateGoal )
        //                .def( "getRating", &IAiGoalEvaluator::getRating )

        //                .def( "setOwner", _setOwnerEvaluator<scene::IActor> )

        //                .def( "getOwner", &IAiGoalEvaluator::getOwner )
        //                .def( "setBias", &IAiGoalEvaluator::setBias )
        //                .def( "getBias", &IAiGoalEvaluator::getBias )];

        //module( L )[class_<AiGoalBuilder, IBuilder, AiGoalBuilderPtr>( "AiGoalBuilder" )
        //                .def( "createGoal", &AiGoalBuilder::createGoal )
        //                .def( "setFactoryName", &AiGoalBuilder::setFactoryName )
        //                .def( "getFactoryName", &AiGoalBuilder::getFactoryName )
        //                .def( "setClassName", &AiGoalBuilder::setClassName )
        //                .def( "getClassName", &AiGoalBuilder::getClassName )];

        //module( L )[class_<AiGoalEvaluatorBuilder, IBuilder, AiGoalEvaluatorBuilderPtr>(
        //                "AiGoalEvaluatorBuilder" )
        //                .def( "createEvaluator", &AiGoalEvaluatorBuilder::createEvaluator )
        //                .def( "getClassName", &AiGoalEvaluatorBuilder::getClassName )
        //                .def( "setClassName", &AiGoalEvaluatorBuilder::setClassName )
        //                .def( "getFactoryName", &AiGoalEvaluatorBuilder::getFactoryName )
        //                .def( "setFactoryName", &AiGoalEvaluatorBuilder::setFactoryName )];

        //module( L )[class_<IAiManager, IScriptObject, AiManagerPtr>( "AiManager" )];

        //module( L )[class_<IPathfinder2, IScriptObject, Pathfinder2Ptr>( "Pathfinder2" )
        //                .def( "setGoal", &IPathfinder2::setGoal )
        //                .def( "searchStep", &IPathfinder2::searchStep )
        //                .def( "getClassName", &IPathfinder2::getClassName )
        //                .def( "setClassName", &IPathfinder2::setClassName )];

        //module( L )[class_<IPathNode2, IScriptObject, PathNode2Ptr>( "PathNode2" )
        //                .def( "getPosition", &IPathNode2::getPosition )
        //                .def( "setPosition", &IPathNode2::setPosition )
        //                .def( "getCost", &IPathNode2::getCost )];

        //module( L )[class_<AiScriptedCompositeGoal, IAiCompositeGoal, boost::shared_ptr<IAiGoal>>(
        //                "AiScriptedCompositeGoal" )
        //                .def( constructor<>() )];

        //module( L )[class_<AiScriptedEvaluator, IAiGoalEvaluator, boost::shared_ptr<IAiGoalEvaluator>>(
        //                "AiScriptedEvaluator" )
        //                .def( constructor<>() )];

        //module( L )[class_<AiScriptedGoal, AiScriptedGoalWrapper, bases<IAiGoal>,
        //                   boost::shared_ptr<IAiGoal>>( "AiScriptedGoal" )
        //                .def( constructor<>() )];

        //module( L )[class_<IAiSteering3, IScriptObject, AiSteering3Ptr>( "IAiSteering3" )

        //                .def( "getForce", &IAiSteering3::getForce )

        //                .def( "getTarget", &IAiSteering3::getTarget )
        //                .def( "setTarget", &IAiSteering3::setTarget )

        //                .def( "getPosition", &IAiSteering3::getPosition )
        //                .def( "setPosition", &IAiSteering3::setPosition )];

        //module( L )[class_<AiSteering3, IAiSteering3, AiSteering3Ptr>( "AiSteering3" )];

        //module( L )[class_<IAiTargeting3, IScriptObject, AiTargeting3Ptr>( "IAiTargeting3" )
        //                .def( "getTargetPosition", &IAiTargeting3::getTargetPosition )
        //                .def( "setTargetPosition", &IAiTargeting3::setTargetPosition )

        //                .def( "getOwner", &IAiTargeting3::getOwner )
        //                .def( "setOwner", &IAiTargeting3::setOwner )

        //                .def( "getTarget", &IAiTargeting3::getTarget )
        //                .def( "setTarget", &IAiTargeting3::setTarget )];

        //module( L )[class_<AiTargeting3, IAiTargeting3, AiSteering3Ptr>( "AiTargeting3" )];

        //module( L )[class_<AiCompositeGoalBase, IAiCompositeGoal, AiCompositeGoalBasePtr>(
        //    "AiCompositeGoalBase" )];

        //module( L )[class_<ThinkGoal, AiCompositeGoalBase, ThinkGoalPtr>( "ThinkGoal" )];

        //LUA_CONST_START( IAiGoal::AiGoalState )
        //LUA_CONST( IAiGoal, IAiGoal::AiGoalState::AGS_READY );
        //LUA_CONST( IAiGoal, IAiGoal::AiGoalState::AGS_EXECUTING );
        //LUA_CONST( IAiGoal, IAiGoal::AiGoalState::AGS_FINISHED );
        //LUA_CONST( IAiGoal, IAiGoal::AiGoalState::AGS_FAILED );
        //LUA_CONST_END;
    }

}  // end namespace fb
