//
//  File: RenderPipelineState.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2016-2017 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../GraphicsAPI.h"
#if DKGL_ENABLE_VULKAN
#include <vulkan/vulkan.h>

#include "../../DKRenderPipeline.h"
#include "../../DKGraphicsDevice.h"

namespace DKFramework
{
	namespace Private
	{
		namespace Vulkan
		{
			class RenderPipelineState : public DKRenderPipelineState
			{
			public:
				RenderPipelineState(DKGraphicsDevice*, VkPipeline, VkPipelineLayout, VkRenderPass);
				~RenderPipelineState(void);

				DKGraphicsDevice* Device(void) override { return device; }

				DKObject<DKGraphicsDevice> device;
				VkPipeline pipeline;
				VkPipelineLayout layout;
				VkRenderPass renderPass;
			};
		}
	}
}

#endif //#if DKGL_ENABLE_VULKAN
