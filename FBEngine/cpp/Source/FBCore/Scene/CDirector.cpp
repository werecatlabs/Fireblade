#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/CDirector.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, CDirector, CResource<IDirector> );

        CDirector::CDirector()
        {
        }

        CDirector::~CDirector()
        {
        }

        void CDirector::load( SmartPtr<ISharedObject> data )
        {
            m_properties = fb::make_ptr<Properties>();
        }

        void CDirector::unload( SmartPtr<ISharedObject> data )
        {
            if( m_properties )
            {
                m_properties->unload( nullptr );
                m_properties = nullptr;
            }
        }

        SmartPtr<IData> CDirector::toData() const
        {
            auto properties = getProperties();
            return properties->toData();
        }

        void CDirector::fromData( SmartPtr<IData> data )
        {
            auto properties = fb::make_ptr<Properties>();
            properties->fromData( data );
            setProperties( properties );
        }

        SmartPtr<Properties> CDirector::getProperties() const
        {
            return m_properties;
        }

        void CDirector::setProperties( SmartPtr<Properties> properties )
        {
            m_properties = properties;
        }

        SmartPtr<IDirector> CDirector::getParent() const
        {
            return nullptr;
        }

        void CDirector::setParent( SmartPtr<IDirector> parent )
        {
        }

        void CDirector::addChild( SmartPtr<IDirector> child )
        {
        }

        void CDirector::removeChild( SmartPtr<IDirector> child )
        {
        }

        void CDirector::removeChildren()
        {
        }

        SmartPtr<IActor> CDirector::findChild( const String &name )
        {
            return nullptr;
        }

        Array<SmartPtr<IActor>> CDirector::getChildren() const
        {
            return Array<SmartPtr<IActor>>();
        }

        SharedPtr<ConcurrentArray<SmartPtr<IActor>>> CDirector::getChildrenPtr() const
        {
            return nullptr;
        }
    }  // namespace scene
}  // namespace fb
