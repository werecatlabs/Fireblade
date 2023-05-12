#include <vector>
#include <string>
#include <FBCore/Base/StringTypes.h>

namespace fb
{
    namespace data
    {
        FB_STRUCT( fsm_data )
        {
            FB_CONSTRUCTOR( fsm_data() {
                newState = 0;
                previousState = 0;
                currentState = 0;
            } )

            FB_ATTR( std::string, name );
            FB_ATTR( std::string, eventName );
            FB_ATTR( int, newState );
            FB_ATTR( int, previousState );
            FB_ATTR( int, currentState );
        };

        FB_STRUCT( AttributeData )
        {
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, value );
        };

        FB_STRUCT( PropertyData )
        {
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, value );
            FB_ARRAY_PROPERTY( AttributeData, attributes );
        };

        FB_STRUCT( PropertiesData )
        {
            FB_ARRAY_PROPERTY( PropertyData, properties );
        };

        FB_STRUCT( camera_data )
        {
            FB_CONSTRUCTOR( camera_data() {
                active = false;
                yFOV = 0.0f;
                nearDistance = 0.0f;
                farDistance = 0.0f;
            } )

            FB_ATTR( bool, active );
            FB_ATTR( float, yFOV );
            FB_ATTR( float, nearDistance );
            FB_ATTR( float, farDistance );
        };

        FB_STRUCT( rotor_data )
        {
            FB_CONSTRUCTOR( rotor_data(){} )

            FB_ARRAY_PROPERTY( std::string, staticNodes );
            FB_ARRAY_PROPERTY( std::string, dynamicNodes );
            FB_ARRAY_PROPERTY( std::string, fadedStaticNodes );
            FB_ARRAY_PROPERTY( std::string, fadedDynamicNodes );
            FB_ARRAY_PROPERTY( std::string, fadedStaticEntities );
            FB_ARRAY_PROPERTY( std::string, fadedDynamicEntities );
        };

        FB_STRUCT( glow_rope_data )
        {
            FB_CONSTRUCTOR( glow_rope_data() {
                materialNames.resize( 0 );
                currentMaterialNames.resize( 0 );
                originalMaterialNames.resize( 0 );
            } )

            FB_ARRAY_PROPERTY( std::string, materialNames );
            FB_ARRAY_PROPERTY( std::string, currentMaterialNames );
            FB_ARRAY_PROPERTY( std::string, originalMaterialNames );
        };

        FB_STRUCT( pid_data )
        {
            FB_CONSTRUCTOR( pid_data() {
                p = 0;
                i = 0;
                d = 0;

                iLim = 0;
                iAccumulator = 0;
                lastErr = 0;
                lastDer = 0;
            } )

            FB_ATTR( float, p );
            FB_ATTR( float, i );
            FB_ATTR( float, d );
            FB_ATTR( float, iLim );
            FB_ATTR( float, iAccumulator );
            FB_ATTR( float, lastErr );
            FB_ATTR( float, lastDer );
        };

        FB_STRUCT( pid_scaler_data )
        {
            FB_CONSTRUCTOR( pid_scaler_data() {
                p = 0;
                i = 0;
                d = 0;
            } )

            FB_ATTR( float, p );
            FB_ATTR( float, i );
            FB_ATTR( float, d );
        };

        FB_STRUCT( stab_settings_data )
        {
            FB_CONSTRUCTOR( stab_settings_data() {
                MaxAxisLock = 0;
                MaxAxisLockRate = 0;
                AxisLockKp = 0;
                VbarMaxAngle = 0;
                VbarGyroSuppress = 0;
                VbarRollPI_Kp = 0;
                VbarRollPI_Ki = 0;
                VbarPitchPI_Kp = 0;
                VbarPitchPI_Ki = 0;
                VbarYawPI_Kp = 0;
                VbarYawPI_Ki = 0;
                VbarDecay = 0;

                VbarPiroComp = 0;

                RollMax = 0;
                PitchMax = 0;
                YawMax = 0;
                RattitudeTransition = 0;
                WeakLevelingKp = 0;
                MaxWeakLevelingRate = 0;

                const int NUM_AXES = 4;
                ManualRate.reserve( NUM_AXES );
                innerPids.reserve( NUM_AXES );
                outerPids.reserve( NUM_AXES );
                Expo.reserve( NUM_AXES );
                InnerLoopEnabled.reserve( NUM_AXES );
                OuterLoopEnabled.reserve( NUM_AXES );
                MaximumRate.reserve( NUM_AXES );
                VbarSensitivity.reserve( NUM_AXES );

                /*
                for (size_t i=0; i<ManualRate.size(); ++i)
                {
                    ManualRate[i] = 0;
                }

                for (size_t i = 0; i < Expo.size(); ++i)
                {
                    Expo[i] = 0;
                }

                for (size_t i = 0; i < InnerLoopEnabled.size(); ++i)
                {
                    InnerLoopEnabled[i] = 0;
                }

                for (size_t i = 0; i < OuterLoopEnabled.size(); ++i)
                {
                    OuterLoopEnabled[i] = 0;
                }

                for (size_t i = 0; i < MaximumRate.size(); ++i)
                {
                    MaximumRate[i] = 0;
                }

                for (size_t i = 0; i < VbarSensitivity.size(); ++i)
                {
                    VbarSensitivity[i] = 0;
                }
                */
            } )

            FB_ATTR( float, MaxAxisLock );
            FB_ATTR( float, MaxAxisLockRate );
            FB_ATTR( float, AxisLockKp );
            FB_ATTR( float, VbarMaxAngle );
            FB_ATTR( float, VbarGyroSuppress );
            FB_ATTR( float, VbarRollPI_Kp );
            FB_ATTR( float, VbarRollPI_Ki );
            FB_ATTR( float, VbarPitchPI_Kp );
            FB_ATTR( float, VbarPitchPI_Ki );
            FB_ATTR( float, VbarYawPI_Kp );
            FB_ATTR( float, VbarYawPI_Ki );
            FB_ATTR( float, VbarDecay );
            FB_ATTR( float, RollMax );
            FB_ATTR( float, PitchMax );
            FB_ATTR( float, YawMax );
            FB_ATTR( float, RattitudeTransition );
            FB_ATTR( float, WeakLevelingKp );
            FB_ATTR( float, MaxWeakLevelingRate );

            FB_ATTR( int, VbarPiroComp );

            FB_ARRAY_PROPERTY( double, ManualRate );
            FB_ARRAY_PROPERTY( pid_data, innerPids );
            FB_ARRAY_PROPERTY( pid_data, outerPids );
            FB_ARRAY_PROPERTY( double, Expo );
            FB_ARRAY_PROPERTY( int, InnerLoopEnabled );
            FB_ARRAY_PROPERTY( int, OuterLoopEnabled );
            FB_ARRAY_PROPERTY( double, MaximumRate );
            FB_ARRAY_PROPERTY( double, VbarSensitivity );
        };

        FB_STRUCT( drone_stab_settings )
        {
            FB_CONSTRUCTOR( drone_stab_settings(){} )

            FB_ATTR( int, currentOptionId );
            FB_ARRAY_PROPERTY( stab_settings_data, settingsData );
        };

        FB_STRUCT( multi_rotor_prop_data )
        {
            FB_CONSTRUCTOR( multi_rotor_prop_data() {
                diameter = 0.0f;
                pitch = 0.0f;
            } )

            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );
            FB_ATTR( float, diameter );
            FB_ATTR( float, pitch );
        };

        FB_STRUCT( multi_rotor_motor_data )
        {
            FB_CONSTRUCTOR( multi_rotor_motor_data() {
                maxTorque = 0.0f;
                maxRPM = 0.0f;
                lastInput = 0.0f;
                slewRateUp = 0.0f;
                slewRateDown = 0.0f;
            } )

            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );
            FB_ATTR( float, maxTorque );
            FB_ATTR( float, maxRPM );
            FB_ATTR( float, lastInput );
            FB_ATTR( float, slewRateUp );
            FB_ATTR( float, slewRateDown );
        };

        FB_STRUCT( multi_rotor_ctrl_data )
        {
            FB_CONSTRUCTOR( multi_rotor_ctrl_data() {
                // stabSettingsData = stab_settings_data();
                maxRPM = 15000.0f;
                maxTorque = 1.0f;
                angularDamping = 0.0f;
                propPitch = 0.0f;
                propDiameter = 0.0f;
                voMultiplier = 1.0f;
                groundEffectMultiplier = 1.0f;
                drag = 0.0f;

                moiX = 0.0f;
                moiY = 0.0f;
                moiZ = 0.0f;

                maxThrust = 0.0f;
                thrustMultiplier = 0.0f;

                mass = 0.0f;
                type = 0;
            } )

            // FB_ATTR(stab_settings_data, stabSettingsData);
            FB_ARRAY_PROPERTY( multi_rotor_prop_data, propData );
            FB_ARRAY_PROPERTY( multi_rotor_motor_data, motorData );
            FB_ARRAY_PROPERTY( collider, colliders );
            FB_ATTR( float, maxRPM );
            FB_ATTR( float, maxTorque );
            FB_ATTR( float, angularDamping );
            FB_ATTR( float, propPitch );
            FB_ATTR( float, propDiameter );
            FB_ATTR( float, voMultiplier );
            FB_ATTR( float, groundEffectMultiplier );
            FB_ATTR( float, drag );
            FB_ATTR( float, moiX );
            FB_ATTR( float, moiY );
            FB_ATTR( float, moiZ );
            FB_ATTR( float, maxThrust );
            FB_ATTR( float, thrustMultiplier );
            FB_ATTR( float, mass );
            FB_ATTR( int, type );
        };

        FB_STRUCT( four_wheeled_vehicle_wheel_data )
        {
            FB_CONSTRUCTOR( four_wheeled_vehicle_wheel_data(){} )

            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );
            FB_ATTR( std::string, wheelName );
        };

        FB_STRUCT( four_wheeled_vehicle_data )
        {
            FB_CONSTRUCTOR( four_wheeled_vehicle_data() {
                angularDamping = 0.0f;
                linearDamping = 0.0f;
                mass = 1.0f;
                wheelMass = 0.0f;
                centreBias = 0.0f;
                frontBias = 1.0f;
                rearBias = 0.0f;
                frontRearSplit = 1.0f;
                isNitro = true;
            } )

            FB_ATTR( float, angularDamping );
            FB_ATTR( float, linearDamping );
            FB_ATTR( float, mass );
            FB_ATTR( float, wheelMass );
            FB_ATTR( vec4, chassisDims );
            FB_ATTR( vec4, moi );
            FB_ATTR( transform_data, cogLocalTransform );
            FB_ATTR( transform_data, cogWorldTransform );
            FB_ATTR( float, centreBias );
            FB_ATTR( float, frontBias );
            FB_ATTR( float, rearBias );
            FB_ATTR( float, frontRearSplit );
            FB_ATTR( bool, isNitro );
            FB_ARRAY_PROPERTY( four_wheeled_vehicle_wheel_data, wheels );
        };

        FB_STRUCT( water_ripples )
        {
            FB_CONSTRUCTOR( water_ripples() {
                displacementTexture = 0;
                displacementWidth = 0;
                displacementHeight = 0;
            } )

            FB_ATTR( int, displacementTexture );
            FB_ATTR( int, displacementWidth );
            FB_ATTR( int, displacementHeight );

            FB_ATTR( vec4, amplitude );
            FB_ATTR( vec4, frequency );
            FB_ATTR( vec4, steepness );
            FB_ATTR( vec4, speed );
            FB_ATTR( vec4, directionAB );
            FB_ATTR( vec4, directionCD );
            FB_ARRAY_PROPERTY( vec4, displacementTextureData );
        };

        FB_STRUCT( model_component_data )
        {
            FB_CONSTRUCTOR( model_component_data() {
                hashName = 0;

                rpm = 0.0f;

                value = 0.0f;
                value1 = 0.0f;
                value2 = 0.0f;
                value3 = 0.0f;

                value4 = 0.0f;
                value5 = 0.0f;
                value6 = 0.0f;
                value7 = 0.0f;

                isControl = 0;
                isVisible = 1;

                isCrashed = 0;
                state = 0;
                isLocalTransform = 0;
                hasTransform = 1;

                objectReferenceHash = 0;
                rendererReferenceHash = 0;
                classTypeHash = 0;
                reserved3 = 0;

                iValue1 = 0;
                iValue2 = 0;
                iValue3 = 0;
                iValue4 = 0;
            } )

            FB_ATTR( int, hashName );
            FB_ATTR( std::string, ident );
            FB_ATTR( std::string, objectReference );
            FB_ATTR( std::string, componentName );
            FB_ATTR( std::string, componentResource );
            FB_ATTR( std::string, prefab );
            FB_ATTR( std::string, parentName );
            FB_ATTR( std::string, sceneNodeName );
            FB_ATTR( std::string, configType );

            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );

            FB_ATTR( vec4, vec1 );
            FB_ATTR( vec4, vec2 );
            FB_ATTR( vec4, vec3 );

            FB_ATTR( float, rpm );

            FB_ATTR( float, value );
            FB_ATTR( float, value1 );
            FB_ATTR( float, value2 );
            FB_ATTR( float, value3 );

            FB_ATTR( float, value4 );
            FB_ATTR( float, value5 );
            FB_ATTR( float, value6 );
            FB_ATTR( float, value7 );

            FB_ATTR( int, isControl );
            FB_ATTR( int, isVisible );

            FB_ATTR( int, rowId );
            FB_ATTR( int, objectId );

            FB_ATTR( int, isCrashed );
            FB_ATTR( int, state );
            FB_ATTR( int, isLocalTransform );
            FB_ATTR( int, hasTransform );

            FB_ATTR( int, objectReferenceHash );
            FB_ATTR( int, rendererReferenceHash );
            FB_ATTR( int, classTypeHash );
            FB_ATTR( int, reserved3 );

            FB_ATTR( int, iValue1 );
            FB_ATTR( int, iValue2 );
            FB_ATTR( int, iValue3 );
            FB_ATTR( int, iValue4 );

            FB_ATTR( std::string, componentType );
            FB_ATTR( std::string, particleSystemName );

            FB_ATTR( PropertiesData, properties );

            FB_ATTR( rotor_data, rotorData );
            FB_ATTR( glow_rope_data, glowRopeData );
            FB_ATTR( multi_rotor_ctrl_data, multiRotorCtrlData );
            FB_ATTR( four_wheeled_vehicle_data, fourWheeledVehicleData );
        };

        FB_STRUCT( model_data )
        {
            FB_CONSTRUCTOR( model_data() {
                modelId = 0;
                modelReferenceId = 0;
                isVisible = 0;
            } )

            FB_ATTR( std::string, name );
            FB_ATTR( std::string, resource );
            FB_ATTR( int, modelId );
            FB_ATTR( int, modelReferenceId );

            FB_ATTR( std::string, prefab );

            FB_ATTR( vec4, position );
            FB_ATTR( vec4, orientation );
            FB_ATTR( vec4, scale );

            FB_ATTR( vec4, startPosition );
            FB_ATTR( vec4, startOrientation );

            FB_ATTR( int, isVisible );
            FB_ARRAY_PROPERTY( model_component_data, components );
        };

        FB_STRUCT( plugin_event )
        {
            FB_CONSTRUCTOR( inline plugin_event() {
                const int reserveSize = 256;

                type.reserve( reserveSize );
                arg1.reserve( reserveSize * 4 );
                arg2.reserve( reserveSize );
                arg3.reserve( reserveSize );
                arg4.reserve( reserveSize );

                type = "";
                arg1 = "";
                arg2 = "";
                arg3 = "";
                arg4 = "";
            } )

            FB_ATTR( std::string, type );
            FB_ATTR( std::string, arg1 );
            FB_ATTR( std::string, arg2 );
            FB_ATTR( std::string, arg3 );
            FB_ATTR( std::string, arg4 );
        };

        FB_STRUCT( plugin_events )
        {
            FB_ARRAY_PROPERTY( plugin_event, events );
        };

        FB_STRUCT( flightfileinfo )
        {
            FB_CONSTRUCTOR( flightfileinfo() {
                recordingFPS = 0.0;
                totalTime = 0.0;
                modelId = 0;
                refModelId = 0;
            } )

            FB_ATTR( std::string, filename );
            FB_ATTR( std::string, title );
            FB_ATTR( std::string, date );
            FB_ATTR( std::string, duration );
            FB_ATTR( std::string, description );
            FB_ATTR( std::string, sceneryName );
            FB_ATTR( std::string, modelName );
            FB_ATTR( std::string, pilotName );
            FB_ATTR( double, recordingFPS );
            FB_ATTR( double, totalTime );
            FB_ATTR( std::string, data );
            FB_ATTR( int, modelId );
            FB_ATTR( int, refModelId );
        };

        FB_STRUCT( s_flight_recording_file )
        {
            FB_ATTR( std::string, pilotName );
            FB_ATTR( std::string, fileName );
            FB_ATTR( std::string, description );
        };

        FB_STRUCT( file_data )
        {
            FB_ATTR( std::string, fileName );
            FB_ATTR( std::string, filePath );
        };

        FB_STRUCT( directorylisting )
        {
            FB_ATTR( std::string, folderName );
            FB_ARRAY_PROPERTY( file_data, files );
            FB_ARRAY_PROPERTY( directorylisting, folders );
        };

        FB_STRUCT( s_component_data )
        {
            FB_CONSTRUCTOR( s_component_data() {
                eleMixing = 0.0f;
                rudMixing = 0.0f;
                ailMixing = 0.0f;
                m_str = "";
                is_host = false;
            }

            )

            FB_ATTR( std::string, applied );
            FB_ATTR( std::string, component_id );

            FB_ATTR( std::string, userName );
            FB_ATTR( std::string, userEmail );
            FB_ATTR( std::string, ticketSubject );
            FB_ATTR( std::string, ticketDescription );

            FB_ATTR( std::string, FlEqAutoHang );
            FB_ATTR( float, eleMixing );
            FB_ATTR( float, rudMixing );
            FB_ATTR( float, ailMixing );

            FB_ATTR( std::string, target );
            FB_ATTR( std::string, eventType );
            FB_ATTR( std::string, eventState );

            FB_ATTR( std::string, user_name );
            FB_ATTR( std::string, message );
            FB_ATTR( std::string, user_id );
            FB_ATTR( bool, is_host );

            std::string m_str;
        };

        FB_STRUCT( dialogstate )
        {
            FB_CONSTRUCTOR( dialogstate() {
                is_relative = false;
                change_position = false;
                change_size = false;
                change_input_rect = false;
                x = 0.0f;
                y = 0.0f;
                width = 0.0f;
                height = 0.0f;
                inputRectMinX = 0.0f;
                inputRectMinY = 0.0f;
                inputRectMaxX = 0.0f;
                inputRectMaxY = 0.0f;
            } )

            FB_ATTR( bool, is_relative );
            FB_ATTR( bool, change_position );
            FB_ATTR( bool, change_size );
            FB_ATTR( bool, change_input_rect );

            FB_ATTR( float, x );
            FB_ATTR( float, y );
            FB_ATTR( float, width );
            FB_ATTR( float, height );

            FB_ATTR( float, inputRectMinX );
            FB_ATTR( float, inputRectMinY );
            FB_ATTR( float, inputRectMaxX );
            FB_ATTR( float, inputRectMaxY );
        };

        FB_STRUCT( model_load_data )
        {
            FB_CONSTRUCTOR( model_load_data() {
                modelId = 0;
                modelReferenceId = 0;
            } )

            FB_ATTR( std::string, name );
            FB_ATTR( std::string, resource );
            FB_ATTR( int, modelId );
            FB_ATTR( int, modelReferenceId );
            FB_ATTR( std::string, prefab );
        };

        FB_STRUCT( scenery_data )
        {
            FB_CONSTRUCTOR( scenery_data() {
                id = 0;
                isNight = false;
            } )

            FB_ATTR( int, id );
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, resource );
            FB_ATTR( std::string, sceneName );
            FB_ATTR( std::string, coneLayout );
            FB_ATTR( bool, isNight );
        };

        FB_STRUCT( collision_object )
        {
            FB_CONSTRUCTOR( collision_object() { radius = 0; } )

            FB_ATTR( std::string, type );
            FB_ATTR( std::string, meshFilePath );
            FB_ATTR( vec4, extents );
            FB_ATTR( vec4, offset );
            FB_ATTR( float, radius );
        };

        FB_STRUCT( chat_data )
        {
            FB_CONSTRUCTOR( chat_data() { is_host = false; } )

            FB_ATTR( std::string, user_id );
            FB_ATTR( std::string, user_name );
            FB_ATTR( std::string, message );
            FB_ATTR( bool, is_host );
        };

        FB_STRUCT( model_wind )
        {
            FB_CONSTRUCTOR( model_wind() {
                speed = 0;
                direction = 0;
                turbulence = 0;
                groundHeight = 0;
                directionOffset = 0;
                fieldRoughness = 0;
                temperature = 0;
                pressure = 0;
                smallTurbulence = 0;
            } )

            FB_ATTR( float, speed );
            FB_ATTR( float, direction );
            FB_ATTR( float, turbulence );
            FB_ATTR( float, groundHeight );
            FB_ATTR( float, directionOffset );
            FB_ATTR( float, fieldRoughness );
            FB_ATTR( float, temperature );
            FB_ATTR( float, pressure );
            FB_ATTR( float, smallTurbulence );
        };

        FB_STRUCT( configured_model_component_data )
        {
            FB_CONSTRUCTOR( configured_model_component_data() {
                id = 0;
                referenceId = 0;
                hashName = 0;
            } )

            FB_ATTR( int, id );
            FB_ATTR( int, referenceId );
            FB_ATTR( int, hashName );
            FB_ATTR( std::string, label );
            FB_ATTR( std::string, image );
            FB_ATTR( std::string, resource );
            FB_ATTR( std::string, className );
            FB_ATTR( std::string, configType );
            FB_ATTR( std::string, objectName );
        };

        FB_STRUCT( configured_model_data )
        {
            FB_CONSTRUCTOR( configured_model_data() {
                id = 0;
                parentId = 0;
                refComponentId = 0;
                scale = 0;
                nitro = 0;
                standard = 0;
                refModelRowId = 0;
            } )

            FB_ATTR( int, id );
            FB_ATTR( int, parentId );
            FB_ATTR( int, refComponentId );
            FB_ATTR( int, scale );
            FB_ATTR( int, nitro );
            FB_ATTR( int, standard );
            FB_ATTR( int, refModelRowId );
            FB_ATTR( std::string, configType );
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, resourceName );
            FB_ATTR( std::string, modelImage );
            FB_ARRAY_PROPERTY( configured_model_component_data, configuredComponents );
        };

        FB_STRUCT( configured_models_data )
        {
            FB_ARRAY_PROPERTY( configured_model_data, configuredModelData );
        };

        FB_STRUCT( debug_text_data )
        {
            FB_CONSTRUCTOR( debug_text_data() {
                id = 0;
                x = 0.0f;
                y = 0.0f;
                visible = true;
            } )

            FB_ATTR( int, id );
            FB_ATTR( float, x );
            FB_ATTR( float, y );
            FB_ATTR( std::string, text );
            FB_ATTR( std::string, eventName );
            FB_ATTR( bool, visible );
        };

        FB_STRUCT( dialog_manager_state )
        {
            FB_CONSTRUCTOR( dialog_manager_state() { fullScreenDialogDisplayed = false; } )

            FB_ATTR( bool, fullScreenDialogDisplayed );
        };

        FB_STRUCT( resource_map_entry )
        {
            FB_CONSTRUCTOR( resource_map_entry() { id = 0; } )

            FB_ATTR( int, id );
            FB_ATTR( std::string, resourceType );
            FB_ATTR( std::string, name );
            FB_ATTR( std::string, prefab );
        };

        FB_STRUCT( resource_map )
        {
            FB_ARRAY_PROPERTY( resource_map_entry, resources );
        };

        FB_STRUCT( layer_data )
        {
            FB_CONSTRUCTOR( layer_data() { value = 0; } )

            FB_ATTR( std::string, name );
            FB_ATTR( int, value );
        };

        FB_STRUCT( constraint_data )
        {
            FB_CONSTRUCTOR( constraint_data() {
                motionAxisX = 0;
                motionAxisY = 0;
                motionAxisZ = 0;

                motionTwist = 0;
                motionSwing1 = 0;
                motionSwing2 = 0;

                actorA = 0;
                actorB = 0;

                rigidBodyA = 0;
                rigidBodyB = 0;
            } )

            FB_ATTR( int, type );

            FB_ATTR( int, motionAxisX );
            FB_ATTR( int, motionAxisY );
            FB_ATTR( int, motionAxisZ );

            FB_ATTR( int, motionTwist );
            FB_ATTR( int, motionSwing1 );
            FB_ATTR( int, motionSwing2 );

            FB_ATTR( int, actorA );
            FB_ATTR( int, actorB );

            FB_ATTR( int, rigidBodyA );
            FB_ATTR( int, rigidBodyB );

            FB_ATTR( vec4, localPositionA );
            FB_ATTR( vec4, localPositionB );

            FB_ATTR( vec4, localOrientationA );
            FB_ATTR( vec4, localOrientationB );
        };

        FB_STRUCT( rigid_body )
        {
            FB_CONSTRUCTOR( rigid_body() {
                numSolverIterations = 0;
                mass = 0;
                drag = 0;
                angularDrag = 0;
                useGravity = false;
                isKinematic = false;
                isStatic = false;
            } )

            FB_ARRAY_PROPERTY( sphere_collider, sphereColliders );
            FB_ARRAY_PROPERTY( plane_collider, planeColliders );
            FB_ARRAY_PROPERTY( box_collider, boxColliders );
            FB_ARRAY_PROPERTY( mesh_collider, meshColliders );
            FB_ARRAY_PROPERTY( terrain_collider, terrainColliders );
            FB_ARRAY_PROPERTY( compond_collider, compondColliders );

            FB_ATTR( int, numSolverIterations );
            FB_ATTR( float, mass );
            FB_ATTR( float, drag );
            FB_ATTR( float, angularDrag );
            FB_ATTR( bool, useGravity );
            FB_ATTR( bool, isKinematic );
            FB_ATTR( bool, isStatic );
            FB_ATTR( layer_data, layer );
            FB_ARRAY_PROPERTY( layer_data, collidableLayers );
            FB_ATTR( object_bounds_data, bounds );
        };

        FB_STRUCT( model_layer_data )
        {
            FB_CONSTRUCTOR( model_layer_data(){} )

            FB_ATTR( layer_data, layer );
            FB_ARRAY_PROPERTY( layer_data, collidableLayers );
        };

        FB_STRUCT( buoyancy_data )
        {
            FB_CONSTRUCTOR( buoyancy_data() {
                actorId = 0;
                density = 0;
                slicesPerAxis = 0;
                isConcave = false;
                voxelsLimit = 0;
                waveVelocity = 0;
            } )

            FB_ATTR( int, actorId );
            FB_ATTR( float, density );
            FB_ATTR( int, slicesPerAxis );
            FB_ATTR( bool, isConcave );
            FB_ATTR( int, voxelsLimit );
            FB_ATTR( float, waveVelocity );
        };

        FB_STRUCT( component_event_data )
        {
            FB_CONSTRUCTOR( component_event_data() { componentId = 0; } )

            FB_ATTR( int, componentId );
            FB_ATTR( std::string, componentType );
            FB_ATTR( buoyancy_data, buoyancyData );
            FB_ATTR( rigid_body, rigidBody );
            FB_ATTR( constraint_data, constraintData );
            FB_ATTR( water_ripples, water );
        };

        FB_STRUCT( actor_event_data )
        {
            FB_CONSTRUCTOR( actor_event_data(){} )

            FB_ATTR( std::string, eventName );
            FB_ATTR( actor_data, actorData );
            FB_ATTR( component_event_data, componentData );
            FB_ATTR( model_component_data, modelComponentData );
            FB_ARRAY_PROPERTY( std::string, strData );
        };

        FB_STRUCT( component_data_options )
        {
            FB_CONSTRUCTOR( component_data_options() { applyOffset = true; } )

            FB_ATTR( bool, applyOffset );
        };

        FB_STRUCT( object_select_data )
        {
            FB_CONSTRUCTOR( object_select_data() { selectedId = 0; } )

            FB_ATTR( int, selectedId );
            FB_ATTR( std::string, objectType );
            FB_ATTR( std::string, objectName );
        };

        FB_STRUCT( movable_object_data )
        {
            FB_CONSTRUCTOR( movable_object_data() {
                name = "";
                type = "";
            } )

            FB_ATTR( std::string, name );
            FB_ATTR( std::string, type );
            FB_ATTR( vec4, minExtent );
            FB_ATTR( vec4, maxExtent );

            FB_ATTR( vec4, extent );
            FB_ATTR( vec4, center );
        };

        FB_STRUCT( scene_node_data )
        {
            FB_CONSTRUCTOR( scene_node_data() {
                name = "";
                hashId = 0;
                isVisible = false;
                isComponentVisible = false;
                isInHierarchy = false;
            } )

            FB_ATTR( std::string, name );
            FB_ATTR( std::string, componentType );
            FB_ATTR( int, hashId );
            FB_ATTR( bool, isVisible );
            FB_ATTR( bool, isComponentVisible );
            FB_ATTR( bool, isInHierarchy );
            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );
            FB_ARRAY_PROPERTY( scene_node_data, children );
            FB_ARRAY_PROPERTY( movable_object_data, objects );
        };

        FB_STRUCT( path_data )
        {
            FB_ATTR( std::string, installationPath );
            FB_ATTR( std::string, mydocumentsPath );
            FB_ATTR( std::string, appdataPath );
            FB_ATTR( std::string, flightDataPath );
        };

        FB_STRUCT( model_physics_setup_flight_data )
        {
            FB_CONSTRUCTOR( model_physics_setup_flight_data() {
                modelType = 1;  // heli
                isNitro = false;
                isFlybar = false;

                model = "Model";
                rotorHead = "RotorHead";
                headTeeter = "HeadTeeter";
                mainRotor = "MainRotor";
                tailRotor = "TailRotor";
                flybar = "Flybar";
                propRotor = "PropRotor";
                engine = "engine";
                swashplate = "swashplate";
                receiver = "accurc";
                flybarController = "flybarctrl";

                groundEffectMax = 1.2f;
                groundEffectDecay = 1.2f;

                calculationRateVal1 = 1.5f;
                calculationRateVal2 = 1.5f;
            } )

            FB_ATTR( int, modelType );
            FB_ATTR( bool, isNitro );
            FB_ATTR( bool, isFlybar );

            FB_ATTR( std::string, model );
            FB_ATTR( std::string, rotorHead );
            FB_ATTR( std::string, headTeeter );
            FB_ATTR( std::string, mainRotor );
            FB_ATTR( std::string, tailRotor );
            FB_ATTR( std::string, flybar );
            FB_ATTR( std::string, propRotor );
            FB_ATTR( std::string, engine );
            FB_ATTR( std::string, swashplate );
            FB_ATTR( std::string, receiver );
            FB_ATTR( std::string, flybarController );

            FB_ATTR( float, groundEffectMax );
            FB_ATTR( float, groundEffectDecay );

            FB_ATTR( float, calculationRateVal1 );
            FB_ATTR( float, calculationRateVal2 );
        };

        FB_STRUCT( model_physics_setup_data )
        {
            FB_ATTR( std::string, name );
            FB_ATTR( model_physics_setup_flight_data, flightData );
        };

        FB_STRUCT( models_physics_setup_data )
        {
            FB_ARRAY_PROPERTY( model_physics_setup_data, data );
        };

        FB_STRUCT( setup_wizard_data )
        {
            FB_ATTR( std::string, language );
            FB_ATTR( std::string, languageCode );
            FB_ATTR( std::string, flybarlessSetting );
            FB_ATTR( std::string, txMake );
            FB_ATTR( std::string, txMode );
            FB_ATTR( std::string, adapter );
        };

        FB_STRUCT( system_settings_data )
        {
            FB_CONSTRUCTOR( system_settings_data() {
                screenWidth = 0;
                screenHeight = 0;
                isFullscreen = false;
                isStartWorkbench = true;
            } )

            // FB_ATTR(resolution_data, resolutionData);
            FB_ATTR( std::string, renderQuality );
            FB_ATTR( int, screenWidth );
            FB_ATTR( int, screenHeight );
            FB_ATTR( bool, isFullscreen );
            FB_ATTR( bool, isStartWorkbench );
            FB_ATTR( bool, isRaceMode );
            FB_ATTR( int, startSceneId );
            FB_ATTR( models_physics_setup_data, modelsPhysicsSetupData );
            FB_ATTR( setup_wizard_data, setupWizardData );
        };

        FB_STRUCT( switch_indicators_data )
        {
            FB_CONSTRUCTOR( switch_indicators_data() {
                rpm1 = 0;
                rpm2 = 0;
                rpm3 = 0;

                tx = 0;
                adapter = 0;
                norm = 0;
                idle = 0;
                hold = 0;

                dualRate = 0;
                gearDown = 0;
                flapDown = 0;
                bail = 0;

                bailMode = 0;
                batteryState = 0;
                batteryVolts = 0;
            } )

            FB_ATTR( int, rpm1 );
            FB_ATTR( int, rpm2 );
            FB_ATTR( int, rpm3 );

            FB_ATTR( int, tx );
            FB_ATTR( int, adapter );
            FB_ATTR( int, norm );
            FB_ATTR( int, idle );
            FB_ATTR( int, hold );

            FB_ATTR( int, dualRate );
            FB_ATTR( int, gearDown );
            FB_ATTR( int, flapDown );
            FB_ATTR( int, bail );

            FB_ATTR( int, bailMode );
            FB_ATTR( float, batteryState );
            FB_ATTR( float, batteryVolts );
        };

        FB_STRUCT( cg_info_mode )
        {
            FB_CONSTRUCTOR( cg_info_mode() {
                mass = 0.0f;
                ailLeft = 0.0f;
                ailRight = 0.0f;
                elevLeft = 0.0f;
                elevRight = 0.0f;
                rudder = 0.0f;
                flapLeft = 0.0f;
                flapRight = 0.0f;
            } )

            FB_ATTR( float, mass );
            FB_ATTR( float, ailLeft );
            FB_ATTR( float, ailRight );
            FB_ATTR( float, elevLeft );
            FB_ATTR( float, elevRight );
            FB_ATTR( float, rudder );
            FB_ATTR( float, flapLeft );
            FB_ATTR( float, flapRight );
            FB_ARRAY_PROPERTY( float, data );
        };

        FB_STRUCT( CMapData )
        {
            FB_CONSTRUCTOR( CMapData() {
                axis = -1;
                button = -1;
                position = 0;
            } )

            FB_ATTR( int, axis );
            FB_ATTR( int, button );
            FB_ATTR( float, position );
        };

        FB_STRUCT( car_data )
        {
            FB_CONSTRUCTOR( car_data() {
                localTransform = transform_data();
                worldTransform = transform_data();

                cgPosition = vec4();
                drag = vec4();
                debugFlags = 0;
            } )

            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );
            FB_ARRAY_PROPERTY( aircraft_engine_data, engineData );
            FB_ARRAY_PROPERTY( aircraft_wheel_data, wheelData );
            FB_ARRAY_PROPERTY( aircraft_ground_effect_data, groundEffectData );
            FB_ATTR( vec4, cgPosition );
            FB_ATTR( vec4, drag );
            FB_ATTR( int, debugFlags );
        };

        FB_STRUCT( truck_data )
        {
            FB_CONSTRUCTOR( truck_data() {
                localTransform = transform_data();
                worldTransform = transform_data();

                cgPosition = vec4();
                drag = vec4();
                debugFlags = 0;
            } )

            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );
            FB_ARRAY_PROPERTY( aircraft_engine_data, engineData );
            FB_ARRAY_PROPERTY( aircraft_wheel_data, wheelData );
            FB_ARRAY_PROPERTY( aircraft_ground_effect_data, groundEffectData );
            FB_ATTR( vec4, cgPosition );
            FB_ATTR( vec4, drag );
            FB_ATTR( int, debugFlags );
        };

        FB_STRUCT( servo_hole_data )
        {
            FB_CONSTRUCTOR( servo_hole_data(){} )

            FB_ATTR( std::string, name );
            FB_ARRAY_PROPERTY( double, controlPoints );
        };

        FB_STRUCT( servo_data )
        {
            FB_CONSTRUCTOR( servo_data(){} )

            FB_ATTR( std::string, name );
            FB_ARRAY_PROPERTY( servo_hole_data, holes );
        };

        FB_STRUCT( swash_plate_position )
        {
            FB_CONSTRUCTOR( swash_plate_position(){} )

            FB_ATTR( std::string, servoName );
            FB_ATTR( std::string, holeName );

            FB_ARRAY_PROPERTY( double, collective );
            FB_ARRAY_PROPERTY( double, pitch );
            FB_ARRAY_PROPERTY( double, roll );
        };

        FB_STRUCT( swash_plate_data )
        {
            FB_CONSTRUCTOR( swash_plate_data(){} )

            FB_ATTR( std::string, name );
            FB_ARRAY_PROPERTY( servo_data, servos );
            FB_ARRAY_PROPERTY( swash_plate_position, positions );
        };

        FB_STRUCT( model_control_data )
        {
            FB_ARRAY_PROPERTY( swash_plate_data, swashPlates );
            FB_ARRAY_PROPERTY( servo_data, servos );
        };
    }  // namespace data
}  // end namespace fb
