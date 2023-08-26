#ifndef CVehicleAiManager_h__
#define CVehicleAiManager_h__

#include <FBCore/Interface/Ai/IVehicleAiManager.h>
#include <memory>

class MLP;

namespace fb
{
    class VehicleAiManager : public IVehicleAiManager
    {
    public:
        VehicleAiManager();
        ~VehicleAiManager() override;

        void addVehicle( SmartPtr<IVehicleController> vehicle ) override;
        void removeVehicle( SmartPtr<IVehicleController> vehicle ) override;

        Array<SmartPtr<IVehicleController>> getVehicles() const override;
        void setVehicles( Array<SmartPtr<IVehicleController>> val );

        SmartPtr<ITrack> getTrack() const;
        void setTrack( SmartPtr<ITrack> val );

        std::shared_ptr<MLP> getMLP() const;
        void setMLP( std::shared_ptr<MLP> val );

        unsigned long getNumberOfActionVariables() const;
        void setNumberOfActionVariables( unsigned long val );

        unsigned long getNumberOfStateVariables() const;
        void setNumberOfStateVariables( unsigned long val );

    protected:
        Array<SmartPtr<IVehicleController>> m_vehicles;

        //Has value +/- depending on whether the AI is steering left or right
        double dSteeringAction;

        //store track segments
        SmartPtr<ITrack> m_track;

        //The multilayer perceptron neural network used to relate
        //state-action pairs to expect long term discounted rewards
        std::shared_ptr<MLP> m_pMLP;

        //Learing
        //The number of variables that is used to represent the state of teh game world to the AI and the number
        //of variables used to represent the actions in can take in each state
        unsigned long m_ulNumberOfActionVariables;
        unsigned long m_ulNumberOfStateVariables;

        //Parameters that control learning
        double dLearningRateDecayFactor;
        double dBestActionSelectionProbability;
        double dDiscountFactor;

        //Actions
        double **ppdActions;
    };
}  // end namespace fb

#endif  // CVehicleAiManager_h__
