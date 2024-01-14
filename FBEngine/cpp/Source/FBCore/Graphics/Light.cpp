#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/Light.h>
#include <FBCore/State/States/LightState.h>

namespace fb::render
{
    Light::Light() = default;

    Light::~Light() = default;

    void Light::setType( LightTypes type )
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                auto iLightType = static_cast<hash32>( type );
                state->setLightType( iLightType );
            }
        }
    }

    auto Light::getType() const -> ILight::LightTypes
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                return static_cast<ILight::LightTypes>( state->getLightType() );
            }
        }

        return LightTypes::LT_DIRECTIONAL;
    }

    void Light::setDiffuseColour( const ColourF &colour )
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                state->setDiffuseColour( colour );
            }
        }
    }

    auto Light::getDiffuseColour() const -> ColourF
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                return state->getDiffuseColour();
            }
        }

        return ColourF::White;
    }

    void Light::setSpecularColour( const ColourF &colour )
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                state->setSpecularColour( colour );
            }
        }
    }

    auto Light::getSpecularColour() const -> ColourF
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                return state->getSpecularColour();
            }
        }

        return ColourF::White;
    }

    void Light::setAttenuation( f32 range, f32 constant, f32 linear, f32 quadratic )
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                state->setRange( range );
                state->setConstant( constant );
                state->setLinear( linear );
                state->setQuadratic( quadratic );
            }
        }
    }

    auto Light::getAttenuationRange() const -> f32
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                return state->getRange();
            }
        }

        return 0.0f;
    }

    auto Light::getAttenuationConstant() const -> f32
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                return state->getConstant();
            }
        }

        return 0.0f;
    }

    auto Light::getAttenuationLinear() const -> f32
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                return state->getLinear();
            }
        }

        return 0.0f;
    }

    auto Light::getAttenuationQuadric() const -> f32
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                return state->getQuadratic();
            }
        }

        return 0.0f;
    }

    void Light::setDirection( const Vector3<real_Num> &vec )
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                state->setDirection( vec );
            }
        }
    }

    auto Light::getDirection() const -> Vector3<real_Num>
    {
        if( auto stateContext = getStateContext() )
        {
            auto state = stateContext->getStateByType<LightState>();
            if( state )
            {
                return state->getDirection();
            }
        }

        return Vector3<real_Num>::zero();
    }
}  // namespace fb::render
