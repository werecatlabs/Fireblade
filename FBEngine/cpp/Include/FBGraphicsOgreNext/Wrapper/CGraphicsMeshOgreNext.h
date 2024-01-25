#ifndef _FB_CGraphicsMeshOgre_H_
#define _FB_CGraphicsMeshOgre_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsMesh.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsObjectOgreNext.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <OgreMesh.h>

namespace fb
{
    namespace render
    {
        class CGraphicsMeshOgreNext : public CGraphicsObjectOgreNext<IGraphicsMesh>
        {
        public:
            CGraphicsMeshOgreNext();
            CGraphicsMeshOgreNext( const CGraphicsMeshOgreNext &other ) = delete;
            CGraphicsMeshOgreNext( SmartPtr<IGraphicsScene> creator );

            ~CGraphicsMeshOgreNext() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

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

            String getMeshName() const override;
            void setMeshName( const String &meshName ) override;

            /** @copydoc IGraphicsObject::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IGraphicsObject::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IGraphicsObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            Ogre::v1::Entity *getEntity() const;

            Ogre::Item *getItem() const;

            void setItem( Ogre::Item *item );

            SmartPtr<ISkeleton> getSkeleton() const;

            void setSkeleton( SmartPtr<ISkeleton> skeleton );

            FB_CLASS_REGISTER_DECL;

        protected:
            class MeshStateListener : public CStateListener
            {
            public:
                MeshStateListener() = default;
                MeshStateListener( CGraphicsMeshOgreNext *owner );
                ~MeshStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;
            };

            class MaterialStateListener : public IStateListener
            {
            public:
                MaterialStateListener() = default;
                MaterialStateListener( CGraphicsMeshOgreNext *owner );
                ~MaterialStateListener() override;

                void unload( SmartPtr<ISharedObject> data ) override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<CGraphicsMeshOgreNext> getOwner() const;
                void setOwner( SmartPtr<CGraphicsMeshOgreNext> owner );

            protected:
                AtomicWeakPtr<CGraphicsMeshOgreNext> m_owner;
            };

            class MaterialEventListener : public IEventListener
            {
            public:
                MaterialEventListener();
                ~MaterialEventListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                SmartPtr<CGraphicsMeshOgreNext> getOwner() const;

                void setOwner( SmartPtr<CGraphicsMeshOgreNext> owner );

            protected:
                SmartPtr<CGraphicsMeshOgreNext> m_owner;
            };

            void setupStateObject() override;

            void materialLoaded( SmartPtr<IMaterial> material );

            Ogre::MeshPtr createMesh( const String &meshName );

            SmartPtr<IStateListener> m_materialStateListener;

            SmartPtr<IAnimationController> m_animationController;

            SmartPtr<ISkeleton> m_skeleton;

            SmartPtr<IMaterial> m_material;
            Array<SmartPtr<IMaterial>> m_materials;

            SmartPtr<MaterialEventListener> m_materialSharedObjectListener;

            Ogre::v1::Entity *m_entity = nullptr;
            Atomic<Ogre::Item *> m_item = nullptr;

            int m_checkVertProcessing = 0;
            int m_hardwareAnimationEnabled = 0;

            String m_meshName;
            String m_materialName;
        };
    } // end namespace render
}     // end namespace fb

#endif
