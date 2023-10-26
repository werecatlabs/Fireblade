#ifndef DeferredShadingSystemTemplate_h__
#define DeferredShadingSystemTemplate_h__



#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrequisites.h>
#include <FBCore/Interface/Scene/IDirector.h>
#include <FBCore/Memory/CSharedObject.h>



namespace fb
{


	class DeferredShadingSystemTemplate : public CSharedObject<scene::IDirector>
	{
	public:
		DeferredShadingSystemTemplate();
		~DeferredShadingSystemTemplate();

		String getGBufferCompositorName() const { return m_sGBufferCompositorName; }
		void setGBufferCompositorName(const String& val) { m_sGBufferCompositorName = val; }

		String getShowLightingCompositorName() const { return m_sShowLightingCompositorName; }
		void setShowLightingCompositorName(const String& val) { m_sShowLightingCompositorName = val; }

		String getShowNormalsCompositorName() const { return m_sShowNormalsCompositorName; }
		void setShowNormalsCompositorName(const String& val) { m_sShowNormalsCompositorName = val; }

		String getShowDepthSpecularCompositorName() const { return m_sShowDepthSpecularCompositorName; }
		void setShowDepthSpecularCompositorName(const String& val) { m_sShowDepthSpecularCompositorName = val; }

		String getShowColourCompositorName() const { return m_sShowColourCompositorName; }
		void setShowColourCompositorName(const String& val) { m_sShowColourCompositorName = val; }

		String getLightCompositionPassName() const { return m_sLightCompositionPassName; }
		void setLightCompositionPassName(const String& val) { m_sLightCompositionPassName = val; }

		String getRenderTargetName() const { return m_renderTargetName; }
		void setRenderTargetName(const String& val) { m_renderTargetName = val; }

		bool isDirty() const;
		void setDirty( bool dirty );

	protected:
		String m_sGBufferCompositorName;
		String m_sShowLightingCompositorName;
		String m_sShowNormalsCompositorName;
		String m_sShowDepthSpecularCompositorName;
		String m_sShowColourCompositorName;

		String m_sLightCompositionPassName;

		String m_renderTargetName;
	};



} // end namespace fb



#endif // DeferredShadingSystemTemplate_h__


