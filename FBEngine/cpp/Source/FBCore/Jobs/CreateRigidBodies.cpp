#include <FBCore/FBCorePCH.h>
#include <FBCore/Jobs/CreateRigidBodies.h>
#include <FBCore/Scene/Components/CollisionMesh.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Mesh/IMeshResource.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, CreateRigidBodies, Job );

    void CreateRigidBodies::execute()
    {
        auto actor = getActor();
        auto cascade = getCascade();

        createRigidBodies( actor, cascade );
    }

    auto CreateRigidBodies::getActor() const -> SmartPtr<scene::IActor>
    {
        return m_actor;
    }

    void CreateRigidBodies::setActor( SmartPtr<scene::IActor> actor )
    {
        m_actor = actor;
    }

    auto CreateRigidBodies::getCascade() const -> bool
    {
        return m_cascade;
    }

    void CreateRigidBodies::setCascade( bool cascade )
    {
        m_cascade = cascade;
    }

    auto CreateRigidBodies::getMakeStatic() const -> bool
    {
        return m_makeStatic;
    }

    void CreateRigidBodies::setMakeStatic( bool makeStatic )
    {
        m_makeStatic = makeStatic;
    }

    auto CreateRigidBodies::isConvex() const -> bool
    {
        return m_isConvex;
    }

    void CreateRigidBodies::setConvex( bool convex )
    {
        m_isConvex = convex;
    }

    void CreateRigidBodies::createRigidBodies( SmartPtr<scene::IActor> actor, bool cascade )
    {
        auto makeStatic = getMakeStatic();
        actor->setStatic( makeStatic );

        auto rigidBody = actor->getComponent<scene::Rigidbody>();
        if( !rigidBody )
        {
            rigidBody = actor->addComponent<scene::Rigidbody>();
        }

        auto meshCollision = actor->getComponent<scene::CollisionMesh>();
        if( !meshCollision )
        {
            meshCollision = actor->addComponent<scene::CollisionMesh>();
        }

        auto convex = isConvex();
        if( !makeStatic )
        {
            convex = true;
        }

        meshCollision->setConvex( convex );

        if( meshCollision )
        {
            auto mesh = actor->getComponent<scene::Mesh>();
            if( mesh )
            {
                meshCollision->setMeshResource( mesh->getMeshResource() );
            }
        }

        if( cascade )
        {
            auto children = actor->getChildren();
            for( auto child : children )
            {
                createRigidBodies( child, cascade );
            }
        }
    }

}  // namespace fb
