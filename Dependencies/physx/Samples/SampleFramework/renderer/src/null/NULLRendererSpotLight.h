/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

#ifndef NULL_RENDERER_SPOT_LIGHT_H
#define NULL_RENDERER_SPOT_LIGHT_H

#include <RendererConfig.h>
#include <RendererSpotLight.h>


namespace SampleRenderer
{

	class NullRendererSpotLight : public RendererSpotLight
	{
	public:
		NullRendererSpotLight (const RendererSpotLightDesc &desc);
		virtual ~NullRendererSpotLight (void);

		virtual void bind(void) const;

	private:
	};

} // namespace SampleRenderer

#endif
