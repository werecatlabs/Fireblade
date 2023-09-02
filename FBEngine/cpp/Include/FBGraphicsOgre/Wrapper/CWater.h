#ifndef WaterStandard_h__
#define WaterStandard_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IWater.h>


namespace fb
{
    namespace render
    {

        class CWater : public IWater
        {
        public:
            CWater();
            ~CWater() override;

            void initialise( SmartPtr<scene::IDirector> objectTemplate );

            void update() override;

            SmartPtr<IGraphicsScene> getSceneManager() const override;
            void setSceneManager( SmartPtr<IGraphicsScene> sceneMgr ) override;

            SmartPtr<ICamera> getCamera() const override;
            void setCamera( SmartPtr<ICamera> camera ) override;

            SmartPtr<IViewport> getViewport() const override;
            void setViewport( SmartPtr<IViewport> viewport ) override;

            WaterMesh *getWaterMesh() const;
            void setWaterMesh( WaterMesh *val );

            Vector3F getPosition() const override;
            void setPosition( const Vector3F &position ) override;

        protected:
            SmartPtr<ISceneNode> m_sceneNode;
            SmartPtr<IGraphicsMesh> m_mesh;

            SmartPtr<IGraphicsScene> m_sceneMgr;
            SmartPtr<ICamera> m_camera;
            SmartPtr<IViewport> m_viewport;
            Ogre::FrameListener *m_frameListener = nullptr;
            WaterMesh *m_waterMesh = nullptr;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // WaterStandard_h__
