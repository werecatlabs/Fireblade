#ifndef CVehicleAi_h__
#define CVehicleAi_h__

#include <FBCore/Interface/Ai/IVehicleAi.h>
#include <memory>

class Learning;

namespace fb
{
    class VehicleAi : public IVehicleAi
    {
    public:
        class VehicleAiAction
        {
        public:
            double steeringAngle;
            double throttle;  //% ranging from 0.0 - 1.0
        };

        class DynamicVehicleAiState
        {
        public:
            double pdGameState[7];
        };

        //class VehicleRecord
        //{
        //public:
        //	VehicleRecord(Vehicle *vehicle) :
        //		vehicle(vehicle) {
        //		bonus = 0.0;
        //	}

        //	bool HasCrashed() { return hasCrashed; };
        //	bool HasCompletedTrack() { return false; };

        //	Vehicle *vehicle;
        //	double bonus;
        //	bool hasCrashed;
        //	CQLearning *pQLearning;
        //	DynamicVehicleAiState m_AiStateT0;		//current state of vehicle
        //};

        VehicleAi();
        ~VehicleAi() override;

        void update( f64 t, f64 dt );
        void load( SmartPtr<ISharedObject> data ) override;
        void reload( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void reset();

        SmartPtr<IVehicleController> getVehicleController() const override;
        void setVehicleController( SmartPtr<IVehicleController> val ) override;

        SmartPtr<IVehicleAiManager> getVehicleManager() const override;
        void setVehicleManager( SmartPtr<IVehicleAiManager> val ) override;

    protected:
        SmartPtr<IVehicleController> m_vehicleController;
        SmartPtr<IVehicleAiManager> m_vehicleManager;
        std::shared_ptr<Learning> m_learning;
    };
}  // end namespace fb

#endif  // CVehicleAi_h__
