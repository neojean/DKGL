//
//  File: DKGraphicsDevice.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2017 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKFoundation.h"
#include "DKCommandQueue.h"
#include "DKShaderModule.h"
#include "DKShader.h"
#include "DKRenderPipeline.h"
#include "DKComputePipeline.h"
#include "DKPipelineReflection.h"
#include "DKGpuBuffer.h"
#include "DKTexture.h"
#include "DKGpuResource.h"

namespace DKFramework
{
	class DKGraphicsDeviceInterface;
	/// @brief Graphics device context
	class DKGL_API DKGraphicsDevice : public DKSharedInstance<DKGraphicsDevice>
	{
	public:
		DKGraphicsDevice();
		~DKGraphicsDevice();

		DKObject<DKCommandQueue> CreateCommandQueue(uint32_t queueTypeFlags);
		DKObject<DKShaderModule> CreateShaderModule(DKShader*);

		DKObject<DKRenderPipelineState> CreateRenderPipeline(const DKRenderPipelineDescriptor&, DKRenderPipelineReflection* reflection = NULL);
		DKObject<DKComputePipelineState> CreateComputePipeline(const DKComputePipelineDescriptor&, DKComputePipelineReflection* reflection = NULL);

		DKObject<DKGpuBuffer> CreateBuffer(size_t, DKGpuBuffer::StorageMode, DKCpuCacheMode);
		DKObject<DKTexture> CreateTexture(const DKTextureDescriptor&);

	private:
		DKGraphicsDeviceInterface* impl;
		friend class DKGraphicsDeviceInterface;
	};
}
