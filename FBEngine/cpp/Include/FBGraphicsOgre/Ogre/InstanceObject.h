#ifndef InstanceObject_h__
#define InstanceObject_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/FBCoreTypes.h>
#include <OgreVector3.h>

namespace fb
{
    namespace render
    {

        class CInstanceObjectOld
        {
        public:
            CInstanceObjectOld();
            ~CInstanceObjectOld();

            void add( Ogre::InstancedEntity *entity );

            bool isVisible() const;
            void setVisible( bool visible );

            const Ogre::Vector3 &getPosition() const;
            void setPosition( const Ogre::Vector3 &val );

            Ogre::InstancedEntity *getBillboard() const;
            void setBillboard( Ogre::InstancedEntity *val );

            SmartPtr<IStateContext> getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> val );

            class InstanceObjectStateListener : public SharedObject<IStateListener>
            {
            public:
                InstanceObjectStateListener( CInstanceObjectOld *instanceObject );
                ~InstanceObjectStateListener();

                void handleStateChanged( const SmartPtr<IStateMessage> &message );
                void handleStateChanged( SmartPtr<IState> &state );
                void handleQuery( SmartPtr<IStateQuery> &query );

                CInstanceObjectOld *m_instanceObject;
            };

            Ogre::Vector3 m_position;
            f32 m_lastDistance;
            u32 m_nextUpdate;
            atomic_bool m_isVisible;
            Array<Ogre::InstancedEntity *> m_instanceEntities;
            Ogre::InstancedEntity *m_billboard;

            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;
        };

    }  // namespace render
}  // end namespace fb

#endif  // InstanceObject_h__
