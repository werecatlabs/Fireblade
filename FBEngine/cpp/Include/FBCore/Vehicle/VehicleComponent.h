#ifndef __VehicleComponent_h__
#define __VehicleComponent_h__

#include <FBCore/Interface/Vehicle/IVehicleController.h>
#include <FBCore/Interface/Vehicle/IVehicleComponent.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{

    template <class T>
    class VehicleComponent : public T
    {
    public:
        VehicleComponent();
        ~VehicleComponent() override;

        void load( SmartPtr<ISharedObject> data ) override;

        void unload( SmartPtr<ISharedObject> data ) override;

        virtual SmartPtr<IVehicleController> getOwner() const;
        virtual void setOwner( SmartPtr<IVehicleController> vehicle );

        virtual void updateTransform() override;
        virtual void updateBodyTransform() override;

        virtual void updateGeometry();

        bool isValid() const override;

        virtual String getName() const override;
        virtual void setName( const String &name ) override;

        virtual Transform3<real_Num> getWorldTransform() const override;
        virtual void setWorldTransform( Transform3<real_Num> val ) override;

        virtual Transform3<real_Num> getLocalTransform() const override;
        virtual void setLocalTransform( Transform3<real_Num> val ) override;

        /** @copydoc IVehicleComponent::setState */
        virtual void setState( IVehicleComponent::State state );

        /** @copydoc IVehicleComponent::getState */
        virtual IVehicleComponent::State getState() const;

        void *getData() const;

        void setData( void *val );

        s32 getDebugId( s32 i ) const;

        void reset() override;

    protected:
        IVehicleComponent::State m_state = IVehicleComponent::State::AWAKE;
        AtomicSmartPtr<IVehicleController> m_owner;
        Transform3<real_Num> m_localTransform;
        Transform3<real_Num> m_worldTransform;
        Array<hash_type> m_ids;
        void *m_data = nullptr;
    };

    template <class T>
    VehicleComponent<T>::VehicleComponent()
    {
        // setWorldTransform(fb::make_ptr<Transform3<real_Num>>());
        // setLocalTransform(fb::make_ptr<Transform3<real_Num>>());

        auto uuid = StringUtil::getUUID();
        m_ids.resize( 100 );

        for( size_t i = 0; i < m_ids.size(); ++i )
        {
            m_ids[i] = StringUtil::getHash( uuid + "_" + StringUtil::toString( static_cast<s32>( i ) ) );
        }
    }

    template <class T>
    VehicleComponent<T>::~VehicleComponent()
    {
        m_owner = nullptr;
    }

    template <class T>
    void VehicleComponent<T>::load( SmartPtr<ISharedObject> data )
    {
    }

    template <class T>
    void VehicleComponent<T>::unload( SmartPtr<ISharedObject> data )
    {
        m_owner = nullptr;
    }

    template <class T>
    SmartPtr<IVehicleController> VehicleComponent<T>::getOwner() const
    {
        return m_owner;
    }

    template <class T>
    void VehicleComponent<T>::setOwner( SmartPtr<IVehicleController> vehicle )
    {
        m_owner = vehicle;
    }

    template <class T>
    void VehicleComponent<T>::updateTransform()
    {
        auto owner = getOwner();
        if( owner )
        {
            auto parentTransform = owner->getWorldTransform();

            FB_ASSERT( parentTransform.getScale().length() > std::numeric_limits<f32>::epsilon() );
            FB_ASSERT( getLocalTransform().getScale().length() > std::numeric_limits<f32>::epsilon() );

            auto localTransform = getLocalTransform();
            auto worldTransform = getWorldTransform();

            worldTransform.transformFromParent( parentTransform, localTransform );
            FB_ASSERT( worldTransform.getScale().length() > std::numeric_limits<f32>::epsilon() );

            setWorldTransform( worldTransform );
        }
    }

    template <class T>
    void VehicleComponent<T>::updateBodyTransform()
    {
        auto owner = getOwner();
        if( owner )
        {
            auto parentTransform = owner->getWorldTransform();

            FB_ASSERT( parentTransform.getScale().length() > std::numeric_limits<f32>::epsilon() );
            FB_ASSERT( getLocalTransform().getScale().length() > std::numeric_limits<f32>::epsilon() );

            auto localTransform = getLocalTransform();
            auto worldTransform = getWorldTransform();

            worldTransform.transformFromParent( parentTransform, localTransform );
            FB_ASSERT( worldTransform.getScale().length() > std::numeric_limits<f32>::epsilon() );
        }
    }

    template <class T>
    void VehicleComponent<T>::updateGeometry()
    {
    }

    template <class T>
    bool VehicleComponent<T>::isValid() const
    {
        auto owner = getOwner();
        return owner != nullptr;
    }

    template <class T>
    String VehicleComponent<T>::getName() const
    {
        const auto handle = this->getHandle();
        if( handle )
        {
            return handle->getName();
        }

        return "";
    }

    template <class T>
    void VehicleComponent<T>::setName( const String &name )
    {
        auto handle = this->getHandle();
        if( handle )
        {
            handle->setName( name );
        }
    }

    template <class T>
    Transform3<real_Num> VehicleComponent<T>::getWorldTransform() const
    {
        return m_worldTransform;
    }

    template <class T>
    void VehicleComponent<T>::setWorldTransform( Transform3<real_Num> val )
    {
        m_worldTransform = val;
    }

    template <class T>
    Transform3<real_Num> VehicleComponent<T>::getLocalTransform() const
    {
        return m_localTransform;
    }

    template <class T>
    void VehicleComponent<T>::setLocalTransform( Transform3<real_Num> val )
    {
        m_localTransform = val;
    }

    template <class T>
    void VehicleComponent<T>::setState( IVehicleComponent::State state )
    {
        m_state = state;
    }

    template <class T>
    IVehicleComponent::State VehicleComponent<T>::getState() const
    {
        return m_state;
    }

    template <class T>
    void *VehicleComponent<T>::getData() const
    {
        return m_data;
    }

    template <class T>
    void VehicleComponent<T>::setData( void *val )
    {
        m_data = val;
    }

    template <class T>
    s32 VehicleComponent<T>::getDebugId( s32 i ) const
    {
        if( i < m_ids.size() )
        {
            return m_ids[i];
        }

        return 0;
    }

    template <class T>
    void VehicleComponent<T>::reset()
    {
    }

}  // namespace fb

#endif  // VehicleComponent_h__
