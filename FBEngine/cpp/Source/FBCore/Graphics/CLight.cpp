#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CLight.h>
#include <FBCore/State/States/LightState.h>

namespace fb
{
    namespace render
    {
        CLight::CLight()
        {
        }

        CLight::~CLight()
        {
        }

        void CLight::setType( LightTypes type )
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    auto iLightType = static_cast<hash32>( type );
                    state->setLightType( iLightType );
                }
            }
        }

        ILight::LightTypes CLight::getType() const
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    return static_cast<ILight::LightTypes>( state->getLightType() );
                }
            }

            return LightTypes::LT_DIRECTIONAL;
        }

        void CLight::setDiffuseColour( const ColourF &colour )
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    state->setDiffuseColour( colour );
                }
            }
        }

        ColourF CLight::getDiffuseColour() const
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    return state->getDiffuseColour();
                }
            }

            return ColourF::White;
        }

        void CLight::setSpecularColour( const ColourF &colour )
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    state->setSpecularColour( colour );
                }
            }
        }

        ColourF CLight::getSpecularColour() const
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    return state->getSpecularColour();
                }
            }

            return ColourF::White;
        }

        void CLight::setAttenuation( f32 range, f32 constant, f32 linear, f32 quadratic )
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    state->setRange( range );
                    state->setConstant( constant );
                    state->setLinear( linear );
                    state->setQuadratic( quadratic );
                }
            }
        }

        f32 CLight::getAttenuationRange() const
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    return state->getRange();
                }
            }

            return 0.0f;
        }

        f32 CLight::getAttenuationConstant() const
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    return state->getConstant();
                }
            }

            return 0.0f;
        }

        f32 CLight::getAttenuationLinear() const
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    return state->getLinear();
                }
            }

            return 0.0f;
        }

        f32 CLight::getAttenuationQuadric() const
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    return state->getQuadratic();
                }
            }

            return 0.0f;
        }

        void CLight::setDirection( const Vector3<real_Num> &vec )
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    state->setDirection( vec );
                }
            }
        }

        Vector3<real_Num> CLight::getDirection() const
        {
            if( auto stateObject = getStateObject() )
            {
                auto state = stateObject->getStateByType<LightState>();
                if( state )
                {
                    return state->getDirection();
                }
            }

            return Vector3<real_Num>::zero();
        }
    }  // namespace render
}  // namespace fb
