#include <FBCore/FBCorePCH.h>
#include <FBCore/AI/VehicleAi.h>
#include <FBCore/AI/VehicleAiManager.h>
#include <FBCore/AI/Learning.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/Vehicle/IVehicleController.h>

namespace fb
{
    VehicleAi::VehicleAi() = default;

    VehicleAi::~VehicleAi() = default;

    void VehicleAi::update( f64 t, f64 dt )
    {
    }

    void VehicleAi::load( SmartPtr<ISharedObject> data )
    {
    }

    void VehicleAi::reload( SmartPtr<ISharedObject> data )
    {
    }

    void VehicleAi::unload( SmartPtr<ISharedObject> data )
    {
    }

    void VehicleAi::reset()
    {
    }

    auto VehicleAi::getVehicleController() const -> SmartPtr<IVehicleController>
    {
        return m_vehicleController;
    }

    void VehicleAi::setVehicleController( SmartPtr<IVehicleController> val )
    {
        m_vehicleController = val;
    }

    auto VehicleAi::getVehicleManager() const -> SmartPtr<IVehicleAiManager>
    {
        //auto p = fb::dynamic_pointer_cast<IVehicleAiManager>(m_vehicleManager);
        //return p;

        return nullptr;
    }

    void VehicleAi::setVehicleManager( SmartPtr<IVehicleAiManager> val )
    {
        m_vehicleManager = fb::dynamic_pointer_cast<VehicleAiManager>( val );

        //u32 ulNumberOfActionVariables = m_vehicleManager->getNumberOfActionVariables();
        //u32 ulNumberOfStateVariables = m_vehicleManager->getNumberOfStateVariables();

        //auto pMLP = m_vehicleManager->getMLP();

        //m_learning = SmartPtr<CQLearning>(new CQLearning(ulNumberOfActionVariables, ulNumberOfStateVariables, pMLP.get()));
    }
}  // namespace fb
