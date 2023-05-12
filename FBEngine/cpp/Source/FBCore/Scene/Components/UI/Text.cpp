#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Text.h>
#include <FBCore/Scene/Components/UI/Layout.h>
#include <FBCore/Scene/Components/UI/CanvasTransform.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Text, IComponent );

        Text::Text()
        {
        }

        Text::~Text()
        {
            const auto &state = getLoadingState();
            if( state != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void Text::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                // createUI();
                UIComponent::load( data );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Text::createUI()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto renderUI = applicationManager->getRenderUI();
                FB_ASSERT( renderUI );

                auto text = renderUI->addElementByType<ui::IUIText>();
                setTextObject( text );
                setElement( text );

                auto textStr = getText();
                text->setText( textStr );

                if( auto actor = getActor() )
                {
                    auto visible = isEnabled() && actor->isEnabledInScene();
                    text->setVisible( visible );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Text::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &state = getLoadingState();
                if( state != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto renderUI = applicationManager->getRenderUI();
                    FB_ASSERT( renderUI );

                    if( m_textObject )
                    {
                        renderUI->removeElement( m_textObject );

                        m_textObject->unload( nullptr );
                        m_textObject = nullptr;
                    }

                    UIComponent::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Text::play()
        {
            if( !m_textObject )
            {
                createUI();
            }
        }

        void Text::edit()
        {
            if( !m_textObject )
            {
                createUI();
            }
        }

        SmartPtr<ui::IUIText> Text::getTextObject() const
        {
            return m_textObject;
        }

        void Text::setTextObject( SmartPtr<ui::IUIText> textObject )
        {
            m_textObject = textObject;
        }

        String Text::getText() const
        {
            return m_text;
        }

        void Text::setText( const String &text )
        {
            m_text = text;

            if( m_textObject )
            {
                m_textObject->setText( text );
            }
        }

        void Text::updateDirty( u32 flags, u32 oldFlags )
        {
            if( auto actor = getActor() )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                    BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                {
                    auto visible = isEnabled() && actor->isEnabledInScene();

                    if( !m_textObject )
                    {
                        createUI();
                    }

                    if( m_textObject )
                    {
                        m_textObject->setVisible( visible );
                    }

                    if( auto text = getTextObject() )
                    {
                        text->setText( m_text );
                    }

                    updateTransform();
                }
            }
        }

        Array<SmartPtr<ISharedObject>> Text::getChildObjects() const
        {
            auto objects = UIComponent::getChildObjects();

            if( m_textObject )
            {
                objects.push_back( m_textObject );
            }

            return objects;
        }

        SmartPtr<Properties> Text::getProperties() const
        {
            auto properties = UIComponent::getProperties();

            properties->setProperty( "text", m_text );
            properties->setProperty( "size", m_size );
            properties->setProperty( "colour", m_colour );

            return properties;
        }

        void Text::setProperties( SmartPtr<Properties> properties )
        {
            properties->getPropertyValue( "text", m_text );
            properties->getPropertyValue( "size", m_size );
            properties->getPropertyValue( "colour", m_colour );

            updateElementState();

            UIComponent::setProperties( properties );
        }

        void Text::updateElementState()
        {
            if( auto text = getTextObject() )
            {
                text->setText( m_text );
                text->setTextSize( m_size );
                text->setColour( m_colour );
            }
        }

    }  // namespace scene
}  // end namespace fb
