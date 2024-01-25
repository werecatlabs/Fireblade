#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/SceneNode.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/State/States/SceneNodeState.h>
#include <FBCore/State/States/TransformState.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, SceneNode, SharedGraphicsObject<ISceneNode> );

    SceneNode::SceneNode() = default;

    SceneNode::~SceneNode()
    {
        unload( nullptr );
    }

    void SceneNode::load( SmartPtr<ISharedObject> data )
    {
    }

    void SceneNode::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            m_creator = nullptr;
            m_parent = nullptr;

            SharedGraphicsObject<ISceneNode>::unload( data );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void SceneNode::detachObjectPtr( IGraphicsObject *object )
    {
    }

    void SceneNode::setParent( SmartPtr<ISceneNode> parent )
    {
        m_parent = parent;
    }

    auto SceneNode::getParent() const -> SmartPtr<ISceneNode>
    {
        auto p = m_parent.load();
        return p.lock();
    }

    auto SceneNode::getCreator() const -> SmartPtr<IGraphicsScene>
    {
        auto p = m_creator.load();
        return p.lock();
    }

    void SceneNode::setCreator( SmartPtr<IGraphicsScene> creator )
    {
        m_creator = creator;
    }

    void SceneNode::setPosition( const Vector3<real_Num> &position )
    {
        FB_ASSERT( position.isFinite() );

        if( auto &stateContext = getStateContext() )
        {
            if( auto &state = stateContext->getStateByType<TransformState>() )
            {
                state->setPosition( position );
            }
        }
    }

    auto SceneNode::getPosition() const -> Vector3<real_Num>
    {
        if( auto &stateContext = getStateContext() )
        {
            if( auto &state = stateContext->getStateByType<TransformState>() )
            {
                return state->getPosition();
            }
        }

        return Vector3<real_Num>::zero();
    }

    auto SceneNode::getWorldPosition() const -> Vector3<real_Num>
    {
        if( auto &stateContext = getStateContext() )
        {
            if( auto &state = stateContext->getStateByType<SceneNodeState>() )
            {
                return state->getAbsolutePosition();
            }
        }

        return Vector3<real_Num>::zero();
    }

    void SceneNode::setRotationFromDegrees( const Vector3<real_Num> &degrees )
    {
        auto orientation = Quaternion<real_Num>::eulerDegrees( degrees.x, degrees.y, degrees.z );
        setOrientation( orientation );
    }

    void SceneNode::setOrientation( const Quaternion<real_Num> &orientation )
    {
        FB_ASSERT( orientation.isSane() );

        if( auto &stateContext = getStateContext() )
        {
            if( auto &state = stateContext->getStateByType<TransformState>() )
            {
                state->setOrientation( orientation );
            }
        }
    }

    auto SceneNode::getOrientation() const -> Quaternion<real_Num>
    {
        if( auto &stateContext = getStateContext() )
        {
            if( auto &state = stateContext->getStateByType<TransformState>() )
            {
                return state->getOrientation();
            }
        }

        return Quaternion<real_Num>::identity();
    }

    auto SceneNode::getWorldOrientation() const -> Quaternion<real_Num>
    {
        if( auto &stateContext = getStateContext() )
        {
            if( auto &state = stateContext->getStateByType<SceneNodeState>() )
            {
                return state->getAbsoluteOrientation();
            }
        }

        return Quaternion<real_Num>::identity();
    }

    void SceneNode::setScale( const Vector3<real_Num> &scale )
    {
        FB_ASSERT( scale.isFinite() );

        if( auto &stateContext = getStateContext() )
        {
            if( auto &state = stateContext->getStateByType<TransformState>() )
            {
                state->setScale( scale );
            }
        }
    }

    auto SceneNode::getScale() const -> Vector3<real_Num>
    {
        if( auto &stateContext = getStateContext() )
        {
            if( auto &state = stateContext->getStateByType<TransformState>() )
            {
                return state->getScale();
            }
        }

        return Vector3<real_Num>::zero();
    }

    auto SceneNode::getWorldScale() const -> Vector3<real_Num>
    {
        if( auto &stateContext = getStateContext() )
        {
            if( auto &state = stateContext->getStateByType<SceneNodeState>() )
            {
                return state->getAbsoluteScale();
            }
        }

        return Vector3<real_Num>::zero();
    }

    auto SceneNode::_getRenderSystemTransform() const -> void *
    {
        return nullptr;
    }

    void SceneNode::lookAt( const Vector3<real_Num> &targetPoint )
    {
        auto direction = targetPoint - getPosition();
        auto orientation = MathUtilF::getOrientationFromDirection( direction, -Vector3F::unitZ(), true,
                                                                   Vector3<real_Num>::unitY() );
        setOrientation( orientation );
    }

    void SceneNode::setFixedYawAxis(
        bool useFixed, const Vector3<real_Num> &fixedAxis /*= Vector3<real_Num>::UNIT_Y */ )
    {
    }

    auto SceneNode::getLocalAABB() const -> AABB3<real_Num>
    {
        return {};
    }

    auto SceneNode::getWorldAABB() const -> AABB3<real_Num>
    {
        return {};
    }

    void SceneNode::attachObject( SmartPtr<IGraphicsObject> object )
    {
    }

    void SceneNode::detachObject( SmartPtr<IGraphicsObject> object )
    {
    }

    void SceneNode::detachAllObjects()
    {
    }

    auto SceneNode::getObjects() const -> Array<SmartPtr<IGraphicsObject>>
    {
        return {};
    }

    auto SceneNode::getNumObjects() const -> u32
    {
        return 0;
    }

    auto SceneNode::addChildSceneNode( const String &name /*= StringUtil::EmptyString */ )
        -> SmartPtr<ISceneNode>
    {
        return nullptr;
    }

    auto SceneNode::addChildSceneNode( const Vector3<real_Num> &position ) -> SmartPtr<ISceneNode>
    {
        return nullptr;
    }

    void SceneNode::addChild( SmartPtr<ISceneNode> child )
    {
    }

    auto SceneNode::removeChild( SmartPtr<ISceneNode> child ) -> bool
    {
        return false;
    }

    void SceneNode::remove()
    {
    }

    void SceneNode::removeChildren()
    {
    }

    auto SceneNode::findChild( const String &name ) -> SmartPtr<ISceneNode>
    {
        return nullptr;
    }

    auto SceneNode::getChildren() const -> Array<SmartPtr<ISceneNode>>
    {
        return {};
    }

    auto SceneNode::getNumChildren() const -> u32
    {
        return 0;
    }

    void SceneNode::needUpdate( bool forceParentUpdate )
    {
    }

    auto SceneNode::clone( SmartPtr<ISceneNode> parent, const String &name ) const
        -> SmartPtr<ISceneNode>
    {
        return nullptr;
    }

    void SceneNode::updateBounds()
    {
    }

    void SceneNode::_getObject( void **ppObject ) const
    {
    }

    auto SceneNode::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = SharedGraphicsObject<ISceneNode>::getProperties();
        return properties;
    }

    void SceneNode::setProperties( SmartPtr<Properties> properties )
    {
        SharedGraphicsObject<ISceneNode>::setProperties( properties );
    }

    auto SceneNode::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto childObjects = SharedGraphicsObject<ISceneNode>::getChildObjects();
        childObjects.emplace_back( getCreator() );
        childObjects.emplace_back( getParent() );
        return childObjects;
    }

}  // namespace fb::render
