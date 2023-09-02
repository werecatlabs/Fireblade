#ifndef _FB_CGraphicsMeshOgre_H_
#define _FB_CGraphicsMeshOgre_H_

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsMesh.h>
#include <FBCore/Interface/Script/IScriptReceiver.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.h>
#include <OgreMesh.h>

namespace fb
{
    namespace render
    {

        class CGraphicsMeshOgre : public CGraphicsObjectOgre<IGraphicsMesh>
        {
        public:
            CGraphicsMeshOgre();
            ~CGraphicsMeshOgre() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            void registerForUpdates( bool registerObject );
            bool isRegisteredForUpdates();

            void setTestOcclusion( bool testOcclusion );
            bool getTestOcclusion() const;

            void setIsOccluder( bool occluder );
            bool isOccluder() const;

            void detachFromParent();
            void attachToParent( SmartPtr<ISceneNode> parent );

            void setMaterialName( const String &materialName, s32 index = -1 ) override;
            String getMaterialName( s32 index = -1 ) const override;

            /** @copydoc IGraphicsMesh::setMaterial */
            void setMaterial( SmartPtr<IMaterial> material, s32 index = -1 ) override;

            /** @copydoc IGraphicsMesh::getMaterial */
            SmartPtr<IMaterial> getMaterial( s32 index = -1 ) const override;

            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;

            void _getObject( void **ppObject ) const override;

            void setHardwareAnimationEnabled( bool enabled ) override;

            void checkVertexProcessing() override;
            SmartPtr<IAnimationController> getAnimationController() override;

            Ogre::Entity *getEntity() const;

            void handleEvent( SmartPtr<IEvent> event );

            String getMeshName() const override;
            void setMeshName( const String &meshName ) override;

            /** Gets the data as a properties object.
            @return The data as a properties object.
            */
            SmartPtr<Properties> getProperties() const override;

            /** Sets the data as a properties object.
            @param properties The properties object.
            */
            void setProperties( SmartPtr<Properties> properties ) override;

            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class ScriptReceiver : public IScriptReceiver
            {
            public:
                ScriptReceiver( CGraphicsMeshOgre *meshObject );

                s32 setProperty( hash_type hash, const String &value ) override;
                s32 setProperty( hash_type hash, const Parameter &param ) override;
                s32 getProperty( hash_type id, Parameter &param ) const override;

            protected:
                CGraphicsMeshOgre *m_meshObject = nullptr;
            };

            class MeshStateListener : public StateListener
            {
            public:
                MeshStateListener() = default;
                ~MeshStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            class MaterialStateListener : public IStateListener
            {
            public:
                MaterialStateListener() = default;
                MaterialStateListener( CGraphicsMeshOgre *owner );
                ~MaterialStateListener() override;

                void unload(SmartPtr<ISharedObject> data) override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<CGraphicsMeshOgre> getOwner() const;
                void setOwner(  SmartPtr<CGraphicsMeshOgre> owner );

            protected:
                AtomicSmartPtr<CGraphicsMeshOgre> m_owner;
            };

            void materialLoaded( SmartPtr<IMaterial> material );

            Ogre::MeshPtr createMesh( const String &meshName );
            void setupStateObject() override;

            SmartPtr<IScriptInvoker> m_scriptInvoker;
            SmartPtr<IScriptReceiver> m_scriptReceiver;

            SmartPtr<IStateListener> m_materialStateListener;

            SmartPtr<IAnimationController> m_animationController;

            SmartPtr<IMaterial> m_material;
            Array<SmartPtr<IMaterial>> m_materials;

            Ogre::Entity *m_entity = nullptr;

            atomic_u32 m_lastMaterialUpdate;
            atomic_u32 m_lastMaterialRequest;

            bool m_checkVertProcessing;
            bool m_hardwareAnimationEnabled;

            String m_meshName;
            String m_materialName;
        };
    }  // end namespace render
}  // end namespace fb

#endif
