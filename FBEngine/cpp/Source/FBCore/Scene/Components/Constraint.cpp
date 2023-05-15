#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Constraint.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Constraint, Component );
        const String Constraint::constraints[4] = { "6dof", "6dofSpring", " ", " " };
        const int Constraint::STIFFNESS = StringUtil::getHash( "stiffness" );
        const u32 Constraint::SET_BREAK_FORCE_HASH = StringUtil::getHash( "SET_BREAK_FORCE_HASH" );

        Constraint::~Constraint()
        {
            Constraint::unload( nullptr );
        }

        Vector3<real_Num> Constraint::getRelativeAngles()
        {
            /*
            btTransform b1 = m_body1->getWorldTransform();
            btQuaternion b1r = b1.getRotation();
            btTransform b2 = m_body2->getWorldTransform();
            btQuaternion b2r = b2.getRotation();


            btQuaternion bDelta = b2r - b1r;


            btVector3 result = quatRotate( bDelta, btVector3(0.0f,100.0f,0.0f));
            */

            return Vector3<real_Num>::zero();
        }

        void Constraint::reset( const Vector3<real_Num> &loc )
        {
            try
            {
                makeUnbreakable( true );

                if( isMotor() )
                {
                    auto d6joint = getD6Joint();
                    if( d6joint )
                    {
                        auto pose = Transform3<real_Num>();
                        d6joint->setDrivePosition( pose );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Constraint::restore()
        {
            auto d6joint = getD6Joint();
            if( d6joint )
            {
                auto scene = getScene();

                String constraintType = getConstraintType();
                setConstraintType( constraintType, tLimits, aLimits );

                restoreFlags();
            }
        }

        void Constraint::initialise( const String &objectID )
        {
            // FB_LOG("Constraint: " + objectID);
            // FB_ASSERT(!StringUtil::isNullOrEmpty(objectID));

            // setLoadingState(Object::LoadingState::Loading);

            // auto applicationManager = core::IApplicationManager::instance();
            // auto systemSettings =
            // fb::static_pointer_cast<SystemSettings>(applicationManager->getSystemSettings());

            // auto physicsManager = applicationManager->getPhysicsManager();
            // auto physics = physicsManager->getPhysics();
            // auto database = applicationManager->getDatabase();

            // auto factoryManager = applicationManager->getFactoryManager();
            // auto taskManager = applicationManager->getTaskManager();

            // Task::Lock physicsLock(taskManager->getTask(Thread::Task::Physics));
            // Task::Lock controlsLock(taskManager->getTask(Thread::Task::Controls));

            ////m_makeBreakable = factoryManager->make_ptr<MakeBreakable,
            /// SmartPtr<Constraint>>(this); /m_makeUnbreakable =
            /// factoryManager->make_ptr<MakeUnbreakable, SmartPtr<Constraint>>(this);

            // s32 iObjectId = StringUtil::parseInt(objectID);
            // FB_ASSERT(iObjectId > 0);
            // FB_ASSERT(iObjectId < std::numeric_limits<s32>::max());
            // setObjectID(iObjectId);

            // SmartPtr<Model> model = getModel();
            // if (model)
            //{
            //	if (!model->getEnablePhysics())
            //	{
            //		return;
            //	}
            // }

            ////#if FB_FINAL
            ////		String actor0;
            ////		String actor1;
            ////		String anchor0;
            ////		String anchor1;
            ////		String anchor1_id;
            ////#endif

            // SmartPtr<Rigidbody> rigidBody;
            // m_localFrame0 = Transform3<real_Num>();
            // m_localFrame1 = Transform3<real_Num>();
            // setChannel(-1);
            // f32 drive_force = 0.0f;
            // f32 drive_damping = 0.0f;
            // String drive_axis;
            // m_servo = "none";
            // m_isMotor = false;
            // m_linear = false;
            // m_rotorName = "MainRotor";

            // auto maxBreakForce = std::numeric_limits<f32>::max();
            // setOriginalBreakForce(maxBreakForce);
            // setOriginalBreakTorque(maxBreakForce);

            // m_clf_scenenode = "";
            ////PxTolerancesScale tolerancesScale;

            // try
            //{
            //	auto modelConfiguration = m_model->getModelConfiguration();
            //	String projectionSql =
            //		"select * from refdb.attribs where title = 'ConstraintProjection' and model_id = " +
            //		modelConfiguration->getRefModelID();
            //	auto projectionResult = database->executeQuery(projectionSql);
            //	if (!projectionResult->eof())
            //	{
            //		bool bProjectEnabled =
            // StringUtil::parseBool(projectionResult->getFieldValue("value"));
            //		setEnableProjection(bProjectEnabled);
            //	}

            //	if (objectID != "")
            //	{
            //		auto resultSet = database->executeQuery(
            //			"select * from refdb.object_attributes where object_id = " + objectID);
            //		/*
            //		String scenenode;
            //		Vector3<real_Num> aLimits;
            //		Vector3<real_Num> tLimits;
            //		String actor0;
            //		String actor1;
            //		String anchor0;
            //		String anchor1;
            //		String anchor1_id;
            //		SmartPtr<Rigidbody> rigidBody;
            //		Transform3<real_Num> localFrame0;
            //		Transform3<real_Num> localFrame1;
            //		m_channel = -1;
            //		*/
            //		while (!resultSet->eof())
            //		{
            //			String name = resultSet->getFieldValue("name");
            //			String value = resultSet->getFieldValue("value");

            //			if (!name.compare("scenenode"))
            //			{
            //				sceneNodeName = resultSet->getFieldValue("value");
            //			}
            //			else if (!name.compare("ctype"))
            //			{
            //				m_constraintType = resultSet->getFieldValue("value");
            //			}
            //			else if (!name.compare("actor0"))
            //			{
            //				actor0 = resultSet->getFieldValue("value");
            //			}
            //			else if (!name.compare("actor1"))
            //			{
            //				actor1 = resultSet->getFieldValue("value");
            //			}
            //			else if (!name.compare("anchor0"))
            //			{
            //				anchor0 = resultSet->getFieldValue("value");
            //			}
            //			else if (!name.compare("anchor1"))
            //			{
            //				anchor1 = resultSet->getFieldValue("value");
            //				anchor1_id = resultSet->getFieldValue("id");
            //			}

            //			else if (!name.compare("drive_force"))
            //			{
            //				drive_force = StringUtil::parseFloat(resultSet->getFieldValue("value"));
            //			}
            //			else if (!name.compare("drive_damping"))
            //			{
            //				float driveDamping = StringUtil::parseFloat(value);
            //				drive_damping = driveDamping;
            //			}
            //			else if (!name.compare("drive_axis"))
            //			{
            //				drive_axis = resultSet->getFieldValue("value");
            //			}
            //			else if (!name.compare("channel"))
            //			{
            //				int iChannel = StringUtil::parseInt(resultSet->getFieldValue("value"));
            //				if (iChannel > 0)
            //				{
            //					setChannel(iChannel - 1);
            //					m_motorDirection = 0.83f;
            //				}
            //				else
            //				{
            //					setChannel(-iChannel - 1);
            //					m_motorDirection = -0.83f;
            //				}

            //				setMotor(true);
            //			}
            //			else if (!name.compare("alimits"))
            //			{
            //				String m = resultSet->getFieldValue("value");
            //				String buf; // Have a buffer String
            //				std::stringstream ss(m); // Insert the String into a stream

            //				Array<String> tokens; // Create vector to hold our words

            //				while (ss >> buf)
            //					tokens.push_back(buf);
            //				aLimits = Vector3<real_Num>(StringUtil::parseFloat(tokens[0]),
            //					StringUtil::parseFloat(tokens[1]),
            //					StringUtil::parseFloat(tokens[2]));
            //			}
            //			else if (!name.compare("tlimits"))
            //			{
            //				String m = resultSet->getFieldValue("value");
            //				String buf; // Have a buffer String
            //				std::stringstream ss(m); // Insert the String into a stream

            //				Array<String> tokens; // Create vector to hold our words

            //				while (ss >> buf)
            //					tokens.push_back(buf);
            //				tLimits = Vector3<real_Num>(StringUtil::parseFloat(tokens[0]),
            //					StringUtil::parseFloat(tokens[1]),
            //					StringUtil::parseFloat(tokens[2]));
            //			}
            //			else if (!name.compare("break_force"))
            //			{
            //				f32 fBreakForce = StringUtil::parseFloat(resultSet->getFieldValue("value"));
            //				setOriginalBreakForce(fBreakForce);
            //			}
            //			else if (!name.compare("break_torque"))
            //			{
            //				f32 fBreakTorque = StringUtil::parseFloat(resultSet->getFieldValue("value"));
            //				setOriginalBreakTorque(fBreakTorque);
            //			}
            //			else if (!name.compare("servo"))
            //			{
            //				m_servo = resultSet->getFieldValue("value");
            //				setServoHash(StringUtil::getHash(m_servo));
            //				m_bServoIsNone = getServo().compare("none");
            //			}
            //			else if (!name.compare("linear"))
            //			{
            //				m_linear = resultSet->getFieldValue("value").compare("true") ? false : true;
            //			}
            //			else if (!name.compare("update_on_break"))
            //			{
            //				m_componentUpdateOnBreak = model->getModelComponent(
            //					StringUtil::getHash(resultSet->getFieldValue("value")));
            //			}
            //			else if (!name.compare("effect_name"))
            //			{
            //				m_effectNameHash = StringUtil::getHash(resultSet->getFieldValue("value"));
            //				m_effectNameNightHash = StringUtil::getHash(resultSet->getFieldValue("value")
            //+
            //"_night");
            //			}
            //			else if (!name.compare("clf_scenenode"))
            //			{
            //				String clf_scenenode = resultSet->getFieldValue("value");
            //				m_clf_scenenode = m_model->getSceneNodeName(clf_scenenode);
            //			}
            //			else if (!name.compare("rotor_name"))
            //			{
            //				m_rotorName = resultSet->getFieldValue("value");
            //			}

            //			resultSet->nextRow();
            //		}

            //		//if (anchor0.find("D6.035") != String::npos)
            //		//{
            //		//	int stop = 0;
            //		//	stop = 0;
            //		//}

            //		Vector3<real_Num> mPos = m_model->getPosition();

            //		SmartPtr<IStateNode> sceneNode = m_model->getSceneNode(sceneNodeName);
            //		if (!sceneNode)
            //		{
            //			String messageStr = String("SceneNode not found: ") + sceneNodeName;
            //			FB_LOG(messageStr);
            //			return;
            //		}

            //		SmartPtr<IStateNode> cdnode = m_model->getSceneNode(sceneNodeName);
            //		String cdname = m_model->getSceneNodeName(cdnode->getName());
            //		Vector3<real_Num> location = cdnode->getPosition();
            //		SmartPtr<IStateNode> p = cdnode->getParent();
            //		while (p)
            //		{
            //			p->_update(true, false);

            //			location += p->getPosition();
            //			p = p->getParent();
            //		}

            //		//Vector3<real_Num> snPos = sceneNode->getPosition();
            //		Vector3<real_Num> snPos = location;
            //		Quaternion<real_Num> snQuat = sceneNode->getOrientation();

            //		if (actor0.compare("null"))
            //		{
            //			rigidBody =
            // m_model->getModelComponentByType<Rigidbody>(StringUtil::getHash(actor0));

            //			//int retries = 0;
            //			//while (!rigidBody && retries++ < 30)
            //			//{
            //			//	Thread::sleep(1.0);
            //			//	rigidBody = m_model->getModelComponent(StringUtil::getHash(actor0));
            //			//}

            //			if (!rigidBody)
            //			{
            //				throw Exception("rigid body not found: " + actor0);
            //			}

            //			rigidBody->checkLoaded(true);

            //			rigidBody->setOriginals();
            //			rigidBody->addConstraint(this);
            //			setBodyA(rigidBody);

            //			m_actor0 = rigidBody->getPhysxRigidbody();

            //			SmartPtr<IStateNode> snActor0 = m_model->getSceneNode(actor0);
            //			if (!snActor0)
            //			{
            //				return;
            //			}

            //			/////////////
            //			Vector3<real_Num> location = snActor0->getPosition();

            //			SmartPtr<IStateNode> p = snActor0->getParent();
            //			while (p)
            //			{
            //				p->_update(true, false);

            //				location += p->getPosition();
            //				p = p->getParent();
            //			}

            //			////////
            //			//Vector3<real_Num> posActor0 = snActor0->getPosition();
            //			Vector3<real_Num> posActor0 = location;
            //			//Vector3<real_Num> posActor0 =
            // Vector3<real_Num>(m_actor0->getGlobalPose().p.X(), m_actor0->getGlobalPose().p.Y(),
            // m_actor0->getGlobalPose().p.Z());

            //			m_anchor0 = m_model->getSceneNode(anchor0);
            //			if (!m_anchor0)
            //			{
            //				FB_LOG_ERROR("Error: node not found. Name: " + anchor0);
            //				return;
            //			}

            //			Vector3<real_Num> posAnchor0 = m_anchor0->getPosition() + mPos;

            //			Vector3<real_Num> temp = posAnchor0 - posActor0;

            //			Vector3<real_Num> localPosAnchor0 = snActor0->getOrientation().inverse() * temp;

            //			Quaternion<real_Num> quatA0 = m_anchor0->getOrientation().inverse();
            //			m_localFrame0 = Transform3<real_Num>(
            //				Vector3<real_Num>(localPosAnchor0.X(), localPosAnchor0.Y(),
            // localPosAnchor0.Z()), 				Quaternion<real_Num>(quatA0.X(), quatA0.Y(),
            // quatA0.Z(), quatA0.W()));
            //		}
            //		else
            //		{
            //			// null = world constraint so anchor has to be world frame and ancor = NULL
            //			m_actor0 = NULL;
            //			SmartPtr<IStateNode> snNodeA0 = m_model->getSceneNode(anchor0);
            //			if (snNodeA0)
            //			{
            //				snNodeA0->_update(true, false);

            //				Vector3<real_Num> posA0 = snNodeA0->_getDerivedPosition();
            //				Quaternion<real_Num> quatA0 = snNodeA0->_getDerivedOrientation();
            //				m_localFrame0 = Transform3<real_Num>(
            //					Vector3<real_Num>(posA0.X(), posA0.Y(), posA0.Z()),
            //					Quaternion<real_Num>(quatA0.X(), quatA0.Y(), quatA0.Z(), quatA0.W()));
            //			}
            //		}

            //		/*
            //		render::ISceneNode* snNodeA1 = m_model->getSceneNode(anchor1);
            //		Vector3<real_Num> posA1 = snNodeA1->getPosition() - snPos;
            //		Quaternion<real_Num> quatA1 = snNodeA1->getOrientation();
            //		localFrame1 = Transform3<real_Num>(Vector3<real_Num>(posA1.X(), posA1.Y(),
            // posA1.Z()), Quaternion<real_Num>(quatA1.X(),quatA1.Y(),quatA1.Z(),quatA1.W()));
            //		*/
            //		rigidBody = m_model->getModelComponentByType<Rigidbody>(StringUtil::getHash(actor1));
            //		if (!rigidBody)
            //		{
            //			FB_LOG_ERROR("rigid body not found: " + actor1);
            //			return;
            //		}

            //		if (rigidBody)
            //		{
            //			rigidBody->checkLoaded(true);
            //		}

            //		rigidBody->setOriginals();
            //		//rigidBody->addConstraint(this);
            //		setBodyB(rigidBody);

            //		//render::ISceneNode* m_sceneNode = rigidBody->getSceneNode();
            //		//Ogre::UserObjectBindings& userObject = m_sceneNode->getUserObjectBindings();
            //		//userObject.setUserAny("JOINT_" + anchor1_id  , Ogre::Any(this));

            //		m_actor1 = rigidBody->getPhysxRigidbody();
            //		//FB_ASSERT(m_actor1);
            //		if (!m_actor1)
            //		{
            //			FB_LOG_ERROR("Error: cannot initialise.");
            //			return;
            //		}

            //		Vector3<real_Num> a1Pos = m_actor1->getTransform().getPosition();

            //		SmartPtr<IStateNode> snActor1 = m_model->getSceneNode(actor1);
            //		/////////////
            //		location = snActor1->getPosition();
            //		p = snActor1->getParent();
            //		while (p)
            //		{
            //			p->_update(true, false);

            //			location += p->getPosition();
            //			p = p->getParent();
            //		}

            //		////////
            //		//Vector3<real_Num> posActor1 = snActor1->getPosition();
            //		Vector3<real_Num> posActor1 = location;
            //		//Vector3<real_Num> posActor1 = Vector3<real_Num>(m_actor1->getGlobalPose().p.X(),
            // m_actor1->getGlobalPose().p.Y(), m_actor1->getGlobalPose().p.Z());

            //		setAnchor1(m_model->getSceneNode(anchor1));
            //		if (getAnchor1())
            //		{
            //			Vector3<real_Num> posAnchor1 = getAnchor1()->getPosition() + mPos;

            //			Vector3<real_Num> temp = posAnchor1 - posActor1;

            //			Vector3<real_Num> localPosAnchor1 = snActor1->getOrientation().inverse() * temp;

            //			Quaternion<real_Num> quatA1 = getAnchor1()->getOrientation();

            //			m_localFrame1 = Transform3<real_Num>(
            //				Vector3<real_Num>(localPosAnchor1.X(), localPosAnchor1.Y(),
            // localPosAnchor1.Z()), 				Quaternion<real_Num>(quatA1.X(), quatA1.Y(),
            // quatA1.Z(), quatA1.W()));
            //		}

            //		SmartPtr<physics::IPhysicsScene3> scene = getScene();
            //

            //		if (!isControlComponent())
            //		{
            //			auto name = getName();
            //			if (name.find("Skid") == String::npos)
            //			{
            //				m_d6joint = physics->d6JointCreate(m_actor0, m_localFrame0, m_actor1,
            // m_localFrame1);

            //				if (m_d6joint)
            //				{
            //					m_d6joint->setUserData(this);
            //				}
            //			}
            //			else
            //			{
            //				auto l0 = Transform3<real_Num>();
            //				auto l1 = Transform3<real_Num>();

            //				if (getAnchor0())
            //				{
            //					SmartPtr<IStateNode> snActor0 = m_model->getSceneNode(actor0);

            //					Vector3<real_Num> posAnchor0 = m_anchor0->_getDerivedPosition() + mPos;

            //					Vector3<real_Num> temp = posAnchor0 - snActor0->_getDerivedPosition();

            //					Vector3<real_Num> localPosAnchor0 =
            // snActor0->_getDerivedOrientation().inverse()
            //* temp;

            //					Quaternion<real_Num> quatA0 = Quaternion<real_Num>::identity();//
            // m_anchor0->_getDerivedOrientation().inverse();

            //					m_localFrame0 = Transform3<real_Num>(
            //						Vector3<real_Num>(localPosAnchor0.X(), localPosAnchor0.Y(),
            // localPosAnchor0.Z()), 						Quaternion<real_Num>(quatA0.X(), quatA0.Y(),
            // quatA0.Z(), quatA0.W()));
            //				}

            //				if (getAnchor1())
            //				{
            //					Vector3<real_Num> posAnchor1 = getAnchor1()->_getDerivedPosition() +
            // mPos;

            //					Vector3<real_Num> temp = posAnchor1 - posActor1;

            //					Vector3<real_Num> localPosAnchor1 =
            // snActor1->_getDerivedOrientation().inverse()
            //* temp;

            //					Quaternion<real_Num> quatA1 = m_bodyB->getOrientation();

            //					m_localFrame1 = Transform3<real_Num>(
            //						Vector3<real_Num>(localPosAnchor1.X(), localPosAnchor1.Y(),
            // localPosAnchor1.Z()), 						Quaternion<real_Num>(quatA1.X(), quatA1.Y(),
            // quatA1.Z(), quatA1.W()));
            //				}

            //				m_fixedJoint = physics->fixedJointCreate(m_actor0, m_localFrame0, m_actor1,
            // m_localFrame1); 				m_fixedJoint->setUserData(this);
            //			}
            //		}
            //		else
            //		{
            //			m_d6joint = physics->d6JointCreate(m_actor0, m_localFrame0, m_actor1,
            // m_localFrame1);

            //			if (m_d6joint)
            //			{
            //				m_d6joint->setUserData(this);
            //			}
            //		}

            //		//PxConstraint* userBuffer[5];
            //		//for (int i = 0; i < 5; i++)
            //		//{
            //		//	userBuffer[i] = nullptr;
            //		//}

            //		//u32 bufferSize = 5;
            //		//m_actor1->getConstraints(userBuffer, bufferSize);

            //		auto constraintType = getConstraintType();
            //		setConstraintType(constraintType, tLimits, aLimits);

            //		if (m_fixedJoint)
            //		{
            //			m_localPose0 = m_fixedJoint->getLocalPose(physics::PxJointActorIndex::eACTOR0);
            //			m_localPose1 = m_fixedJoint->getLocalPose(physics::PxJointActorIndex::eACTOR1);
            //			m_fixedJoint->setBreakForce(m_breakForce, m_breakTorque);
            //		}

            //		if (m_d6joint)
            //		{
            //			m_d6joint->setConstraintFlag(physics::PxConstraintFlag::ePROJECTION, false);

            //			//m_d6joint->setProjectionLinearTolerance(0.0001f);
            //			m_d6joint->setProjectionLinearTolerance(0.1f);
            //			m_d6joint->setProjectionAngularTolerance(0.1f);

            //			Transform3<real_Num> position;
            //			if (drive_force != 0.0)
            //			{
            //				auto drive = physics->createConstraintDrive();
            //				drive->setStiffness(drive_force);
            //				drive->setDamping(drive_damping);
            //				drive->setForceLimit(std::numeric_limits<f32>::max());
            //				drive->setIsAcceleration(false);

            //				if (!drive_axis.compare("x"))
            //				{
            //					m_d6joint->setDrive(physics::PxD6Drive::eX, drive);
            //				}
            //				else if (!drive_axis.compare("y"))
            //				{
            //					m_d6joint->setDrive(physics::PxD6Drive::eY, drive);
            //				}
            //				else if (!drive_axis.compare("z"))
            //				{
            //					m_d6joint->setDrive(physics::PxD6Drive::eZ, drive);
            //				}
            //			}

            //			m_d6joint->setBreakForce(m_breakForce, m_breakTorque);
            //			position = m_d6joint->getDrivePosition();

            //			if (m_isMotor)
            //			{
            //				auto drive = physics->createConstraintDrive();
            //				drive->setStiffness(22250.0f);
            //				drive->setDamping(0.0f);
            //				drive->setForceLimit(std::numeric_limits<f32>::max());
            //				drive->setIsAcceleration(false);

            //				if (!m_linear)
            //				{
            //					m_d6joint->setDrive(physics::PxD6Drive::eTWIST, drive);
            //				}
            //				else
            //				{
            //					m_d6joint->setDrive(physics::PxD6Drive::eX, drive);
            //				}
            //			}

            //			if (!anchor1.compare("X_lft") || !anchor1.compare("X_rght"))
            //			{
            //				m_d6joint->setConstraintFlag(physics::PxConstraintFlag::eREPORTING, true);
            //				m_reportForce = true;
            //			}

            //			m_d6joint->setConstraintFlag(physics::PxConstraintFlag::eVISUALIZATION, true);
            //			setFlag(true);

            //			m_localPose0 = m_d6joint->getLocalPose(physics::PxJointActorIndex::eACTOR0);
            //			m_localPose1 = m_d6joint->getLocalPose(physics::PxJointActorIndex::eACTOR1);
            //		}
            //	}

            //	auto modelType = m_model->getModelType();
            //	if (modelType == Model::E_MODEL_TYPE_HELICOPTER)
            //	{
            //		setupRotor();
            //	}

            //	setLoadingState(Object::LoadingState::Loaded);
            //}
            // catch (std::exception& e)
            //{
            //	FB_LOG_EXCEPTION(e);
            //}
        }

        void Constraint::setupRotor()
        {
            // if (!getRotor())
            //{
            //	String rotorName = getRotorName();
            //	if (!StringUtil::isNullOrEmpty(rotorName))
            //	{
            //		const int rotorHash = StringUtil::getHash(rotorName);
            //		SmartPtr<Rotor> rotor = m_model->getModelComponentByType<Rotor>(rotorHash);
            //		if (rotor)
            //		{
            //			setRotor(rotor);
            //		}
            //		else
            //		{
            //			FB_LOG_ERROR("Rotor not found: " + rotorName);
            //		}
            //	}

            //	if (getChannel() == 99)
            //	{
            //		SmartPtr<Rotor> rotor = getRotor();
            //		if (rotor)
            //		{
            //			//rotor->setRotatingGraphicsName(sceneNodeName);
            //		}
            //	}
            //}
        }

        void Constraint::setConstraintType( const String &constraintType, Vector3<real_Num> &tLimits,
                                            Vector3<real_Num> &aLimits )
        {
            // using namespace physics;

            // FB_ASSERT(!StringUtil::isNullOrEmpty(constraintType));
            // FB_ASSERT(MathUtil<real_Num>::isFinite(tLimits));
            // FB_ASSERT(MathUtil<real_Num>::isFinite(aLimits));

            // auto d6joint = getD6Joint();
            // if (d6joint)
            //{
            //	//	PxTolerancesScale tolerancesScale;

            //	char transX = constraintType[0];
            //	char transY = constraintType[1];
            //	char transZ = constraintType[2];

            //	char rotX = constraintType[3];
            //	char rotY = constraintType[4];
            //	char rotZ = constraintType[5];

            //	--
            //	switch (constraintType[0])
            //	{
            //	case 'L':
            //		d6joint->setMotion(physics::PxD6Axis::eX, PxD6Motion::eLOCKED);
            //		break;
            //	case 'F':
            //		d6joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
            //		break;
            //	case 'R':
            //	{
            //		d6joint->setMotion(PxD6Axis::eX, PxD6Motion::eLIMITED);
            //		//d6joint->setLinearLimit(PxJointLinearLimit(tolerancesScale, tLimits.x, tLimits.y));
            //	}
            //	break;
            //	}

            //	//	--
            //	//	switch (constraintType[1])
            //	//	{
            //	//	case 'L':
            //	//		d6joint->setMotion(PxD6Axis::eY, PxD6Motion::eLOCKED);
            //	//		break;
            //	//	case 'F':
            //	//		d6joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
            //	//		break;
            //	//	case 'R':
            //	//		d6joint->setMotion(PxD6Axis::eY, PxD6Motion::eLIMITED);
            //	//		d6joint->setLinearLimit(PxJointLinearLimit(tolerancesScale, tLimits.x,
            // tLimits.y));
            //	//		break;
            //	//	}
            //	//	--
            //	//	switch (constraintType[2])
            //	//	{
            //	//	case 'L':
            //	//		d6joint->setMotion(PxD6Axis::eZ, PxD6Motion::eLOCKED);
            //	//		break;
            //	//	case 'F':
            //	//		d6joint->setMotion(PxD6Axis::eZ, PxD6Motion::eFREE);
            //	//		break;
            //	//	case 'R':
            //	//		d6joint->setMotion(PxD6Axis::eZ, PxD6Motion::eLIMITED);
            //	//		d6joint->setLinearLimit(PxJointLinearLimit(tolerancesScale, tLimits.x,
            // tLimits.y));
            //	//		break;
            //	//	}

            //	//	--
            //	//	--

            //	//	switch (constraintType[3])
            //	//	{
            //	//	case 'L':
            //	//		d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
            //	//		break;
            //	//	case 'F':
            //	//		d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//		break;
            //	//	case 'R':
            //	//	{
            //	//		PxJointAngularLimitPair twist_limit(aLimits.x, aLimits.y, aLimits.z);
            //	//		if (twist_limit.isValid())
            //	//		{
            //	//			d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLIMITED);
            //	//			d6joint->setTwistLimit(twist_limit);
            //	//		}
            //	//		else
            //	//		{
            //	//			d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//			FB_LOG_ERROR(getName() + ": Twist limits not valid. Model: " +
            // m_model->getName());
            //	//		}
            //	//	}
            //	//	break;
            //	//	}

            //	//	switch (constraintType[4])
            //	//	{
            //	//	case 'L':
            //	//		d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
            //	//		break;
            //	//	case 'F':
            //	//		d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//		break;
            //	//	case 'R':
            //	//	{
            //	//		if (aLimits.x < 0.00001f)
            //	//			aLimits.x = 0.00001f;

            //	//		if (aLimits.y < 0.00001f)
            //	//			aLimits.y = 0.00001f;

            //	//		if (aLimits.z < 0.00001f)
            //	//			aLimits.z = 0.00001f;

            //	//		PxJointLimitCone swing_limit(aLimits.x, aLimits.y, aLimits.z);
            //	//		if (swing_limit.isValid())
            //	//		{
            //	//			d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLIMITED);
            //	//			d6joint->setSwingLimit(swing_limit);
            //	//		}
            //	//		else
            //	//		{
            //	//			d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//			FB_LOG_ERROR(getName() + ": SWING1 limits not valid. Model: " +
            // m_model->getName());
            //	//		}
            //	//	}
            //	//	break;
            //	//	}
            //	//	switch (constraintType[5])
            //	//	{
            //	//	case 'L':
            //	//		d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
            //	//		break;
            //	//	case 'F':
            //	//		d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
            //	//		break;
            //	//	case 'R':
            //	//	{
            //	//		if (aLimits.x < 0.00001f)
            //	//			aLimits.x = 0.00001f;

            //	//		if (aLimits.y < 0.00001f)
            //	//			aLimits.y = 0.00001f;

            //	//		if (aLimits.z < 0.00001f)
            //	//			aLimits.z = 0.00001f;

            //	//		PxJointLimitCone swing_limit(aLimits.x, aLimits.y, aLimits.z);
            //	//		if (swing_limit.isValid())
            //	//		{
            //	//			d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLIMITED);
            //	//			d6joint->setSwingLimit(swing_limit);
            //	//		}
            //	//		else
            //	//		{
            //	//			d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
            //	//			FB_LOG_ERROR(getName() + ": SWING2 limits not valid. Model: " +
            // m_model->getName());
            //	//		}
            //	//	}
            //	//	break;
            //	//	}

            //	//	/////////////////////////////////////////////////////////

            //	//	--
            //	//	/*
            //	//	if(!m_constraintType.compare("LLLFLL"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("LLLRLL"))  //
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLIMITED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);

            //	//	PxJointLimitPair twist_limit(-1.0, 1.0, 1.0);
            //	//	d6joint->setTwistLimit(twist_limit);

            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("LLLFFF"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("FLLLFF"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("LLLLLL"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);

            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("FLLFLL"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
            //	//	}

            //	//	else if(!m_constraintType.compare("RLLLRR")) // not used on trex
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLIMITED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLIMITED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLIMITED);
            //	//	d6joint->setLinearLimit(PxJointLimit(0.01f, 0.01f));
            //	//	PxJointLimitCone swing_limit(0.25, 0.25, 0.02);
            //	//	d6joint->setSwingLimit(swing_limit);

            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("FLLFFF"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("LLLFRR")) //
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLIMITED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLIMITED);

            //	//	PxJointLimitCone swing_limit(0.15, 0.15, 0.02);
            //	//	d6joint->setSwingLimit(swing_limit);
            //	//	}

            //	//	--
            //	//	else if(!m_constraintType.compare("LFLLLL"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("LFLFLL"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
            //	//	}

            //	//	--
            //	//	else if(!m_constraintType.compare("LRLLLL")) //
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLIMITED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
            //	//	d6joint->setLinearLimit(PxJointLimit(0.0075f, 0.0075f));
            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("FRLFFF"))  //
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLIMITED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
            //	//	d6joint->setLinearLimit(PxJointLimit(0.001f, 0.001f));
            //	//	}
            //	//	--
            //	//	else if(!m_constraintType.compare("FLFFFF"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
            //	//	}

            //	//	else if(!m_constraintType.compare("LLLRFF"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLIMITED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
            //	//	PxJointLimitPair twist_limit(-0.5, 0.5, 0.5);
            //	//	d6joint->setTwistLimit(twist_limit);

            //	//	}
            //	//	else if(!m_constraintType.compare("LLLLFF"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

            //	//	}

            //	//	else if(!m_constraintType.compare("FFLFFF"))
            //	//	{
            //	//	d6joint->setMotion(PxD6Axis::eX,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eY,     PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eZ,     PxD6Motion::eLOCKED);
            //	//	d6joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
            //	//	d6joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

            //	//	}
            //	//	*/
            //}
        }

        void Constraint::forceBreak()
        {
            // SmartPtr<IConstraintD6> d6joint = getD6Joint();
            // if (d6joint)
            //{
            //	d6joint->release();
            //	d6joint = nullptr;
            // }
        }

        void Constraint::resetPositions()
        {
            auto localPose0 = getLocalPose0();
            auto localPose1 = getLocalPose1();

            auto d6joint = getD6Joint();
            if( d6joint )
            {
                d6joint->setLocalPose( physics::PxJointActorIndex::eACTOR0, localPose0 );
                d6joint->setLocalPose( physics::PxJointActorIndex::eACTOR1, localPose1 );
            }

            auto fixedJoint = getFixedJoint();
            if( fixedJoint )
            {
                fixedJoint->setLocalPose( physics::PxJointActorIndex::eACTOR0, localPose0 );
                fixedJoint->setLocalPose( physics::PxJointActorIndex::eACTOR1, localPose1 );
            }
        }

        void Constraint::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                auto bodyA = getBodyA();
                auto bodyB = getBodyB();

                Transform3<real_Num> transformA;
                Transform3<real_Num> transformB;

                m_positionJoint = physicsManager->d6JointCreate(
                    bodyA ? bodyA->getRigidDynamic() : nullptr, transformA,
                    bodyB ? bodyB->getRigidDynamic() : nullptr, transformB );

                m_positionJoint->setMotion( physics::PxD6Axis::eX, physics::PxD6Motion::eLOCKED );
                m_positionJoint->setMotion( physics::PxD6Axis::eY, physics::PxD6Motion::eFREE );
                m_positionJoint->setMotion( physics::PxD6Axis::eZ, physics::PxD6Motion::eLOCKED );

                m_positionJoint->setMotion( physics::PxD6Axis::eSWING1, physics::PxD6Motion::eLOCKED );
                m_positionJoint->setMotion( physics::PxD6Axis::eSWING2, physics::PxD6Motion::eLOCKED );
                m_positionJoint->setMotion( physics::PxD6Axis::eTWIST, physics::PxD6Motion::eFREE );

                constexpr auto maxForce = std::numeric_limits<f32>::max();
                m_positionJoint->setBreakForce( maxForce, maxForce );

                m_positionJoint->setConstraintFlag( physics::PxConstraintFlag::eVISUALIZATION, true );

                m_bodyA = bodyA;
                m_bodyB = bodyB;

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Constraint::reload( SmartPtr<ISharedObject> data )
        {
            try
            {
                unload( data );
                load( data );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Constraint::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                auto taskManager = applicationManager->getTaskManager();
                if( !taskManager )
                {
                    return;
                }

                auto scene = getScene();

                if( m_d6joint )
                {
                    m_d6joint->unload( nullptr );
                    m_d6joint = nullptr;
                }

                if( m_positionJoint )
                {
                    m_positionJoint->unload( nullptr );
                    m_positionJoint = nullptr;
                }

                if( m_fixedJoint )
                {
                    m_fixedJoint->unload( nullptr );
                    m_fixedJoint = nullptr;
                }

                if( m_bodyA )
                {
                    // m_bodyA->removeConstraint(this);
                    m_bodyA = nullptr;
                }

                if( m_bodyB )
                {
                    // m_bodyB->removeConstraint(this);
                    m_bodyB = nullptr;
                }

                m_actor0 = nullptr;
                m_actor1 = nullptr;
                m_d6joint = nullptr;
                m_positionJoint = nullptr;
                m_fixedJoint = nullptr;
                m_bodyA = nullptr;
                m_bodyB = nullptr;

                m_motorDirection = 0;
                m_rotate = 0;
                m_pos = 0;
                m_breakForce = 0;
                m_breakTorque = 0;

                setChannel( -1 );
                m_effectNameHash = 0;

                m_isMotor = false;
                setFlag( false );
                m_linear = false;
                m_bServoIsNone = true;

                m_makeBreakable = nullptr;
                m_makeUnbreakable = nullptr;

                Component::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Constraint::setupDrive()
        {
            Transform3<real_Num> position;
            // if (m_driveForce != 0.0)
            //{
            //	PxD6JointDrive drive(m_driveForce, m_driveDamping, PX_MAX_F32, false); // PX_MAX_F32
            //	if (!drive_axis.compare("x"))
            //	{
            //		m_d6joint->setDrive(PxD6Drive::eX, drive);
            //	}
            //	else if (!drive_axis.compare("y"))
            //	{
            //		m_d6joint->setDrive(PxD6Drive::eY, drive);
            //	}
            //	else if (!drive_axis.compare("z"))
            //	{
            //		m_d6joint->setDrive(PxD6Drive::eZ, drive);
            //	}
            // }

            position = m_d6joint->getDrivePosition();

            // if (m_isMotor)
            //{
            //	PxD6JointDrive drive(22250.0f, 0.0f, PX_MAX_F32, false);

            //	if (!m_linear)
            //	{
            //		m_d6joint->setDrive(PxD6Drive::eTWIST, drive);
            //	}
            //	else
            //	{
            //		m_d6joint->setDrive(PxD6Drive::eX, drive);
            //	}
            //}
        }

        void Constraint::awake()
        {
        }

        void Constraint::play()
        {
            reload( nullptr );
        }

        SmartPtr<Properties> Constraint::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();

            auto mass = 0.0f;
            properties->setProperty( "mass", mass );

            auto bodyHashA = 0;
            properties->setProperty( "bodyA", bodyHashA );

            auto bodyHashB = 0;
            properties->setProperty( "bodyB", bodyHashB );

            return properties;
        }

        void Constraint::setProperties( SmartPtr<Properties> properties )
        {
        }

        Array<SmartPtr<ISharedObject>> Constraint::getChildObjects() const
        {
            auto objects = Component::getChildObjects();

            if( m_bodyA )
            {
                objects.push_back( m_bodyA );
            }

            if( m_bodyB )
            {
                objects.push_back( m_bodyB );
            }

            return objects;
        }

        Constraint::Type Constraint::getType() const
        {
            return m_type;
        }

        void Constraint::setType( Type val )
        {
            m_type = val;
        }

        void Constraint::create( SmartPtr<Rigidbody> bodyA, SmartPtr<Rigidbody> bodyB,
                                 const Transform3<real_Num> &transformA,
                                 const Transform3<real_Num> &transformB )
        {
            return;

            auto applicationManager = core::IApplicationManager::instance();
            auto physics = applicationManager->getPhysicsManager();

            SmartPtr<physics::IPhysicsBody3> pBodyA;
            SmartPtr<physics::IPhysicsBody3> pBodyB;

            Transform3<real_Num> tA;
            Transform3<real_Num> tB;

            // if (bodyA)
            //{
            //	pBodyA = bodyA->getRigidActor();
            // }

            // if (bodyB)
            //{
            //	pBodyB = bodyB->getRigidActor();
            // }

            // m_fixedJoint = physics->fixedJointCreate(pBodyA, tA, pBodyB, tB);

            //*applicationManager->getPhysics(),
            //	bodyA ? bodyA->getPhysxRigidbody() : NULL, PxTransform::createIdentity(),
            //	bodyB ? bodyB->getPhysxRigidbody() : NULL, PxTransform::createIdentity());

            // m_fixedJoint->setConstraintFlag(physics::PxConstraintFlag::eCOLLISION_ENABLED, false);

            m_positionJoint =
                physics->d6JointCreate( bodyA ? bodyA->getRigidDynamic() : nullptr, transformA,
                                        bodyB ? bodyB->getRigidDynamic() : nullptr, transformB );

            m_positionJoint->setMotion( physics::PxD6Axis::eX, physics::PxD6Motion::eLOCKED );
            m_positionJoint->setMotion( physics::PxD6Axis::eY, physics::PxD6Motion::eFREE );
            m_positionJoint->setMotion( physics::PxD6Axis::eZ, physics::PxD6Motion::eLOCKED );

            m_positionJoint->setMotion( physics::PxD6Axis::eSWING1, physics::PxD6Motion::eLOCKED );
            m_positionJoint->setMotion( physics::PxD6Axis::eSWING2, physics::PxD6Motion::eLOCKED );
            m_positionJoint->setMotion( physics::PxD6Axis::eTWIST, physics::PxD6Motion::eFREE );

            auto maxForce = std::numeric_limits<f32>::max();
            m_positionJoint->setBreakForce( maxForce, maxForce );

            m_positionJoint->setConstraintFlag( physics::PxConstraintFlag::eVISUALIZATION, true );

            m_bodyA = bodyA;
            m_bodyB = bodyB;

            // getFSM()->setCurrentState(Modelscene::MODEL_COMPONENT_STATE_READY);
            // getFSM()->setNewState(Modelscene::MODEL_COMPONENT_STATE_READY);
        }

        void Constraint::setupSuspension( SmartPtr<Rigidbody> bodyA, SmartPtr<Rigidbody> bodyB )
        {
            // auto applicationManager = core::IApplicationManager::instance();

            ////m_fixedJoint = PxFixedJointCreate(*applicationManager->getPhysics(),
            ////	bodyA ? bodyA->getPhysxRigidbody() : NULL, PxTransform::createIdentity(),
            ////	bodyB ? bodyB->getPhysxRigidbody() : NULL, PxTransform::createIdentity());

            ////m_fixedJoint->setConstraintFlag(PxConstraintFlag::eCOLLISION_ENABLED, false);

            // Transform3<real_Num> transformA = Transform3<real_Num>();
            // Transform3<real_Num> transformB = Transform3<real_Num>();

            // if (bodyA)
            //{
            //	transformA = bodyA->getGlobalPose();
            // }

            // if (bodyB)
            //{
            //	transformB = bodyB->getGlobalPose();
            //	//transformB = transformB.getInverse();
            // }

            // SmartPtr<IPhysicsManager3> physics =
            // applicationManager->getPhysicsManager()->getPhysics(); m_positionJoint =
            // physics->PxD6JointCreate( 	bodyA ? bodyA->getPhysxRigidbody() : nullptr, 	transformA,
            // bodyB
            //? bodyB->getPhysxRigidbody() : nullptr, 	transformB);

            // m_positionJoint->setMotion(PxD6Axis::eX, PxD6Motion::eLOCKED);
            // m_positionJoint->setMotion(PxD6Axis::eY, PxD6Motion::eLOCKED);
            // m_positionJoint->setMotion(PxD6Axis::eZ, PxD6Motion::eLOCKED);

            // m_positionJoint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
            // m_positionJoint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
            // m_positionJoint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);

            // m_positionJoint->setBreakForce(PX_MAX_F32, PX_MAX_F32);

            // m_positionJoint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);

            // m_bodyA = bodyA;
            // m_bodyB = bodyB;

            // getFSM()->setCurrentState(Modelscene::MODEL_COMPONENT_STATE_READY);
            // getFSM()->setNewState(Modelscene::MODEL_COMPONENT_STATE_READY);
        }

        void Constraint::initialise( SmartPtr<Rigidbody> bodyA, SmartPtr<Rigidbody> bodyB )
        {
            // auto applicationManager = core::IApplicationManager::instance();

            // m_fixedJoint = PxFixedJointCreate(*applicationManager->getPhysics(),
            //	bodyA ? bodyA->getPhysxRigidbody() : NULL, PxTransform::createIdentity(),
            //	bodyB ? bodyB->getPhysxRigidbody() : NULL, PxTransform::createIdentity());

            // m_fixedJoint->setConstraintFlag(PxConstraintFlag::eCOLLISION_ENABLED, false);

            // Transform3<real_Num> transformA = Transform3<real_Num>();
            // Transform3<real_Num> transformB = Transform3<real_Num>();

            // if (bodyA)
            //{
            //	transformA = bodyA->getGlobalPose();
            // }

            // if (bodyB)
            //{
            //	transformB = bodyB->getGlobalPose();
            //	transformB = transformB.getInverse();
            // }

            // auto physics = applicationManager->getPhysicsManager()->getPhysics();
            // m_positionJoint = physics->d6JointCreate(
            //	bodyA ? bodyA->getPhysxRigidbody() : NULL,
            //	transformA,
            //	bodyB ? bodyB->getPhysxRigidbody() : NULL,
            //	transformB);

            // m_positionJoint->setMotion(physics::PxD6Axis::eX, physics::PxD6Motion::eLOCKED);
            // m_positionJoint->setMotion(physics::PxD6Axis::eY, physics::PxD6Motion::eFREE);
            // m_positionJoint->setMotion(physics::PxD6Axis::eZ, physics::PxD6Motion::eLOCKED);

            // m_positionJoint->setMotion(physics::PxD6Axis::eSWING1, physics::PxD6Motion::eLOCKED);
            // m_positionJoint->setMotion(physics::PxD6Axis::eSWING2, physics::PxD6Motion::eLOCKED);
            // m_positionJoint->setMotion(physics::PxD6Axis::eTWIST, physics::PxD6Motion::eFREE);

            // auto breakForce = std::numeric_limits<f32>::max();
            // m_positionJoint->setBreakForce(breakForce, breakForce);

            // m_positionJoint->setConstraintFlag(physics::PxConstraintFlag::eVISUALIZATION, true);

            // m_bodyA = bodyA;
            // m_bodyB = bodyB;

            // getFSM()->setCurrentState(Modelscene::MODEL_COMPONENT_STATE_READY);
            // getFSM()->setNewState(Modelscene::MODEL_COMPONENT_STATE_READY);
        }

        int Constraint::lookup( String shape )
        {
            // const int available = sizeof(constraints) / sizeof(String);

            // for (int i = 0; i != available; i++)
            //{
            //	if (shape == constraints[i])
            //	{
            //		return i;
            //	}
            // }
            // return Rigidbody::ITEM_NONE;

            return 0;
        }

        void Constraint::preUpdate()
        {
            //			Modelscene::preUpdate();
            //
            //			switch (task)
            //			{
            //			case Thread::Task::Primary:
            //			{
            //			}
            //			break;
            //			case Thread::Task::Physics:
            //			{
            //				auto applicationManager = core::IApplicationManager::instance();
            //				auto systemSettings =
            // fb::static_pointer_cast<SystemSettings>(applicationManager->getSystemSettings());
            //				SmartPtr<PhysicsManager>& physicsManager =
            // applicationManager->getPhysicsManager();
            //
            //				if (systemSettings->isSceneryLoading())
            //				{
            //					makeUnbreakable(true);
            //				}
            //
            //				SmartPtr<IFSM>& fsm = getFSM();
            //				if (fsm)
            //				{
            //					Modelscene::ModelComponentStates state =
            //(Modelscene::ModelComponentStates)fsm-> 						getCurrentState();
            // switch (state)
            //					{
            //					case Modelscene::MODEL_COMPONENT_STATE_FLIGHT:
            //					{
            //#if FB_BUILD_EDITOR_PLUGIN
            //						updateDriveEditor();
            //#else
            //						updateDrive();
            //#endif
            //
            //						//if (m_d6joint != nullptr && m_reportForce)
            //						//{
            //						//	Vector3<real_Num> force, torque;
            //						//	m_d6joint->getConstraint()->getForce(force, torque);
            //						//	m_model->setActualMass(force.magnitude() / 9.81f);
            //						//	//printf("Mass: %f\n", -force.y / 9.81f);
            //						//}
            //					}
            //					break;
            //					case Modelscene::(u32)VehicleComponent::VehicleComponentState::Workbench:
            //					{
            //#if FB_BUILD_EDITOR_PLUGIN
            //						updateDriveEditor();
            //#else
            //						updateDrive();
            //#endif
            //
            //						//if (m_d6joint != nullptr && m_reportForce)
            //						//{
            //						//	Vector3<real_Num> force, torque;
            //						//	m_d6joint->getConstraint()->getForce(force, torque);
            //						//	m_model->setActualMass(force.magnitude() / 9.81f);
            //						//	//printf("Mass: %f\n", -force.y / 9.81f);
            //						//}
            //					}
            //					break;
            //					default:
            //					{
            //					}
            //					}
            //				}
            //			}
            //			break;
            //			case Thread::Task::Controls:
            //			{
            //				SmartPtr<IFSM>& fsm = getFSM();
            //				if (fsm)
            //				{
            //					Modelscene::ModelComponentStates state =
            //(Modelscene::ModelComponentStates)fsm-> 						getCurrentState();
            // switch (state)
            //					{
            //					case Modelscene::MODEL_COMPONENT_STATE_FLIGHT:
            //					case Modelscene::(u32)VehicleComponent::VehicleComponentState::Workbench:
            //					{
            //#if FB_BUILD_EDITOR_PLUGIN
            //						if (isControlComponent())
            //						{
            //							if (m_servoCached)
            //							{
            //								if (!m_servoCached->isMeasuring())
            //								{
            //									if (m_linear)
            //									{
            //										//Transform3<real_Num> OutputPose;
            //										//OutputPose.p =
            // Vector3<real_Num>((f32)m_servoCached->getCurrentPos(), 0.0, 0.0);
            //
            //										//if (OutputPose.isSane())
            //										//{
            //										//	if (m_d6joint)
            //										//	{
            //										//		m_d6joint->setDrivePosition(OutputPose);
            //										//	}
            //										//}
            //									}
            //									else
            //									{
            //										QuaternionD orietation;
            //										orietation.fromAngleAxis(m_servoCached->getCurrentPos(),
            // Vector3D::UNIT_X);
            //
            //										Quaternion<real_Num>
            // poseOrientation((f32)orietation.X(), (f32)orietation.Y(), (f32)orietation.Z(),
            //(f32)orietation.W());
            //										//Transform3<real_Num> t =
            // Transform3<real_Num>(poseOrientation);
            //										//if (t.isSane())
            //										//{
            //										//	if (m_d6joint)
            //										//	{
            //										//		m_d6joint->setDrivePosition(t);
            //										//	}
            //										//}
            //									}
            //								}
            //							}
            //						}
            //#else
            //						if (isControlComponent())
            //						{
            //							if (m_servoCached)
            //							{
            //								if (m_linear)
            //								{
            //									Transform3<real_Num> OutputPose;
            //									OutputPose.setPosition(Vector3<real_Num>((f32)m_servoCached->getCurrentPos(),
            // 0.0, 0.0));
            //
            //									if (OutputPose.isSane())
            //									{
            //										if (m_d6joint)
            //										{
            //											m_d6joint->setDrivePosition(OutputPose);
            //										}
            //									}
            //								}
            //								else
            //								{
            //									QuaternionD orietation;
            //									orietation.fromAngleAxis(m_servoCached->getCurrentPos(),
            // Vector3D::UNIT_X);
            //
            //									Quaternion<real_Num> poseOrientation((f32)orietation.X(),
            //(f32)orietation.Y(), (f32)orietation.Z(), (f32)orietation.W());
            // Transform3<real_Num> t =
            // Transform3<real_Num>(poseOrientation); 									if (t.isSane())
            //									{
            //										if (m_d6joint)
            //										{
            //											m_d6joint->setDrivePosition(t);
            //										}
            //									}
            //								}
            //							}
            //						}
            //#endif
            //					};
            //					}
            //				}
            //			}
            //			break;
            //			default:
            //			{
            //			}
            //			};
        }

        void Constraint::setDrivePosition( float position )
        {
            // FB_ASSERT(MathF::isFinite(position));
            // FB_ASSERT(Thread::getCurrentTask() != Thread::Task::Primary);

            // SmartPtr<physics::IPhysicsScene3> scene = getScene();
            //

            // float rotate = 0.005f;
            // Quaternion<real_Num> orietation;
            // orietation.fromAngleAxis(rotate, Vector3<real_Num>::UNIT_X);

            // if (m_actor0)
            //{
            //	m_actor0->wakeUp();
            // }

            // if (m_actor1)
            //{
            //	m_actor1->wakeUp();
            // }

            // Transform3<real_Num> pose = Transform3<real_Num>(
            //	Quaternion<real_Num>(orietation.X(), orietation.Y(), orietation.Z(), orietation.W()));
            // m_d6joint->setDrivePosition(pose);
        }

        void Constraint::setBodyA( SmartPtr<Rigidbody> val )
        {
            m_bodyA = val;
        }

        void Constraint::setBodyB( SmartPtr<Rigidbody> val )
        {
            m_bodyB = val;
        }

        void Constraint::setBreakForce( float force, float torque )
        {
            // if (!isControlComponent())
            //{
            //	Thread::Task task = (Thread::Task)Thread::getCurrentTask();
            //	if (task != Thread::Task::Physics)
            //	{
            //		SmartPtr<IPhysicsScene3> scene = getScene();
            //

            //		SmartPtr<IConstraintD6> d6joint = getD6Joint();
            //		if (d6joint)
            //		{
            //			d6joint->setBreakForce(force, torque);
            //		}

            //		SmartPtr<IConstraintFixed3> fixedJoint = getFixedJoint();
            //		if (fixedJoint)
            //		{
            //			fixedJoint->setBreakForce(force, torque);
            //		}
            //	}
            //	else
            //	{
            //		SmartPtr<IConstraintD6> d6joint = getD6Joint();
            //		if (d6joint)
            //		{
            //			d6joint->setBreakForce(force, torque);
            //		}

            //		SmartPtr<IConstraintFixed3> fixedJoint = getFixedJoint();
            //		if (fixedJoint)
            //		{
            //			fixedJoint->setBreakForce(force, torque);
            //		}
            //	}
            //}
        }

        void Constraint::enterFlightState()
        {
            try
            {
                auto actor = getActor();
                if( actor )
                {
                    // actor->unregisterAllComponent(this);

                    if( !isControlComponent() )
                    {
                        // if (isMotor())
                        {
                            // actor->registerComponentUpdate(Thread::Task::Physics,
                            // Thread::UpdateState::PreUpdate, this);
                            // actor->registerComponentUpdate(Tasks::TaskIds::TASK_ID_PHYSICS,
                            //	Thread::UpdateState::Update, this);
                            // actor->registerComponentUpdate(Tasks::TaskIds::TASK_ID_PHYSICS,
                            // Thread::UpdateState::PostUpdate, this);
                        }
                    }
                    else
                    {
                        if( isMotor() )
                        {
                            // actor->registerComponentUpdate(Thread::Task::Physics,
                            // Thread::UpdateState::PreUpdate, this);
                            // actor->registerComponentUpdate(Tasks::TaskIds::TASK_ID_PHYSICS,
                            //	Thread::UpdateState::Update, this);
                            // actor->registerComponentUpdate(Tasks::TaskIds::TASK_ID_PHYSICS,
                            // Thread::UpdateState::PostUpdate, this);

                            // actor->registerComponentUpdate(Thread::Task::Controls,
                            // Thread::UpdateState::PreUpdate, this);
                            // actor->registerComponentUpdate(Tasks::TaskIds::TASK_ID_CONTROLS,
                            //	Thread::UpdateState::Update, this);
                            // actor->registerComponentUpdate(Tasks::TaskIds::TASK_ID_CONTROLS,
                            // Thread::UpdateState::PostUpdate, this);
                        }
                    }
                }

                // auto applicationManager = core::IApplicationManager::instance();
                // auto& jobQueue = applicationManager->getJobQueue();
                // auto& taskManager = applicationManager->getTaskManager();
                // auto factoryManager = applicationManager->getFactoryManager();
                // auto& physicsTask = taskManager->getTask(Thread::Task::Physics);
                // auto& controlsTask = taskManager->getTask(Thread::Task::Controls);

                // Task::Lock physicsLock(physicsTask);
                // Task::Lock controlsLock(controlsTask);

                // if (!m_servoCached)
                //{
                //	s32 iServoHash = getServoHash();
                //	if (iServoHash != 0)
                //	{
                //		m_servoCached = m_model->getModelComponentByType<Servo>(iServoHash);
                //		if (m_servoCached)
                //		{
                //			//m_servoCached->setConstraint(this);
                //		}
                //	}
                // }

                resetPositions();
                makeUnbreakable( true );
                setBrokenState( false );

                String servoName = getServo();
                m_servoNoneCompare = servoName.compare( "none" );

                // if (getEnableProjection())
                //{
                //	SmartPtr<FlagSetJob> flagSetJob = factoryManager->make_ptr<FlagSetJob>();
                //	//flagSetJob->setConstraint(this);
                //	flagSetJob->setFlag(physics::PxConstraintFlag::ePROJECTION);
                //	flagSetJob->setFlagValue(true);
                //	flagSetJob->setCoroutine(true);

                //	if (isControlComponent())
                //	{
                //		controlsTask->queueJob(flagSetJob);
                //	}
                //	else
                //	{
                //		physicsTask->queueJob(flagSetJob);
                //	}
                //}

                // if (!isControlComponent())
                //{
                //	SmartPtr<PxD6Joint> d6joint = getD6Joint();
                //	if (d6joint)
                //	{
                //		setLocalPose(PxJointActorIndex::eACTOR0, m_localFrame0);
                //		setLocalPose(PxJointActorIndex::eACTOR1, m_localFrame1);
                //	}
                // }

                // SmartPtr<IStateNode> rotatingGraphicsNode = getRotatingGraphicsNode();
                // if (rotatingGraphicsNode)
                //{
                //	SmartPtr<Rigidbody> body = rotatingGraphicsNode->getRigidBody();
                //	if (body)
                //	{
                //		body->setHasRenderTransform(false);
                //	}
                // }

                // if (m_bodyA)
                //{
                //	m_bodyA->wakeUp(1.0);
                // }

                // if (m_bodyB)
                //{
                //	m_bodyB->wakeUp(1.0);
                // }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Constraint::makeBroken()
        {
            setBrokenState( true );
        }

        void Constraint::makeBreakable( bool bNow )
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto timer = applicationManager->getTimer();

            // auto systemSettings =
            // fb::static_pointer_cast<SystemSettings>(applicationManager->getSystemSettings()); auto&
            // appFSM = applicationManager->getFSM();

            // auto factoryManager = applicationManager->getFactoryManager();
            // auto& pluginInterface = applicationManager->getPluginInterface();

            // auto& jobQueue = applicationManager->getJobQueue();
            // auto& taskManager = applicationManager->getTaskManager();

            // if (systemSettings->isSceneryLoading())
            //{
            //	return;
            // }

            // f64 crashTime = 5.0;
            // if (m_model->getModelType() == Model::E_MODEL_TYPE_PLANE)
            //{
            //	crashTime = 10.0;
            // }

            // if (applicationManager->isPauseMenuActive())
            //{
            //	crashTime = (f64)1e10;
            // }

            // if (systemSettings->getEnableCrashing())
            //{
            //	SaracenTypes::ApplicationState appState =
            //(SaracenTypes::ApplicationState)appFSM->getCurrentState(); 	if (appFSM->getStateTime()
            //> 5.0)
            //	{
            //		if (appState == SaracenTypes::ApplicationState::FB_STATE_FLIGHT)
            //		{
            //			if (!isControlComponent())
            //			{
            //				SmartPtr<IFSM>& fsm = getFSM();
            //				Modelscene::ModelComponentStates modelComponent =
            //(Modelscene::ModelComponentStates)fsm
            //					->getCurrentState();
            //				if (modelComponent == Modelscene::MODEL_COMPONENT_STATE_FLIGHT)
            //				{
            //					if (fsm->getStateTime() > crashTime)
            //					{
            //						m_breakableCount++;

            //						//if (m_breakableCount++ == 0)
            //						{
            //							if (!bNow)
            //							{
            //								if (pluginInterface->isLoadComplete())
            //								{
            //									auto physicsTask =
            // taskManager->getTask(Thread::Task::Physics); auto controlsTask =
            // taskManager->getTask(Thread::Task::Controls);

            //									//m_makebreakable->wait();

            //									if (m_makeBreakable->isFinished())
            //									{
            //										m_makeBreakable->setCoroutine(true);

            //										if (!isControlComponent())
            //										{
            //											physicsTask->queueJob(m_makeBreakable);
            //											//jobQueue->queueJob(makebreakable);
            //										}
            //									}
            //								}
            //							}
            //							else
            //							{
            //								m_breakableTime = timer->now();
            //								f32 fBreakForce = getOriginalBreakForce();
            //								f32 fBreakTorque = getOriginalBreakTorque();
            //								setBreakForce(fBreakForce, fBreakTorque);
            //							}
            //						}
            //					}
            //				}
            //			}
            //		}
            //	}
            //}
        }

        void Constraint::makeUnbreakable( bool bNow )
        {
            // if (getLoadingState() == Object::LoadingState::Loaded)
            //{
            //	SmartPtr<IFSM> fsm = getFSM();
            //	if (fsm)
            //	{
            //		Modelscene::ModelComponentStates state =
            //(Modelscene::ModelComponentStates)fsm-> 			getCurrentState();

            //		switch (state)
            //		{
            //		case Modelscene::MODEL_COMPONENT_STATE_CRASH:
            //		case Modelscene::MODEL_COMPONENT_STATE_RESET:
            //		case Modelscene::MODEL_COMPONENT_STATE_FLIGHT:
            //		{
            //			if (!isControlComponent())
            //			{
            //				m_breakableCount--;
            //				if (m_breakableCount < 0)
            //				{
            //					m_breakableCount = 0;
            //				}

            //				if (m_breakableCount <= 0)
            //				{
            //					if (!bNow)
            //					{
            //						auto applicationManager = core::IApplicationManager::instance();
            //						auto factoryManager = applicationManager->getFactoryManager();
            //						auto& jobQueue = applicationManager->getJobQueue();
            //						auto& taskManager = applicationManager->getTaskManager();
            //						auto physicsTask =
            // taskManager->getTask(Thread::Task::Physics); 						auto
            // controlsTask = taskManager->getTask(Thread::Task::Controls);

            //						if (m_makeUnbreakable)
            //						{
            //							m_makeUnbreakable->setCoroutine(true);

            //							if (!isControlComponent())
            //							{
            //								physicsTask->queueJob(m_makeUnbreakable);
            //								//jobQueue->queueJob(m_makeUnbreakable);
            //							}
            //						}
            //					}
            //					else
            //					{
            //						m_breakableCount = 0;
            //						m_breakableTime = std::numeric_limits<f64>::max();
            //						//setBreakForce(PX_MAX_F32, PX_MAX_F32);
            //					}
            //				}
            //			}
            //		}
            //		break;
            //		default:
            //		{
            //			if (!bNow)
            //			{
            //				auto applicationManager = core::IApplicationManager::instance();
            //				auto factoryManager = applicationManager->getFactoryManager();
            //				auto& jobQueue = applicationManager->getJobQueue();
            //				auto& taskManager = applicationManager->getTaskManager();
            //				auto physicsTask = taskManager->getTask(Thread::Task::Physics);
            //				auto controlsTask = taskManager->getTask(Thread::Task::Controls);

            //				if (m_makeUnbreakable)
            //				{
            //					m_makeUnbreakable->setCoroutine(true);

            //					if (!isControlComponent())
            //					{
            //						physicsTask->queueJob(m_makeUnbreakable);
            //						//jobQueue->queueJob(m_makeUnbreakable);
            //					}
            //				}
            //			}
            //			else
            //			{
            //				m_breakableCount = 0;
            //				m_breakableTime = std::numeric_limits<f64>::max();
            //				//setBreakForce(PX_MAX_F32, PX_MAX_F32);
            //			}
            //		}
            //		}
            //	}
            //}
        }

        void Constraint::enterCrashState()
        {
        }

        void Constraint::leaveCrashState()
        {
        }

        void Constraint::updateFlightState()
        {
        }

        void Constraint::leaveFlightState()
        {
            // try
            //{
            //	SmartPtr<IFSM> fsm = getFSM();
            //	Modelscene::ModelComponentStates state =
            //(Modelscene::ModelComponentStates)fsm->getCurrentState(); 	if (state !=
            // Modelscene::MODEL_COMPONENT_STATE_CRASH)
            //	{
            //		auto applicationManager = core::IApplicationManager::instance();
            //		auto taskManager = applicationManager->getTaskManager();

            //		makeUnbreakable(false);

            //		SmartPtr<IStateNode> rotatingGraphicsNode = getRotatingGraphicsNode();
            //		if (rotatingGraphicsNode)
            //		{
            //			Quaternion<real_Num> q = Quaternion<real_Num>::identity();
            //			rotatingGraphicsNode->setOrientation(q);
            //		}

            //		SmartPtr<Rotor> rotor = getRotor();
            //		if (rotor)
            //		{
            //			SmartPtr<IStateNode> rotatingGraphicsNode = rotor->getRotatingGraphicsNode();
            //			if (rotatingGraphicsNode)
            //			{
            //				Quaternion<real_Num> mastOrietation = Quaternion<real_Num>::identity();
            //				rotatingGraphicsNode->setOrientation(mastOrietation);
            //			}
            //		}
            //	}
            //}
            // catch (std::exception& e)
            //{
            //	FB_LOG_EXCEPTION(e);
            //}
        }

        void Constraint::enterWorkbenchState()
        {
            // try
            //{
            //	auto actor = getActor();
            //	if (actor)
            //	{
            //		//actor->unregisterAllComponent(this);

            //		if (!isControlComponent())
            //		{
            //			//if (isMotor())
            //			//{
            //			//	actor->registerComponentUpdate(Thread::Task::Physics,
            //			//		Thread::UpdateState::PreUpdate, this);
            //			//	actor->registerComponentUpdate(Thread::Task::Physics,
            //			//		Thread::UpdateState::Update, this);
            //			//	actor->registerComponentUpdate(Thread::Task::Physics,
            //			//		Thread::UpdateState::PostUpdate, this);
            //			//}
            //		}
            //		else
            //		{
            //			//if (isMotor())
            //			//{
            //			//	actor->registerComponentUpdate(Thread::Task::Physics,
            //			//		Thread::UpdateState::PreUpdate, this);
            //			//	actor->registerComponentUpdate(Thread::Task::Physics,
            //			//		Thread::UpdateState::Update, this);
            //			//	actor->registerComponentUpdate(Thread::Task::Physics,
            //			//		Thread::UpdateState::PostUpdate, this);

            //			//	actor->registerComponentUpdate(Thread::Task::Controls,
            //			//		Thread::UpdateState::PreUpdate, this);
            //			//	actor->registerComponentUpdate(Thread::Task::Controls,
            //			//		Thread::UpdateState::Update, this);
            //			//	actor->registerComponentUpdate(Thread::Task::Controls,
            //			//		Thread::UpdateState::PostUpdate, this);
            //			//}
            //		}
            //	}

            //	auto applicationManager = core::IApplicationManager::instance();
            //	auto& jobQueue = applicationManager->getJobQueue();
            //	auto& taskManager = applicationManager->getTaskManager();
            //	auto factoryManager = applicationManager->getFactoryManager();
            //	auto& physicsTask = taskManager->getTask(Thread::Task::Physics);
            //	auto& controlsTask = taskManager->getTask(Thread::Task::Controls);

            //	Task::Lock physicsLock(physicsTask);
            //	Task::Lock controlsLock(controlsTask);

            //	makeUnbreakable(true);

            //	//rescale();

            //	if (!m_servoCached)
            //	{
            //		s32 iServoHash = getServoHash();
            //		if (iServoHash != 0)
            //		{
            //			m_servoCached = m_model->getModelComponentByType<Servo>(iServoHash);
            //			if (m_servoCached)
            //			{
            //				//m_servoCached->setConstraint(this);
            //			}
            //		}
            //	}

            //	resetPositions();

            //	m_servoNoneCompare = getServo().compare("none");

            //	//SmartPtr<IStateNode> rotatingGraphicsNode = getRotatingGraphicsNode();
            //	//if (rotatingGraphicsNode)
            //	//{
            //	//	SmartPtr<Rigidbody> body = rotatingGraphicsNode->getRigidBody();
            //	//	if (body)
            //	//	{
            //	//		body->setHasRenderTransform(true);
            //	//	}
            //	//}

            //	if (getEnableProjection())
            //	{
            //		SmartPtr<FlagSetJob> flagSetJob = factoryManager->make_ptr<FlagSetJob>();
            //		//flagSetJob->setConstraint(this);
            //		flagSetJob->setFlag(physics::PxConstraintFlag::ePROJECTION);
            //		flagSetJob->setFlagValue(true);
            //		flagSetJob->setCoroutine(true);

            //		if (isControlComponent())
            //		{
            //			controlsTask->queueJob(flagSetJob);
            //		}
            //		else
            //		{
            //			physicsTask->queueJob(flagSetJob);
            //		}
            //	}

            //	if (m_bodyA)
            //	{
            //		m_bodyA->wakeUp(1.0);
            //	}

            //	if (m_bodyB)
            //	{
            //		m_bodyB->wakeUp(1.0);
            //	}
            //}
            // catch (std::exception& e)
            //{
            //	FB_LOG_EXCEPTION(e);
            //}
        }

        void Constraint::updateWorkbenchState()
        {
        }

        void Constraint::leaveWorkbenchState()
        {
            // try
            //{
            //	SmartPtr<IStateNode> rotatingGraphicsNode = getRotatingGraphicsNode();
            //	if (rotatingGraphicsNode)
            //	{
            //		Quaternion<real_Num> q = Quaternion<real_Num>::identity();
            //		rotatingGraphicsNode->setOrientation(q);
            //	}

            //	SmartPtr<Rotor> rotor = getRotor();
            //	if (rotor)
            //	{
            //		SmartPtr<IStateNode> rotatingGraphicsNode = rotor->getRotatingGraphicsNode();
            //		if (rotatingGraphicsNode)
            //		{
            //			Quaternion<real_Num> mastOrietation = Quaternion<real_Num>::identity();
            //			rotatingGraphicsNode->setOrientation(mastOrietation);
            //		}
            //	}
            //}
            // catch (std::exception& e)
            //{
            //	FB_LOG_EXCEPTION(e);
            //}
        }

        void Constraint::enterDestroyedState()
        {
        }

        void Constraint::updateDestroyedState()
        {
        }

        void Constraint::leaveDestroyedState()
        {
            setBrokenState( false );
        }

        void Constraint::setBrokenState( bool bBroken )
        {
            // PxConstraintFlag::eBROKEN is a read only flag

            // if (m_d6joint)
            //{
            //	m_d6joint->setConstraintFlag(PxConstraintFlag::eBROKEN, bBroken);
            // }

            // if (m_fixedJoint)
            //{
            //	m_fixedJoint->setConstraintFlag(PxConstraintFlag::eBROKEN, bBroken);
            // }
        }

        void Constraint::updateDrive()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto systemSettings =
            // fb::static_pointer_cast<SystemSettings>(applicationManager->getSystemSettings()); auto&
            // physicsSettings = systemSettings->getPhysicsSettings();

            ////#if !FB_FINAL
            ////		if (m_d6joint)
            ////		{
            ////			SmartPtr<IPhysicsScene3> physicsScene = getScene();
            ////			FB_STATE_PXSCENE_READ_LOCK(physicsScene);
            ////
            ////			m_twist = m_d6joint->getTwist();
            ////			m_swingYAngle = m_d6joint->getSwingYAngle();
            ////			m_swingZAngle = m_d6joint->getSwingZAngle();
            ////		}
            ////#endif

            // if (getFlag())
            //{
            //	if (isMotor())
            //	{
            //		s32 iChannel = getChannel();
            //		if (iChannel == 99)
            //		{
            //			f64 rpm = 0.0;
            //			f64 angle = 0.0;

            //			SmartPtr<Rotor> rotor = getRotor();
            //			if (rotor)
            //			{
            //				rpm = rotor->getRPM();
            //				angle = -rotor->getRotation();
            //			}

            //			f64 degrees = (rpm / 60.0) * 360.0;

            //			if (m_motorDirection > 0.0f)
            //			{
            //				m_rotate += (degrees * MathD::deg_to_rad()) * dt;
            //			}
            //			else
            //			{
            //				m_rotate -= (degrees * MathD::deg_to_rad()) * dt;
            //			}

            //			QuaternionD orietation;
            //			orietation.fromAngleAxis(angle, Vector3D::UNIT_X);

            //			Transform3<real_Num> pose;// = Transform3<real_Num>(
            //				//Quaternion<real_Num>((f32)orietation.X(), (f32)orietation.Y(),
            //(f32)orietation.Z(), (f32)orietation.W()));

            //			bool bUsingControlSystem = physicsSettings->getEnableControlSystem();

            //			SmartPtr<IFSM>& fsm = getFSM();
            //			Modelscene::ModelComponentStates state =
            //(Modelscene::ModelComponentStates)fsm->getCurrentState();

            //			if (state == Modelscene::MODEL_COMPONENT_STATE_FLIGHT)
            //			{
            //				if (!bUsingControlSystem)
            //				{
            //					// rotate the scenenode instead of the rigidbody
            //					if (m_rotatingGraphicsNode)
            //					{
            //						Quaternion<real_Num> mastOrietation;
            //						mastOrietation.fromAngleAxis((f32)m_rotate,
            // Vector3<real_Num>::negativeY());
            // m_rotatingGraphicsNode->setOrientation(mastOrietation);
            //					}
            //				}
            //				else
            //				{
            //					if (rotor)
            //					{
            //						//rotor->setRotationAnimationPosition(m_rotate);

            //						if (m_d6joint)
            //						{
            //							m_d6joint->setDrivePosition(pose);
            //						}
            //					}
            //				}
            //			}
            //			else
            //			{
            //				if (m_d6joint)
            //				{
            //					m_d6joint->setDrivePosition(pose);
            //				}
            //			}
            //		}
            //		else
            //		{
            //			auto& receiver = applicationManager->getModelRx();
            //			s32 iChannel = getChannel();

            //			f32 pos = 0.0f;

            //			if (receiver)
            //			{
            //				if (receiver->getOutput2(iChannel, &pos))
            //				{
            //					f64 motorDirection = m_motorDirection;
            //					pos = pos * motorDirection;
            //					m_pos = pos;
            //				}
            //			}

            //			if (m_servoCached)
            //			{
            //				if (!m_linear)
            //				{
            //					m_servoCached->updateDrive(m_pos, dt);
            //				}
            //				else
            //				{
            //					m_servoCached->updateLinearDrive(m_pos, dt);
            //				}
            //			}
            //		}
            //	}
            //}

            // if (m_fixedJoint)
            //{
            //	u32 val = 0;// (m_fixedJoint->getConstraintFlags()& PxConstraintFlag::eBROKEN);
            //	if (val != 0)
            //	{
            //		if (m_bodyB)
            //		{
            //			auto bodyFSM = m_bodyB->getFSM();
            //			if (bodyFSM)
            //			{
            //				if (bodyFSM->getCurrentState() !=
            // Modelscene::MODEL_COMPONENT_STATE_DESTROYED)
            //				{
            //					bodyFSM->setNewState(Modelscene::MODEL_COMPONENT_STATE_DESTROYED);
            //				}
            //			}
            //		}
            //	}
            // }
        }

        void Constraint::updateDriveEditor()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto systemSettings =
            // fb::static_pointer_cast<SystemSettings>(applicationManager->getSystemSettings()); auto&
            // physicsSettings = systemSettings->getPhysicsSettings();

            ////#if !FB_FINAL
            ////		if (m_d6joint)
            ////		{
            ////			SmartPtr<IPhysicsScene3> physicsScene = getScene();
            ////			FB_STATE_PXSCENE_READ_LOCK(physicsScene);
            ////
            ////			m_twist = m_d6joint->getTwist();
            ////			m_swingYAngle = m_d6joint->getSwingYAngle();
            ////			m_swingZAngle = m_d6joint->getSwingZAngle();
            ////		}
            ////#endif

            // if (getFlag())
            //{
            //	if (isMotor())
            //	{
            //		s32 iChannel = getChannel();
            //		if (iChannel == 99)
            //		{
            //			f64 rpm = 0.0;
            //			f64 angle = 0.0;

            //			SmartPtr<Rotor> rotor = getRotor();
            //			if (rotor)
            //			{
            //				rpm = rotor->getRPM();
            //				angle = -rotor->getRotation();
            //			}

            //			f64 degrees = (rpm / 60.0) * 360.0;

            //			if (m_motorDirection > 0.0f)
            //			{
            //				m_rotate += (degrees * MathD::deg_to_rad()) * dt;
            //			}
            //			else
            //			{
            //				m_rotate -= (degrees * MathD::deg_to_rad()) * dt;
            //			}

            //			QuaternionD orietation;
            //			orietation.fromAngleAxis(angle, Vector3D::UNIT_X);

            //			Transform3<real_Num> pose;// = Transform3<real_Num>(
            //				//Quaternion<real_Num>((f32)orietation.X(), (f32)orietation.Y(),
            //(f32)orietation.Z(), (f32)orietation.W()));

            //			bool bUsingControlSystem = physicsSettings->getEnableControlSystem();

            //			SmartPtr<IFSM>& fsm = getFSM();
            //			Modelscene::ModelComponentStates state =
            //(Modelscene::ModelComponentStates)fsm->getCurrentState();

            //			if (state == Modelscene::MODEL_COMPONENT_STATE_FLIGHT)
            //			{
            //				if (!bUsingControlSystem)
            //				{
            //					// rotate the scenenode instead of the rigidbody
            //					if (m_rotatingGraphicsNode)
            //					{
            //						Quaternion<real_Num> mastOrietation;
            //						mastOrietation.fromAngleAxis((f32)m_rotate,
            // Vector3<real_Num>::negativeY());
            // m_rotatingGraphicsNode->setOrientation(mastOrietation);
            //					}
            //				}
            //				else
            //				{
            //					if (rotor)
            //					{
            //						//rotor->setRotationAnimationPosition(m_rotate);

            //						if (m_d6joint)
            //						{
            //							m_d6joint->setDrivePosition(pose);
            //						}
            //					}
            //				}
            //			}
            //			else
            //			{
            //				if (m_d6joint)
            //				{
            //					m_d6joint->setDrivePosition(pose);
            //				}
            //			}
            //		}
            //		else
            //		{
            //			auto& receiver = applicationManager->getModelRx();
            //			s32 iChannel = getChannel();

            //			f32 pos = 0.0f;

            //			if (receiver)
            //			{
            //				if (receiver->getOutput2(iChannel, &pos))
            //				{
            //					f64 motorDirection = m_motorDirection;
            //					pos = pos * motorDirection;
            //					m_pos = pos;
            //				}
            //			}

            //			if (m_servoCached)
            //			{
            //				if (!m_servoCached->isMeasuring())
            //				{
            //					if (!m_linear)
            //					{
            //						m_servoCached->updateDrive(m_pos, dt);
            //					}
            //					else
            //					{
            //						m_servoCached->updateLinearDrive(m_pos, dt);
            //					}
            //				}
            //				else
            //				{
            //					int stop = 0;
            //					stop = 0;
            //				}
            //			}
            //		}
            //	}
            //}

            // if (m_fixedJoint)
            //{
            //	u32 val = 0;// (m_fixedJoint->getConstraintFlags()& PxConstraintFlag::eBROKEN);
            //	if (val != 0)
            //	{
            //		if (m_bodyB)
            //		{
            //			auto bodyFSM = m_bodyB->getFSM();
            //			if (bodyFSM)
            //			{
            //				if (bodyFSM->getCurrentState() !=
            // Modelscene::MODEL_COMPONENT_STATE_DESTROYED)
            //				{
            //					bodyFSM->setNewState(Modelscene::MODEL_COMPONENT_STATE_DESTROYED);
            //				}
            //			}
            //		}
            //	}
            // }
        }

        void Constraint::setBreakState( bool state )
        {
            // m_breakState = state;
            // if (m_componentUpdateOnBreak != nullptr)
            //{
            //	m_componentUpdateOnBreak->setBreakState(state);
            // }
        }

        void Constraint::enterResetState()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto& taskManager = applicationManager->getTaskManager();

            // Task::Lock physicsLock(taskManager->getTask(Thread::Task::Physics));
            // Task::Lock controlsLock(taskManager->getTask(Thread::Task::Controls));

            // makeUnbreakable(true);

            // String servoName = getServo();
            // m_servoNoneCompare = servoName.compare("none");

            // if (m_d6joint)
            //{
            //	m_d6joint->setConstraintFlag(physics::PxConstraintFlag::ePROJECTION, false);
            // }
        }

        void Constraint::updateResetState()
        {
        }

        void Constraint::leaveResetState()
        {
        }

        u32 Constraint::getEffectNameHash()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto systemSettings =
            // fb::static_pointer_cast<SystemSettings>(applicationManager->getSystemSettings());
            // FB_ASSERT(applicationManager);
            // FB_ASSERT(systemSettings);

            // if (!systemSettings->getNightMode())
            //{
            //	return m_effectNameHash;
            // }
            // else
            //{
            //	return m_effectNameNightHash;
            // }

            return 0;
        }

        SmartPtr<physics::IConstraintD6> Constraint::getD6Joint()
        {
            return m_d6joint;
        }

        void Constraint::setOriginalBreakForce( f32 breakForce )
        {
            m_breakForce = breakForce;
        }

        void Constraint::setOriginalBreakTorque( f32 breakTorque )
        {
            m_breakTorque = breakTorque;
        }

        bool Constraint::isBroken() const
        {
            if( m_d6joint )
            {
                u32 val = ( m_d6joint->getConstraintFlags() & physics::PxConstraintFlag::eBROKEN );
                if( val != 0 )
                {
                    return true;
                }
            }

            // if (m_fixedJoint)
            //{
            //	u32 val = (m_fixedJoint->getConstraintFlags() & PxConstraintFlag::eBROKEN);
            //	if (val != 0)
            //	{
            //		return true;
            //	}
            // }

            return false;
        }

        void Constraint::restoreFlags()
        {
            SmartPtr<Rigidbody> rbA = getBodyA();

            // if (rbA != nullptr)
            //{
            //	SmartPtr<IRigidBody3>& rbActorA = rbA->getPhysxRigidbody();
            //	if (rbActorA)
            //	{
            //		int nbShapes = rbActorA->getNbShapes();
            //		if (nbShapes > 0)
            //		{
            //			Array<PxShape*> shapes;
            //			shapes.resize(nbShapes);

            //			rbA->getPhysxRigidbody()->getShapes(&shapes[0], nbShapes);

            //			for (int i = 0; i < nbShapes; i++)
            //			{
            //				if (shapes[i]->userData != nullptr)
            //				{
            //					((ShapeData*)shapes[i]->userData)->m_isBroken = false;
            //				}
            //			}
            //		}
            //	}
            //}

            // SmartPtr<Rigidbody> rbB = getBodyB();
            // if (rbB != nullptr)
            //{
            //	SmartPtr<IRigidBody3>& body = rbB->getPhysxRigidbody();
            //	if (body)
            //	{
            //		int nbShapes = body->getNbShapes();
            //		if (nbShapes > 0)
            //		{
            //			Array<PxShape*> shapes;
            //			shapes.resize(nbShapes);

            //			body->getShapes(&shapes[0], nbShapes);

            //			for (int i = 0; i < nbShapes; i++)
            //			{
            //				if (shapes[i]->userData != nullptr)
            //				{
            //					((ShapeData*)shapes[i]->userData)->m_isBroken = false;
            //				}
            //			}
            //		}
            //	}
            //}

            // m_breakState = false;

            // if (m_componentUpdateOnBreak != nullptr)
            //{
            //	m_componentUpdateOnBreak->setBreakState(false);
            // }
        }

        
        // void Constraint::setDrive(PxD6Drive::Enum index, const SmartPtr<IPhysicsConstraint3> drive)
        //{
        //	//if (m_d6joint)
        //	//{
        //	//	m_d6joint->setDrive(index, drive);
        //	//}
        // }

        void Constraint::setLocalPosition( physics::PxJointActorIndex::Enum actor,
                                           const Vector3<real_Num> &pos )
        {
            // if (m_d6joint)
            //{
            //	Transform3<real_Num> transform = m_d6joint->getLocalPose(actor);
            //	transform.p = Vector3<real_Num>(pos.X(), pos.Y(), pos.Z());
            //	m_d6joint->setLocalPose(actor, transform);
            // }

            // if (m_positionJoint)
            //{
            //	Transform3<real_Num> transform = m_positionJoint->getLocalPose(actor);
            //	transform.p = Vector3<real_Num>(pos.X(), pos.Y(), pos.Z());
            //	m_positionJoint->setLocalPose(actor, transform);
            // }

            // if (m_fixedJoint)
            //{
            //	Transform3<real_Num> transform = m_fixedJoint->getLocalPose(actor);
            //	transform.p = Vector3<real_Num>(pos.X(), pos.Y(), pos.Z());
            //	m_fixedJoint->setLocalPose(actor, transform);
            // }
        }

        Transform3<real_Num> Constraint::getLocalPose( physics::PxJointActorIndex::Enum actor ) const
        {
            // auto physicsScene = getScene();
            // FB_STATE_PXSCENE_WRITE_LOCK(physicsScene);

            // if (m_d6joint)
            //{
            //	return m_d6joint->getLocalPose(actor);
            // }

            // if (m_positionJoint)
            //{
            //	return m_positionJoint->getLocalPose(actor);
            // }

            // if (m_fixedJoint)
            //{
            //	return m_fixedJoint->getLocalPose(actor);
            // }

            return Transform3<real_Num>();
        }

        void Constraint::setLocalPose( physics::PxJointActorIndex::Enum actor,
                                       const Transform3<real_Num> &localPose )
        {
            // auto physicsScene = getScene();
            // FB_STATE_PXSCENE_WRITE_LOCK(physicsScene);

            // if (m_d6joint)
            //{
            //	m_d6joint->setLocalPose(actor, localPose);
            // }

            // if (m_positionJoint)
            //{
            //	m_positionJoint->setLocalPose(actor, localPose);
            // }

            // if (m_fixedJoint)
            //{
            //	m_fixedJoint->setLocalPose(actor, localPose);
            // }
        }

        SmartPtr<physics::IPhysicsScene3> Constraint::getScene() const
        {
            // SmartPtr<physics::IPhysicsScene3> scene = nullptr;
            // if (m_bodyA && m_bodyA->getPhysicScene())
            //{
            //	scene = m_bodyA->getPhysicScene();
            // }
            // else if (m_bodyB && m_bodyB->getPhysicScene())
            //{
            //	scene = m_bodyB->getPhysicScene();
            // }

            return nullptr;
        }

        void Constraint::setMotion( physics::PxD6Axis::Enum axis, physics::PxD6Motion::Enum type )
        {
            // if (m_positionJoint)
            //{
            //	m_positionJoint->setMotion(axis, type);
            // }
        }

        bool Constraint::isControlComponent() const
        {
            // return (m_bodyA && m_bodyA->isControlComponent()) || (m_bodyB &&
            // m_bodyB->isControlComponent());

            return false;
        }

        void Constraint::updateComponentState()
        {
        }

        void Constraint::setServoHash( int val )
        {
            m_servoHash = val;
        }

        SmartPtr<physics::IConstraintFixed3> Constraint::getFixedJoint() const
        {
            return m_fixedJoint;
        }

        void Constraint::setFixedJoint( SmartPtr<physics::IConstraintFixed3> val )
        {
            m_fixedJoint = val;
        }

        bool Constraint::isMotor() const
        {
            return m_isMotor;
        }

        void Constraint::setMotor( bool val )
        {
            m_isMotor = val;
        }

        SmartPtr<ITransformNode> Constraint::getAnchor0() const
        {
            return m_anchor0;
        }

        void Constraint::setAnchor0( SmartPtr<ITransformNode> val )
        {
            m_anchor0 = val;
        }

        SmartPtr<ITransformNode> Constraint::getAnchor1() const
        {
            return m_anchor1;
        }

        void Constraint::setAnchor1( SmartPtr<ITransformNode> val )
        {
            m_anchor1 = val;
        }

        SmartPtr<ITransformNode> Constraint::getRotatingGraphicsNode() const
        {
            return m_rotatingGraphicsNode;
        }

        Constraint::MakeBreakable::~MakeBreakable()
        {
        }

        Constraint::MakeBreakable::MakeBreakable( SmartPtr<Constraint> constraint ) :
            m_constraint( constraint )
        {
        }

        Constraint::MakeBreakable::MakeBreakable()
        {
        }

        void Constraint::MakeBreakable::execute()
        {
            // SmartPtr<IFSM> fsm = m_constraint->getFSM();
            // while (fsm->getStateTime() > 5.0)
            //{
            //	Thread::yield();
            // }

            if( m_constraint )
            {
                f32 fBreakForce = m_constraint->getOriginalBreakForce();
                f32 fBreakTorque = m_constraint->getOriginalBreakTorque();
                m_constraint->setBreakForce( fBreakForce, fBreakTorque );
            }
        }

        void Constraint::MakeBreakable::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto timer = applicationManager->getTimer();

            // f64 stateTime = 0.0;

            // SmartPtr<IFSM> fsm = m_constraint->getFSM();
            // if (fsm)
            //{
            //	Modelscene::ModelComponentStates state =
            //(Modelscene::ModelComponentStates)fsm->getCurrentState(); 	if (state ==
            // Modelscene::MODEL_COMPONENT_STATE_FLIGHT)
            //	{
            //		stateTime = fsm->getStateTime();
            //	}
            //	else
            //	{
            //		rYield->stop();
            //	}
            // }
            // else
            //{
            //	rYield->stop();
            // }

            // JOB_BEGIN()
            //	//JOB_YIELD_UNTIL_TRUE(stateTime > 1.0)

            //	if (m_constraint)
            //	{
            //		auto applicationManager = core::IApplicationManager::instance();
            //		auto systemSettings =
            // fb::static_pointer_cast<SystemSettings>(applicationManager->getSystemSettings());

            //		m_constraint->m_breakableTime = timer->now();

            //		f32 fBreakForce = m_constraint->getOriginalBreakForce();
            //		f32 fBreakTorque = m_constraint->getOriginalBreakTorque();

            //		fBreakForce = MathF::Pow(fBreakForce,
            // systemSettings->getCrashSensitivityMultiplier()); 		fBreakTorque =
            // MathF::Pow(fBreakTorque, systemSettings->getCrashSensitivityMultiplier());

            //		m_constraint->setBreakForce(fBreakForce, fBreakTorque);
            //	}
            // JOB_END()
        }

        void Constraint::setupUpdateRegistration()
        {
            auto actor = getActor();
            if( actor )
            {
                // actor->registerComponentUpdate(Tasks::TaskIds::ET_PRIMARY,
                // Thread::UpdateState::PreUpdate, this);
                // actor->registerComponentUpdate(Tasks::TaskIds::ET_PRIMARY,
                // Thread::UpdateState::Update, this);
                // actor->registerComponentUpdate(Tasks::TaskIds::ET_PRIMARY,
                // Thread::UpdateState::PostUpdate, this);
            }
        }

        String Constraint::getConstraintType() const
        {
            return m_constraintType;
        }

        String Constraint::getClfSenenode() const
        {
            return m_clf_scenenode;
        }

        String Constraint::getServo() const
        {
            return m_servo;
        }

        f64 Constraint::timeSinceBreakable() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();
            return timer->now() - m_breakableTime;
        }

        void Constraint::setRotatingGraphicsNode( SmartPtr<ITransformNode> val )
        {
            m_rotatingGraphicsNode = val;
        }

        String Constraint::getRotorName() const
        {
            return m_rotorName;
        }

        void Constraint::setRotorName( const String &val )
        {
            m_rotorName = val;
        }

        void Constraint::setChannel( int val )
        {
            m_channel = val;
        }

        bool Constraint::getFlag() const
        {
            return m_flag;
        }

        void Constraint::setFlag( bool val )
        {
            m_flag = val;
        }

        bool Constraint::getEnableProjection() const
        {
            return m_enableProjection;
        }

        void Constraint::setEnableProjection( bool val )
        {
            m_enableProjection = val;
        }

        Constraint::MakeUnbreakable::MakeUnbreakable( SmartPtr<Constraint> constraint ) :
            m_constraint( constraint )
        {
        }

        Constraint::MakeUnbreakable::MakeUnbreakable()
        {
        }

        Constraint::MakeUnbreakable::~MakeUnbreakable()
        {
        }

        void Constraint::MakeUnbreakable::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            while( timer->now() > ( m_constraint->m_breakableTime + 1.0 ) )
            {
                Thread::yield();
            }

            // if (m_constraint)
            //{
            //	m_constraint->m_breakableTime = std::numeric_limits<f64>::max();
            //	m_constraint->setBreakForce(PX_MAX_F32, PX_MAX_F32);
            // }
        }

        void Constraint::MakeUnbreakable::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            // f64 stateTime = 0.0;

            // SmartPtr<IFSM> fsm = m_constraint->getFSM();
            // if (fsm)
            //{
            //	Modelscene::ModelComponentStates state =
            //(Modelscene::ModelComponentStates)fsm->getCurrentState(); 	if (state ==
            // Modelscene::MODEL_COMPONENT_STATE_FLIGHT)
            //	{
            //		stateTime = fsm->getStateTime();
            //	}
            //	else
            //	{
            //		rYield.stop();
            //	}
            // }
            // else
            //{
            //	rYield.stop();
            // }

            // JOB_BEGIN()
            //	JOB_YIELD_UNTIL_TRUE(timer->now() > (m_constraint->m_breakableTime + 1.0))
            //	if (m_constraint)
            //	{
            //		m_constraint->m_breakableTime = std::numeric_limits<f64>::max();
            //		m_constraint->setBreakForce(PX_MAX_F32, PX_MAX_F32);
            //	}
            // JOB_END()
        }

        Constraint::FlagSetJob::FlagSetJob( SmartPtr<Constraint> constraint ) :
            m_constraint( constraint )
        {
        }

        Constraint::FlagSetJob::FlagSetJob()
        {
        }

        Constraint::FlagSetJob::~FlagSetJob()
        {
        }

        void Constraint::FlagSetJob::execute()
        {
            Thread::sleep( 3.0 );

            // SmartPtr<Constraint> constraint = getConstraint();
            // if (constraint)
            //{
            //	SmartPtr<IFSM> fsm = constraint->getFSM();

            //	RawPtr<PxJoint> j = constraint->getD6Joint();
            //	if (j)
            //	{
            //		PxConstraintFlag::Enum flag = getFlag();
            //		bool bFlagValue = getFlagValue();
            //		j->setConstraintFlag(flag, bFlagValue);
            //	}
            //}
        }

        void Constraint::FlagSetJob::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
        {
            SmartPtr<Constraint> constraint = getConstraint();
            if( !constraint )
            {
                rYield->stop();
                return;
            }

            if( constraint )
            {
                if( constraint->getLoadingState() != LoadingState::Loaded )
                {
                    rYield->stop();
                }
            }

            // SmartPtr<IFSM> fsm = constraint->getFSM();
            // if (!fsm)
            //{
            //	rYield->stop();
            //	return;
            // }

            // JOB_BEGIN()
            //	JOB_YIELD_UNTIL_TRUE(fsm && fsm->getStateTime() > 5.0)
            //	RawPtr<PxJoint> j = constraint->getD6Joint();
            // if (j)
            //{
            //	PxConstraintFlag::Enum flag = getFlag();
            //	bool bFlagValue = getFlagValue();
            //	j->setConstraintFlag(flag, bFlagValue);
            // }

            // JOB_END()
        }

        physics::PxConstraintFlag::Enum Constraint::FlagSetJob::getFlag() const
        {
            return m_flag;
        }

        void Constraint::FlagSetJob::setFlag( physics::PxConstraintFlag::Enum val )
        {
            m_flag = val;
        }

        bool Constraint::FlagSetJob::getFlagValue() const
        {
            return m_flagValue;
        }

        void Constraint::FlagSetJob::setFlagValue( bool val )
        {
            m_flagValue = val;
        }

        SmartPtr<Constraint> Constraint::FlagSetJob::getConstraint() const
        {
            return m_constraint;
        }

        void Constraint::FlagSetJob::setConstraint( SmartPtr<Constraint> val )
        {
            m_constraint = val;
        }

        Transform3<real_Num> Constraint::getLocalPose0() const
        {
            return m_localPose0;
        }

        Transform3<real_Num> Constraint::getLocalPose1() const
        {
            return m_localPose1;
        }

        void Constraint::setLocalPose0( Transform3<real_Num> t )
        {
            m_localPose0 = t;
        }

        void Constraint::setLocalPose1( Transform3<real_Num> t )
        {
            m_localPose1 = t;
        }

        Transform3<real_Num> Constraint::getLocalFrame1() const
        {
            return m_localFrame1;
        }

        void Constraint::setLocalFrame1( Transform3<real_Num> t )
        {
            m_localFrame1 = t;
        }

        Transform3<real_Num> Constraint::getLocalFrame0() const
        {
            return m_localFrame0;
        }

        void Constraint::setLocalFrame0( Transform3<real_Num> t )
        {
            m_localFrame0 = t;
        }

        Constraint::SetupLocalPose::SetupLocalPose()
        {
        }

        Constraint::SetupLocalPose::SetupLocalPose( SmartPtr<Constraint> constraint ) :
            m_constraint( constraint )
        {
        }

        Constraint::SetupLocalPose::~SetupLocalPose()
        {
        }

        void Constraint::SetupLocalPose::execute()
        {
        }

        void Constraint::SetupLocalPose::coroutine_execute_step( SmartPtr<IObjectYield> &rYield )
        {
            SmartPtr<Constraint> constraint = m_constraint;
            if( !constraint )
            {
                rYield->stop();
                return;
            }

            if( constraint )
            {
                if( constraint->getLoadingState() != LoadingState::Loaded )
                {
                    rYield->stop();
                }
            }

            // SmartPtr<IFSM> fsm = constraint->getFSM();
            // if (!fsm)
            //{
            //	rYield->stop();
            //	return;
            // }

            // JOB_BEGIN()
            //	JOB_YIELD_UNTIL_TRUE(fsm && fsm->getStateTime() > 5.0)

            //	auto localPose0 = constraint->getLocalPose0();
            // auto localPose1 = constraint->getLocalPose1();

            // SmartPtr<physics::IConstraintD6> d6joint = constraint->getD6Joint();
            // if (d6joint)
            //{
            //	d6joint->setConstraintFlag(physics::PxConstraintFlag::ePROJECTION, false);
            //	d6joint->setLocalPose(physics::PxJointActorIndex::eACTOR0, localPose0);
            //	d6joint->setLocalPose(physics::PxJointActorIndex::eACTOR1, localPose1);
            // }

            ////SmartPtr<IConstraintFixed3> fixedJoint = constraint->getFixedJoint();
            ////if (fixedJoint)
            ////{
            ////	fixedJoint->setLocalPose(PxJointActorIndex::eACTOR0, localPose0);
            ////	fixedJoint->setLocalPose(PxJointActorIndex::eACTOR1, localPose1);
            ////}

            // JOB_END()
        }

        SmartPtr<Rigidbody> &Constraint::getBodyA()
        {
            return m_bodyA;
        }

        const SmartPtr<Rigidbody> &Constraint::getBodyA() const
        {
            return m_bodyA;
        }

        SmartPtr<Rigidbody> &Constraint::getBodyB()
        {
            return m_bodyB;
        }

        const SmartPtr<Rigidbody> &Constraint::getBodyB() const
        {
            return m_bodyB;
        }

        int Constraint::getServoHash() const
        {
            return m_servoHash;
        }

        float Constraint::getOriginalBreakForce() const
        {
            return m_breakForce;
        }

        float Constraint::getOriginalBreakTorque() const
        {
            return m_breakTorque;
        }

        int Constraint::getChannel() const
        {
            return m_channel;
        }
    }  // namespace scene
}  // end namespace fb
