#ifndef UserComponent_h__
#define UserComponent_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Interface/Script/IScriptReceiver.h>

namespace fb
{
    namespace scene
    {
        class UserComponent : public Component
        {
        public:
            static const hash_type UPDATE_HASH;

            /** Default constructor. */
            UserComponent();

            /** Virtual destructor. */
            ~UserComponent() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            static void updateComponents();

            void update() override;

            SmartPtr<IScriptInvoker> getInvoker() const;

            void setInvoker( SmartPtr<IScriptInvoker> invoker );

            SmartPtr<IScriptReceiver> getReceiver() const;

            void setReceiver( SmartPtr<IScriptReceiver> receiver );

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            bool getUpdateInEditMode() const;

            void setUpdateInEditMode( bool updateInEditMode );

            SmartPtr<IScriptClass> getScriptClass() const;

            void setScriptClass( SmartPtr<IScriptClass> scriptClass );

            String getClassName() const;

            void setClassName( const String &className );

            FB_CLASS_REGISTER_DECL;

        protected:
            struct usercomponent_data
            {
                bool dirty = false;
                UserComponent *owner = nullptr;
            };

            class ScriptReceiver : public IScriptReceiver
            {
            public:
                ScriptReceiver();
                ScriptReceiver( UserComponent *owner );
                ~ScriptReceiver() override;

                s32 setProperty( hash_type hash, const String &value ) override;

                s32 getProperty( hash_type hash, String &value ) const override;

                s32 setProperty( hash_type hash, const Parameter &param ) override;

                s32 setProperty( hash_type hash, const Parameters &params ) override;

                s32 setProperty( hash_type hash, void *param ) override;

                s32 getProperty( hash_type hash, Parameter &param ) const override;

                s32 getProperty( hash_type hash, Parameters &params ) const override;

                s32 getProperty( hash_type hash, void *param ) const override;

                s32 callFunction( hash_type hash, const Parameters &params,
                                  Parameters &results ) override;

                s32 callFunction( hash_type hash, SmartPtr<ISharedObject> object,
                                  Parameters &results ) override;

                UserComponent *getOwner() const;

                void setOwner( UserComponent *owner );

            private:
                UserComponent *m_owner = nullptr;
            };

            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            void createScriptData();
            void destroyScriptData();

            bool m_updateInEditMode = false;

            String m_className;

            SmartPtr<IScriptClass> m_scriptClass;

            /// Used to call script functions.
            SmartPtr<IScriptInvoker> m_invoker;

            /// Used to receive script calls.
            SmartPtr<IScriptReceiver> m_receiver;

            /// The data used by the script system.
            SmartPtr<IScriptData> m_scriptData;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // UserComponent_h__
