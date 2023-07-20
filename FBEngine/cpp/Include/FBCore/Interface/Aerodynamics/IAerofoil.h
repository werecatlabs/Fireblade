#ifndef IAerofoil_h__
#define IAerofoil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/LinearSpline1.h>

namespace fb
{

    /** Interface for an aerofoil class. */
    class IAerofoil : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IAerofoil() override = default;

        // virtual SmartPtr<LinearSpline1<real_Num>>& getCL() = 0;
        // virtual const SmartPtr<LinearSpline1<real_Num>>& getCL() const = 0;
        // virtual void setCL(const SmartPtr<LinearSpline1<real_Num>>& val) = 0;

        // virtual SmartPtr<LinearSpline1<real_Num>>& getCD() = 0;
        // virtual const SmartPtr<LinearSpline1<real_Num>>& getCD() const = 0;
        // virtual void setCD(const SmartPtr<LinearSpline1<real_Num>>& val) = 0;

        // virtual SmartPtr<LinearSpline1<real_Num>>& getCM() = 0;
        // virtual const SmartPtr<LinearSpline1<real_Num>>& getCM() const = 0;
        // virtual void setCM(const SmartPtr<LinearSpline1<real_Num>>& val) = 0;

        virtual SmartPtr<IAircraft> &getAircraft() = 0;
        virtual const SmartPtr<IAircraft> &getAircraft() const = 0;
        virtual void setAircraft( SmartPtr<IAircraft> val ) = 0;

        virtual bool getReverseValues() const = 0;
        virtual void setReverseValues( bool val ) = 0;
    };

}  // end namespace fb

#endif  // IAerofoil_h__
