#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/PhysicsBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

#include "FBLuabind/Helpers/PhysicsHelper3.h"

#include <FBPhysics2/CRigidBody2.h>
#include <FBPhysics2/Particle2.h>

namespace fb
{

    //PhysicsShape2Ptr _createCollisionShape( IPhysicsManager2 *mgr, const char *pShapeName )
    //{
    //    String shapeName = pShapeName;
    //    if( shapeName == ( "sphere" ) )
    //    {
    //        return mgr->createCollisionShape( physics::PST_SPHERE );
    //    }
    //    else if( shapeName == ( "box" ) )
    //    {
    //        return mgr->createCollisionShape( physics::PST_BOX );
    //    }

    //    return nullptr;
    //}

    //SphereShape2Ptr _createSphere( IPhysicsManager2 *mgr )
    //{
    //    return nullptr;  // mgr->createCollisionShape(PST_SPHERE);
    //}

    //BoxShape2Ptr _createBox( IPhysicsManager2 *mgr )
    //{
    //    return nullptr;  // mgr->createCollisionShape(PST_BOX);
    //}

    //RigidBody2Ptr _createRigidBody( IPhysicsManager2 *mgr )
    //{
    //    return mgr->createRigidBody();
    //}

    //RigidBody2Ptr _createRigidBodyFromShape( IPhysicsManager2 *mgr, PhysicsShape2Ptr shape )
    //{
    //    return mgr->createRigidBody( shape );
    //}

    //RigidBody2Ptr _createRigidBodyFromSphere( IPhysicsManager2 *mgr, SphereShape2Ptr shape )
    //{
    //    return mgr->createRigidBody( shape );
    //}

    //RigidBody2Ptr _createRigidBodyFromBox( IPhysicsManager2 *mgr, BoxShape2Ptr shape )
    //{
    //    return mgr->createRigidBody( shape );
    //}

    //void _setUserData( IPhysicsBody2 *body, IScriptObject *obj )
    //{
    //    body->setUserData( obj );
    //}

    //SmartPtr<IScriptObject> _getUserData( IPhysicsBody2 *body )
    //{
    //    SmartPtr<IScriptObject> pObject( (IScriptObject *)body->getUserData() );
    //    return pObject;
    //}

    //void _setRigidBody( PhysicsContainer2 *container, lua_Integer hash, RigidBody2Ptr rigidBody )
    //{
    //    hash32 id = *reinterpret_cast<hash32 *>( &hash );
    //    container->setObject( id, rigidBody );
    //}

    //void _addBody( PhysicsResponse2 *physicsResponse, IScriptObject *obj )
    //{
    //    //physicsResponse->addBody(obj);
    //}

    //RigidBody2Ptr _getRigidBody( PhysicsContainer2 *container, const char *name )
    //{
    //    auto hash = StringUtil::getHash( name );
    //    return container->getRigidBody( hash );
    //}

    //RigidBody2Ptr _getRigidBodyHash( PhysicsContainer2 *container, lua_Integer hash )
    //{
    //    hash32 id = *reinterpret_cast<hash32 *>( &hash );
    //    return container->getRigidBody( id );
    //}

    //void _setShape( PhysicsContainer2 *container, lua_Integer hash, PhysicsShape2Ptr shape )
    //{
    //    hash32 id = *reinterpret_cast<hash32 *>( &hash );
    //    container->setShape( id, shape );
    //}

    //PhysicsShape2Ptr _getShape( PhysicsContainer2 *container, lua_Integer hash )
    //{
    //    hash32 id = *reinterpret_cast<hash32 *>( &hash );
    //    return container->getShape( id );
    //}

    //void _addVelocity( IPhysicsBody2 *body, const Vector2<real_Num> &vel )
    //{
    //    body->addVelocity( vel );
    //}

    //StateObjectPtr _getStateObject( IPhysicsBody2 *body )
    //{
    //    return nullptr;  // body->getStateObject().get();
    //}

    //void _setBodyPosition( IPhysicsBody2 *body, const Vector2F &position )
    //{
    //    body->setPosition( Vector2<real_Num>( position.X(), position.Y() ) );
    //}

    //void _addBodyForce( IPhysicsBody2 *body, const Vector2F &force )
    //{
    //    body->addForce( Vector2<real_Num>( force.X(), force.Y() ) );
    //}

    //void _setBodyForce( IPhysicsBody2 *body, const Vector2F &force )
    //{
    //    body->setForce( Vector2<real_Num>( force.X(), force.Y() ) );
    //}

    template <class T>
    void _setCollisionType( T *body, lua_Integer mask )
    {
        body->setCollisionType( *reinterpret_cast<u32 *>( &mask ) );
    }

    template <class T>
    lua_Integer _getCollisionType( T *body )
    {
        u32 mask = body->getCollisionType();
        return *reinterpret_cast<lua_Integer *>( &mask );
    }

    template <class T>
    void _setCollisionMask( T *body, lua_Integer mask )
    {
        body->setCollisionMask( *reinterpret_cast<u32 *>( &mask ) );
    }

    template <class T>
    lua_Integer _getCollisionMask( T *body )
    {
        u32 mask = body->getCollisionMask();
        return *reinterpret_cast<lua_Integer *>( &mask );
    }

    //void _setObjectType( IPhysicsBody2 *body, lua_Integer hash )
    //{
    //    body->setObjectType( *reinterpret_cast<hash32 *>( &hash ) );
    //}

    //lua_Integer _getObjectType( IPhysicsBody2 *body )
    //{
    //    hash32 hash = body->getObjectType();
    //    return *reinterpret_cast<lua_Integer *>( &hash );
    //}

    //PhysicsTerrainPtr _createTerrain( IPhysicsManager3 *physicsMgr, TerrainTemplatePtr objectTemplate )
    //{
    //    return physicsMgr->createTerrain( objectTemplate );
    //}

    void bindPhysics( lua_State *L )
    {
        //using namespace luabind;

        //module( L )[class_<IPhysicsBody2, IScriptObject, boost::shared_ptr<IObject>>( "IPhysicsBody2" )
        //                .def( "setPosition", &IPhysicsBody2::setPosition )
        //                //.def("setPosition", _setBodyPosition )
        //                .def( "getPosition", &IPhysicsBody2::getPosition )

        //                .def( "setTargetPosition", &IPhysicsBody2::setTargetPosition )
        //                .def( "getTargetPosition", &IPhysicsBody2::getTargetPosition )

        //                .def( "setKinematicMode", &IPhysicsBody2::setKinematicMode )
        //                .def( "getKinematicMode", &IPhysicsBody2::getKinematicMode )

        //                .def( "addVelocity", _addVelocity )
        //                .def( "addVelocity", &IPhysicsBody2::addVelocity )
        //                .def( "setVelocity", &IPhysicsBody2::setVelocity )
        //                .def( "getVelocity", &IPhysicsBody2::getVelocity )

        //                //.def("addForce", _addBodyForce )
        //                .def( "addForce", &IPhysicsBody2::addForce )
        //                //.def("setForce", _setBodyForce )
        //                .def( "setForce", &IPhysicsBody2::setForce )
        //                .def( "getForce", &IPhysicsBody2::getForce )

        //                .def( "addTorque", &IPhysicsBody2::addTorque )
        //                .def( "setTorque", &IPhysicsBody2::setTorque )
        //                .def( "getTorque", &IPhysicsBody2::getTorque )

        //                .def( "setLinearDampValue", &IPhysicsBody2::setLinearDampValue )
        //                .def( "getLinearDampValue", &IPhysicsBody2::getLinearDampValue )

        //                .def( "setAngularDampValue", &IPhysicsBody2::setAngularDampValue )
        //                .def( "getAngularDampValue", &IPhysicsBody2::getAngularDampValue )

        //                .def( "setAirResistance", &IPhysicsBody2::setAirResistance )
        //                .def( "getAirResistance", &IPhysicsBody2::getAirResistance )

        //                .def( "setRestitution", &IPhysicsBody2::setRestitution )
        //                .def( "getRestitution", &IPhysicsBody2::getRestitution )

        //                .def( "setMass", &IPhysicsBody2::setMass )
        //                .def( "getMass", &IPhysicsBody2::getMass )
        //                .def( "getMassInv", &IPhysicsBody2::getMassInv )

        //                .def( "setFlag", &IPhysicsBody2::setFlag )
        //                .def( "getFlag", &IPhysicsBody2::getFlag )

        //                .def( "setObjectType", _setObjectType )
        //                .def( "getObjectType", _getObjectType )

        //                .def( "setWorldId", &IPhysicsBody2::setWorldId )
        //                .def( "getWorldId", &IPhysicsBody2::getWorldId )

        //                .def( "setEnabled", &IPhysicsBody2::setEnabled )
        //                .def( "isEnabled", &IPhysicsBody2::isEnabled )

        //                .def( "getLocalAABB", &IPhysicsBody2::getLocalAABB )
        //                .def( "getWorldAABB", &IPhysicsBody2::getWorldAABB )

        //                .def( "setMaterialId", &IPhysicsBody2::setMaterialId )
        //                .def( "getMaterialId", &IPhysicsBody2::getMaterialId )

        //                .def( "setCollisionType", _setCollisionType<IPhysicsBody2> )
        //                .def( "getCollisionType", _getCollisionType<IPhysicsBody2> )

        //                .def( "setCollisionMask", _setCollisionMask<IPhysicsBody2> )
        //                .def( "getCollisionMask", _getCollisionMask<IPhysicsBody2> )

        //                .def( "setSleep", &IPhysicsBody2::setSleep )
        //                .def( "isSleeping", &IPhysicsBody2::isSleeping )

        //                .def( "setContraintAABB", &IPhysicsBody2::setContraintAABB )
        //                .def( "getContraintAABB", &IPhysicsBody2::getContraintAABB )

        //                .def( "setUserData", _setUserData )
        //                .def( "getUserData", _getUserData )

        //                .def( "getStateObject", _getStateObject )

        //                .def( "setGravity", &IPhysicsBody2::setGravity )
        //                .def( "getGravity", &IPhysicsBody2::getGravity )

        //                .def( "setEnableGravity", &IPhysicsBody2::setEnableGravity )
        //                .def( "getEnableGravity", &IPhysicsBody2::getEnableGravity )];

        //module( L )[class_<IPhysicsMaterial2, IScriptObject, boost::shared_ptr<IObject>>(
        //                "IPhysicsMaterial2" )
        //                .def( "setFriction", &IPhysicsMaterial2::setFriction )
        //                .def( "getFriction", &IPhysicsMaterial2::getFriction )

        //                .def( "setRestitution", &IPhysicsMaterial2::setRestitution )
        //                .def( "getRestitution", &IPhysicsMaterial2::getRestitution )

        //                .def( "setContactPosition", &IPhysicsMaterial2::setContactPosition )
        //                .def( "getContactPosition", &IPhysicsMaterial2::getContactPosition )

        //                .def( "setContactNormal", &IPhysicsMaterial2::setContactNormal )
        //                .def( "getContactNormal", &IPhysicsMaterial2::getContactNormal )

        //                .def( "setPhysicsBodyA", &IPhysicsMaterial2::setPhysicsBodyA )
        //                .def( "getPhysicsBodyA", &IPhysicsMaterial2::getPhysicsBodyA )

        //                .def( "setPhysicsBodyB", &IPhysicsMaterial2::setPhysicsBodyB )
        //                .def( "getPhysicsBodyB", &IPhysicsMaterial2::getPhysicsBodyB )];

        //module( L )[class_<IRigidBody2, IPhysicsBody2, boost::shared_ptr<IObject>>( "IRigidBody2" )];

        //module( L )[class_<CRigidBody2, IRigidBody2, boost::shared_ptr<IObject>>( "CRigidBody2" )];

        //module( L )[class_<IPhysicsParticle2, IPhysicsBody2, boost::shared_ptr<IObject>>(
        //    "IPhysicsParticle2" )];

        //module( L )[class_<Particle2, IPhysicsParticle2, boost::shared_ptr<IObject>>( "Particle2" )];

        //module(
        //    L )[class_<IPhysicsShape2, IScriptObject, boost::shared_ptr<IObject>>( "PhysicsShape2" )];

        //module( L )[class_<ISphereShape2, IPhysicsShape2, boost::shared_ptr<IObject>>( "SphereShape2" )
        //                .def( "getRadius", &ISphereShape2::getRadius )
        //                .def( "setRadius", &ISphereShape2::setRadius )];

        //module( L )[class_<IBoxShape2, IPhysicsShape2, boost::shared_ptr<IObject>>( "BoxShape2" )
        //                .def( "setAABB", &IBoxShape2::setAABB )
        //                .def( "getAABB", &IBoxShape2::getAABB )];

        //module(
        //    L )[class_<IPhysicsWorld2, IScriptObject, boost::shared_ptr<IObject>>( "PhysicsWorld2" )
        //            .def( "removeRigidBody", &IPhysicsWorld2::removeRigidBody )
        //            .def( "addRigidBody", &IPhysicsWorld2::addRigidBody )];

        //module( L )[class_<IPhysicsManager2, IScriptObject, boost::shared_ptr<IObject>>(
        //                "PhysicsManager2" )
        //                .def( "createCollisionShape", _createCollisionShape )
        //                .def( "createSphere", _createSphere )
        //                .def( "createBox", _createBox )
        //                .def( "createRigidBody", _createRigidBody )
        //                .def( "createRigidBody", _createRigidBodyFromShape )
        //                //.def("createRigidBody", _createRigidBodyFromSphere )
        //                //.def("createRigidBody", _createRigidBodyFromBox )

        //                .def( "addWorld", &IPhysicsManager2::addWorld )
        //                .def( "removeWorld", &IPhysicsManager2::removeWorld )
        //                .def( "findWorld", &IPhysicsManager2::findWorld )];

        //module(
        //    L )[class_<PhysicsContainer2, IComponent, boost::shared_ptr<IObject>>( "PhysicsContainer2" )
        //            .def( "getRigidBody", _getRigidBody )
        //            .def( "getRigidBody", _getRigidBodyHash )
        //            .def( "setRigidBody", _setRigidBody )

        //            .def( "getShape", _getShape )
        //            .def( "setShape", _setShape )];

        //module(
        //    L )[class_<PhysicsResponse2, IComponent, boost::shared_ptr<IObject>>( "PhysicsResponse2" )
        //            .def( "addBody", &PhysicsResponse2::addBody )
        //            .def( "addBody", _addBody )
        //            .def( "getUseMessages", &PhysicsResponse2::getUseMessages )
        //            .def( "setUseMessages", &PhysicsResponse2::setUseMessages )];

        ////
        //// 3d physics
        ////

        //module(
        //    L )[class_<IPhysicsShape3, IScriptObject, boost::shared_ptr<IObject>>( "IPhysicsShape3" )];

        //module( L )[class_<IBoxShape3, IPhysicsShape3, boost::shared_ptr<IObject>>( "IBoxShape3" )
        //                .def( "getExtents", &IBoxShape3::getExtents )
        //                .def( "setExtents", &IBoxShape3::setExtents )

        //                .def( "getAABB", &IBoxShape3::getAABB )
        //                .def( "setAABB", &IBoxShape3::setAABB )];

        //module( L )[class_<IPhysicsBody3, IScriptObject, boost::shared_ptr<IObject>>( "IPhysicsBody3" )
        //                //.def("setPosition", &IPhysicsBody3::setPosition )
        //                //.def("getPosition", &IPhysicsBody3::getPosition )

        //                .def( "getMass", &IPhysicsBody3::getMass )
        //                .def( "setMass", &IPhysicsBody3::setMass )

        //                .def( "setCollisionType", _setCollisionType<IPhysicsBody3> )
        //                .def( "getCollisionType", _getCollisionType<IPhysicsBody3> )

        //                .def( "setCollisionMask", _setCollisionMask<IPhysicsBody3> )
        //                .def( "getCollisionMask", _getCollisionMask<IPhysicsBody3> )];

        //module( L )[class_<IRigidBody3, IPhysicsBody3, boost::shared_ptr<IObject>>( "IRigidBody3" )];

        //module( L )[class_<IRigidStatic3, IPhysicsBody3, boost::shared_ptr<IObject>>( "IRigidStatic3" )];

        //module( L )[class_<ICharacterController3, IPhysicsBody3, boost::shared_ptr<IObject>>(
        //                "CharacterController3" )
        //                //.def("setPosition", &ICharacterController3::setPosition )
        //                //.def("getPosition", &ICharacterController3::getPosition )

        //                .def( "setOrientation", &ICharacterController3::setOrientation )
        //                .def( "getOrientation", &ICharacterController3::getOrientation )];

        //module(
        //    L )[class_<IPhysicsTerrain, IScriptObject, boost::shared_ptr<IObject>>( "PhysicsTerrain" )
        //            .def( "setCollisionType", _setCollisionType<IPhysicsTerrain> )
        //            .def( "getCollisionType", _getCollisionType<IPhysicsTerrain> )

        //            .def( "setCollisionMask", _setCollisionMask<IPhysicsTerrain> )
        //            .def( "getCollisionMask", _getCollisionMask<IPhysicsTerrain> )];

        //module( L )[class_<IPhysicsManager3, IScriptObject, boost::shared_ptr<IObject>>(
        //                "PhysicsManager3" )
        //                .def( "addCharacter", &IPhysicsManager3::addCharacter )
        //                .def( "createTerrain", &IPhysicsManager3::createTerrain )
        //                .def( "createTerrain", _createTerrain )

        //                .def( "createCollisionShape", PhysicsHelper3::createCollisionShape )

        //                .def( "createRigidBody", PhysicsHelper3::createRigidBody1 )
        //                .def( "createRigidBody", PhysicsHelper3::createRigidBody2 )

        //                .def( "createRigidStatic", PhysicsHelper3::createRigidStatic1 )
        //                .def( "createRigidStatic", PhysicsHelper3::createRigidStatic2 )

        //                .def( "getEnableDebugDraw", &IPhysicsManager3::getEnableDebugDraw )
        //                .def( "setEnableDebugDraw", &IPhysicsManager3::setEnableDebugDraw )];
    }

}  // end namespace fb
