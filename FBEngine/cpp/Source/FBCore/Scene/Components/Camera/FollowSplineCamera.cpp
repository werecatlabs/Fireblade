#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/FollowSplineCamera.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {

        FollowSplineCamera::FollowSplineCamera() :
            // m_camera(0),
            m_curNodeIdx( 0 ),
            m_totalSplineFollowTime( 120.f ),
            m_totalSplineDistance( 0.0f ),
            m_followSpline( false )
        {
            m_translationSpeed = 10.0f;
        }

        FollowSplineCamera::~FollowSplineCamera()
        {
        }

        void FollowSplineCamera::initialize()
        {
        }

        void FollowSplineCamera::update( u32 taskId, f64 t, f64 dt )
        {
            // if (!m_followSpline)
            //	return;

            // Ogre::Camera* ogreCamera = 0;
            // Ogre::SceneNode* cameraSceneNode = 0;
            ////ViewOgreWindow* viewWindow = 0;
            ////FBSystem::getSingletonPtr()->getCameraManager()->getViewPortCamera(3,
            ///(void**)&ogreCamera);

            // Wm5::Vector3f wmPos0 = curve->GetPosition(m_splineTime);
            // m_position = Vector3F(wmPos0.X(), wmPos0.Y(), wmPos0.Z());

            // m_splineTime += (1.0f / m_totalSplineFollowTime) * dt;
            // if (m_splineTime >= 1.0f)
            //{
            //	m_splineTime = 0.0f;
            //	m_curNodeIdx = 0;
            //	selectNextNode();
            // }

            // Ogre::Vector3 ogreCameraPos(m_position);
            // ogreCameraPos.z = -ogreCameraPos.z;
            // ogreCamera->setPosition(ogreCameraPos);

            // ogreCamera->setOrientation(m_rotationalSpline->interpolate(m_splineTime));
        }

        void FollowSplineCamera::selectNextNode()
        {
            nextNode();
        }

        void FollowSplineCamera::nextNode()
        {
            /*if(m_splineCtrlNodes.empty())
                return;

            m_curNodeIdx = (m_curNodeIdx + 1) % m_splineCtrlNodes.size();
            m_curCtrlNode = m_splineCtrlNodes[m_curNodeIdx];

            CompPropertiesPtr compProperties = m_curCtrlNode->getComponent(CompProperties::Type);
            if(!compProperties)
                return;

            FBPropertyGroupPtr propertyGrp = compProperties->getPropertyGroup();

            String sPosition;
            propertyGrp->GetPropertyValue("Position", sPosition);
            m_targetPosition = FBStringUtil::parseVector3<f32>(sPosition);

            String sTargetRotation;
            propertyGrp->GetPropertyValue("Yaw", sTargetRotation);
            m_targetRotation.Y() = FBStringUtil::parseFloat(sTargetRotation);

            propertyGrp->GetPropertyValue("Pitch", sTargetRotation);
            m_targetRotation.X() = FBStringUtil::parseFloat(sTargetRotation);

            propertyGrp->GetPropertyValue("Roll", sTargetRotation);
            m_targetRotation.Z() = FBStringUtil::parseFloat(sTargetRotation);*/
        }

        void FollowSplineCamera::prevNode()
        {
        }

        void FollowSplineCamera::setPosition( const Vector3F &position )
        {
        }

        Vector3F FollowSplineCamera::getPosition() const
        {
            return m_position;
        }

        void FollowSplineCamera::setTargetPosition( const Vector3F &position )
        {
        }

        Vector3F FollowSplineCamera::getTargetPosition() const
        {
            return m_position;
        }

        void FollowSplineCamera::setOrientation( const QuaternionF &orientation )
        {
            m_orientation = orientation;
        }

        QuaternionF FollowSplineCamera::getOrientation() const
        {
            /*Ogre::Quaternion ogreOrientation = m_camera->getOrientation();
            Ogre::Matrix3 ogreMatrix;
            ogreOrientation.ToRotationMatrix(ogreMatrix);

            f32 xAngle;
            f32 yAngle;
            f32 zAngle;
            ogreMatrix.ToEulerAnglesXYZ(xAngle, yAngle, zAngle);*/
            // m_orientation = QuaternionF(&ogreOrientation[0]);
            return m_orientation;
        }

        Vector3F FollowSplineCamera::getDirection() const
        {
            // Ogre::Camera* ogreCamera = 0;
            // Ogre::SceneNode* cameraSceneNode = 0;
            ////ViewOgreWindow* viewWindow = 0;
            ////FBSystem::getSingletonPtr()->getCameraManager()->getViewPortCamera(3,
            ///(void**)&ogreCamera);

            // Ogre::Vector3 dir = ogreCamera->getRealDirection();
            // dir.z = -dir.z;
            // return Vector3F(&dir[0]);

            return Vector3F::zero();
        }

        void FollowSplineCamera::setPropertyValue( const String &name, const String &value )
        {
            // if (name.equals_ignore_case("MoveSpeed"))
            //{
            //	m_totalSplineFollowTime = StringUtil::parseFloat(value);
            // }
        }

        void FollowSplineCamera::setProperties( const Properties &propertyGroup )
        {
            m_propertyGroup = propertyGroup;
        }

        void FollowSplineCamera::getProperties( Properties &propertyGroup ) const
        {
            propertyGroup = m_propertyGroup;
        }

        void FollowSplineCamera::startFollowingSpline( LinearSpline3F &linearSpline,
                                                       RotationalSpline3F &rotationSpline )
        {
            /*if(m_currentSpline.isNull() || spline != m_currentSpline)
            {
                m_currentSpline = spline;
                m_splineTime = 0.0f;

                if(!m_currentSpline.isNull())
                    m_currentSpline->GetChildren(m_splineCtrlNodes);

                m_curNodeIdx = 0;
            }

            m_ctrlPoints.clear();

            SplinePtr cameraSpline = m_currentSpline;
            if(!cameraSpline.isNull())
                cameraSpline->getControlPoints(m_ctrlPoints);
            //m_spline.initialize(m_ctrlPoints);

            if(m_ctrlPoints.empty())
                return;

            Array<Vector3F>& nodePositions = m_ctrlPoints;


            //build spline
            Wm4::Vector3f* ctrlPoints = new Wm4::Vector3f[nodePositions.size()];
            f32* weights = new f32[nodePositions.size()];
            f32* times = new f32[nodePositions.size()];
            s32 degree = nodePositions.size()-1;

            static f32 pointWeight = 0.1f;
            f32 distanceSplineDistance = 0.0f;

            for(u32 curPosIdx=0; curPosIdx<nodePositions.size(); ++curPosIdx)
            {
                Vector3F nodePos0 = nodePositions[curPosIdx];
                ctrlPoints[curPosIdx] = Wm4::Vector3f(nodePos0.X(), nodePos0.Y(), nodePos0.Z());

                weights[curPosIdx] = pointWeight;
                times[curPosIdx] = (f32)curPosIdx / (f32)(nodePositions.size()-1);

                if(curPosIdx > 0)
                {
                    Vector3F prevPosition = nodePositions[curPosIdx-1];
                    distanceSplineDistance += (nodePos0 - prevPosition).length();
                }
            }

            m_totalSplineDistance = distanceSplineDistance;

            s32 numNodes = (s32)(nodePositions.size()) - 1;
            if(numNodes < 0)
                numNodes = 0;

            curve = new Wm4::NaturalSpline3f(Wm4::NaturalSpline3f::BT_FREE, numNodes, times, ctrlPoints);
            //Wm4::SingleCurve3f* curve = new Wm4::NURBSCurve3f(nodePositions.size(), ctrlPoints,
            weights, degree, false, true);
            //Wm4::SingleCurve3f* curve = new Wm4::BezierCurve3f(degree, ctrlPoints);
            curve->SetTimeInterval (0.f, 1.f);


            Array<Vector3F> ctrlRotations;
            if(!cameraSpline.isNull())
                cameraSpline->getControlRotations(ctrlRotations);

            m_rotationalSpline = new Ogre::RotationalSpline;

            for(u32 curPosIdx=0; curPosIdx<ctrlRotations.size(); ++curPosIdx)
            {
                Vector3F targetRotation = ctrlRotations[curPosIdx];

                QuaternionF xAxis;
                QuaternionF yAxis;
                QuaternionF zAxis;
                xAxis.fromAngleAxis(FBMathf::degToRad(targetRotation.X()), Vector3F(1,0,0));
                yAxis.fromAngleAxis(FBMathf::degToRad(targetRotation.Y()), Vector3F(0,1,0));
                zAxis.fromAngleAxis(FBMathf::degToRad(-targetRotation.Z()), Vector3F(0,0,1));	//minus
            because rotation should be clockwise

                QuaternionF final = yAxis*xAxis*zAxis;

                Ogre::Quaternion ogreCamOrientation(final);

                Ogre::Matrix3 rotationMat;
                ogreCamOrientation.ToRotationMatrix(rotationMat);
                rotationMat[0][2] = -rotationMat[0][2];
                rotationMat[1][2] = -rotationMat[1][2];
                rotationMat[2][0] = -rotationMat[2][0];
                rotationMat[2][1] = -rotationMat[2][1];
                ogreCamOrientation.FromRotationMatrix(rotationMat);

                m_rotationalSpline->addPoint(ogreCamOrientation);
            }


            //smgr->setActiveCamera(m_camera);
            m_followSpline = true;

            nextNode();*/
        }

        void FollowSplineCamera::stopFollowingSpline()
        {
            m_followSpline = false;
        }

        Ray3F FollowSplineCamera::getCameraToViewportRay( const Vector2F &screenPosition ) const
        {
            Ray3F ray;

            return ray;
        }

        const Properties &FollowSplineCamera::getPropertyGroup() const
        {
            return m_propertyGroup;
        }

        void FollowSplineCamera::setTranslationSpeed( f32 speed )
        {
            m_translationSpeed = speed;
        }

        f32 FollowSplineCamera::getTranslationSpeed() const
        {
            return m_translationSpeed;
        }

        void FollowSplineCamera::setRotationSpeed( f32 speed )
        {
            m_rotationSpeed = speed;
        }

        f32 FollowSplineCamera::getRotationSpeed() const
        {
            return m_rotationSpeed;
        }

        void FollowSplineCamera::setTotalSplineFollowTime( f32 totalTime )
        {
            m_totalSplineFollowTime = totalTime;
        }

        f32 FollowSplineCamera::getTotalSplineFollowTime() const
        {
            return m_totalSplineFollowTime;
        }

        f32 FollowSplineCamera::getCurSplineTime() const
        {
            return m_splineTime;
        }

        f32 FollowSplineCamera::getCurSplineDistance() const
        {
            return m_splineTime * m_totalSplineDistance;
        }

        f32 FollowSplineCamera::getTotalSplineDistance() const
        {
            return m_totalSplineDistance;
        }
    }  // namespace scene
}  // end namespace fb
