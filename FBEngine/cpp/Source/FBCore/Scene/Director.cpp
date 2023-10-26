#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Director.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, Director, Resource<IDirector> );

        Director::Director()
        {
        }

        Director::~Director()
        {
        }

        void Director::load( SmartPtr<ISharedObject> data )
        {
            m_properties = fb::make_ptr<Properties>();
        }

        void Director::unload( SmartPtr<ISharedObject> data )
        {
            if( m_properties )
            {
                m_properties->unload( nullptr );
                m_properties = nullptr;
            }
        }

        SmartPtr<ISharedObject> Director::toData() const
        {
            return m_properties;
        }

        void Director::fromData( SmartPtr<ISharedObject> data )
        {
            setProperties( data );
        }

        SmartPtr<Properties> Director::getProperties() const
        {
            return m_properties;
        }

        void Director::setProperties( SmartPtr<Properties> properties )
        {
            m_properties = properties;
        }

        SmartPtr<IDirector> Director::getParent() const
        {
            return nullptr;
        }

        void Director::setParent( SmartPtr<IDirector> parent )
        {
        }

        void Director::addChild( SmartPtr<IDirector> child )
        {
        }

        void Director::removeChild( SmartPtr<IDirector> child )
        {
        }

        void Director::removeChildren()
        {
        }

        SmartPtr<IDirector> Director::findChild( const String &name )
        {
            return nullptr;
        }

        Array<SmartPtr<IDirector>> Director::getChildren() const
        {
            return Array<SmartPtr<IDirector>>();
        }

        SharedPtr<ConcurrentArray<SmartPtr<IDirector>>> Director::getChildrenPtr() const
        {
            return nullptr;
        }
    }  // namespace scene
}  // namespace fb
