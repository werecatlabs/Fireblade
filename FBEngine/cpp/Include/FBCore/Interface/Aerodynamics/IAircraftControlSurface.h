#ifndef IAircraftControlSurface_h__
#define IAircraftControlSurface_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Vehicle/IVehicleComponent.h>
#include <FBCore/Math/LinearSpline1.h>

namespace fb
{
    
    class IAircraftControlSurface : public IVehicleComponent
    {
    public:
        ~IAircraftControlSurface() override = default;

        virtual s32 getSurfaceId() const = 0;
        virtual void setSurfaceId( s32 val ) = 0;

        virtual real_Num getCurrentDeflection() const = 0;
        virtual void setCurrentDeflection( real_Num val ) = 0;

        virtual bool isReversed() const = 0;
        virtual void setReversed( bool val ) = 0;

        virtual Array<bool> &getAffectedSections() = 0;
        virtual const Array<bool> &getAffectedSections() const = 0;
        virtual void setAffectedSections( const Array<bool> &val ) = 0;

        virtual bool isAffectedSection( int index ) const = 0;

        virtual void modifyWingGeometry( s32 sectionIndex, Vector3<real_Num> &pointA,
                                         Vector3<real_Num> &pointB, Vector3<real_Num> &pointC,
                                         Vector3<real_Num> &pointD ) = 0;

        virtual void modifyWingGeometry( s32 sectionIndex, Vector3<real_Num> &pointA,
                                         Vector3<real_Num> &pointB, Vector3<real_Num> &pointC,
                                         Vector3<real_Num> &pointD, real_Num t ) = 0;

        virtual void modifyMainWingGeometry( s32 SectionIndex, Vector3<real_Num> &pointA,
                                             Vector3<real_Num> &pointB, Vector3<real_Num> &pointC,
                                             Vector3<real_Num> &pointD, real_Num t ) = 0;

        virtual SmartPtr<LinearSpline1<real_Num>> getClLookup() const = 0;
        virtual void setClLookup( SmartPtr<LinearSpline1<real_Num>> linearSpline1 ) = 0;

        virtual SmartPtr<LinearSpline1<real_Num>> getCdLookup() const = 0;
        virtual void setCdLookup( SmartPtr<LinearSpline1<real_Num>> linearSpline1 ) = 0;

        virtual SmartPtr<LinearSpline1<real_Num>> getCmLookup() const = 0;
        virtual void setCmLookup( SmartPtr<LinearSpline1<real_Num>> linearSpline1 ) = 0;

        virtual real_Num getClMultiplier() const = 0;
        virtual void setClMultiplier( real_Num clMultiplier ) = 0;

        virtual real_Num getCdMultiplier() const = 0;
        virtual void setCdMultiplier( real_Num cdMultiplier ) = 0;

        virtual real_Num getCmMultiplier() const = 0;
        virtual void setCmMultiplier( real_Num cmMultiplier ) = 0;

        virtual real_Num getStallControlCL() const = 0;
        virtual void setStallControlCL( real_Num val ) = 0;

        virtual real_Num getStallControlCD() const = 0;
        virtual void setStallControlCD( real_Num val ) = 0;

        virtual real_Num getStallControlCM() const = 0;
        virtual void setStallControlCM( real_Num val ) = 0;

        virtual real_Num getStallThreshold() const = 0;
        virtual void setStallThreshold( real_Num val ) = 0;

        virtual real_Num getMinDeflectionDegrees() const = 0;
        virtual void setMinDeflectionDegrees( real_Num minDeflectionDegrees ) = 0;

        virtual real_Num getMaxDeflectionDegrees() const = 0;
        virtual void setMaxDeflectionDegrees( real_Num maxDeflectionDegrees ) = 0;

        virtual real_Num getAoAMultiplier() const = 0;
        virtual void setAoAMultiplier( real_Num val ) = 0;

        virtual SmartPtr<IAircraftWing> getMainWing() const = 0;
        virtual void setMainWing( SmartPtr<IAircraftWing> val ) = 0;

        virtual SmartPtr<IAircraftWing> getControlWing() const = 0;
        virtual void setControlWing( SmartPtr<IAircraftWing> val ) = 0;

        virtual real_Num getRootHingeDistanceFromTrailingEdge() const = 0;
        virtual void setRootHingeDistanceFromTrailingEdge( real_Num val ) = 0;

        virtual real_Num getTipHingeDistanceFromTrailingEdge() const = 0;
        virtual void setTipHingeDistanceFromTrailingEdge( real_Num val ) = 0;
    };
}  // namespace fb

#endif  // IAircraftControlSurface_h__
