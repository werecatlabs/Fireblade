#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/TableLayout.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>
#include <FBCore/Interface/Scene/ITransform.h>

namespace fb::scene
{

    FB_CLASS_REGISTER_DERIVED( fb::scene, TableLayout, LayoutContainer );

    TableLayout::TableLayout() = default;

    TableLayout::~TableLayout() = default;

    void TableLayout::updateTransform()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto actor = getActor();

        auto dirty = false;

        if( !m_tableParent )
        {
            m_tableParent = sceneManager->createActor();
            m_tableParent->setName( "TableParent" );

            actor->addChild( m_tableParent );

            dirty = true;
        }

        for( int row = 0; row < numRows; row++ )
        {
            for( int col = 0; col < numColumns; col++ )
            {
                // Create a cell GameObject
                auto cellName =
                    "Cell_" + StringUtil::toString( row ) + "_" + StringUtil::toString( col );

                if( !m_tableParent->findChild( cellName ) )
                {
                    auto cell = sceneManager->createActor();
                    cell->setName( cellName );
                    m_tableParent->addChild( cell );

                    // Add a transform component to the cell
                    auto transform = cell->addComponent<LayoutTransform>();
                    transform->setSize( Vector2F( cellWidth, cellHeight ) );
                    transform->setPosition( Vector2F( col * cellWidth, row * cellHeight ) );

                    dirty = true;
                }
            }
        }

        if( dirty )
        {
            applicationManager->triggerEvent( IEvent::Type::Scene, IEvent::addActor, Array<Parameter>(),
                                              this, actor, nullptr );
        }
    }

    auto TableLayout::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = LayoutContainer::getProperties();
        properties->setProperty( "numRows", numRows );
        properties->setProperty( "numColumns", numColumns );

        properties->setProperty( "cellWidth", cellWidth );
        properties->setProperty( "cellHeight", cellHeight );

        properties->setPropertyAsButton( "create", "create" );

        return properties;
    }

    void TableLayout::setProperties( SmartPtr<Properties> properties )
    {
        LayoutContainer::setProperties( properties );

        properties->getPropertyValue( "numRows", numRows );
        properties->getPropertyValue( "numColumns", numColumns );

        properties->getPropertyValue( "cellWidth", cellWidth );
        properties->getPropertyValue( "cellHeight", cellHeight );

        updateTransform();
    }
}  // namespace fb::scene
