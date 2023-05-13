#ifndef SampleVehicle_h__
#define SampleVehicle_h__

#include <FBApplication/CApplicationClient.h>

namespace fb
{
    
    class SampleVehicle : public application::CApplicationClient
    {
    public:
        SampleVehicle();
        ~SampleVehicle() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

    protected:
        void createPlugins() override;

        void createVehicle();

        void createScene() override;

        SmartPtr<scene::IActor> m_boxGround;
        SmartPtr<scene::IActor> m_terrain;

        SmartPtr<scene::IActor> m_cameraActor;
        SmartPtr<scene::IActor> m_vehicleActor;
        SmartPtr<scene::IActor> m_vehicleMesh;
        SmartPtr<IVehicleController> m_vehicleController;

        Array<SmartPtr<scene::IActor>> m_boxes;
    };
} // end namespace fb

#endif  // Vehicle_h__
