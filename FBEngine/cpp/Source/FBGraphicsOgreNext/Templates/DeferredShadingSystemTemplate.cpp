#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/Templates/DeferredShadingSystemTemplate.h"



namespace fb
{



	DeferredShadingSystemTemplate::DeferredShadingSystemTemplate()
	{
		m_sGBufferCompositorName = "DeferredShading/GBuffer";
		m_sShowLightingCompositorName = "DeferredShading/ShowLit";
		m_sShowNormalsCompositorName = "DeferredShading/ShowNormals";
		m_sShowDepthSpecularCompositorName = "DeferredShading/ShowDepthSpecular";
		m_sShowColourCompositorName = "DeferredShading/ShowColour";
	}

	DeferredShadingSystemTemplate::~DeferredShadingSystemTemplate()
	{

	}

	bool DeferredShadingSystemTemplate::isDirty() const
	{
		return false;
	}

	void DeferredShadingSystemTemplate::setDirty( bool dirty )
	{
	}



} // end namespace fb


