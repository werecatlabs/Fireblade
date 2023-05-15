#ifndef _CLight_H
#define _CLight_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/ILight.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsObjectOgre.h>

namespace fb
{
    namespace render
    {
        class CLightOgre : public CGraphicsObjectOgre<ILight>
        {
        public:
            CLightOgre();
            ~CLightOgre() override;

            void load( SmartPtr<ISharedObject> data ) override;

            void unload( SmartPtr<ISharedObject> data ) override;

            void attachToParent( SmartPtr<ISceneNode> parent ) override;
            void detachFromParent( SmartPtr<ISceneNode> parent ) override;

            void setRenderQueueGroup( u8 renderQueue );

            void setVisibilityFlags( u32 flags ) override;
            u32 getVisibilityFlags() const override;

            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const override;

            void _getObject( void **ppObject ) const override;

            // ILight functions
            void setType( LightTypes type ) override;
            LightTypes getType() const override;

            void setDiffuseColour( const ColourF &colour ) override;
            ColourF getDiffuseColour() const override;

            void setSpecularColour( const ColourF &colour ) override;
            ColourF getSpecularColour() const override;

            void setAttenuation( f32 range, f32 constant, f32 linear, f32 quadratic ) override;
            f32 getAttenuationRange() const override;
            f32 getAttenuationConstant() const override;
            f32 getAttenuationLinear() const override;
            f32 getAttenuationQuadric() const override;

            void setDirection( const Vector3F &vec ) override;

            Vector3F getDirection() const override;

            Ogre::Light *getLight() const;
            void setLight(Ogre::Light *light);

            FB_CLASS_REGISTER_DECL;

        protected:
            class CLightStateListener : public StateListener
            {
            public:
                CLightStateListener() = default;
                ~CLightStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CLightOgre *getOwner() const;
                void setOwner( CLightOgre *owner );

            private:
                CLightOgre *m_owner = nullptr;
            };

            void setupStateObject() override;

            Ogre::SceneNode *m_dummyNode = nullptr;
            Ogre::Light *m_light = nullptr;

            String m_name;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
