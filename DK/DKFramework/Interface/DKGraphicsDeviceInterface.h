//
//  File: DKGraphicsDeviceInterface.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKGraphicsDevice.h"

namespace DKFramework
{
	/// @brief Interface for graphics device (GPU) control.
	class DKGraphicsDeviceInterface
	{
	public:
		virtual ~DKGraphicsDeviceInterface() {}

		virtual DKString DeviceName() const = 0;

        virtual DKObject<DKCommandQueue> CreateCommandQueue(DKGraphicsDevice*, uint32_t) = 0;
		virtual DKObject<DKRenderPipelineState> CreateRenderPipeline(DKGraphicsDevice*, const DKRenderPipelineDescriptor&, DKPipelineReflection*) = 0;
		virtual DKObject<DKComputePipelineState> CreateComputePipeline(DKGraphicsDevice*, const DKComputePipelineDescriptor&, DKPipelineReflection*) = 0;

        virtual DKObject<DKShaderModule> CreateShaderModule(DKGraphicsDevice*, DKShader*) = 0;
        virtual DKObject<DKShaderBindingSet> CreateShaderBindingSet(DKGraphicsDevice*, const DKShaderBindingSetLayout&) = 0;

        virtual DKObject<DKGpuBuffer> CreateBuffer(DKGraphicsDevice*, size_t, DKGpuBuffer::StorageMode, DKCpuCacheMode) = 0;
        virtual DKObject<DKTexture> CreateTexture(DKGraphicsDevice*, const DKTextureDescriptor&) = 0;
        virtual DKObject<DKSamplerState> CreateSamplerState(DKGraphicsDevice*, const DKSamplerDescriptor&) = 0;
        virtual DKObject<DKGpuEvent> CreateEvent(DKGraphicsDevice*) = 0;
        virtual DKObject<DKGpuSemaphore> CreateSemaphore(DKGraphicsDevice*) = 0;

		static DKGraphicsDeviceInterface* CreateInterface();
		static DKGraphicsDeviceInterface* Instance(DKGraphicsDevice* p) { return p->impl; }
	};
}
