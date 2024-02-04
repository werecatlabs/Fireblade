#ifndef SampleVehicle_h__
#define SampleVehicle_h__

#include <FBCore/Application.h>
#include "FBCore/Core/Parameter.h"
#include "FBCore/Interface/System/IEventListener.h"

namespace fb
{

    class SampleVehicle : public core::Application
    {
    public:
        SampleVehicle();
        ~SampleVehicle() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void reset();

    protected:
        class InputListener : public IEventListener
        {
        public:
            InputListener() = default;
            ~InputListener() override = default;

            void unload( SmartPtr<ISharedObject> data ) override;

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

            bool inputEvent( SmartPtr<IInputEvent> event );
            bool updateEvent( const SmartPtr<IInputEvent> &event );

            void setPriority( s32 priority );

            s32 getPriority() const;

            SmartPtr<SampleVehicle> getOwner() const;

            void setOwner( SmartPtr<SampleVehicle> owner );

        protected:
            SmartPtr<SampleVehicle> m_owner;
            u32 m_priority = 1000;
        };

        void createPlugins() override;

        void createScene() override;

        SmartPtr<InputListener> m_inputListener;

        SmartPtr<scene::IActor> m_boxGround;
        SmartPtr<scene::IActor> m_terrain;

        SmartPtr<scene::IActor> m_cameraActor;
        SmartPtr<scene::IActor> m_vehicleActor;
        SmartPtr<scene::IActor> m_vehicleMesh;
        SmartPtr<IVehicleController> m_vehicleController;

        Array<SmartPtr<scene::IActor>> m_boxes;
    };
}  // end namespace fb

#endif  // Vehicle_h__
