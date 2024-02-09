#ifndef CGraphicsScene_h__
#define CGraphicsScene_h__

#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>

namespace fb
{
    namespace render
    {
        class GraphicsScene : public SharedGraphicsObject<IGraphicsScene>
        {
        public:
            GraphicsScene();
            ~GraphicsScene() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            String getName() const override;

            void setName( const String &name ) override;

            String getType() const;

            void setType( const String &type );

            ColourF getUpperHemisphere() const override;
            void setUpperHemisphere( const ColourF &upperHemisphere ) override;

            ColourF getLowerHemisphere() const override;
            void setLowerHemisphere( const ColourF &lowerHemisphere ) override;

            Vector3<real_Num> getHemisphereDir() const override;
            void setHemisphereDir( Vector3<real_Num> hemisphereDir ) override;

            f32 getEnvmapScale() const override;
            void setEnvmapScale( f32 envmapScale ) override;

            void lock() override;

            void unlock() override;

            FB_CLASS_REGISTER_DECL;

        protected:
            ColourF m_upperHemisphere = ColourF::White;
            ColourF m_lowerHemisphere = ColourF::White;
            Vector3<real_Num> m_hemisphereDir = Vector3<real_Num>::unitY();
            f32 m_envmapScale = 1.0f;

            String m_type;

            mutable RecursiveMutex m_mutex;
        };
    } // namespace render
}     // namespace fb

#endif  // CGraphicsScene_h__
