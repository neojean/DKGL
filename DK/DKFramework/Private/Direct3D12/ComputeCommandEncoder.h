//
//  File: ComputeCommandEncoder.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2015-2017 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../GraphicsAPI.h"
#if DKGL_ENABLE_DIRECT3D12
#include "d3d12_headers.h"

#include "../../DKComputeCommandEncoder.h"
#include "CommandBuffer.h"

namespace DKFramework
{
	namespace Private
	{
		namespace Direct3D
		{
			class ComputeCommandEncoder : public DKComputeCommandEncoder
			{
			public:
				ComputeCommandEncoder(ID3D12GraphicsCommandList*, CommandBuffer*);
				~ComputeCommandEncoder();


				void EndEncoding() override;
				DKCommandBuffer* Buffer() override;
                bool IsCompleted() const override;

				ComPtr<ID3D12GraphicsCommandList> commandList;
				DKObject<CommandBuffer> commandBuffer;
			};
		}
	}
}

#endif //#if DKGL_ENABLE_DIRECT3D12
