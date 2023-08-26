#include <FBCore/FBCorePCH.h>
#include <FBCore/AI/VehicleAiManager.h>
#include <FBCore/AI/MLP.h>
#include <FBCore/Interface/Ai/ITrack.h>
#include <FBCore/Interface/Vehicle/IVehicleController.h>
#include <algorithm>

namespace fb
{
    VehicleAiManager::VehicleAiManager()
    {
        setNumberOfActionVariables( 6 );
        setNumberOfStateVariables( 1 );

        //m_pMLP = SmartPtr<CMLP>(new CMLP(getNumberOfActionVariables() + getNumberOfStateVariables(), 10, 1));
        m_pMLP->Reset();
        m_pMLP->Load( "VehicleAi.mlp" );

        //allocate memory for Ai actions
        ppdActions = new double *[3];
        for( int i = 0; i < 3; i++ )
        {
            ppdActions[i] = new double[1];
        }

        //ai actions
        ppdActions[0][0] = -0.3;
        ppdActions[1][0] = +0.3;
        ppdActions[2][0] = 0.0;
    }

    VehicleAiManager::~VehicleAiManager()
    {
        m_pMLP->Save( "VehicleAi.mlp" );
        m_pMLP = nullptr;

        //deallocate the memory used to store the state Array
        for( int i = 0; i < 3; i++ )
        {
            delete[] ppdActions[i];
        }

        delete[] ppdActions;
    }

    Array<SmartPtr<IVehicleController>> VehicleAiManager::getVehicles() const
    {
        return m_vehicles;
    }

    void VehicleAiManager::setVehicles( Array<SmartPtr<IVehicleController>> val )
    {
        m_vehicles = val;
    }

    SmartPtr<ITrack> VehicleAiManager::getTrack() const
    {
        return m_track;
    }

    void VehicleAiManager::setTrack( SmartPtr<ITrack> val )
    {
        m_track = val;
    }

    std::shared_ptr<MLP> VehicleAiManager::getMLP() const
    {
        return m_pMLP;
    }

    void VehicleAiManager::setMLP( std::shared_ptr<MLP> val )
    {
        m_pMLP = val;
    }

    unsigned long VehicleAiManager::getNumberOfActionVariables() const
    {
        return m_ulNumberOfActionVariables;
    }

    void VehicleAiManager::setNumberOfActionVariables( unsigned long val )
    {
        m_ulNumberOfActionVariables = val;
    }

    unsigned long VehicleAiManager::getNumberOfStateVariables() const
    {
        return m_ulNumberOfStateVariables;
    }

    void VehicleAiManager::setNumberOfStateVariables( unsigned long val )
    {
        m_ulNumberOfStateVariables = val;
    }

    void VehicleAiManager::addVehicle( SmartPtr<IVehicleController> vehicle )
    {
        m_vehicles.push_back( vehicle );
    }

    void VehicleAiManager::removeVehicle( SmartPtr<IVehicleController> vehicle )
    {
        auto it = std::find( m_vehicles.begin(), m_vehicles.end(), vehicle );
        if( it != m_vehicles.end() )
        {
            m_vehicles.erase( it );
        }
    }
}  // namespace fb
