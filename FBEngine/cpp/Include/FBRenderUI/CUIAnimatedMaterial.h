#ifndef CAnimatedMaterial_h__
#define CAnimatedMaterial_h__

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIAnimatedMaterial.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUIAnimatedMaterial : public CUIElement<IUIAnimatedMaterial>
        {
        public:
            CUIAnimatedMaterial();
            ~CUIAnimatedMaterial() override;

            void initialise( SmartPtr<IUIElement> &parent ) override;
            void initialise( SmartPtr<IUIElement> &parent, const Properties &properties ) override;
            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            void update() override;

            void onToggleVisibility() override;

            void setMaterialName( const String &materialName ) override;
            String getMaterialName() const override;

            void play() override;
            void pause() override;
            void stop() override;

            void setPosition( const Vector2F &position ) override;

            void setSize( const Vector2F &size ) override;

        private:
            void setFrameTime( f32 time );
            f32 getFrameTime() const;
            // void setAnimationFunction(Ogre::ControllerFunctionRealPtr func);
            u32 getCurrentFrame() const;
            u32 getNumFrames() const;
            void OnEnterFrame() const;

            // class AnimationFunction : public Ogre::ControllerFunction<f32>
            //{
            // public:
            //	AnimationFunction(f32 sequenceTime, f32 timeOffset = 0.0f);

            //	/** Overridden function. */
            //	f32 calculate(f32 source);

            //	/** Set the time value manually. */
            //	void setTime(f32 timeVal);

            //	f32 getTime() const { return mTime; }

            //	/** Set the sequence duration value manually. */
            //	void setSequenceTime(f32 seqVal);

            //	bool isPlaying() const { return m_isPlaying; }
            //	void setIsPlaying(bool val) { m_isPlaying = val; }

            //	bool getLoop() const { return m_loop; }
            //	void setLoop(bool val) { m_loop = val; }

            // protected:
            //	f32 mSeqTime;
            //	f32 mTime;
            //	bool m_isPlaying;
            //	bool m_loop;
            // };

            f32 m_time;
            u32 m_currentFrame;
            u32 m_numFrames;
            bool m_isPlaying;

            SmartPtr<render::IOverlayElementContainer> m_container;
            // AnimationFunction* m_animationFunction;
        };

    } // end namespace ui
}     // end namespace fb

#endif  // CAnimatedMaterial_h__
