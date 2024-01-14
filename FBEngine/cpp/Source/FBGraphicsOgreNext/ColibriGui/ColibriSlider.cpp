
#include "ColibriGui/ColibriSlider.h"

#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/ColibriSkinManager.h"

namespace Colibri
{
    Slider::Slider( ColibriManager *manager ) :
        Widget( manager ),
        IdObject( Ogre::Id::generateNewId<Progressbar>() ),
        m_currentValue( 0 ),
        m_minValue( 0 ),
        m_maxValue( 10 ),
        m_denominator( 1 ),
        m_cursorOffset( 0.0f ),
        m_lineSize( 5.0f ),
        m_handleProportion( 0.8f ),
        m_handleTopLeftProportion( 0.5f ),
        m_vertical( false ),
        m_alwaysInside( false ),
        m_excludeBorders( false ),
        m_handleBorderIsHalo( false )
    {
        memset( m_layers, 0, sizeof( m_layers ) );

        m_clickable = true;
        m_keyboardNavigable = true;
        m_consumesScroll = true;

        m_autoSetNextWidget[Borders::Left] = false;
        m_autoSetNextWidget[Borders::Right] = false;
    }
    //-------------------------------------------------------------------------
    void Slider::_initialize()
    {
        for( size_t i = 0u; i < 2u; ++i )
        {
            m_layers[i] = m_manager->createWidget<Renderable>( this );
            m_layers[i]->_initialize();
        }

        const Ogre::IdString skinPackName =
            m_manager->getDefaultSkinPackName( SkinWidgetTypes::SliderLine );
        const SkinManager *skinManager = m_manager->getSkinManager();

        const SkinPack *defaultSkinPack = skinManager->findSkinPack( skinPackName, LogSeverity::Fatal );

        m_lineSize = defaultSkinPack->sliderLineSize;
        m_handleProportion[0] = defaultSkinPack->sliderHandleProportion[0];
        m_handleProportion[1] = defaultSkinPack->sliderHandleProportion[1];
        m_handleTopLeftProportion = defaultSkinPack->sliderPositionTopLeftProportion;
        m_alwaysInside = defaultSkinPack->sliderAlwaysInside;
        m_excludeBorders = defaultSkinPack->sliderExcludeBorders;
        m_handleBorderIsHalo = defaultSkinPack->sliderHandleBorderIsHalo;

        getSliderLine()->_setSkinPack( m_manager->getDefaultSkin( SkinWidgetTypes::SliderLine ) );
        getSliderHandle()->_setSkinPack( m_manager->getDefaultSkin( SkinWidgetTypes::SliderHandle ) );

        Widget::_initialize();

        /*const Ogre::IdString skinPackName =
            m_manager->getDefaultSkinPackName( SkinWidgetTypes::SliderLine );
        const SkinManager *skinManager = m_manager->getSkinManager();

        const SkinPack *defaultSkinPack = skinManager->findSkinPack( skinPackName, LogSeverity::Fatal );

        if( !defaultSkinPack )
            return;*/
    }
    //-------------------------------------------------------------------------
    void Slider::_destroy()
    {
        Widget::_destroy();

        // m_layers[i] are children of us, so they will be destroyed by our super class
        for( size_t i = 0u; i < 2u; ++i )
            m_layers[i] = 0;
    }
    //-------------------------------------------------------------------------
    void Slider::setSkinPack( Ogre::IdString linePackName, Ogre::IdString handlePackName )
    {
        if( linePackName != Ogre::IdString() )
        {
            const SkinManager *skinManager = m_manager->getSkinManager();
            const SkinPack *linePack = skinManager->findSkinPack( linePackName, LogSeverity::Fatal );
            m_lineSize = linePack->sliderLineSize;
            m_handleProportion[0] = linePack->sliderHandleProportion[0];
            m_handleProportion[1] = linePack->sliderHandleProportion[1];
            m_handleTopLeftProportion = linePack->sliderPositionTopLeftProportion;
            m_alwaysInside = linePack->sliderAlwaysInside;
            m_excludeBorders = linePack->sliderExcludeBorders;
            m_handleBorderIsHalo = linePack->sliderHandleBorderIsHalo;

            getSliderLine()->setSkinPack( linePackName );
        }

        if( handlePackName != Ogre::IdString() )
            getSliderHandle()->setSkinPack( handlePackName );
    }
    //-------------------------------------------------------------------------
    Renderable *Slider::getSliderLine()
    {
        return m_layers[0];
    }
    //-------------------------------------------------------------------------
    Renderable *Slider::getSliderHandle()
    {
        return m_layers[1];
    }
    //-------------------------------------------------------------------------
    void Slider::setVisualsEnabled( bool bEnabled )
    {
        for( size_t i = 0u; i < 2u; ++i )
            m_layers[i]->setVisualsEnabled( bEnabled );
    }
    //-------------------------------------------------------------------------
    bool Slider::isVisualsEnabled() const
    {
        return m_layers[0]->isVisualsEnabled();
    }
    //-------------------------------------------------------------------------
    void Slider::setState( States::States state, bool smartHighlight )
    {
        Widget::setState( state, smartHighlight );

        if( state == States::Pressed )
        {
            processCursorPosition( m_manager->getMouseCursorPosNdc(), true );
        }
        else
            m_cursorOffset = 0.0f;
    }
    //-------------------------------------------------------------------------
    void Slider::updateSlider()
    {
        if( !m_layers[0] )
            return;  //_initialize hasn't been called yet

        const Ogre::Vector2 fullSize = getSize();
        const Ogre::Vector2 frameSize =
            fullSize - ( m_excludeBorders ? m_layers[0]->getBorderCombined() : Ogre::Vector2::ZERO );

        const Ogre::Vector2 frameOrigin =
            m_excludeBorders ? m_layers[0]->getBorderTopLeft() : Ogre::Vector2::ZERO;

        const Ogre::Vector2 handleTopLeftBorder =
            m_handleBorderIsHalo ? m_layers[1]->getBorderTopLeft() : Ogre::Vector2::ZERO;
        const Ogre::Vector2 handleCombinedBorder =
            m_handleBorderIsHalo ? m_layers[1]->getBorderCombined() : Ogre::Vector2::ZERO;

        if( !m_vertical )
        {
            const bool rightToLeft = m_manager->shouldSwapRTL( HorizWidgetDir::AutoLTR );

            const float sliderLineHeight = m_lineSize;
            const Ogre::Vector2 handleSize = m_handleProportion * fullSize.y;
            const float handlePadding = m_alwaysInside ? 0.0f : handleSize.x;

            // Slider line (centered)
            const float lineTop = ( frameSize.y - sliderLineHeight ) * 0.5f;

            // Horizontally: Half a handle is added to the line on each side as padding.
            // Vertically: Center the line
            m_layers[0]->setTopLeft(
                Ogre::Vector2( handlePadding * 0.5f + handleTopLeftBorder.x, lineTop ) );

            // Other than the padding, the width is used to its full, but the height is always constant.
            const float reducedLineWidth = fullSize.x - handlePadding - handleCombinedBorder.x;
            m_layers[0]->setSize( Ogre::Vector2( reducedLineWidth, sliderLineHeight ) );

            const float slideableArea = frameSize.x - handleSize.x;

            // Slider handle
            m_layers[1]->setSize( handleSize );

            const float mostTopCenter = lineTop - handleSize.y * 0.5f;
            const float mostBottomCenter = lineTop + sliderLineHeight + handleSize.y * 0.5f;

            const float sliderValueUnorm = getCurrentValueUnorm();
            const float targetSliderValue = rightToLeft ? ( 1.0f - sliderValueUnorm ) : sliderValueUnorm;

            Ogre::Vector2 handleTopLeft(
                frameOrigin.x + ( slideableArea * targetSliderValue ) - handleTopLeftBorder.x,
                Ogre::Math::lerp( mostTopCenter, mostBottomCenter, m_handleTopLeftProportion ) -
                    handleSize.y * 0.5f );
            m_layers[1]->setTopLeft( handleTopLeft );
        }
        else
        {
            const float sliderLineWidth = m_lineSize;
            const Ogre::Vector2 handleSize = m_handleProportion * fullSize.x;
            const float handlePadding = m_alwaysInside ? 0.0f : handleSize.y;

            // Slider line (centered)
            const float lineLeft = ( frameSize.x - sliderLineWidth ) * 0.5f;

            // Vertically: Half a handle is added to the line on each side as padding.
            // Horizontally: Center the line
            m_layers[0]->setTopLeft(
                Ogre::Vector2( lineLeft, handlePadding * 0.5f + handleTopLeftBorder.y ) );

            // Other than the padding, the width is used to its full, but the height is always constant.
            const float reducedLineHeight = fullSize.y - handlePadding - handleCombinedBorder.y;
            m_layers[0]->setSize( Ogre::Vector2( sliderLineWidth, reducedLineHeight ) );

            const float slideableArea = frameSize.y - handleSize.y;

            // Slider handle
            m_layers[1]->setSize( handleSize );

            const float mostLeftCenter = lineLeft - handleSize.x * 0.5f;
            const float mostRightCenter = lineLeft + sliderLineWidth + handleSize.x * 0.5f;

            const float sliderValueUnorm = getCurrentValueUnorm();
            const float targetSliderValue = 1.0f - sliderValueUnorm;
            Ogre::Vector2 handleTopLeft(
                Ogre::Math::lerp( mostLeftCenter, mostRightCenter, m_handleTopLeftProportion ) -
                    handleSize.x * 0.5f,
                frameOrigin.y + ( slideableArea * targetSliderValue ) );
            m_layers[1]->setTopLeft( handleTopLeft );
        }

        const Ogre::Vector2 handleTopLeft = m_layers[1]->getLocalTopLeft();
        const Ogre::Vector2 handleBottomRight = m_layers[1]->getLocalBottomRight();
        if( ( handleTopLeft.x < 0.0f || handleTopLeft.y < 0.0f ||  //
              handleBottomRight.x > m_size.x || handleBottomRight.y > m_size.y ) )
        {
            static bool sWarnedOnce = false;
            if( !sWarnedOnce )
            {
                char tmpBuffer[384];
                Ogre::LwString errorMsg(
                    Ogre::LwString::FromEmptyPointer( tmpBuffer, sizeof( tmpBuffer ) ) );

                errorMsg =
                    "Slider handle ends outside of parent. It will be clipped. You can shrink "
                    "slider_line_size, shrink slider_handle_proportion, change "
                    "slider_handle_position_top_left_proportion to move the handle around until it "
                    "doesn't clip, or set the the slider size to at least ";

                if( !m_vertical )
                {
                    const float lineTop = ( frameSize.y - m_lineSize ) * 0.5f;
                    errorMsg.a( "height = ", lineTop );
                }
                else
                {
                    const float lineLeft = ( frameSize.x - m_lineSize ) * 0.5f;
                    errorMsg.a( "width = ", lineLeft );
                }

                LogListener *logListener = m_manager->getLogListener();
                logListener->log( errorMsg.c_str(), LogSeverity::Warning );
                sWarnedOnce = true;
            }
        }

        // This snap isn't perfect because it only snaps to local coordinates, not final NDC coord.
        // However it still does a very good job at preventing the handle from "wobbling" as
        // the user moves it
        m_layers[1]->setTopLeft( m_manager->snapToPixels( handleTopLeft ) );

        for( size_t i = 0u; i < 2u; ++i )
            m_layers[i]->updateDerivedTransformFromParent( false );
    }
    //-------------------------------------------------------------------------
    void Slider::setTransformDirty( uint32_t dirtyReason )
    {
        // Only update if our size is directly being changed, not our parent's
        if( ( dirtyReason & ( TransformDirtyParentCaller | TransformDirtyScale ) ) ==
            TransformDirtyScale )
        {
            updateSlider();
        }

        Widget::setTransformDirty( dirtyReason );
    }
    //-------------------------------------------------------------------------
    void Slider::processCursorPosition( const Ogre::Vector2 &pos, bool cursorBegin )
    {
        if( m_currentState == States::Pressed && this->intersects( pos ) )
        {
            const float sliderValueUnorm = getCurrentValueUnorm();

            if( !m_vertical )
            {
                const bool rightToLeft = m_manager->shouldSwapRTL( HorizWidgetDir::AutoLTR );

                const float sliderWidth =
                    getSliderLine()->getDerivedBottomRight().x - getSliderLine()->getDerivedTopLeft().x;
                const float mouseRelativeX = pos.x - m_derivedTopLeft.x;
                float posX = mouseRelativeX / sliderWidth;
                if( rightToLeft )
                    posX = 1.0f - posX;

                if( cursorBegin && getSliderHandle()->intersects( pos ) )
                {
                    // The user actually clicked on the handle, rather than part of the line.
                    // If this happens, apply an offset to the mouse movements, so the handle doesn't
                    // jump.
                    m_cursorOffset = posX - sliderValueUnorm;
                }

                const float fCursorOffset = float( m_cursorOffset );
                const float fMinValue = float( m_minValue );
                const float fMaxValue = float( m_maxValue );

                setCurrentValue( static_cast<int32_t>(
                    roundf( ( posX - fCursorOffset ) * ( fMaxValue - fMinValue ) ) + fMinValue ) );
            }
            else
            {
                const float sliderHeight =
                    getSliderLine()->getDerivedBottomRight().y - getSliderLine()->getDerivedTopLeft().y;
                const float mouseRelativeY = pos.y - m_derivedTopLeft.y;
                float posY = 1.0f - mouseRelativeY / sliderHeight;

                if( cursorBegin && getSliderHandle()->intersects( pos ) )
                {
                    // The user actually clicked on the handle, rather than part of the line.
                    // If this happens, apply an offset to the mouse movements, so the handle doesn't
                    // jump.
                    m_cursorOffset = posY - sliderValueUnorm;
                }

                const float fCursorOffset = float( m_cursorOffset );
                const float fMinValue = float( m_minValue );
                const float fMaxValue = float( m_maxValue );

                setCurrentValue( static_cast<int32_t>(
                    roundf( ( posY - fCursorOffset ) * ( fMaxValue - fMinValue ) ) + fMinValue ) );
            }

            m_manager->callActionListeners( this, Action::ValueChanged );
        }
    }
    //-------------------------------------------------------------------------
    void Slider::notifyCursorMoved( const Ogre::Vector2 &posNDC )
    {
        processCursorPosition( posNDC );
    }
    //-------------------------------------------------------------------------
    void Slider::_notifyActionKeyMovement( Borders::Borders direction )
    {
        if( !m_vertical )
        {
            const bool rightToLeft = m_manager->shouldSwapRTL( HorizWidgetDir::AutoLTR );
            const int32_t targetDirectionAmount = rightToLeft ? -1 : 1;

            if( direction == Borders::Left )
                setCurrentValue( m_currentValue - targetDirectionAmount );
            else if( direction == Borders::Right )
                setCurrentValue( m_currentValue + targetDirectionAmount );
        }
        else
        {
            const int32_t targetDirectionAmount = 1;
            if( direction == Borders::Top )
                setCurrentValue( m_currentValue + targetDirectionAmount );
            else if( direction == Borders::Bottom )
                setCurrentValue( m_currentValue - targetDirectionAmount );
        }

        m_manager->callActionListeners( this, Action::ValueChanged );
    }
    //-------------------------------------------------------------------------
    void Slider::setElementsSize( const Ogre::Vector2 &handleProportion, const float lineSize,
                                  const float handleTopLeftProportion )
    {
        m_handleProportion = handleProportion;
        m_lineSize = lineSize;
        m_handleTopLeftProportion = handleTopLeftProportion;
        updateSlider();
    }
    //-------------------------------------------------------------------------
    void Slider::setCurrentValue( int32_t currentValue )
    {
        if( m_currentValue != currentValue )
        {
            m_currentValue = Ogre::Math::Clamp( currentValue, m_minValue, m_maxValue );
            updateSlider();
        }
    }
    //-------------------------------------------------------------------------
    void Slider::setDenominator( int32_t denominator )
    {
        if( m_denominator != denominator )
        {
            m_denominator = denominator;
            updateSlider();
        }
    }
    //-------------------------------------------------------------------------
    float Slider::getCurrentValueProcessed() const
    {
        return static_cast<float>( m_currentValue ) / static_cast<float>( m_denominator );
    }
    //-------------------------------------------------------------------------
    float Slider::getCurrentValueUnorm() const
    {
        if( m_minValue == m_maxValue )
            return 1.0f;
        return static_cast<float>( m_currentValue - m_minValue ) /
               static_cast<float>( m_maxValue - m_minValue );
    }
    //-------------------------------------------------------------------------
    void Slider::setRange( int32_t minValue, int32_t maxValue )
    {
        if( minValue <= maxValue )
        {
            m_minValue = minValue;
            m_maxValue = maxValue;
        }
        else
        {
            LogListener *logListener = m_manager->getLogListener();

            char tmpBuffer[128];
            Ogre::LwString msg( Ogre::LwString::FromEmptyPointer( tmpBuffer, sizeof( tmpBuffer ) ) );
            msg.a( "Invalid range for Slider::setRange min = ", minValue, " max = ", maxValue );
            logListener->log( msg.c_str(), LogSeverity::Warning );
            COLIBRI_ASSERT_LOW( minValue <= maxValue );
        }

        updateSlider();
    }
    //-------------------------------------------------------------------------
    void Slider::setAlwaysInside( bool bAlwaysInside )
    {
        m_alwaysInside = bAlwaysInside;
        updateSlider();
    }
    //-------------------------------------------------------------------------
    void Slider::setVertical( bool bVertical )
    {
        m_vertical = bVertical;

        m_autoSetNextWidget[Borders::Left] = bVertical;
        m_autoSetNextWidget[Borders::Right] = bVertical;
        m_autoSetNextWidget[Borders::Top] = !bVertical;
        m_autoSetNextWidget[Borders::Bottom] = !bVertical;

        updateSlider();
    }
}  // namespace Colibri
