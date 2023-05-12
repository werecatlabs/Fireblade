namespace fb
{
    namespace data
    {
        // FB_STRUCT(aircraft_curve_value)
        //{
        //	FB_ATTR(float, time);
        //	FB_ATTR(float, value);
        // };

        // FB_STRUCT(aircraft_curve_data)
        //{
        //	FB_ARRAY_PROPERTY(aircraft_curve_value, values);
        // };

        // FB_STRUCT(aircraft_aerofoil_data)
        //{
        //	FB_ATTR(aircraft_curve_data, cl);
        //	FB_ATTR(aircraft_curve_data, cd);
        //	FB_ATTR(aircraft_curve_data, cm);

        //	FB_ATTR(aircraft_curve_data, clPlus10);
        //	FB_ATTR(aircraft_curve_data, cdPlus10);
        //	FB_ATTR(aircraft_curve_data, cmPlus10);

        //	FB_ATTR(aircraft_curve_data, clMinus10);
        //	FB_ATTR(aircraft_curve_data, cdMinus10);
        //	FB_ATTR(aircraft_curve_data, cmMinus10);
        //};

        // FB_STRUCT(aircraft_attachment_data)
        //{
        //	FB_ATTR(transform_data, localTransform);
        //	//FB_ATTR(transform_data, worldTransform);
        //	FB_ATTR(bool, controllable);
        //	FB_ATTR(std::string, name);
        // };

        // FB_STRUCT_INHERIT(aircraft_wing_data, public aircraft_attachment_data)
        //{
        //	FB_CONSTRUCTOR(
        //		aircraft_wing_data()
        //	{
        //		subDivision = vec4(0,0,0);

        //		sectionCount = 10;
        //		wingTipWidthZeroToOne = 1.0f;
        //		wingTipSweep = 0.0f;
        //		wingTipAngle = 0.0f;
        //		cdOverride = 0.045f;
        //		wingArea = 0.0f;
        //		angleOfAttack = 0.0f;

        //		liftLineChordPosition = 0.5f;
        //		liftLineChordPositionReversed = 0.5f;

        //		liftLineAlphaStart = 70.0f;
        //		liftLineAlphaEnd = 110.0f;

        //		aoaMultiplier = 1.0f;
        //		cdMultiplier = 1.0f;
        //		clMultiplier = 1.0f;
        //		cmMultiplier = 1.0f;
        //		stallControlCL = 0.0f;
        //		stallControlCD = 0.0f;
        //		stallControlCM = 0.0f;
        //		stallThreshold = 1.0f;

        //		isMirror = false;
        //	}
        //	)

        //	FB_ATTR(std::string, type);
        //	FB_ATTR(int, sectionCount);
        //	FB_ATTR(float, wingTipWidthZeroToOne);
        //	FB_ATTR(float, wingTipSweep);
        //	FB_ATTR(float, wingTipAngle);
        //	FB_ATTR(float, cdOverride);
        //	FB_ATTR(float, wingArea);
        //	FB_ATTR(float, angleOfAttack);

        //	FB_ATTR(float, liftLineChordPosition);
        //	FB_ATTR(float, liftLineChordPositionReversed);

        //	FB_ATTR(float, liftLineAlphaStart);
        //	FB_ATTR(float, liftLineAlphaEnd);

        //	FB_ATTR(float, aoaMultiplier);
        //	FB_ATTR(float, cdMultiplier);
        //	FB_ATTR(float, clMultiplier);
        //	FB_ATTR(float, cmMultiplier);
        //
        //	FB_ATTR(float, stallControlCL);
        //	FB_ATTR(float, stallControlCD);
        //	FB_ATTR(float, stallControlCM);
        //	FB_ATTR(float, stallThreshold);

        //	FB_ATTR(bool, isMirror);

        //	FB_ATTR(vec4, wingBoxColliderMin);
        //	FB_ATTR(vec4, wingBoxColliderMax);
        //	FB_ATTR(vec4, wingRootLeadingEdge);
        //	FB_ATTR(vec4, wingRootTrailingEdge);
        //	FB_ATTR(vec4, wingTipLeadingEdge);
        //	FB_ATTR(vec4, wingTipTrailingEdge);
        //	FB_ATTR(vec4, rootLiftPosition);
        //	FB_ATTR(vec4, tipLiftPosition);
        //	FB_ATTR(vec4, subDivision);

        //	FB_ATTR(std::string, aerofoilName);
        //	FB_ATTR(std::string, controlSurfaceName);
        //	FB_ATTR(std::string, propWashName);
        //	FB_ATTR(std::string, groundEffectName);
        //	FB_ATTR(std::string, mirrorObject);

        //	FB_INHERIT(aircraft_attachment_data);
        //};

        // FB_STRUCT_INHERIT(aircraft_engine_data, public aircraft_attachment_data)
        //{
        //	FB_CONSTRUCTOR(
        //		aircraft_engine_data()
        //	{
        //		rpmToUseFastProp = 300.0f;
        //		idleRPM = 400.0f;
        //		maxRPM = 2800.0f;
        //		forceAtMaxRPM = 4000.0f;
        //		rpmToAddPerKTOfSpeed = 10.0f;
        //		rpmLerpSpeed = 1.5f;
        //		pitchAtIdleRPM = 0.5f;
        //		pitchAtMaxRPM = 1.0f;
        //		currentRPM = 0.0f;
        //		desiredRPM = 0.0f;
        //		engineRunVolume = 0.0f;
        //		currentEngineState = 0;
        //	}
        //	)

        //	FB_ATTR(transform_data, worldAnimatedPropellerPivot);
        //	FB_ATTR(transform_data, localAnimatedPropellerPivot);

        //	FB_ATTR(vec4, animatedPropellerPivotRotateAxis);

        //	FB_ATTR(float, rpmToUseFastProp);
        //	FB_ATTR(float, idleRPM);
        //	FB_ATTR(float, maxRPM);
        //	FB_ATTR(float, forceAtMaxRPM);

        //	FB_ATTR(float, rpmToAddPerKTOfSpeed);
        //	FB_ATTR(float, rpmLerpSpeed);
        //	FB_ATTR(float, pitchAtIdleRPM);
        //	FB_ATTR(float, pitchAtMaxRPM);

        //	FB_ATTR(float, currentRPM);
        //	FB_ATTR(float, desiredRPM);
        //	FB_ATTR(float, engineRunVolume);

        //	FB_ATTR(float, thrustMultiplier);
        //	FB_ATTR(float, torqueMultiplier);

        //	FB_ATTR(int, currentEngineState);

        //	FB_ATTR(vec4, thrust);

        //	FB_INHERIT(aircraft_attachment_data);
        //};

        // FB_STRUCT_INHERIT(aircraft_input_data, public base_data)
        //{
        //	FB_ATTR(std::string, name);
        //	FB_ATTR(bool, invert);
        //	FB_ATTR(std::string, axisName);
        //	FB_ATTR(std::string, buttonName);
        // };

        // FB_STRUCT_INHERIT(aircraft_control_surface_data, public aircraft_attachment_data)
        //{
        //	FB_CONSTRUCTOR(
        //		aircraft_control_surface_data()
        //	{
        //		maxDeflectionDegrees = 30.0f;
        //		rootHingeDistanceFromTrailingEdge = 0.25f;
        //		tipHingeDistanceFromTrailingEdge = 0.25f;
        //		currentDeflection = 0.0f;
        //	}
        //	)

        //	FB_ATTR(aircraft_input_data, inputData);
        //	FB_ATTR(float, maxDeflectionDegrees);
        //	FB_ATTR(float, minDeflectionDegrees);
        //	FB_ATTR(float, rootHingeDistanceFromTrailingEdge);
        //	FB_ATTR(float, tipHingeDistanceFromTrailingEdge);
        //	FB_ATTR(float, currentDeflection);

        //	FB_ATTR(float, aoaMultiplier);
        //	FB_ATTR(float, clMultiplier);
        //	FB_ATTR(float, cdMultiplier);
        //	FB_ATTR(float, cmMultiplier);

        //	FB_ATTR(float, stallControlCL);
        //	FB_ATTR(float, stallControlCD);
        //	FB_ATTR(float, stallControlCM);
        //	FB_ATTR(float, stallThreshold);

        //	FB_ARRAY_PROPERTY(float, clLookup);
        //	FB_ARRAY_PROPERTY(float, cdLookup);
        //	FB_ARRAY_PROPERTY(float, cmLookup);

        //	FB_ARRAY_PROPERTY(int, affectedSections);

        //	FB_ATTR(vec4, modelRotationAxis);;

        //	FB_ATTR(int, surfaceId);
        //	FB_ATTR(bool, reverse);

        //	FB_INHERIT(aircraft_attachment_data);
        //};

        // FB_STRUCT_INHERIT(aircraft_prop_wash_data, public aircraft_attachment_data)
        //{
        //	FB_CONSTRUCTOR(
        //		aircraft_prop_wash_data()
        //	{
        //		propwashSource = "";
        //		strength = 0.01f;
        //		maxSpeed = (float)1e10;
        //	}
        //	)

        //	FB_ARRAY_PROPERTY(int, affectedSections);
        //	FB_ARRAY_PROPERTY(float, sectionMultipliers);
        //	FB_ARRAY_PROPERTY(float, throttleMultipliers);
        //	FB_ATTR(std::string, propwashSource);
        //	FB_ATTR(float, strength);
        //	FB_ATTR(float, maxSpeed);

        //	FB_INHERIT(aircraft_attachment_data);
        //};

        // FB_STRUCT_INHERIT(aircraft_ground_effect_data, public aircraft_attachment_data)
        //{
        //	FB_CONSTRUCTOR(
        //		aircraft_ground_effect_data()
        //	{
        //		rayCastAxis = data::vec4(0.0f, -1.0f, 0.0f);
        //		wingspan = 10.0f;
        //	}
        //	)

        //	FB_ATTR(vec4, rayCastAxis);
        //	FB_ATTR(float, wingspan);

        //	FB_INHERIT(aircraft_attachment_data);
        //};

        // FB_STRUCT_INHERIT(aircraft_aerofoil_data, public aircraft_attachment_data)
        //{
        //	FB_CONSTRUCTOR(
        //		aircraft_aerofoil_data()
        //	{
        //	}
        //	)

        //		FB_ATTR(float, IdleRPM);
        //};

        // FB_STRUCT(aircraft_cog_data)
        //{
        //	FB_CONSTRUCTOR(
        //		aircraft_cog_data()
        //	{
        //	}
        //	)

        //		FB_ATTR(float, IdleRPM);
        //};

        // FB_STRUCT(esc_data)
        //{
        //	FB_CONSTRUCTOR(
        //		esc_data()
        //	{
        //		mode = 0;
        //		selectedRPM = 1;
        //		rpm1 = 2000.0f * 0.5f;
        //		rpm2 = 2000.0f * 0.7f;
        //		rpm3 = 2000.0f * 1.0f;
        //	}
        //	)

        //	FB_ATTR(int, mode);
        //	FB_ATTR(int, selectedRPM);
        //	FB_ATTR(float, rpm1);
        //	FB_ATTR(float, rpm2);
        //	FB_ATTR(float, rpm3);
        //};

        FB_STRUCT_INHERIT( vehicle_database_data, public base_data )
        {
            FB_CONSTRUCTOR( vehicle_database_data() { isReferenceModel = false; } )

            FB_ATTR( std::string, dataFolder );
            FB_ATTR( std::string, dataFile );
            FB_ATTR( std::string, fileName );
            FB_ATTR( std::string, label );
            FB_ATTR( std::string, scenePath );
            FB_ATTR( std::string, shadowPath );
            FB_ATTR( std::string, scriptName );
            FB_ATTR( std::string, className );
            FB_ATTR( std::string, initFunction );
            FB_ATTR( std::string, updateFunction );
            FB_ATTR( std::string, modelID );
            FB_ATTR( std::string, refModelID );
            FB_ATTR( std::string, resourceName );
            FB_ATTR( std::string, prefabName );

            FB_ATTR( bool, isReferenceModel );
        };

        FB_STRUCT( vehicle_wheel_data )
        {
            FB_CONSTRUCTOR( vehicle_wheel_data() {
                mass = 1.0f;
                radius = 0.02f;
                wheelDamping = 0.25f;
                suspensionDistance = 0.1f;
                springRate = 10.0f;
                suspensionDamper = 0;
                targetPosition = 0;
                forwardExtremumSlip = 0.4f;
                forwardExtrememValue = 1.0f;
                forwardAsymptoteSlip = 0.8f;
                forwardAsymptoteValue = 0.5f;
                forwardStiffness = 1.0f;
                sidewaysExtremumSlip = 0.2f;
                sidewaysExtrememValue = 1.0f;
                sidewaysAsymptoteSlip = 0.5f;
                sidewaysAsymptoteValue = 0.5f;
                sidewaysStiffness = 1.0f;
                isSteeringWheel = false;
            } )

            FB_ATTR( float, mass );
            FB_ATTR( float, radius );
            FB_ATTR( float, wheelDamping );
            FB_ATTR( float, suspensionDistance );
            FB_ATTR( float, springRate );
            FB_ATTR( float, suspensionDamper );
            FB_ATTR( float, targetPosition );
            FB_ATTR( float, forwardExtremumSlip );
            FB_ATTR( float, forwardExtrememValue );
            FB_ATTR( float, forwardAsymptoteSlip );
            FB_ATTR( float, forwardAsymptoteValue );
            FB_ATTR( float, forwardStiffness );
            FB_ATTR( float, sidewaysExtremumSlip );
            FB_ATTR( float, sidewaysExtrememValue );
            FB_ATTR( float, sidewaysAsymptoteSlip );
            FB_ATTR( float, sidewaysAsymptoteValue );
            FB_ATTR( float, sidewaysStiffness );
            FB_ATTR( bool, isSteeringWheel );
        };

        FB_STRUCT_INHERIT( vehicle_data, public base_data )
        {
            FB_CONSTRUCTOR( vehicle_data() {
                aircraftEnabledAtStart = true;
                overrideInertiaTensor = false;
                inertiaTensor = data::vec4();
                rollwiseDamping = 1.0f;
                sectionMultiplier = 1.0f;
                debugFlags = 0;
            } )

            FB_ATTR( transform_data, localTransform );
            FB_ATTR( transform_data, worldTransform );
            FB_ARRAY_PROPERTY( aircraft_engine_data, engineData );
            FB_ARRAY_PROPERTY( aircraft_wing_data, wingData );
            FB_ARRAY_PROPERTY( aircraft_control_surface_data, controlSurfaceData );
            FB_ARRAY_PROPERTY( aircraft_prop_wash_data, propWashData );
            FB_ARRAY_PROPERTY( aircraft_wheel_data, wheelData );
            FB_ARRAY_PROPERTY( aircraft_ground_effect_data, groundEffectData );
            FB_ATTR( bool, aircraftEnabledAtStart );
            FB_ATTR( bool, overrideInertiaTensor );
            FB_ATTR( vec4, inertiaTensor );
            FB_ATTR( vec4, cgPosition );
            FB_ATTR( vec4, drag );
            FB_ATTR( float, rollwiseDamping );
            FB_ATTR( float, sectionMultiplier );
            FB_ATTR( int, debugFlags );
        };
    }  // end namespace data
}  // end namespace fb
