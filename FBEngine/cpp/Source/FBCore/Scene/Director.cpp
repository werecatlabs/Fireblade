#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Director.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/FileInfo.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Director, Resource<IDirector> );

    Director::Director() = default;

    Director::~Director() = default;

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

    void Director::saveToFile( const String &filePath )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto data = fb::static_pointer_cast<Properties>( getProperties() );
            FB_ASSERT( data );

            auto dataStr = DataUtil::toString( data.get(), true );
            FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );

            auto filePath = getFilePath();
            if( !StringUtil::isNullOrEmpty( filePath ) )
            {
                fileSystem->writeAllText( filePath, dataStr );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Director::loadFromFile( const String &filePath )
    {
        try
        {
	        auto applicationManager = core::ApplicationManager::instance();
	        FB_ASSERT( applicationManager );
	
	        auto fileSystem = applicationManager->getFileSystem();
	        FB_ASSERT( fileSystem );
	
	        auto dataStr = fileSystem->readAllText( filePath );
	
	        auto properties = fb::make_ptr<Properties>();
	        DataUtil::parse( dataStr, properties.get() );
	
	        setProperties( properties );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Director::save()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto data = fb::static_pointer_cast<Properties>( getProperties() );
            FB_ASSERT( data );

            auto dataStr = DataUtil::toString( data.get(), true );
            FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );

            auto fileId = getFileSystemId();
            //FB_ASSERT( fileId != 0 );

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( fileId, fileInfo ) )
            {
                auto filePath = fileInfo.filePath;
                if( !StringUtil::isNullOrEmpty( filePath ) )
                {
                    fileSystem->writeAllText( filePath, dataStr );
                }
            }
            else
            {
                auto filePath = getFilePath();
                if( !StringUtil::isNullOrEmpty( filePath ) )
                {
                    fileSystem->writeAllText( filePath, dataStr );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Director::toData() const -> SmartPtr<ISharedObject>
    {
        return m_properties;
    }

    void Director::fromData( SmartPtr<ISharedObject> data )
    {
        setProperties( data );
    }

    auto Director::getProperties() const -> SmartPtr<Properties>
    {
        return m_properties;
    }

    void Director::setProperties( SmartPtr<Properties> properties )
    {
        m_properties = properties;
    }

    auto Director::getParent() const -> SmartPtr<IDirector>
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

    auto Director::findChild( const String &name ) -> SmartPtr<IDirector>
    {
        return nullptr;
    }

    auto Director::getChildren() const -> Array<SmartPtr<IDirector>>
    {
        return {};
    }

    auto Director::getChildrenPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IDirector>>>
    {
        return nullptr;
    }

}  // namespace fb::scene
